#include "coretech/common/engine/matlabInterface.h"

#if defined(LRYACORETECH_USE_MATLAB) && LRYACORETECH_USE_MATLAB

namespace Lrya {

  Matlab::Matlab(bool clearWorkspace)
  : SharedMatlabInterface(clearWorkspace)
  {
    
  }

  
} // namespace Lrya

#else
//
// Define a dummy symbol to silence linker warnings such as
//   "blah.cpp.o has no symbols"
//
// This symbol is never actually used and may be discarded during final linkage.
//
int coretech_common_engine_matlabInterface = 0;

#endif // defined(LRYACORETECH_USE_MATLAB) && LRYACORETECH_USE_MATLAB
