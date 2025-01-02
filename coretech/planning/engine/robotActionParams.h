#ifndef _LRYACORETECH_PLANNING_ROBOT_ACTION_PARAMS_H_
#define _LRYACORETECH_PLANNING_ROBOT_ACTION_PARAMS_H_

#include "json/json-forwards.h"

namespace Lrya
{

namespace Util {
class JsonWriter;
}

namespace Planning
{

// This struct holds values that the motions will use to calculate costs, based on the turning radius and
// velocity of the robot
struct RobotActionParams
{
  RobotActionParams();

  void Reset();

  // returns true on success
  bool Import(const Json::Value& config);
  void Dump(Util::JsonWriter& writer) const;

  double halfWheelBase_mm;
  double maxVelocity_mmps;
  double maxReverseVelocity_mmps;
  double oneOverMaxVelocity;
};


}
}

#endif
