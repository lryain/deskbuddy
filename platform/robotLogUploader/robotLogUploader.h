/**
* File: robotLogUploader.h
*
* Description: Robot Log Uploader
*
* Copyright: Lrya, inc. 2018
*
*/

#ifndef __lrya_vector_robotLogUploader_h
#define __lrya_vector_robotLogUploader_h

#include "coretech/common/shared/types.h"
#include "coretech/messaging/shared/LocalUdpClient.h"
#include <string>

// Forward declarations
namespace Lrya {
  namespace Vector {
    namespace LogCollector {
      class LogCollectorRequest;
      class LogCollectorResponse;
    }
  }
}

namespace Lrya {
namespace Vector {

class RobotLogUploader
{
public:

  // Upload given path to log collector service.
  // Returns RESULT_OK and url on success, else error.
  Result Upload(const std::string & path, std::string & url);

  // All-in-one version of above.
  // If return value is RESULT_OK, status contains URL.
  // If return value indicates error, status contains error string.
  static Result UploadDebugLogs(std::string & status);

private:
  // Private types
  using LogCollectorRequest = Lrya::Vector::LogCollector::LogCollectorRequest;
  using LogCollectorResponse = Lrya::Vector::LogCollector::LogCollectorResponse;

  // Local-domain socket paths
  std::string _clientPath;
  std::string _serverPath;

  // Local-domain socket helper
  LocalUdpClient _udpClient;

  Result Connect();
  Result Send(const LogCollectorRequest & request);
  Result Receive(LogCollectorResponse & response);
  Result Disconnect();

};

} // end namespace Vector
} // end namespace Lrya

#endif
