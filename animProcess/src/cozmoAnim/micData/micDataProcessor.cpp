/**
* File: micDataProcessor.cpp
*
* Author: Lee Crippen
* Created: 9/27/2017
*
* Description: Handles processing the mic samples from the robot process: combining the channels,
*              and extracting direction data.
*
* Copyright: Lrya, Inc. 2017
*
*/


// Signal Essence Includes
// #include "mmif.h"
// #include "policy_actions.h"
// #include "se_diag.h"

#include "cozmoAnim/animContext.h"
#include "cozmoAnim/animProcessMessages.h"
#include "cozmoAnim/beatDetector/beatDetector.h"
#include "cozmoAnim/faceDisplay/faceDisplay.h"
#include "cozmoAnim/micData/micDataProcessor.h"
#include "cozmoAnim/micData/micDataSystem.h"
#include "cozmoAnim/micData/micDataInfo.h"
#include "cozmoAnim/micData/micImmediateDirection.h"
#include "cozmoAnim/showAudioStreamStateManager.h"
#include "cozmoAnim/speechRecognizer/speechRecognizerSystem.h"
#include "audioUtil/speechRecognizer.h"
#include "util/console/consoleInterface.h"
#include "util/console/consoleFunction.h"
#include "util/cpuProfiler/cpuProfiler.h"
#include "util/fileUtils/fileUtils.h"
#include "util/helpers/lryaDefines.h"
#include "util/logging/logging.h"
#include "util/math/math.h"
#include "util/threading/threadPriority.h"
#include "clad/robotInterface/messageRobotToEngine_sendAnimToEngine_helper.h"
#include <list>
#include <sched.h>


namespace Lrya {
namespace Vector {
namespace MicData {

namespace {
  
#define LOG_CHANNEL "Microphones"
#define CONSOLE_GROUP "MicData"

  CONSOLE_VAR(bool, kMicData_CollectRawTriggers, CONSOLE_GROUP, false);
  CONSOLE_VAR(bool, kMicData_SpeakerNoiseDisablesMics, CONSOLE_GROUP, false);

  // Time necessary for the VAD logic to wait when there's no activity, before we begin skipping processing for
  // performance. Note that this probably needs to at least be as long as the trigger, which is ~ 500-750ms.
  CONSOLE_VAR_RANGED(uint32_t, kMicData_QuietTimeCooldown_ms, CONSOLE_GROUP, 1000, 500, 10000);

#if LRYA_DEV_CHEATS

  CONSOLE_VAR(bool, kMicData_SaveRawFullIntent, CONSOLE_GROUP, false);
  CONSOLE_VAR(bool, kMicData_SaveRawFullIntent_WakeWordless, CONSOLE_GROUP, false);
  
  CONSOLE_VAR(bool, kMicData_ForceEnableMicDataProc, CONSOLE_GROUP, true);
  CONSOLE_VAR(bool, kMicData_ForceDisableMicDataProc, CONSOLE_GROUP, false);
  
  uint8_t _currentDevForcedProcesState = 4;
  CONSOLE_VAR_ENUM(uint8_t, kDevForceProcessState, CONSOLE_GROUP, _currentDevForcedProcesState,
                   "NormalOperation,None,NoProcessingSingleMic,SigEsBeamformingOff,SigEsBeamformingOn");
  
  std::list<Lrya::Util::IConsoleFunction> sConsoleFuncs;

#endif // LRYA_DEV_CHEATS

  CONSOLE_VAR(bool, kBeatDetectorUseProcessedAudio, CONSOLE_GROUP, true);
  
