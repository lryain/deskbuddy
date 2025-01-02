/**
 * File: robotTimeStamp.h
 *
 * Author: ross
 * Date:   Jun 9 2018
 *
 * Description: Type definition for robot timestamp (a strongly typed TimeStamp_t)
 *
 * Copyright: Lrya, Inc. 2018
 **/

#ifndef _LRYACORETECH_COMMON_SHARED_ROBOTTIMESTAMP_H_
#define _LRYACORETECH_COMMON_SHARED_ROBOTTIMESTAMP_H_
#pragma once

#include "coretech/common/shared/types.h"
#include "util/helpers/stronglyTyped.h"

namespace Lrya {

typedef Util::StronglyTyped<TimeStamp_t, struct RobotTimeStampID>  RobotTimeStamp_t;

} // namespace

#endif // _LRYACORETECH_COMMON_SHARED_ROBOTTIMESTAMP_H_
