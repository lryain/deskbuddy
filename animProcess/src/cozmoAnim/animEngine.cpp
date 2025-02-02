/*
 * File:          animEngine.cpp
 * Date:          6/26/2017
 *
 * Description:   A platform-independent container for spinning up all the pieces
 *                required to run Vector Animation Process.
 *
 * Author: Kevin Yoon
 *
 * Modifications:
 */

#include "cozmoAnim/animEngine.h"

#include "cozmoAnim/alexa/alexa.h"
#include "cozmoAnim/animComms.h"
#include "cozmoAnim/animContext.h"
#include "cozmoAnim/animProcessMessages.h"
#include "cozmoAnim/audio/cozmoAudioController.h"
#include "cozmoAnim/audio/microphoneAudioClient.h"
#include "cozmoAnim/audio/engineRobotAudioInput.h"
#include "cozmoAnim/audio/sdkAudioComponent.h"
#include "cozmoAnim/animation/animationStreamer.h"
#include "cozmoAnim/animation/streamingAnimationModifier.h"
#include "cozmoAnim/backpackLights/animBackpackLightComponent.h"
#include "cozmoAnim/faceDisplay/faceInfoScreenManager.h"
#include "cozmoAnim/micData/micDataSystem.h"
#include "cozmoAnim/perfMetricAnim.h"
#include "cozmoAnim/robotDataLoader.h"
#include "cozmoAnim/showAudioStreamStateManager.h"
#include "cozmoAnim/textToSpeech/textToSpeechComponent.h"

#include "coretech/common/engine/opencvThreading.h"
#include "coretech/common/engine/utils/timer.h"
#include "coretech/vision/shared/spriteCache/spriteCache.h"
#include "audioEngine/multiplexer/audioMultiplexer.h"

#include "webServerProcess/src/webService.h"

#include "osState/osState.h"

#include "util/cpuProfiler/cpuProfiler.h"
#include "util/logging/logging.h"

#define LOG_CHANNEL    "AnimEngine"
#define NUM_ANIM_OPENCV_THREADS 0

