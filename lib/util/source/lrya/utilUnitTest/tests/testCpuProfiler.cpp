/**
 * File: testCpuProfiler
 *
 * Author: Mark Wesley
 * Created: 06/23/16
 *
 * Description: Unit tests for CpuProfiler
 *
 * Copyright: Lrya, Inc. 2016
 *
 * --gtest_filter=CpuProfiler*
 **/


#include "util/console/consoleInterface.h"
#include "util/cpuProfiler/cpuProfiler.h"
#include "util/helpers/includeGTest.h"

#include <thread>

#if LRYA_PROFILING_ENABLED

namespace Lrya {
namespace Util {
  CONSOLE_VAR_EXTERN(bool, kProfilerLogSlowTicks);
}
}


namespace
{


const bool kLogProfiles = false;
  
  
void SubFuncQuick()
{
  LRYA_CPU_PROFILE("SubFuncQuick");
}

  
void SubFuncSleepShort()
{
  LRYA_CPU_PROFILE("SubFuncSleep50");
  usleep(50);
}


void SubFuncSleepLong()
{
  LRYA_CPU_PROFILE("SubFuncSleep100");
  usleep(100);
}

  
void SubFuncB(int num)
{
  LRYA_CPU_PROFILE("SubFuncB");
  for (int i=0; i<num;++i)
  {
    if (((num+i) % 2) == 0)
    {
      SubFuncSleepShort();
    }
    else
    {
      SubFuncSleepLong();
    }
  }
}
  

void SubFuncA()
{
  LRYA_CPU_PROFILE("SubFuncA");
  for (int i=0; i<3;++i)
  {
    SubFuncB(i);
  }
}


void FakeMainConfig(double maxTickTime = FLT_MAX, uint32_t logFrequency = Lrya::Util::CpuThreadProfiler::kLogFrequencyNever)
{
  LRYA_CPU_TICK("MainConfig", maxTickTime, logFrequency);
  LRYA_CPU_PROFILE("MainConfig");
  SubFuncA();
}

  
void FakeMainA()
{
  LRYA_CPU_TICK("MainThreadA", FLT_MAX, Lrya::Util::CpuThreadProfiler::kLogFrequencyNever);
  LRYA_CPU_PROFILE("FakeMainA");
  SubFuncA();
}
  
  
void FakeMainB()
{
  LRYA_CPU_TICK("MainThreadB", FLT_MAX, Lrya::Util::CpuThreadProfiler::kLogFrequencyNever);
  LRYA_CPU_PROFILE("FakeMainB");
  SubFuncA();
}


void FakeMainC()
{
  LRYA_CPU_TICK("MainThreadC", FLT_MAX, Lrya::Util::CpuThreadProfiler::kLogFrequencyNever);
  LRYA_CPU_PROFILE("FakeMainC");
  SubFuncA();
  {
    LRYA_CPU_PROFILE("LotsOfQuick");
    for (int i=0; i <100; ++i)
    {
      SubFuncQuick();
    }
  }
}


void FakeMainD()
{
  LRYA_CPU_TICK("MainThreadD", FLT_MAX, Lrya::Util::CpuThreadProfiler::kLogFrequencyNever);
  LRYA_CPU_PROFILE("FakeMainD");
  SubFuncA();
}

  
} // end anonymous namespace


const char* SafeGetSampleName(const Lrya::Util::CpuThreadProfile& profile, size_t index)
{
  if (index < profile.GetSampleCount())
  {
    return profile.GetSample(index).GetName();
  }
  else
  {
    return "";
  }
}


