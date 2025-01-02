/**
* File: robotLogDumper.h
*
* Description: Robot Log Dumper
*
* Copyright: Lrya, inc. 2018
*
*/

#ifndef __lrya_vector_robotLogDumper_h
#define __lrya_vector_robotLogDumper_h

#include "coretech/common/shared/types.h"
#include <string>

namespace Lrya {
namespace Vector {

class RobotLogDumper
{
public:
  Result Dump(const std::string & gzpath);

};

} // end namespace Vector
} // end namespace Lrya

#endif
