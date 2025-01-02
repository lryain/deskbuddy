/**
 * File: cpuProfiler
 *
 * Author: Mark Wesley
 * Created: 06/10/16
 *
 * Description: A lightweight (minimal skewing) CPU profiler
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/


#ifndef __Util_CpuProfiler_CpuProfiler_H__
#define __Util_CpuProfiler_CpuProfiler_H__


#include "util/cpuProfiler/cpuProfilerClock.h"
#include "util/cpuProfiler/cpuProfilerSettings.h"
#include "util/cpuProfiler/cpuProfileSampleShared.h"
#include "util/cpuProfiler/cpuThreadId.h"
#include "util/cpuProfiler/cpuThreadProfiler.h"
#include "util/logging/logging.h"
#include <assert.h>
#include <vector>
#include <atomic>

#if LRYA_CPU_PROFILER_ENABLED

  #define LRYA_CPU_CONSOLEVARGROUP "CpuProfiler"

  #define LRYA_CPU_PROFILER_WARN_ON_NO_PROFILER     0   // Enable to track down calls on untracked threads

  namespace Lrya {
  namespace Util {


  // ================================================================================
  // CpuProfiler
  
  class CpuProfiler
  {
  public:
    static const char* CpuProfilerLogging() {
      return "Never,Every tick,Every 2 ticks,Every 4 ticks,Every 8 ticks,Every 16 ticks,Every 32 ticks";
    }
    
    static uint32_t CpuProfilerLoggingTime(int option) {
      static uint32_t times[] = {Lrya::Util::CpuThreadProfiler::kLogFrequencyNever, 1, 2, 4, 8, 16, 32};
      return times[option];
    }

    CpuProfiler()
      : _threadProfilerCount(0)
      , _creationTimePoint(CpuProfileClock::now())
    {
    }
    
    ~CpuProfiler()
    {
    }
    
    void Reset(); // Unsafe to call when any profiled threads are ticking
    
    static CpuProfiler& GetInstance();
    
    CpuThreadProfiler*  GetThreadProfiler(CpuThreadId threadId); // returns nullptr if not already created
    CpuThreadProfiler*  GetThreadProfilerByName(const char* threadName); // This is the threadName from LRYA_CPU_TICK() not the OS thread name
    
    // GetOrAddThreadProfiler: return same as GetThreadProfiler() if already exists, otherwise attempts to add and
    // return that (returns nullptr on failure to add)
    CpuThreadProfiler*  GetOrAddThreadProfiler(CpuThreadId threadId, const char* threadName, double maxTickTime_ms, uint32_t logFreq);
    
    void RemoveThreadProfiler(CpuThreadId threadId);
    
    // Helper static method
    static CpuThreadProfiler* GetOrAddCurrentThreadProfiler(const char* threadName, double maxTickTime_ms, uint32_t logFreq)
    {
      return GetInstance().GetOrAddThreadProfiler( GetCurrentThreadId(), threadName, maxTickTime_ms, logFreq );
    }
    
    static CpuThreadProfiler* GetCurrentThreadProfiler()
    {
      return GetInstance().GetThreadProfiler( GetCurrentThreadId() );
    }
    
    static void RemoveCurrentThreadProfiler()
    {
      return GetInstance().RemoveThreadProfiler( GetCurrentThreadId() );
    }
    
    uint32_t GetThreadProfilerCount() const { return _threadProfilerCount; }
    const CpuThreadProfiler* GetThreadProfilerByIndex(uint32_t index) const
    {
      return (index < _threadProfilerCount) ? &_threadProfilers[index] : nullptr;
    }
    CpuThreadProfiler* GetThreadProfilerByIndex(uint32_t index)
    {
      return (index < _threadProfilerCount) ? &_threadProfilers[index] : nullptr;
    }
    
  private:
    
    CpuThreadProfiler           _threadProfilers[kCpuProfilerMaxThreads];
    std::atomic<uint32_t>       _threadProfilerCount;
    CpuProfileClock::time_point _creationTimePoint;
    
    void CheckAndUpdateProfiler(CpuThreadProfiler& profiler, double maxTickTime_ms, uint32_t logFreq) const;
  };
  
    
  // ================================================================================
  // Helper Classes and Macros:
  
  
  // ================================================================================
  // ScopedCpuTick
    
  class ScopedCpuTick
  {
  public:
    
    explicit ScopedCpuTick(const char* tickName, float maxTickTime_ms, uint32_t logFreq, bool oneTimeUse = false)
      : _tickName(tickName)
      , _started(false)
      , _isOneTimeUse(oneTimeUse)
    {
      CpuThreadProfiler* profiler = CpuProfiler::GetOrAddCurrentThreadProfiler(tickName, maxTickTime_ms, logFreq);
      if (profiler)
      {
        profiler->StartTick();
        _started = true;
      }
      else
      {
        PRINT_NAMED_WARNING("ScopedCpuTick.FailedToAddProfiler", "Unable To Profile Thread '%s'", tickName);
      }
    }
    
    ~ScopedCpuTick()
    {
      if (_started)
      {
        CpuThreadProfiler* profiler = CpuProfiler::GetCurrentThreadProfiler();
        if (profiler)
        {
          profiler->EndTick();
          if (_isOneTimeUse)
          {
            profiler->SetHasStaleSettings();
          }
        }
        else
        {
          PRINT_NAMED_ERROR("ScopedCpuTick.LostProfiler", "Profiler for '%s' Removed During Tick?", _tickName);
        }
        _started = false;
      }
    }
    
    const char* GetName() const { return _tickName; }

  private:
    
    const char* _tickName;
    bool        _started;
    bool        _isOneTimeUse;
  };

  
  // ================================================================================
  // ScopedCpuProfile
  
  class ScopedCpuProfile
  {
  public:
    
    explicit ScopedCpuProfile(CpuProfileSampleShared& sharedData, bool isActive)
      : _sharedData(sharedData)
      , _active(isActive)
    {
      if (_active)
      {
        _startTime = CpuProfileClock::now();
      }
    }
    
    ~ScopedCpuProfile()
    {
      Stop();
    }
    
    void Stop()
    {
      if (_active)
      {
        CpuThreadProfiler* profiler = CpuProfiler::GetCurrentThreadProfiler();
        if (profiler)
        {
          CpuProfileClock::time_point endTime = CpuProfileClock::now();
          profiler->AddSample(_startTime, endTime, _sharedData);
        }
        else
        {
          // Don't warn before any ticks are running (e.g. on loading)
          if (CpuProfiler::GetInstance().GetThreadProfilerCount() > 0)
          {
          #if LRYA_CPU_PROFILER_WARN_ON_NO_PROFILER
            PRINT_NAMED_WARNING("ScopedCpuTick.NoThreadProfiler",
                                "No Thread Profiler for Thread '%s' sample '%s' - needs an LRYA_CPU_TICK",
                                GetCurrentThreadName().c_str(), _sharedData.GetName());
          #endif // LRYA_CPU_PROFILER_WARN_ON_NO_PROFILER
          }
        }
        _active = false;
      }
    }
    
  private:
    
    CpuProfileSampleShared&     _sharedData;
    CpuProfileClock::time_point _startTime;
    bool                        _active; // allows Stop() to be called manually, and conditional Scoped profilers to be used
  };
  
    
  } // end namespace Util
  } // end namespace Lrya


  // Macros for creating unique variable names using __LINE__ macro
  // Needs the multiple levels of indirection so that __LINE__ is expanded to the line number
  #define LRYA_CPU_UNIQUE_VAR_NAME3(varName, lineNum)   varName ## lineNum
  #define LRYA_CPU_UNIQUE_VAR_NAME2(varName, lineNum)   LRYA_CPU_UNIQUE_VAR_NAME3(varName, lineNum)
  #define LRYA_CPU_UNIQUE_VAR_NAME(varName)             LRYA_CPU_UNIQUE_VAR_NAME2(varName, __LINE__)


  // NOTE: Name must be static, we don't copy them!
  //       If we need dynamic strings we can add a LRYA_CPU_MAKE_DYNAMIC_STRING() function to allow a (semi?)permanent
  //       pool of names to be managed and later psudo-"garbage-collected" (cycle the list so the name lasts for e.g. 2 frames)

  #define LRYA_CPU_PROFILE(name)                \
    static Lrya::Util::CpuProfileSampleShared   LRYA_CPU_UNIQUE_VAR_NAME(sSharedData)(name); \
    Lrya::Util::ScopedCpuProfile                LRYA_CPU_UNIQUE_VAR_NAME(scopedCpuProfile)(LRYA_CPU_UNIQUE_VAR_NAME(sSharedData), true)

  // NOTE: LRYA_CPU_PROFILE_START + LRYA_CPU_PROFILE_STOP still create a scoped timer that will stop when it goes out
  //       of scope (same as LRYA_CPU_PROFILE), it just gives you a handle so you can also stop earlier if desired
  #define LRYA_CPU_PROFILE_START(varName, name) \
    static Lrya::Util::CpuProfileSampleShared   LRYA_CPU_UNIQUE_VAR_NAME(sSharedData)(name); \
    Lrya::Util::ScopedCpuProfile                varName(LRYA_CPU_UNIQUE_VAR_NAME(sSharedData), true)

  #define LRYA_CPU_PROFILE_STOP(varName)        \
    varName.Stop()

  #define LRYA_CPU_TICK(tickName, maxTickTime_ms, logFreq) \
    Lrya::Util::ScopedCpuTick                     LRYA_CPU_UNIQUE_VAR_NAME(scopedCpuTick)(tickName, maxTickTime_ms, logFreq)

  #define LRYA_CPU_TICK_ONE_TIME(tickName) \
    Lrya::Util::ScopedCpuTick                     LRYA_CPU_UNIQUE_VAR_NAME(scopedCpuTick)(tickName, 0, 0, true)

  #define LRYA_CPU_REMOVE_THIS_THREAD()         Lrya::Util::CpuProfiler::RemoveCurrentThreadProfiler()

#else  // LRYA_CPU_PROFILER_ENABLED


#define LRYA_CPU_PROFILE(name) 

#define LRYA_CPU_PROFILE_START(varName, name) 

#define LRYA_CPU_PROFILE_STOP(varName) 

#define LRYA_CPU_TICK(tickName, maxTickTime_ms, logFreq) 

#define LRYA_CPU_TICK_ONE_TIME(tickName) 

#define LRYA_CPU_REMOVE_THIS_THREAD() 

#endif // LRYA_CPU_PROFILER_ENABLED


#endif // __Util_CpuProfiler_CpuProfiler_H__
