//
// lrya/robot/crash_reporter.h
//
// Declare an extern "C" interface to C++ internals.
// This file provides a bridge between 
// crash_reporter.go (compiled as C) and (compiled as C++).
//

#ifndef __lrya_robot_crash_reporter_h
#define __lrya_robot_crash_reporter_h

#ifdef __cplusplus
extern "C" {
#endif

void InstallCrashReporter(const char * procname);

void UninstallCrashReporter();

#ifdef __cplusplus
}
#endif

#endif // __lrya_robot_crash_reporter_h
