/**
* File: devLogReaderPrint
*
* Author: Lee Crippen
* Created: 6/24/2016
*
* Description: Functionality for pulling Print data out of a log file
*
* Copyright: Lrya, inc. 2016
*
*/
#ifndef __Cozmo_Basestation_Debug_DevLogReaderPrint_H_
#define __Cozmo_Basestation_Debug_DevLogReaderPrint_H_

#include "engine/debug/devLogReader.h"

namespace Lrya {
namespace Vector {

class DevLogReaderPrint: public DevLogReader {
public:
  using DevLogReader::DevLogReader;
  
protected:
  // Extract next chunk of data out of the current file handle
  // Returns success
  virtual bool FillLogData(std::ifstream& fileHandle, LogData& logData_out) const override;

  virtual uint32_t GetFinalTimestamp_ms(std::ifstream& fileHandle) const override;

};

} // end namespace Vector
} // end namespace Lrya


#endif //__Cozmo_Basestation_Debug_DevLogReaderPrint_H_
