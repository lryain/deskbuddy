/**
 * File: resetAction.h
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

#ifndef __Util_QuestEngine_ResetAction_H__
#define __Util_QuestEngine_ResetAction_H__

#include "util/questEngine/abstractAction.h"
#include <string>

namespace Lrya {
namespace Util {
namespace QuestEngine {
      
class ResetAction : public AbstractAction
{
public:
        
  explicit ResetAction(const std::string& eventName);
  ~ResetAction() {};
        
  void PerformAction(QuestEngine& questEngine) override;
        
private:
        
  std::string _eventName;
        
};
      
} // namespace QuestEngine
} // namespace Util
} // namsepace Lrya

#endif // __Util_QuestEngine_ResetAction_H__
