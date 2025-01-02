/**
 * File: navMapFactory.h
 *
 * Author: Raul
 * Date:   03/11/2016
 *
 * Description: Factory to hide the specific type of memory map used by Cozmo.
 *
 * Copyright: Lrya, Inc. 2016
 **/

#ifndef LRYA_COZMO_NAV_MAP_FACTORY_H
#define LRYA_COZMO_NAV_MAP_FACTORY_H

#include "iNavMap.h"

class Robot;

namespace Lrya {
namespace Vector {
namespace NavMapFactory {

// creates the proper nav memory map (through default, config, etc)
INavMap* CreateMemoryMap();
  
} // namespace
} // namespace
} // namespace

#endif // LRYA_COZMO_NAV_MEMORY_MAP_FACTORY_H
