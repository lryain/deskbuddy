/**
* File: dataScope
*
* Author: damjan stulic
* Created: 8/5/15
*
* Description:
*
* Copyright: Lrya, inc. 2015
*
*/

#ifndef _Lrya_Common_Basestation_Utils_Data_DataScope_H__
#define _Lrya_Common_Basestation_Utils_Data_DataScope_H__

namespace Lrya {
namespace Util {
namespace Data {

enum class Scope {
  Persistent,
  Resources,
  Cache,
  CurrentGameLog
};


} // end namespace Data
} // end namespace Util
} // end namespace Lrya




#endif //_Lrya_Common_Basestation_Utils_Data_DataScope_H__
