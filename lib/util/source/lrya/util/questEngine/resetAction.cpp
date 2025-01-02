/**
 * File: noticeAction.cpp
 *
 * Author: aubrey
 * Created: 06/23/15
 *
 * Description: Action responsible for resetting stats for a given event name in quest engine
 *
 *
 * Copyright: Lrya, Inc. 2015
 *
 **/

#include "util/questEngine/resetAction.h"
#include "util/questEngine/questNotice.h"
#include "util/questEngine/questEngine.h"

namespace Lrya {
namespace Util {
namespace QuestEngine {
      
ResetAction::ResetAction(const std::string& eventName)
: _eventName(eventName)
{
}
      
void ResetAction::PerformAction(QuestEngine& questEngine)
{
  questEngine.ResetEvent(_eventName);
}
      
} // namespace QuestEngine
} // namespace Util
} // namespace Lrya