namespace Lrya {
namespace Vector {
namespace Anim {

#if LRYA_CPU_PROFILER_ENABLED
  CONSOLE_VAR_RANGED(float, kAnimEngine_TimeMax_ms,     LRYA_CPU_CONSOLEVARGROUP, 33, 2, 33);
  CONSOLE_VAR_ENUM(u8,      kAnimEngine_TimeLogging,    LRYA_CPU_CONSOLEVARGROUP, 0, Util::CpuProfiler::CpuProfilerLogging());
#endif

AnimEngine::AnimEngine(Util::Data::DataPlatform* dataPlatform)
  : _isInitialized(false)
  , _context(std::make_unique<AnimContext>(dataPlatform))
  , _animationStreamer(std::make_unique<AnimationStreamer>(_context.get()))
{
#if LRYA_CPU_PROFILER_ENABLED
  // Initialize CPU profiler early and put tracing file at known location with no dependencies on other systems
  Lrya::Util::CpuProfiler::GetInstance();
  Lrya::Util::CpuThreadProfiler::SetChromeTracingFile(
      dataPlatform->pathToResource(Util::Data::Scope::Cache, "vic-anim-tracing.json").c_str());
  Lrya::Util::CpuThreadProfiler::SendToWebVizCallback([&](const Json::Value& json) { _context->GetWebService()->SendToWebViz("cpuprofile", json); });
#endif

  if (Lrya::Util::gTickTimeProvider == nullptr) {
    Lrya::Util::gTickTimeProvider = BaseStationTimer::getInstance();
  }

  _microphoneAudioClient.reset(new Audio::MicrophoneAudioClient(_context->GetAudioController()));

#if LRYA_PROFILE_ANIMCOMMS_SOCKET_BUFFER_STATS
  AnimComms::InitSocketBufferStats();
#endif

}

AnimEngine::~AnimEngine()
{
  _context->GetWebService()->Stop();

#if LRYA_PROFILE_ANIMCOMMS_SOCKET_BUFFER_STATS
  AnimComms::ReportSocketBufferStats();
#endif

  if (Lrya::Util::gTickTimeProvider == BaseStationTimer::getInstance()) {
    Lrya::Util::gTickTimeProvider = nullptr;
  }
  BaseStationTimer::removeInstance();
}

Result AnimEngine::Init()
{
  printf("3.1.1.1. in AnimEngine::Init()...\n");
  if (_isInitialized) {
    LOG_INFO("AnimEngine.Init.ReInit", "Reinitializing already-initialized CozmoEngineImpl with new config.");
  }

  uint32_t seed = 0; // will choose random seed
# ifdef LRYA_PLATFORM_OSX
  {
    seed = 1; // Setting to non-zero value for now for repeatable testing.
  }
# endif
  _context->SetRandomSeed(seed);
//   printf("3.1.1.2. done SetRandomSeed\n");

  OSState::getInstance()->SetUpdatePeriod(1000);

  RobotDataLoader * dataLoader = _context->GetDataLoader();
//   printf("3.1.1.3. done _context->GetDataLoader()\n");
  dataLoader->LoadConfigData();
//   printf("3.1.1.4. done dataLoader->LoadConfigData()\n");
  dataLoader->LoadNonConfigData();
//   printf("3.1.1.5. done dataLoader->LoadNonConfigData()\n");

  _ttsComponent = std::make_unique<TextToSpeechComponent>(_context.get());
  _context->GetMicDataSystem()->Init(*dataLoader);
//   printf("3.1.1.6. done _context->GetMicDataSystem()->Init()\n");

  // animation streamer must be initialized after loading non config data (otherwise there are no animations loaded)
  _animationStreamer->Init(_ttsComponent.get());
//   printf("3.1.1.7. done _animationStreamer->Init(_ttsComponent.get())\n");
  _context->GetBackpackLightComponent()->Init();
//   printf("3.1.1.8. done _context->GetBackpackLightComponent()->Init()\n");

  // Create and set up EngineRobotAudioInput to receive Engine->Robot messages and broadcast Robot->Engine
  auto* audioMux = _context->GetAudioMultiplexer();
//   printf("3.1.1.9. done _context->GetAudioMultiplexer()\n");
  auto regId = audioMux->RegisterInput( new Audio::EngineRobotAudioInput() );
  // Easy access to Audio Controller
  _audioControllerPtr = _context->GetAudioController();
//   printf("3.1.2.0. done _context->GetAudioController()\n");

  // Set up message handler
  auto * audioInput = static_cast<Audio::EngineRobotAudioInput*>(audioMux->GetInput(regId));
  _streamingAnimationModifier = std::make_unique<StreamingAnimationModifier>(_animationStreamer.get(), audioInput, _ttsComponent.get());
//   printf("3.1.2.1. done _streamingAnimationModifier\n");

  // set up audio stream state manager
  {
    _context->GetShowAudioStreamStateManager()->SetAnimationStreamer(_animationStreamer.get());
  }

//   printf("3.1.2.2. done set up audio stream state manager\n");

  AnimProcessMessages::Init(this, _animationStreamer.get(), _streamingAnimationModifier.get(), audioInput, _context.get());
//   printf("3.1.2.3. done AnimProcessMessages::Init()\n");

  _context->GetWebService()->Start(_context->GetDataPlatform(),
                                   _context->GetDataLoader()->GetWebServerAnimConfig());
//   printf("3.1.2.4. done _context->GetWebService()->Start()\n");
  FaceInfoScreenManager::getInstance()->Init(_context.get(), _animationStreamer.get());
//   printf("3.1.2.5. done FaceInfoScreenManager::getInstance()->Init()\n");

  _context->GetAlexa()->Init(_context.get());
//   printf("3.1.2.6. _context->GetAlexa()->Init()\n");

  const auto pm = _context->GetPerfMetric();
  pm->Init(_context->GetDataPlatform(), _context->GetWebService());
//   printf("3.1.2.7. pm->Init()\n");
  pm->SetAnimationStreamer(_animationStreamer.get());
  if (pm->GetAutoRecord())
  {
    pm->Start();
  }
//   printf("3.1.2.8. pm->Start()\n");

  // Make sure OpenCV isn't threading
  Result cvResult = SetNumOpencvThreads( NUM_ANIM_OPENCV_THREADS, "AnimEngine.Init" );
  if( RESULT_OK != cvResult )
  {
    return cvResult;
  }
//   printf("3.1.2.9. done SetNumOpencvThreads()!! \n");

  _sdkAudioComponent = std::make_unique<SdkAudioComponent>(_context.get());

  printf("3.1.3.0. AnimEngine.Init.Success  ------>  return!! \n");
  LOG_INFO("AnimEngine.Init.Success","Success");
  _isInitialized = true;

  return RESULT_OK;
}

Result AnimEngine::Update(const BaseStationTime_t currTime_nanosec)
{
  LRYA_CPU_TICK("AnimEngine::Update", kAnimEngine_TimeMax_ms, Util::CpuProfiler::CpuProfilerLoggingTime(kAnimEngine_TimeLogging));
  if (!_isInitialized) {
    LOG_ERROR("AnimEngine.Update", "Cannot update AnimEngine before it is initialized.");
    return RESULT_FAIL;
  }

  //
  // Declare some invariants. These components are always present after successful initialization.
  //
  DEV_ASSERT(_context, "AnimEngine.Update.InvalidContext");
  DEV_ASSERT(_ttsComponent, "AnimEngine.Update.InvalidTTSComponent");
  DEV_ASSERT(_animationStreamer, "AnimEngine.Update.InvalidAnimationStreamer");
  DEV_ASSERT(_streamingAnimationModifier, "AnimEngine.Update.InvalidStreamingAnimationModifier");
  DEV_ASSERT(_sdkAudioComponent, "AnimEngine.Update.InvalidSdkComponent");

#if LRYA_PROFILE_ANIMCOMMS_SOCKET_BUFFER_STATS
  {
    // Update socket buffer counters
    AnimComms::UpdateSocketBufferStats();
  }
#endif

  BaseStationTimer::getInstance()->UpdateTime(currTime_nanosec);

//   printf("1.0 -------------> start _context->GetWebService()->Update()\n");fflush(stdout);
  _context->GetWebService()->Update();

//   printf("1.1 -------------> start AnimProcessMessages::Update()\n");fflush(stdout);
  Result result = AnimProcessMessages::Update(currTime_nanosec);
  if (RESULT_OK != result) {
    LOG_WARNING("AnimEngine.Update", "Unable to process messages (result %d)", result);
    return result;
  }

//   printf("1.2 -------------> start OSState::getInstance()->Update()\n");fflush(stdout);
  OSState::getInstance()->Update(currTime_nanosec);

//   printf("1.3 -------------> start _ttsComponent->Update()\n");fflush(stdout);
  _ttsComponent->Update();

//   printf("1.4 -------------> start _context->GetDataLoader()->GetSpriteCache()->Update()\n");fflush(stdout);
  // Clear out sprites that have passed their cache time
  _context->GetDataLoader()->GetSpriteCache()->Update(currTime_nanosec);

//   printf("1.5 -------------> start _streamingAnimationModifier->ApplyAlterationsBeforeUpdate(_animationStreamer.get())\n");fflush(stdout);
  // Update animations
  _streamingAnimationModifier->ApplyAlterationsBeforeUpdate(_animationStreamer.get());

//   printf("1.6 -------------> start _animationStreamer->Update()\n");fflush(stdout);
  _animationStreamer->Update();

//   printf("1.7 -------------> start ApplyAlterationsAfterUpdate()\n");fflush(stdout);
  _streamingAnimationModifier->ApplyAlterationsAfterUpdate(_animationStreamer.get());

//   printf("1.8 -------------> start Update audio controller\n");fflush(stdout);
  // Update audio controller
  if (_audioControllerPtr != nullptr) {
    // Update mic info in Audio Engine
    const auto& micDirectionMsg = _context->GetMicDataSystem()->GetLatestMicDirectionMsg();
    _microphoneAudioClient->ProcessMessage(micDirectionMsg);
    // Tick the Audio Engine at the end of each anim frame
//     printf("1.8.1 -------------> start _audioControllerPtr->Update()\n");fflush(stdout);
    _audioControllerPtr->Update();
  }

//   printf("1.9 -------------> start Update backpack lights\n");fflush(stdout);
  // Update backpack lights
  _context->GetBackpackLightComponent()->Update();

#if LRYA_PROFILE_ANIMCOMMS_SOCKET_BUFFER_STATS
  {
    // Update socket buffer counters
//     printf("1.9.1 -------------> start Update socket buffer counters\n");fflush(stdout);
    AnimComms::UpdateSocketBufferStats();
  }
#endif

//   printf("1.10 -------------> done!\n");fflush(stdout);
  return RESULT_OK;
}

void AnimEngine::RegisterTickPerformance(const float tickDuration_ms,
                                         const float tickFrequency_ms,
                                         const float sleepDurationIntended_ms,
                                         const float sleepDurationActual_ms) const
{
  _context->GetPerfMetric()->Update(tickDuration_ms, tickFrequency_ms,
                                    sleepDurationIntended_ms, sleepDurationActual_ms);
}

void AnimEngine::HandleMessage(const RobotInterface::TextToSpeechPrepare & msg)
{
  DEV_ASSERT(_ttsComponent, "AnimEngine.TextToSpeechPrepare.InvalidTTSComponent");
  _ttsComponent->HandleMessage(msg);
}

void AnimEngine::HandleMessage(const RobotInterface::TextToSpeechPlay & msg)
{
  DEV_ASSERT(_ttsComponent, "AnimEngine.TextToSpeechPlay.InvalidTTSComponent");
  _ttsComponent->HandleMessage(msg);
}

void AnimEngine::HandleMessage(const RobotInterface::TextToSpeechCancel & msg)
{
  DEV_ASSERT(_ttsComponent, "AnimEngine.TextToSpeechCancel.InvalidTTSComponent");
  _ttsComponent->HandleMessage(msg);
}

void AnimEngine::HandleMessage(const RobotInterface::SetLocale & msg)
{
  const std::string locale(msg.locale, msg.locale_length);

  LOG_INFO("AnimEngine.SetLocale", "Set locale to %s", locale.c_str());

  if (_context != nullptr) {
    _context->SetLocale(locale);
  }

  if (_ttsComponent != nullptr) {
    _ttsComponent->SetLocale(locale);
  }
}

void AnimEngine::HandleMessage(const RobotInterface::ExternalAudioChunk & msg)
{
  DEV_ASSERT(_sdkAudioComponent, "AnimEngine.ExternalAudioChunk.InvalidSDKAudioComponent");
  _sdkAudioComponent->HandleMessage(msg);
}

void AnimEngine::HandleMessage(const RobotInterface::ExternalAudioComplete & msg)
{
  DEV_ASSERT(_sdkAudioComponent, "AnimEngine.ExternalAudioComplete.InvalidSDKAudioComponent");
  _sdkAudioComponent->HandleMessage(msg);
}

void AnimEngine::HandleMessage(const RobotInterface::ExternalAudioCancel & msg)
{
  DEV_ASSERT(_sdkAudioComponent, "AnimEngine.ExternalAudioCancel.InvalidSDKAudioComponent");
  _sdkAudioComponent->HandleMessage(msg);
}

void AnimEngine::HandleMessage(const RobotInterface::ExternalAudioPrepare & msg)
{
  DEV_ASSERT(_sdkAudioComponent, "AnimEngine.ExternalAudioPrepare.InvalidSDKAudioComponent");
  _sdkAudioComponent->HandleMessage(msg);
}


} // namespace Anim
} // namespace Vector
} // namespace Lrya