TEST(CpuProfiler, OneTickProfile)
{
  Lrya::Util::CpuProfiler& cpuProfiler = Lrya::Util::CpuProfiler::GetInstance();
  cpuProfiler.Reset();
  // Don't throw any samples out based on time (otherwise results depend on machine speed)
  Lrya::Util::CpuThreadProfiler::SetMinSampleDuration_ms(-0.01);
  
  FakeMainA();
  
  Lrya::Util::CpuThreadProfiler* threadProfMain = cpuProfiler.GetThreadProfiler( Lrya::Util::GetCurrentThreadId() );
  
  ASSERT_NE(threadProfMain, nullptr);
  
  EXPECT_EQ( cpuProfiler.GetThreadProfilerByName("maiNtHReadA"), threadProfMain ); // Case insensitive lookup
  
  const Lrya::Util::CpuThreadProfile& profile = threadProfMain->GetCurrentProfile();
  
  EXPECT_EQ(profile.GetTickNum(), 1);
  EXPECT_EQ(profile.GetSampleCount(), 8);
  
  threadProfMain->SortProfile();
  EXPECT_EQ(profile.GetSampleCount(), 8);
  // samples are sorted by start time, earliest first
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainA"), 0 );
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
  EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
}


TEST(CpuProfiler, OneSlowTickProfile)
{
  Lrya::Util::kProfilerLogSlowTicks = true;
  Lrya::Util::CpuProfiler& cpuProfiler = Lrya::Util::CpuProfiler::GetInstance();
  cpuProfiler.Reset();
  
  FakeMainConfig(0.0f); // It's guarenteed to take longer than 0ms, so this should trigger a slow log
  
  Lrya::Util::CpuThreadProfiler* threadProfMain = cpuProfiler.GetThreadProfiler( Lrya::Util::GetCurrentThreadId() );
  
  ASSERT_NE(threadProfMain, nullptr);
}


TEST(CpuProfiler, MultiTicksFreqProfile)
{
  Lrya::Util::CpuProfiler& cpuProfiler = Lrya::Util::CpuProfiler::GetInstance();
  cpuProfiler.Reset();
  // Don't throw any samples out based on time (otherwise results depend on machine speed)
  Lrya::Util::CpuThreadProfiler::SetMinSampleDuration_ms(-0.01);
  
  for (int i=0; i < 4; ++i)
  {
    FakeMainConfig(FLT_MAX, 3);
    
    Lrya::Util::CpuThreadProfiler* threadProfMain = cpuProfiler.GetThreadProfiler( Lrya::Util::GetCurrentThreadId() );
    ASSERT_NE(threadProfMain, nullptr);
    const Lrya::Util::CpuThreadProfile& profile = threadProfMain->GetCurrentProfile();
    EXPECT_EQ(profile.GetTickNum(), i+1);
    EXPECT_EQ(profile.GetSampleCount(), 8);
  }
}


TEST(CpuProfiler, MultiThreaded)
{
  Lrya::Util::CpuProfiler& cpuProfiler = Lrya::Util::CpuProfiler::GetInstance();
  cpuProfiler.Reset();
  // Don't throw any samples out based on time (otherwise results depend on machine speed)
  Lrya::Util::CpuThreadProfiler::SetMinSampleDuration_ms(-0.01);
  
  std::thread threadA = std::thread(FakeMainA);
  std::thread threadB = std::thread(FakeMainB);
  std::thread threadC = std::thread(FakeMainC);
  std::thread threadD = std::thread(FakeMainD);
  
  // Have to ask for thread ids immediately (before the thread ends and clears them)
  std::thread::id threadA_id = threadA.get_id();
  std::thread::id threadB_id = threadB.get_id();
  std::thread::id threadC_id = threadC.get_id();
  std::thread::id threadD_id = threadD.get_id();
  
  // Wait for threads to complete
  threadA.join();
  threadB.join();
  threadC.join();
  threadD.join();
  
  Lrya::Util::CpuThreadProfiler* threadProfA = cpuProfiler.GetThreadProfiler(threadA_id);
  Lrya::Util::CpuThreadProfiler* threadProfB = cpuProfiler.GetThreadProfiler(threadB_id);
  Lrya::Util::CpuThreadProfiler* threadProfC = cpuProfiler.GetThreadProfiler(threadC_id);
  Lrya::Util::CpuThreadProfiler* threadProfD = cpuProfiler.GetThreadProfiler(threadD_id);
  
  ASSERT_NE(threadProfA, nullptr);
  ASSERT_NE(threadProfB, nullptr);
  ASSERT_NE(threadProfC, nullptr);
  ASSERT_NE(threadProfD, nullptr);
  
  {
    threadProfA->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfA->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 8);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
  }
  
  {
    threadProfB->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfB->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 8);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
  }
  
  {
    threadProfC->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfC->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 109);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainC"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 8), "LotsOfQuick"), 0 );
    for (int i=0; i < 100; ++i)
    {
      EXPECT_EQ( strcmp(SafeGetSampleName(profile, 9+i), "SubFuncQuick"), 0 );
    }
  }
  
  {
    threadProfD->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfD->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 8);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainD"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
  }
  
  if (kLogProfiles)
  {
    threadProfA->SortAndLogProfile();
    threadProfB->SortAndLogProfile();
    threadProfC->SortAndLogProfile();
    threadProfD->SortAndLogProfile();
  }
}


