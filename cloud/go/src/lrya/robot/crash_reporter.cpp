//
// lrya/robot/crash_reporter.cpp
//
// Wrapper methods so C++ library functions can be called from go.
// Calls are stubbed out unless LRYA_PLATFORM_MATEOS is defined.
//

#include "crash_reporter.h"

#ifdef LRYA_PLATFORM_MATEOS
#include "platform/deskmateCrashReports/deskmateCrashReporter.h"
#endif

extern "C"
{

void InstallCrashReporter(const char* proctag)
{
#ifdef LRYA_PLATFORM_MATEOS
  Lrya::Vector::InstallCrashReporter(proctag);
#endif
}

void UninstallCrashReporter()
{
#ifdef LRYA_PLATFORM_MATEOS
  Lrya::Vector::UninstallCrashReporter();
#endif
}

} // end extern "C"
