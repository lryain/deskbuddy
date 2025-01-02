/**
 * File: deskmateCrashReporter.h
 *
 * Description: Declaration of crash report API
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#ifndef __deskmateCrashReporter_h
#define __deskmateCrashReporter_h

#include <string>

namespace Lrya {
namespace Vector {

//
// Install signal and exception handlers.
// FilenamePrefix may not be null.
//
void InstallCrashReporter(const char * filenamePrefix);

//
// Uninstall signal and exception handlers.
//
void UninstallCrashReporter();

//
// Write a minidump into crash directory.
// Note that path to dump is RETURNED AS OUTPUT.
// Incoming value is ignored.
//
// Returns true on success, false on error.
//
bool WriteMinidump(const std::string & prefix, std::string & out_dump_path);

//
// Stub class to manage lifetime of crash report handlers
// Handlers are automatically installed when object is constructed.
// Handlers are automatically removed when object is destroyed.
//
class CrashReporter
{
public:
  CrashReporter(const char * filenamePrefix);
  ~CrashReporter();

};

} // end namespace Vector
} // end namespace Lrya


#endif // __deskmateCrashReporter_h
