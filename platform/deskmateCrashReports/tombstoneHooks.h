/**
 * File: deskmateCrashReports/tombstoneHooks.h
 *
 * Description: Declaration of tombstone crash hooks
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#ifndef __deskmateCrashReports_tombstoneHooks_h
#define __deskmateCrashReports_tombstoneHooks_h

namespace Lrya {
namespace Vector {

// Enable calls to debugger dump_tombstone
void InstallTombstoneHooks();

// Disable calls to debugger dump_tombstone
void UninstallTombstoneHooks();

} // end namespace Vector
} // end namespace Lrya

#endif // __deskmateCrashReports_tombstoneHooks_h