TEST(CpuProfiler, MultiThreadedWithMain)
{
  Lrya::Util::CpuProfiler& cpuProfiler = Lrya::Util::CpuProfiler::GetInstance();
  cpuProfiler.Reset();
  // Don't throw any samples out based on time (otherwise results depend on machine speed)
  Lrya::Util::CpuThreadProfiler::SetMinSampleDuration_ms(-0.01);
  
  std::thread threadB = std::thread(FakeMainB);
  std::thread threadC = std::thread(FakeMainC);
  std::thread threadD = std::thread(FakeMainD);
  
  // Have to ask for thread ids immediately (before the thread ends and clears them)
  std::thread::id threadB_id = threadB.get_id();
  std::thread::id threadC_id = threadC.get_id();
  std::thread::id threadD_id = threadD.get_id();
  
  // Wait for threads to complete
  FakeMainA();
  threadB.join();
  threadC.join();
  threadD.join();
  
  Lrya::Util::CpuThreadProfiler* threadProfMain = cpuProfiler.GetThreadProfiler(Lrya::Util::GetCurrentThreadId());
  Lrya::Util::CpuThreadProfiler* threadProfB = cpuProfiler.GetThreadProfiler(threadB_id);
  Lrya::Util::CpuThreadProfiler* threadProfC = cpuProfiler.GetThreadProfiler(threadC_id);
  Lrya::Util::CpuThreadProfiler* threadProfD = cpuProfiler.GetThreadProfiler(threadD_id);
  
  ASSERT_NE(threadProfMain, nullptr);
  ASSERT_NE(threadProfB, nullptr);
  ASSERT_NE(threadProfC, nullptr);
  ASSERT_NE(threadProfD, nullptr);
  
  EXPECT_EQ( strcmp(threadProfMain->GetThreadName(), "MainThreadA"), 0 );
  EXPECT_EQ( strcmp(threadProfB->GetThreadName(), "MainThreadB"), 0 );
  EXPECT_EQ( strcmp(threadProfC->GetThreadName(), "MainThreadC"), 0 );
  EXPECT_EQ( strcmp(threadProfD->GetThreadName(), "MainThreadD"), 0 );
  
  {
    threadProfMain->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfMain->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 8);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
  }
  
  {
    threadProfB->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfB->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 8);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
  }
  
  {
    threadProfC->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfC->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 109);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainC"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 8), "LotsOfQuick"), 0 );
    for (int i=0; i < 100; ++i)
    {
      EXPECT_EQ( strcmp(SafeGetSampleName(profile, 9+i), "SubFuncQuick"), 0 );
    }
  }
  
  {
    threadProfD->SortProfile();
    const Lrya::Util::CpuThreadProfile& profile = threadProfD->GetCurrentProfile();
    
    EXPECT_EQ(profile.GetSampleCount(), 8);
    // samples are sorted by start time, earliest first
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 0), "FakeMainD"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 1), "SubFuncA"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 2), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 3), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 4), "SubFuncSleep100"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 5), "SubFuncB"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 6), "SubFuncSleep50"), 0 );
    EXPECT_EQ( strcmp(SafeGetSampleName(profile, 7), "SubFuncSleep100"), 0 );
  }
  
  if (kLogProfiles)
  {
    threadProfMain->SortAndLogProfile();
    threadProfB->SortAndLogProfile();
    threadProfC->SortAndLogProfile();
    threadProfD->SortAndLogProfile();
  }
}

#endif // LRYA_PROFILING_ENABLED
