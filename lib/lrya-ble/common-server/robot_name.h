/**
 * File: robot_name.h
 *
 * Author: seichert
 * Created: 4/23/2018
 *
 * Description: Get/Set the Robot's name
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/

#pragma once

#include <string>

namespace Lrya {

const std::string kRobotNamePropertyKey("lrya.robot.name");
const std::string kProductNamePropertyKey("ro.lrya.product.name");
const std::string kDefaultProductName("Vector");

std::string GetRobotName();

} // namespace Lrya
