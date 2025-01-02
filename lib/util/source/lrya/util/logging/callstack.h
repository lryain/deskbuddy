/**
 * File: callstack
 *
 * Author: raul
 * Created: 06/18/2016
 *
 * Description: Functions related to debugging the callstack.
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/

#ifndef __Util_Logging_Callstack_H_
#define __Util_Logging_Callstack_H_

#include <string>

namespace Lrya{
namespace Util {

// dumps current callstack to log
void sDumpCallstack(const std::string& name);

} // namespace Util
} // namespace Lrya

#endif //
