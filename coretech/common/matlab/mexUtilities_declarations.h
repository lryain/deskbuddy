#ifndef _MEX_UTILITES_DECLARATIONS_H_
#define _MEX_UTILITES_DECLARATIONS_H_

#include "lrya/common/robot/config.h"

#include "mex.h"
#include "engine.h"

#include "lrya/common/robot/matlabInterface.h"
#include "lrya/common/matlab/mexWrappers.h"

namespace Lrya
{
  // Find the min an max values of the Matlab array
  template<typename Type> void maxAndMin(const mxArray *array, Type &minValue, Type &maxValue);

  // Count the numbers of each unique value for the integer Matlab array, then allocate and return a Matlab array with the unique values
  // TODO: return the counts too?
  // TODO: currently only works up to s32 precision (no u32, s64, u64)
  template<typename Type> mxArray* findUnique(const mxArray *array, const Type minValue, const Type maxValue);
} // namespace Lrya

#endif // #ifndef _MEX_UTILITES_DECLARATIONS_H_
