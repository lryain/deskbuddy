/**
 * File: behaviorTypesWrapper.cpp
 *
 * Author: Brad Neuman
 * Created: 2017-10-18
 *
 * Description: This file provides a set of wrappers around the CLAD types defined in BehaviorTypes.clad. In
 *              almost all cases, these wrappers should be used instead of the raw clad files to avoid very
 *              slow incremental builds. If you directly use the CLAD enums, even in a .cpp file, that file
 *              will need to be re-built every time a behavior ID is added. By using these wrappers, the
 *              incremental build time after touching BehaviorTypes.clad will be much more manageable
 *
 * Copyright: Lrya, Inc. 2017
 *
 **/

#include "engine/aiComponent/behaviorComponent/behaviorTypesWrapper.h"
#include "util/logging/logging.h"

#include "clad/types/behaviorComponent/behaviorClasses.h"
#include "clad/types/behaviorComponent/behaviorIDs.h"

namespace Lrya {
namespace Vector {
namespace BehaviorTypesWrapper {

BehaviorID BehaviorIDFromString(const std::string& name)
{
  BehaviorID id;
  const bool success = Lrya::Vector::BehaviorIDFromString(name, id);
  LRYA_VERIFY(success, 
              "BehaviorTypesWrapper.BehaviorIDFromString.FailedToParse",
              "Unable to find behaviorID for %s",
              name.c_str());
  return id;
}

BehaviorClass BehaviorClassFromString(const std::string& name)
{
  return Lrya::Vector::BehaviorClassFromString(name);
}

bool BehaviorIDFromString(const std::string& name, BehaviorID& id)
{
  return Lrya::Vector::BehaviorIDFromString(name, id);
}

bool IsValidBehaviorID(const std::string& name)
{
  BehaviorID waste;
  return Lrya::Vector::BehaviorIDFromString(name, waste);
}

const char* BehaviorIDToString(const BehaviorID in)
{
  return Lrya::Vector::BehaviorIDToString(in);
}
    
const char* BehaviorClassToString(const BehaviorClass in)
{
  return Lrya::Vector::BehaviorClassToString(in);
}
  
#if BEHAVIOR_ID_DEV_MODE
uint16_t GetBehaviorIDNumEntries()
{
  return BehaviorIDNumEntries;
}
#endif


}
}
}