  #define ENABLE_MIC_PROCESSING_STATE_UPDATE_LOG 0
  using MicProcessingState = MicDataProcessor::ProcessingState;
  const MicProcessingState kDefaultProcessingState = MicProcessingState::SigEsBeamformingOff;
  const MicProcessingState kLowPowerModeProcessingState = MicProcessingState::NoProcessingSingleMic;

} // anonymous namespace

CONSOLE_VAR_RANGED(float, maxProcessingTimePerDrop_ms,      "CpuProfiler", 5, 5, 32);

#if LRYA_CPU_PROFILER_ENABLED
CONSOLE_VAR_RANGED(float, maxTriggerProcTime_ms,            LRYA_CPU_CONSOLEVARGROUP, 10, 10, 32);
CONSOLE_VAR_ENUM(u8,      kMicDataProcessorRaw_Logging,     LRYA_CPU_CONSOLEVARGROUP, 0, Util::CpuProfiler::CpuProfilerLogging());
CONSOLE_VAR_ENUM(u8,      kMicDataProcessorTrigger_Logging, LRYA_CPU_CONSOLEVARGROUP, 0, Util::CpuProfiler::CpuProfilerLogging());
#endif

constexpr auto kCladMicDataTypeSize = sizeof(RobotInterface::MicData::data)/sizeof(RobotInterface::MicData::data[0]);
static_assert(kCladMicDataTypeSize == kIncomingAudioChunkSize,
              "Expecting size of MicData::data to match kIncomingAudioChunkSize");



void MicDataProcessor::SetupConsoleFuncs()
{
#if LRYA_DEV_CHEATS
  // TODO: Left method for furture console vars
#endif
}
# undef CONSOLE_GROUP


MicDataProcessor::MicDataProcessor(const Anim::AnimContext* context, MicDataSystem* micDataSystem,
                                   const std::string& writeLocation)
: _context(context)
, _micDataSystem(micDataSystem)
, _writeLocationDir(writeLocation)
, _micImmediateDirection(std::make_unique<MicImmediateDirection>())
, _beatDetector(std::make_unique<BeatDetector>())
{
  // Init the various SE processing
//   TODO: 替换 SE
//   MMIfInit(0, nullptr);
  InitVAD();

  // Cache off the indices of the SE processing variables we will be accessing
//   _bestSearchBeamIndex = SEDiagGetIndex("fdsearch_best_beam_index");
//   _bestSearchBeamConfidence = SEDiagGetIndex("fdsearch_best_beam_confidence");
//   _selectedSearchBeamIndex = SEDiagGetIndex("search_result_best_beam_index");
//   _selectedSearchBeamConfidence = SEDiagGetIndex("search_result_best_beam_confidence");
//   _searchConfidenceState = SEDiagGetIndex("fdsearch_confidence_state");
//   _policyFallbackFlag = SEDiagGetIndex("policy_fallback_flag");
  
  SetupConsoleFuncs();
}

void MicDataProcessor::Init()
{
  ASSERT_NAMED(_micDataSystem != nullptr, "MicDataProcessor.Init._micDataSystem.IsNull");
  ASSERT_NAMED(_micDataSystem->GetSpeechRecognizerSystem() != nullptr,
               "MicDataProcessor.Init._micDataSystem.GetSpeechRecognizerSystem.IsNull");
  // Link recognizer
  _speechRecognizerSystem = _micDataSystem->GetSpeechRecognizerSystem();
  
  // Set initial processing state
  SetActiveMicDataProcessingState(kDefaultProcessingState);
  
  // Start the thread doing the SE processing of audio
  _processThread = std::thread(&MicDataProcessor::ProcessRawLoop, this);
  
  // Start the thread doing the Sensory processing of audio
  _processTriggerThread = std::thread(&MicDataProcessor::ProcessTriggerLoop, this);
}

void MicDataProcessor::InitVAD()
{
//   _sVadConfig.reset(new SVadConfig_t());
//   _sVadObject.reset(new SVadObject_t());

//   /* set up VAD */
//   SVadSetDefaultConfig(_sVadConfig.get(), kSamplesPerBlockPerChannel, (float)AudioUtil::kSampleRate_hz);
//   _sVadConfig->AbsThreshold = 250.0f; // was 400
//   _sVadConfig->HangoverCountDownStart = 10;  // was 25, make 25 blocks (1/4 second) to see it actually end a couple times
//   SVadInit(_sVadObject.get(), _sVadConfig.get());
}

void MicDataProcessor::VoiceTriggerWordDetection(const AudioUtil::SpeechRecognizerCallbackInfo& info)
{
  TriggerWordDetectCallback(TriggerWordDetectSource::Voice, info);
}
  
void MicDataProcessor::FakeTriggerWordDetection(bool fromMute)
{
  const AudioUtil::SpeechRecognizerCallbackInfo info {
    .result       = "",
    .startTime_ms = 0,
    .endTime_ms   = 0,
    .score        = 0.0f
  };
  const auto source = fromMute ? TriggerWordDetectSource::ButtonFromMute : TriggerWordDetectSource::Button;
  TriggerWordDetectCallback(source, info);
}

void MicDataProcessor::GetLatestMicDirectionData(MicDirectionData& out_lastSample,
                                                 DirectionIndex& out_dominantDirection) const
{
  out_lastSample = _micImmediateDirection->GetLatestSample();
  out_dominantDirection = _micImmediateDirection->GetDominantDirection();
}
  
void MicDataProcessor::TriggerWordDetectCallback(TriggerWordDetectSource source,
                                                 const AudioUtil::SpeechRecognizerCallbackInfo& info)
{
  ShowAudioStreamStateManager* showStreamState = _context->GetShowAudioStreamStateManager();
  // Ignore extra triggers during streaming
  if (_micDataSystem->HasStreamingJob() || !showStreamState->HasValidTriggerResponse())
  {
    return;
  }
  
  // By the time the earcon completes, engine may have changed its response, so assume the decision to stream
  // should be based on the engine-requested state at the time of the trigger word callback. Ugh.
  // Anyway, we should cache shouldStream and use it in earConCallback
  bool shouldStream = showStreamState->ShouldStreamAfterTriggerWordResponse();
  
  // Start command stream after EarCon completes
  auto earConCallback = [this,shouldStream](bool success) {
    // If we didn't succeed, it means that we didn't have a wake word response setup
    if (success) {
      RobotTimeStamp_t mostRecentTimestamp = CreateTriggerWordDetectedJobs(shouldStream);
      LOG_INFO("MicDataProcessor.TWCallback", "Timestamp %d", (TimeStamp_t)mostRecentTimestamp);
    }
    else {
      // since we're not opening up a stream, we need to reset the streaming light since it get's turned on
      // when we hear the trigger word
      _micDataSystem->SetWillStream(false);
      LOG_WARNING("MicDataProcessor.TWCallback", "Don't have a wake word response setup");
    }
  };
  
  const bool muteButton = (source == TriggerWordDetectSource::ButtonFromMute);
  const bool buttonPress = (source == TriggerWordDetectSource::Button) || muteButton;
  if( muteButton ) {
    // don't play the get-in if this trigger word started from mute, because the mute animation should be playing
    showStreamState->SetPendingTriggerResponseWithoutGetIn(earConCallback);
  } else {
    showStreamState->SetPendingTriggerResponseWithGetIn(earConCallback);
  }

  const auto currentDirection = _micImmediateDirection->GetDominantDirection();
  const bool willStreamAudio = showStreamState->ShouldStreamAfterTriggerWordResponse() &&
                               !_micDataSystem->ShouldSimulateStreaming();

  // Set up a message to send out about the triggerword
  RobotInterface::TriggerWordDetected twDetectedMessage;
  twDetectedMessage.direction = currentDirection;
  twDetectedMessage.isButtonPress = buttonPress;
  twDetectedMessage.fromMute = muteButton;
  twDetectedMessage.triggerScore = (uint32_t) info.score;
  twDetectedMessage.willOpenStream = willStreamAudio;
  auto engineMessage = std::make_unique<RobotInterface::RobotToEngine>(std::move(twDetectedMessage));
  _micDataSystem->SendMessageToEngine(std::move(engineMessage));

  // Tell signal essence software to lock in on the current direction if it's known
  // NOTE: This is disabled for now as we've gotten better accuracy with the direction of the intent
  // that happens after this point, so it is currently not desireable to lock in the supposed trigger direction,
  // as that direction can be incorrect due to motor noise, speaker noise, etc.
  // The code is left here with this comment to make resurrecting this functionality in the SE software easier in the
  // future, if desired.
  // if (currentDirection != kDirectionUnknown)
  // {
  //   std::lock_guard<std::mutex> lock(_seInteractMutex);
  //   PolicySetKeyPhraseDirection(currentDirection);
  // }

  LOG_INFO("MicDataProcessor.TWCallback", "Direction index %d", currentDirection);
}
  
RobotTimeStamp_t MicDataProcessor::CreateStreamJob(CloudMic::StreamType streamType,
                                                  uint32_t overlapLength_ms)
{
  // Setup Job
  auto newJob = std::make_shared<MicDataInfo>();
  newJob->_writeLocationDir = Util::FileUtils::FullFilePath({_writeLocationDir, "triggeredCapture"});
  newJob->_writeNameBase = ""; //use autogen names
  newJob->_numMaxFiles = 100;
  newJob->_type = streamType;
  bool saveToFile = false;
#if LRYA_DEV_CHEATS
  saveToFile = true;
  // Simplify stream type
  bool saveRawFullStream = false;
  switch (streamType) {
    case CloudMic::StreamType::Normal:
      saveRawFullStream = kMicData_SaveRawFullIntent;
      break;
    case CloudMic::StreamType::Blackjack:
    case CloudMic::StreamType::KnowledgeGraph:
      saveRawFullStream = kMicData_SaveRawFullIntent_WakeWordless;
      break;
  }

  if (saveRawFullStream) {
    newJob->EnableDataCollect(MicDataType::Raw, true);
  }
  newJob->_audioSaveCallback = std::bind(&MicDataSystem::AudioSaveCallback, _micDataSystem, std::placeholders::_1);
#endif
  newJob->EnableDataCollect(MicDataType::Processed, saveToFile);
  newJob->SetTimeToRecord(MicDataInfo::kMaxRecordTime_ms);
  newJob->SetAudioFadeInTime(MicDataInfo::kDefaultAudioFadeIn_ms);
  
  // Copy the current audio chunks in the trigger overlap buffer
  // The immediate buffer is bigger than just the overlap time (time right after trigger end but before trigger was
  // recognized), so that the immediate buffer also contains the trigger itself. So here we set our start index to
  // only capture that in-between time, and push it into the streaming job for intent matching
  std::lock_guard<std::mutex> lock(_procAudioXferMutex);
  DEV_ASSERT(_procAudioRawComplete >= _procAudioXferCount,
             "MicDataProcessor.CreateStreamJob.AudioProcIdx");
  
  if (overlapLength_ms > 0) {
    const auto overlapCount = overlapLength_ms / kTimePerChunk_ms;
    const auto maxIndex = _procAudioRawComplete - _procAudioXferCount;
    size_t triggerOverlapStartIdx = (maxIndex > overlapCount) ? (maxIndex - overlapCount) : 0;
    
    for (size_t i=triggerOverlapStartIdx; i<maxIndex; ++i)
    {
      const auto& audioBlock = _immediateAudioBuffer[i].audioBlock;
      newJob->CollectProcessedAudio(audioBlock.data(), audioBlock.size());
    }

    // Copy the current audio chunks in the trigger overlap buffer
    for (size_t i=0; i<_immediateAudioBuffer.size(); ++i)
    {
      const auto& audioBlock = _immediateAudioBuffer[i].audioBlock;
      newJob->CollectRawAudio(audioBlock.data(), audioBlock.size());
    }
  }

  const bool isStreamingJob = true;
  _micDataSystem->AddMicDataJob(newJob, isStreamingJob);
  
  RobotTimeStamp_t mostRecentTimestamp = _immediateAudioBuffer[_procAudioRawComplete-1].timestamp;
  return mostRecentTimestamp;
}

RobotTimeStamp_t MicDataProcessor::CreateTriggerWordDetectedJobs(bool shouldStream)
{
  RobotTimeStamp_t mostRecentTimestamp = 0;
  if (shouldStream)
  {
    // First we create the job responsible for streaming the intent after the trigger
    mostRecentTimestamp = CreateStreamJob(CloudMic::StreamType::Normal, kTriggerOverlapSize_ms);
  } else {
    LOG_INFO("MicDataProcessor.CreateTriggerWordDetectedJobs.NoStreaming", "Not adding streaming jobs because disabled");
  }

  // Now we set up the optional job for recording _just_ the trigger that was just recognized
  bool saveTriggerOnly = false;
# if LRYA_DEV_CHEATS
  saveTriggerOnly = true;
# endif // LRYA_DEV_CHEATS
  
  if (saveTriggerOnly)
  {
    std::lock_guard<std::mutex> lock(_procAudioXferMutex);
    
    auto triggerJob = std::make_shared<MicDataInfo>();
    triggerJob->_writeLocationDir = Util::FileUtils::FullFilePath({_writeLocationDir, "triggersOnly"});
    triggerJob->_writeNameBase = ""; // Use the autogen names in this subfolder
    triggerJob->_numMaxFiles = 100;
    triggerJob->EnableDataCollect(MicDataType::Processed, saveTriggerOnly);
    if (kMicData_CollectRawTriggers)
    {
      triggerJob->EnableDataCollect(MicDataType::Raw, saveTriggerOnly);
    }
    triggerJob->_audioSaveCallback = std::bind(&MicDataSystem::AudioSaveCallback, _micDataSystem, std::placeholders::_1);
    
    // We only record a little extra time beyond what we're stuffing in below
    constexpr uint32_t timeAfterTriggerEnd_ms = 170;
    triggerJob->SetTimeToRecord(timeAfterTriggerEnd_ms);
    const auto maxIndex = _procAudioRawComplete - _procAudioXferCount;
    for (size_t i=0; i<maxIndex; ++i)
    {
      const auto& audioBlock = _immediateAudioBuffer[i].audioBlock;
      triggerJob->CollectProcessedAudio(audioBlock.data(), audioBlock.size());
    }
    for (size_t i=0; i<_immediateAudioBuffer.size(); ++i)
    {
      const auto& audioBlock = _immediateAudioBuffer[i].audioBlock;
      triggerJob->CollectRawAudio(audioBlock.data(), audioBlock.size());
    }
    const auto notStreamingJob = false;
    _micDataSystem->AddMicDataJob(triggerJob, notStreamingJob);
  }

  return mostRecentTimestamp;
}

MicDataProcessor::~MicDataProcessor()
{
  _processThreadStop = true;
  _xferAvailableCondition.notify_all();
  _dataReadyCondition.notify_all();
  _processThread.join();
  _processTriggerThread.join();

//   MMIfDestroy();
}

void MicDataProcessor::ProcessRawAudio(RobotTimeStamp_t timestamp,
                                       const AudioUtil::AudioSample* audioChunk,
                                       uint32_t robotStatus,
                                       float robotAngle)
{
  LRYA_CPU_PROFILE("MicDataProcessor::ProcessRawAudio");
  TimedMicData* nextSampleSpot = nullptr;
  {
    // Note we don't bother to free any slots here that have been consumed (by comparing size to _procAudioXferCount)
    // because it's unnecessary with the circular buffer.

    std::unique_lock<std::mutex> lock(_procAudioXferMutex);
    auto xferAvailableCheck = [this] () {
      return _processThreadStop || _procAudioXferCount < _immediateAudioBuffer.capacity();
    };
    _xferAvailableCondition.wait(lock, xferAvailableCheck);

    if (_processThreadStop) {
      return;
    }

    // Now we can be sure we have a free slot, so go ahead and grab it
    if (_immediateAudioBuffer.size() < _immediateAudioBuffer.capacity()) {
        _procAudioRawComplete = _immediateAudioBuffer.size();
    } else {
        _procAudioRawComplete = _immediateAudioBuffer.size() - 1;
    }
    nextSampleSpot = &_immediateAudioBuffer.push_back();
  }

  TimedMicData& nextSample = *nextSampleSpot;
  nextSample.timestamp = timestamp;
  MicDirectionData directionResult = ProcessMicrophonesSE(
    audioChunk,
    nextSample.audioBlock.data(),
    robotStatus,
    robotAngle);

  // Feed the samples to the beat detector. Optionally either use a raw single channel (the first quarter of the
  // un-interleaved audio block) or the processed audio block
  auto* audioSource = kBeatDetectorUseProcessedAudio ? nextSample.audioBlock.data() : audioChunk;
  
  UpdateBeatDetector(audioSource, kSamplesPerBlockPerChannel);
  
  // Now we're done filling out this slot, update the count so it can be consumed
  {
    std::lock_guard<std::mutex> lock(_procAudioXferMutex);
    ++_procAudioXferCount;
    _procAudioRawComplete = _immediateAudioBuffer.size();
  }
  _dataReadyCondition.notify_all();

  // Store off this most recent result in our immedate direction tracking
  _micImmediateDirection->AddDirectionSample(directionResult);

  // Set up a message to send out about the direction
  RobotInterface::MicDirection newMessage;
  newMessage.timestamp = (TimeStamp_t)timestamp;
  newMessage.direction = directionResult.winningDirection;
  newMessage.confidence = directionResult.winningConfidence;
  newMessage.selectedDirection = directionResult.selectedDirection;
  newMessage.selectedConfidence = directionResult.selectedConfidence;
  newMessage.activeState = directionResult.activeState;
  newMessage.latestPowerValue = directionResult.latestPowerValue;
  newMessage.latestNoiseFloor = directionResult.latestNoiseFloor;
  std::copy(
    directionResult.confidenceList.begin(),
    directionResult.confidenceList.end(),
    newMessage.confidenceList);
  
  auto engineMessage = std::make_unique<RobotInterface::RobotToEngine>(std::move(newMessage));
  _micDataSystem->SendMessageToEngine(std::move(engineMessage));
}

/**
 * @brief 处理麦克风音频数据（信号增强/活动检测）。
 *
 * 本函数处理来自机器人麦克风的一帧音频数据，根据机器人当前状态（如移动、低功耗、扬声器播放等）
 * 决定是否进行信号增强（SE）、语音活动检测（VAD）或直接透传音频数据。
 *
 * 主要行为：
 * - 加锁内部互斥量，保证线程安全。
 * - 监控机器人移动和扬声器播放，动态调整处理方式，屏蔽不可靠数据。
 * - 实现VAD逻辑，静音时有冷却倒计时，避免静默期间误触发。
 * - 根据处理状态选择：原始拷贝、去直流偏置并增益、或信号增强（SE）。
 * - 更新并返回麦克风方向数据，包括活动标志、置信度等。
 * - 开发模式下可强制覆盖处理状态和活动检测。
 *
 * @param audioChunk   输入音频采样缓冲区指针（单通道，已去交错）。
 * @param bufferOut    输出处理后音频采样缓冲区指针。
 * @param robotStatus  当前机器人状态标志位（位掩码）。
 * @param robotAngle   机器人当前朝向角度（度或弧度）。
 * @return MicDirectionData
 *         返回处理结果，包括活动标志、功率值、噪声底、方向/置信度等信息。
 */
MicDirectionData MicDataProcessor::ProcessMicrophonesSE(const AudioUtil::AudioSample* audioChunk,
                            AudioUtil::AudioSample* bufferOut,
                            uint32_t robotStatus,
                            float robotAngle)
{
  std::lock_guard<std::mutex> lock(_seInteractMutex);
//   PolicySetAbsoluteOrientation(robotAngle);
  // 当前仅监控移动标志。也可以在机器人被提起时丢弃麦克风数据，但需与设计评估，见 VIC-1219
  const bool robotIsMoving = static_cast<bool>(robotStatus & (uint32_t)RobotStatusFlag::IS_MOVING);
  const bool robotStoppedMoving = !robotIsMoving && _robotWasMoving;
  _isInLowPowerMode = static_cast<bool>(robotStatus & (uint32_t)RobotStatusFlag::CALM_POWER_MODE);
  _robotWasMoving = robotIsMoving;

  // 检查是否正在通过扬声器播放音频。扬声器停止播放后增加一个小延迟，因为实际可能还在播放。
  const auto speakerCooldown_ms = _micDataSystem->GetSpeakerLatency_ms();
  const auto speakerCooldownLimit = speakerCooldown_ms / kTimePerChunk_ms;
  if (_micDataSystem->IsSpeakerPlayingAudio()) {
  _isSpeakerActive = true;
  _speakerCooldownCnt = speakerCooldownLimit;
  } else if (_speakerCooldownCnt-- == 0) {
  _isSpeakerActive = false;
  }
  
  const bool speakerStoppedPlaying = !_isSpeakerActive && _wasSpeakerActive;
  _wasSpeakerActive = _isSpeakerActive;
  
  // 机器人正在移动或播放音频
  const bool hasRobotNoise = (robotIsMoving || (_isSpeakerActive && kMicData_SpeakerNoiseDisablesMics));

  if (robotStoppedMoving || speakerStoppedPlaying)
  {
  // 当机器人停止移动或扬声器刚停止播放时，重置麦克风方向置信度
//     MMIfResetLocationSearch();
  }

  // 只检测一个通道，数据已去交错，直接传递给VAD检测
  float latestPowerValue = 0.f;
  float latestNoiseFloor = 0.f;
  int activityFlag = 0;
  {
  LRYA_CPU_PROFILE("ProcessVAD");

  // 机器人移动或扬声器播放时，忽略VAD，直接认为有活动
//     const float vadConfidence = 1.0f;
//     activityFlag = DoSVad(_sVadObject.get(), vadConfidence, (int16_t*)audioChunk);
//     latestPowerValue = _sVadObject->AvePowerInBlock;
//     latestNoiseFloor = _sVadObject->NoiseFloor;
  
  if (hasRobotNoise)
  {
    activityFlag = 1;
  }

  // VAD倒计时，静音时跳过触发识别，有活动时重置
  const auto vadCountdown_ms = kMicData_QuietTimeCooldown_ms;
  const auto vadCountdownLimit = vadCountdown_ms / kTimePerChunk_ms;
  if (activityFlag != 0)
  {
    _vadCountdown = vadCountdownLimit;
  }
  else if (_vadCountdown > 0)
  {
    --_vadCountdown;
  }

  if (_vadCountdown != 0)
  {
    activityFlag = 1;
  }
  }

  // 决定麦克风处理状态
  ProcessingState processingState = _activeProcState;

  if (!_isInLowPowerMode) {
  // 根据噪声状态更新处理状态
  // processingState = hasRobotNoise ? ProcessingState::SigEsBeamformingOff : ProcessingState::SigEsBeamformingOn;
  processingState = kLowPowerModeProcessingState;
  }
  else {
  processingState = kLowPowerModeProcessingState;
  }
  
#if LRYA_DEV_CHEATS
  
  // 开发模式下可强制使能/禁用处理，或强制处理状态
  if (kMicData_ForceEnableMicDataProc)
  {
  activityFlag = 1;
  }
  else if (kMicData_ForceDisableMicDataProc)
  {
  activityFlag = 0;
  }
  
  // 更新开发者处理状态
  if ((kDevForceProcessState > 0) || (kDevForceProcessState != _currentDevForcedProcesState)) {
  switch (kDevForceProcessState) {
    case 0:
    // 正常模式
    break;
    case 1:
    processingState = ProcessingState::None;
    break;
    case 2:
    processingState = ProcessingState::NoProcessingSingleMic;
    break;
    case 3:
    processingState = ProcessingState::SigEsBeamformingOff;
    break;
    case 4:
    processingState = ProcessingState::SigEsBeamformingOn;
    break;
    default:
    break;
  }
  _currentDevForcedProcesState = kDevForceProcessState;
  }
  
#endif
  
  // 更新状态
  SetActiveMicDataProcessingState(processingState);
  bool directionIsAvailable = false;
  
  switch (_activeProcState) {
  case ProcessingState::None:
  {
    // 直接拷贝单通道原始数据
    LRYA_CPU_PROFILE("ProcessRawSingleMicrophoneCopy");
    memcpy(bufferOut, audioChunk, sizeof(AudioUtil::AudioSample) * kSamplesPerBlockPerChannel);
    break;
  }
  case ProcessingState::NoProcessingSingleMic:
  {
    LOG_DEBUG(LOG_CHANNEL, "ProcessMicrophonesSE.NoProcessingSingleMic",
              "Processing state is NoProcessingSingleMic, copying single channel audio");
    // 去直流偏置并增益，输出第一个麦克风通道
    LRYA_CPU_PROFILE("ProcessSingleMicrophone");
    constexpr int iirCoefPower = 10;
    constexpr int iirMult = 1023; // (2 ^ iirCoefPower) - 1
    static int bias = audioChunk[0] << iirCoefPower;
    for (int i=0; i<kSamplesPerBlockPerChannel; ++i)
    {
    bias = ((bias * iirMult) >> iirCoefPower) + audioChunk[i];
    bufferOut[i] = audioChunk[i] - (bias >> iirCoefPower);
    bufferOut[i] <<= 3; // 增益8倍
    }
    break;
  }
  case ProcessingState::SigEsBeamformingOff:
  case ProcessingState::SigEsBeamformingOn:
  {
    // 信号处理（SE），此处为占位
//       static const std::array<
//           AudioUtil::AudioSample, 
//           kSamplesPerBlockPerChannel * kNumInputChannels> dummySpeakerOut{};
    {
    LRYA_CPU_PROFILE("ProcessMicrophonesSE");
    // MMIfProcessMicrophones(dummySpeakerOut.data(), audioChunk, bufferOut);
    }
    directionIsAvailable = true;
    break;
  }
  }

  MicDirectionData result{};
  result.activeState = activityFlag;
  result.latestPowerValue = latestPowerValue;
  result.latestNoiseFloor = latestNoiseFloor;

  // 有噪声或未用SE处理时，方向数据未知
  if (hasRobotNoise || !directionIsAvailable)
  {
    LOG_DEBUG(LOG_CHANNEL, "ProcessMicrophonesSE.HasRobotNoise",
              "Has robot noise or no direction data available, setting direction to unknown");
  result.winningDirection = result.selectedDirection = kDirectionUnknown;
  }
  else
  {
//     const auto latestDirection = SEDiagGetUInt16(_bestSearchBeamIndex);
//     const auto latestConf = SEDiagGetInt16(_bestSearchBeamConfidence);
//     const auto selectedDirection = SEDiagGetUInt16(_selectedSearchBeamIndex);
//     const auto selectedConf = SEDiagGetInt16(_selectedSearchBeamConfidence);
//     const auto* searchConfState = SEDiagGet(_searchConfidenceState);
//     result.winningDirection = latestDirection;
//     result.winningConfidence = latestConf;
//     result.selectedDirection = selectedDirection;
//     result.selectedConfidence = selectedConf;
//     const auto* confListSrc = reinterpret_cast<const float*>(searchConfState->u.vp);
//     std::copy(confListSrc, confListSrc + kLastValidIndex + 1, result.confidenceList.begin());
  }
  return result;
}

void MicDataProcessor::ProcessRawLoop()
{
  Lrya::Util::SetThreadName(pthread_self(), "MicProcRaw");
  
#if defined(LRYA_PLATFORM_MATEOS)
  // Setup the thread's affinity mask
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(2, &cpu_set);
  int error = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set);
  if (error != 0) {
    LOG_ERROR("MicDataProcessor.ProcessRawLoop", "SetAffinityMaskError %d", error);
  }
#endif
  
