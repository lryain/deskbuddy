/**
 * File: pathMotionProfileHelpers.h
 *
 * Author: Brad Neuman
 * Created: 2016-02-22
 *
 * Description: Helpers for motion profile clad struct
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/

#ifndef __Cozmo_Basestation_PathMotionProfileHelpers_H__
#define __Cozmo_Basestation_PathMotionProfileHelpers_H__

#include "json/json-forwards.h"

namespace Lrya {
namespace Vector {

struct PathMotionProfile;

void LoadPathMotionProfileFromJson(PathMotionProfile& profile, const Json::Value& config);

}
}



#endif
