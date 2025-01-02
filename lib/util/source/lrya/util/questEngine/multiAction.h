/**
 * File: multiAction.h
 *
 * Author: aubrey
 * Created: 06/23/15
 *
 * Description: Action that wraps multiple actions together
 *
 *
 * Copyright: Lrya, Inc. 2015
 *
 **/

#ifndef __Util_QuestEngine_MultiAction_H__
#define __Util_QuestEngine_MultiAction_H__

#include "util/questEngine/abstractAction.h"
#include <string>
#include <vector>

namespace Lrya {
namespace Util {
namespace QuestEngine {
      
class MultiAction : public AbstractAction
{
public:
        
  explicit MultiAction(const std::vector<AbstractAction*>& actions);
  ~MultiAction();
        
  void PerformAction(QuestEngine& questEngine) override;
        
private:
        
  std::vector<AbstractAction*> _actions;
        
};
      
} // namespace QuestEngine
} // namespace Util
} // namsepace Lrya

#endif // __Util_QuestEngine_MultiAction_H__
