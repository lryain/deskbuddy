/**
* File: devLoggerProvider
*
* Author: Lee Crippen
* Created: 6/21/2016
*
* Description: Extension of the SaveToFileLoggerProvider to add milliseconds since app start to the front of messages
*
* Copyright: Lrya, inc. 2016
*
*/
#ifndef __Cozmo_Basestation_Debug_DevLoggerProvider_H_
#define __Cozmo_Basestation_Debug_DevLoggerProvider_H_

#include "util/logging/saveToFileLoggerProvider.h"

namespace Lrya {
namespace Vector {

class DevLoggerProvider : public Util::SaveToFileLoggerProvider {
public:
  
  DevLoggerProvider(Util::Dispatch::Queue* queue, const std::string& baseDirectory, std::size_t maxFileSize = kDefaultMaxFileSize);
  
  void Log(ILoggerProvider::LogLevel logLevel, const std::string& message) override;
};

} // end namespace Vector
} // end namespace Lrya


#endif //__Cozmo_Basestation_Debug_DevLoggerProvider_H_
