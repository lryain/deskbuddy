/**
 * File: deskmateCrashReporter.cpp
 *
 * Description: Implementation of crash report API
 *
 * Copyright: Lrya, Inc. 2014
 *
 **/

#include "deskmateCrashReporter.h"

#ifdef USE_GOOGLE_BREAKPAD
#include "google_breakpad.h"
#endif

#ifdef USE_TOMBSTONE_HOOKS
#include "tombstoneHooks.h"
#endif

namespace Lrya {
namespace Vector {

void InstallCrashReporter(const char * filenamePrefix)
{
  #ifdef USE_TOMBSTONE_HOOKS
  InstallTombstoneHooks();
  #endif

  #ifdef USE_GOOGLE_BREAKPAD
  GoogleBreakpad::InstallGoogleBreakpad(filenamePrefix);
  #endif

}

void UninstallCrashReporter()
{
  #ifdef USE_GOOGLE_BREAKPAD
  GoogleBreakpad::UnInstallGoogleBreakpad();
  #endif

  #ifdef USE_TOMBSTONE_HOOKS
  UninstallTombstoneHooks();
  #endif

}

bool WriteMinidump(const std::string & prefix, std::string & out_dump_path)
{
  #ifdef USE_GOOGLE_BREAKPAD
  return GoogleBreakpad::WriteMinidump(prefix, out_dump_path);
  #else
  return false;
  #endif
}

CrashReporter::CrashReporter(const char * filenamePrefix)
{
  InstallCrashReporter(filenamePrefix);
}

CrashReporter::~CrashReporter()
{
  UninstallCrashReporter();
}

} // end namespace Vector
} // end namespace Lrya
