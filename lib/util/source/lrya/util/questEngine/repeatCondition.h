/**
 * File: repeatCondition.h
 *
 * Author: aubrey
 * Created: 07/07/15
 *
 * Description: Condition based on time since rule last triggered
 *
 *
 * Copyright: Lrya, Inc. 2015
 *
 **/

#ifndef __Util_QuestEngine_RepeatCondition_H__
#define __Util_QuestEngine_RepeatCondition_H__

#include "util/questEngine/abstractCondition.h"
#include <stdint.h>
#include <ctime>
#include <string>

namespace Lrya {
namespace Util {
namespace QuestEngine {
      
      
class RepeatCondition : public AbstractCondition
{
public:
       
  explicit RepeatCondition(const std::string& ruleId, const double secondsSinceLast);
  ~RepeatCondition() {};
        
  bool IsSatisfied(QuestEngine& questEngine, std::tm& eventTime) const override;
        
private:
  
  std::string _ruleId;
  
  double _secondsSinceLast;
  
};
     
} // namespace QuestEngine
} // namespace Util
} // namespace Lrya

#endif // __Util_QuestEngine_RepeatCondition_H__
