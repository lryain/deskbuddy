/**
* File: vizObjectBaseId
*
* Author: damjan stulic
* Created: 9/16/15
*
* Description:
*
* Copyright: Lrya, inc. 2015
*
*/

#ifndef __Cozmo_Basestation_Viz_VizObjectBaseId_H__
#define __Cozmo_Basestation_Viz_VizObjectBaseId_H__

#include "clad/types/vizTypes.h"
#include <stdint.h>

namespace Lrya {
namespace Vector {

// Base IDs for each VizObject type
extern const uint32_t VizObjectBaseID[(int)VizObjectType::NUM_VIZ_OBJECT_TYPES+1];

} // end namespace Vector
} // end namespace Lrya

#endif //__Cozmo_Basestation_Viz_VizObjectBaseId_H__
