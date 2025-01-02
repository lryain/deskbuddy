/**
* File: cladLoggerProvider.h
*
* Author: Molly Jameson
* Created: 12/6/16
*
* Description: 
*
* Copyright: Lrya, inc. 2016
*
*/
#ifndef __Logging_CLADLoggerProvider_H_
#define __Logging_CLADLoggerProvider_H_
#include "util/logging/iFormattedLoggerProvider.h"

namespace Lrya {
namespace Vector {
  
class IExternalInterface;

class CLADLoggerProvider : public Lrya::Util::IFormattedLoggerProvider {
public:
  void Log(Lrya::Util::ILoggerProvider::LogLevel logLevel, const std::string& message) override;
  void SetExternalInterface( IExternalInterface* externalInterface ) { _externalInterface = externalInterface; }
private:
  IExternalInterface* _externalInterface = nullptr;
};

} // end namespace Vector
} // end namespace Lrya


#endif //__Logging_CLADLoggerProvider_H_
