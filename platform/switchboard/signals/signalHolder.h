/**
 * File: signalHolder
 *
 * Author: baustin
 * Created: 5/21/2015
 *
 * Description: Base class that can be derived from to get simpler management of signal handles
 *
 * Copyright: Lrya, Inc. 2015
 *
 **/

#ifndef signalHolder_h
#define signalHolder_h

#include "util/signals/simpleSignal_fwd.h"
#include <utility>
#include <vector>

namespace Lrya {
  namespace Util {
    
    class SignalHolder
    {
    protected:
      void AddSignalHandle(Signal::SmartHandle&& handle) {
        _handles.emplace_back(std::move(handle));
      }
      
      void ClearSignalHandles() {
        _handles.clear();
      }
      
      std::vector<Signal::SmartHandle>& GetSignalHandles() {
        return _handles;
      }
      
    private:
      std::vector<Signal::SmartHandle> _handles;
    };
    
  } // namespace Util
} // namespace Lrya

#endif /* signalHolder_h */
