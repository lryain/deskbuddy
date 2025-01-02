/**
 * File: cozmoAPI.h
 *
 * Author: Lee Crippen
 * Created: 08/19/15
 *
 * Description: Point of entry for anything needing to interact with Vector.
 *
 * Copyright: Lrya, Inc. 2015
 *
 * COZMO_PUBLIC_HEADER
 **/

#ifndef __Lrya_Vector_CozmoAPI_h__
#define __Lrya_Vector_CozmoAPI_h__
#include "coretech/common/shared/types.h"
#include "util/export/export.h"
#include "util/helpers/noncopyable.h"
#include "json/json.h"

#include <mutex>

namespace Lrya {

// Forward declarations
namespace Util {
namespace Data {
  class DataPlatform;
}
}

namespace Vector {

class CozmoEngine;

class CozmoAPI : private Util::noncopyable
{
public:
  LRYA_VISIBLE bool Start(Util::Data::DataPlatform* dataPlatform, const Json::Value& config);

  LRYA_VISIBLE bool Update(const BaseStationTime_t currentTime_nanosec);

  // Activate A/B experiment
  LRYA_VISIBLE uint32_t ActivateExperiment(const uint8_t* requestBuffer, size_t requestLen,
                                           uint8_t* responseBuffer, size_t responseLen);

  LRYA_VISIBLE void RegisterEngineTickPerformance(const float tickDuration_ms,
                                                  const float tickFrequency_ms,
                                                  const float sleepDurationIntended_ms,
                                                  const float sleepDurationActual_ms) const;

  LRYA_VISIBLE ~CozmoAPI();

private:
  class EngineInstanceRunner
  {
  public:
    EngineInstanceRunner(Util::Data::DataPlatform* dataPlatform,
                        const Json::Value& config, bool& initResult);

    virtual ~EngineInstanceRunner();

    bool Update(const BaseStationTime_t currentTime_nanosec);
    CozmoEngine* GetEngine() const { return _engineInstance.get(); }
    void SyncWithEngineUpdate(const std::function<void()>& func) const;

    // Designate calling thread as owner of engine updates
    void SetEngineThread();

  private:
    std::unique_ptr<CozmoEngine> _engineInstance;
    mutable std::mutex _updateMutex;
  }; // class EngineInstanceRunner

  // Our running instance, if we have one
  std::unique_ptr<EngineInstanceRunner> _engineRunner;
}; // class CozmoAPI

} // namespace Vector
} // namespace Lrya

#endif // __Lrya_Vector_CozmoAPI_h__
