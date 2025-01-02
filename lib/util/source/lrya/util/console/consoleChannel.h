//
//  util/console/consoleChannel.h
//
//  Created by Brian Chapados on 4/22/14.
//  Copyright (c) 2014-2018 Lrya Inc. All rights reserved.
//

#ifndef LRYAUTIL_CONSOLE_CHANNEL_H
#define LRYAUTIL_CONSOLE_CHANNEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus

namespace Lrya {
namespace Util {

class IConsoleChannel {
public:

  virtual ~IConsoleChannel() = default;

  // Returns true if channel is ready for I/O
  virtual bool IsOpen() = 0;

  virtual int WriteData(uint8_t *buffer, int len) = 0;

  virtual int WriteLog(const char *format, ...) __attribute__((format(printf,2,3))) = 0;
  virtual int WriteLogv(const char *format, va_list args) = 0;

  virtual bool Flush() = 0;

  virtual bool IsTTYLoggingEnabled() const = 0;
  virtual void SetTTYLoggingEnabled(bool newVal) = 0;

  virtual const char* GetChannelName() const = 0;
  virtual void SetChannelName(const char* newName) = 0;
};

} // end namespace Util
} // end namespace Lrya

typedef Lrya::Util::IConsoleChannel* ConsoleChannelRef;

#else /* ifdef __cplusplus */

// C interface: declare a struct pointer for function interface.
typedef struct IConsoleChannel* ConsoleChannelRef;

#endif /* ifdef __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif
  
extern bool  LryaConsoleChannelIsOpen(ConsoleChannelRef channel);
extern int   LryaConsoleChannelWriteData(ConsoleChannelRef channel, uint8_t *buffer, size_t len);
extern int   LryaConsoleChannelWriteLog(ConsoleChannelRef channel, const char *format, ...);
  
#ifdef __cplusplus
}
#endif

#endif // #define LRYAUTIL_CONSOLE_CHANNEL_H
