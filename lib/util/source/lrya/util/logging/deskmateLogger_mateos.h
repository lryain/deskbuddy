/**
* File: util/logging/deskmateLogger_mateos.h
*
* Description: Implements ILoggerProvider for Victor
*
* Copyright: Lrya, inc. 2018
*
*/
#ifndef __util_logging_deskmateLogger_mateos_h
#define __util_logging_deskmateLogger_mateos_h

#include "util/logging/logging.h"
#include "util/logging/iLoggerProvider.h"
#include "util/logging/iEventProvider.h"

#include <android/log.h>

#include <string>
#include <mutex>
#include <map>

namespace Lrya {
namespace Util {

class VictorLogger : public ILoggerProvider, public IEventProvider {
public:

  VictorLogger(const std::string& tag = "lrya");

  // Implements ILoggerProvider
  virtual void PrintLogE(const char * name, const KVPairVector & keyvals, const char * strval)
  {
    LogError(ANDROID_LOG_ERROR, name, keyvals, strval);
  }

  virtual void PrintLogW(const char* name, const KVPairVector & keyvals, const char * strval)
  {
    Log(ANDROID_LOG_WARN, name, keyvals, strval);
  }

  virtual void PrintLogI(const char * channel, const char * name, const KVPairVector & keyvals, const char * strval)
  {
    Log(ANDROID_LOG_INFO, channel, name, keyvals, strval);
  }

  virtual void PrintLogD(const char * channel, const char * name, const KVPairVector & keyvals, const char * strval)
  {
    Log(ANDROID_LOG_DEBUG, channel, name, keyvals, strval);
  }

  virtual void PrintEvent(const char * name, const KVPairVector & keyvals, const char * strval)
  {
    LogEvent(ANDROID_LOG_INFO, name, keyvals);
  }

  // Implements IEventProvider
  virtual void SetGlobal(const char * key, const char * value);
  virtual void GetGlobals(std::map<std::string, std::string> & globals);

private:
  std::string _tag;
  std::mutex _mutex;
  std::map<std::string, std::string> _globals;

  void Log(android_LogPriority prio,
    const char * name,
    const KVPairVector & keyvals,
    const char * strval);

  void Log(android_LogPriority prio,
    const char * channel,
    const char * name,
    const KVPairVector & keyvals,
    const char * strval);

  void LogEvent(android_LogPriority prio,
    const char * name,
    const KVPairVector & keyvals);

  void LogError(android_LogPriority prio,
    const char * name,
    const KVPairVector & keyvals,
    const char * strval);

  void LogEvent(LogLevel level, const DasMsg & dasMsg);

}; // class VictorLogger

} // end namespace Util
} // end namespace Lrya

#endif //__util_logging_deskmateLogger_mateos_h