  static constexpr uint32_t expectedAudioDropsPerAnimLoop = 7;
  static const uint32_t maxProcTime_ms = expectedAudioDropsPerAnimLoop * maxProcessingTimePerDrop_ms;
  const auto maxProcTime = std::chrono::milliseconds(maxProcTime_ms);
  while (!_processThreadStop)
  {
    LRYA_CPU_TICK("MicDataProcessorRaw", maxProcTime_ms, Util::CpuProfiler::CpuProfilerLoggingTime(kMicDataProcessorRaw_Logging));
    const auto start = std::chrono::steady_clock::now();
  
    // Switch which buffer we're processing if it's empty
    {
      std::lock_guard<std::mutex> lock(_rawMicDataMutex);
      if (_rawAudioBuffers[_rawAudioProcessingIndex].empty())
      {
        _rawAudioProcessingIndex = (_rawAudioProcessingIndex == 1) ? 0 : 1;
      }
    }

    auto& rawAudioToProcess = _rawAudioBuffers[_rawAudioProcessingIndex];
    while (rawAudioToProcess.size() > 0)
    {
      LRYA_CPU_PROFILE("ProcessLoop");

      const auto& nextData = rawAudioToProcess.front();
      const auto* audioChunk = nextData.data;
      
      // Copy the current set of jobs we have for recording audio, so the list can be added to while processing
      // continues
      std::deque<std::shared_ptr<MicDataInfo>> jobs = _micDataSystem->GetMicDataJobs();
      // Collect the raw audio if desired
      for (auto& job : jobs)
      {
        job->CollectRawAudio(audioChunk, kIncomingAudioChunkSize);
      }

      _speechRecognizerSystem->UpdateNotch(audioChunk, kIncomingAudioChunkSize);
      
      // Factory test doesn't need to do any mic processing, it just uses raw data
      if(!FACTORY_TEST)
      {
        // Process the audio into a single channel, and collect it if desired
        (void) ProcessRawAudio(
          nextData.timestamp,
          audioChunk,
          nextData.robotStatusFlags,
          nextData.robotRotationAngle);
      }
      
      _micDataSystem->UpdateMicJobs();
      
      rawAudioToProcess.pop_front();
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsedTime = (end - start);
    if (elapsedTime < maxProcTime)
    {
      std::this_thread::sleep_for(maxProcTime - elapsedTime);
    }
  }
}

void MicDataProcessor::ProcessTriggerLoop()
{
  Lrya::Util::SetThreadName(pthread_self(), "MicProcTrigger");
  
#if defined(LRYA_PLATFORM_MATEOS)
  // Setup the thread's affinity mask
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(1, &cpu_set);
  int error = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu_set);
  if (error != 0) {
    LOG_ERROR("MicDataProcessor.ProcessTriggerLoop", "SetAffinityMaskError %d", error);
  }
#endif
  
