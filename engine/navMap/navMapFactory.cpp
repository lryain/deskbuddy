/**
 * File: navMapFactory.cpp
 *
 * Author: Raul
 * Date:   03/11/2016
 *
 * Description: Factory to hide the specific type of memory map used by Cozmo.
 *
 * Copyright: Lrya, Inc. 2016
 **/

 
#include "memoryMap/memoryMap.h"

namespace Lrya {
namespace Vector {
namespace NavMapFactory {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
INavMap* CreateMemoryMap()
{
  return new MemoryMap();
}

} // namespace
} // namespace
} // namespace
