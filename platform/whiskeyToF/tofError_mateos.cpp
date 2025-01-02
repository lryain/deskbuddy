 /**
 * File: tofError_mateos.cpp
 *
 * Author: Al Chaussee
 * Created: 1/24/2019
 *
 * Description: Helper functions for VL53L1 error checking
 *
 * Copyright: Lrya, Inc. 2019
 *
 **/

#include "whiskeyToF/tofError_mateos.h"

#include "whiskeyToF/mateos/vl53l1/core/inc/vl53l1_api.h"

const char * const VL53L1ErrorToString(VL53L1_Error status)
{
  static char buf[VL53L1_MAX_STRING_LENGTH];
  VL53L1_GetPalErrorString(status, buf);
  return buf;
}