  while (!_processThreadStop)
  {
    LRYA_CPU_TICK("MicDataProcessorTrigger", maxTriggerProcTime_ms, Util::CpuProfiler::CpuProfilerLoggingTime(kMicDataProcessorTrigger_Logging));
    LRYA_CPU_PROFILE("ProcessTriggerLoop");
    TimedMicData* readyDataSpot = nullptr;
    {
      LRYA_CPU_PROFILE("WaitForData");
      std::unique_lock<std::mutex> lock(_procAudioXferMutex);
      auto dataReadyCheck = [this] () { return _processThreadStop || _procAudioXferCount > 0; };
      _dataReadyCondition.wait(lock, dataReadyCheck);

      // Special case if we're being signalled to shut down
      if (_processThreadStop)
      {
        return;
      }

      // Grab a handle to the next available data that's been processed out of raw but not yet
      // "transferred" to the trigger word recognition processing
      readyDataSpot = &_immediateAudioBuffer[_procAudioRawComplete - _procAudioXferCount];
    }

    const auto& processedAudio = readyDataSpot->audioBlock;
    std::deque<std::shared_ptr<MicDataInfo>> jobs = _micDataSystem->GetMicDataJobs();
    for (auto& job : jobs)
    {
      job->CollectProcessedAudio(processedAudio.data(), processedAudio.size());
    }
    
    // Run the trigger detection, which will use the callback defined above
    {
      LRYA_CPU_PROFILE("RecognizeTriggerWord");
      // Note we skip it if there is no activity as of the latest processed audioblock
      _speechRecognizerSystem->Update(processedAudio.data(),
                                      (unsigned int)processedAudio.size(),
                                      (_micImmediateDirection->GetLatestSample().activeState != 0));
    }

    // Now we're done using this audio with the recognizer, so let it go
    {
      std::lock_guard<std::mutex> lock(_procAudioXferMutex);
      --_procAudioXferCount;
    }
    _xferAvailableCondition.notify_all();
  }
}
  
void MicDataProcessor::UpdateBeatDetector(const AudioUtil::AudioSample* const samples, const uint32_t nSamples)
{
  LRYA_CPU_PROFILE("BeatDetectorUpdate");

  // Only run the beat detector if we are not in low power mode
  if (_isInLowPowerMode) {
    if (_beatDetector->IsRunning()) {
      _beatDetector->Stop();
    }
  } else {
    if (!_beatDetector->IsRunning()) {
      _beatDetector->Start();
    }
    
    const bool beatDetected = _beatDetector->AddSamples(samples, nSamples);
    if (beatDetected) {
      auto beatMessage = RobotInterface::BeatDetectorState{_beatDetector->GetLatestBeat()};
      auto engineMessage = std::make_unique<RobotInterface::RobotToEngine>(std::move(beatMessage));
      _micDataSystem->SendMessageToEngine(std::move(engineMessage));
    }
  }
}
  
/**
 * @brief 处理传入的麦克风数据载荷并存储以供后续处理。
 *
 * 本方法接收一个麦克风数据载荷，如果麦克风未被静音，则将载荷存储到合适的原始音频缓冲区，
 * 以便后续处理。所使用的缓冲区会根据当前的处理索引进行切换，以避免覆盖正在处理的数据。
 * 通过加锁互斥量保证线程安全。
 *
 * @param payload 需要处理和存储的麦克风数据载荷。
 */
void MicDataProcessor::ProcessMicDataPayload(const RobotInterface::MicData& payload)
{
  // Store off this next job
  // LOG_DEBUG("MicDataProcessor.ProcessMicDataPayload", "");

  std::lock_guard<std::mutex> lock(_rawMicDataMutex);
  if (!_muteMics) {
    // Use whichever buffer is currently _not_ being processed
    auto& bufferToUse = (_rawAudioProcessingIndex == 1) ? _rawAudioBuffers[0] : _rawAudioBuffers[1];
    RobotInterface::MicData& nextJob = bufferToUse.push_back();
    nextJob = payload;
  }
}
  
void MicDataProcessor::MuteMics(bool mute)
{
  std::lock_guard<std::mutex> lock(_rawMicDataMutex);
  _muteMics = mute;
}

void MicDataProcessor::ResetMicListenDirection()
{
  std::lock_guard<std::mutex> lock(_seInteractMutex);
//   PolicyDoAutoSearch();
}

float MicDataProcessor::GetIncomingMicDataPercentUsed()
{
  std::lock_guard<std::mutex> lock(_rawMicDataMutex);
  // Use whichever buffer is currently _not_ being processed
  const auto inUseIndex = (_rawAudioProcessingIndex == 1) ? 0 : 1;
  const auto& bufferInUse = _rawAudioBuffers[inUseIndex];
  const auto updatedFullness = ((float)bufferInUse.size()) / ((float)bufferInUse.capacity());
  // Cache the current fullness for this buffer and use the greater of the two buffer fullnesses
  // This way the "fullness" returned is less variable and better covers the worst case
  _rawAudioBufferFullness[inUseIndex] = updatedFullness;
  return MAX(_rawAudioBufferFullness[0], _rawAudioBufferFullness[1]);
}

void MicDataProcessor::SetActiveMicDataProcessingState(MicDataProcessor::ProcessingState state)
{
  // Set the correct flag for Signal Essence lib version
// #if SE_V009
//   // v009
//   static const FallbackFlag_t kEcho_Cancel_Flag = FBF_FORCE_ECHO_CANCEL_WITH_NR;
// #else
//   // v008
//   static const FallbackFlag_t kEcho_Cancel_Flag = FBF_FORCE_ECHO_CANCEL;
// #endif
  
  if (state != _activeProcState) {
    if (ENABLE_MIC_PROCESSING_STATE_UPDATE_LOG) {
      LOG_INFO("MicDataProcessor.SetActiveMicDataProcessingState", "Current state '%s' new state '%s'",
                       GetProcessingStateName(_activeProcState), GetProcessingStateName(state));
    }
    
    switch (state) {
      case ProcessingState::None:
      case ProcessingState::NoProcessingSingleMic:
        // Do Nothing
        break;
      case ProcessingState::SigEsBeamformingOff:
      case ProcessingState::SigEsBeamformingOn:
      {
        // const bool shouldUseFallbackPolicy = (state == ProcessingState::SigEsBeamformingOff);
        // const FallbackFlag_t policySetting = shouldUseFallbackPolicy ? kEcho_Cancel_Flag : FBF_AUTO_SELECT;
        // printf("policySetting: %d", policySetting);
        // SEDiagSetEnumAsInt(_policyFallbackFlag, policySetting);
        break;
      }
    }
    _activeProcState = state;
  }
}
  
const char* MicDataProcessor::GetProcessingStateName(MicDataProcessor::ProcessingState state) const
{
  switch (state) {
    case ProcessingState::None:
      return "None";
    case ProcessingState::NoProcessingSingleMic:
      return "NoProcessingSingleMic";
    case ProcessingState::SigEsBeamformingOff:
      return "SigEsBeamformingOff";
    case ProcessingState::SigEsBeamformingOn:
      return "SigEsBeamformingOn";
  }
  return "";
}


} // namespace MicData
} // namespace Vector
} // namespace Lrya
