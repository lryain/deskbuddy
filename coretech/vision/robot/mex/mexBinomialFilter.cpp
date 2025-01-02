#include "mex.h"

#include "lrya/common/types.h"
#include "lrya/common/robot/array2d.h"
#include "lrya/common/robot/config.h"
#include "lrya/common/robot/matlabInterface.h"

#include "coretech/vision/robot/imageProcessing.h"

#include "lrya/common/matlab/mexWrappers.h"
#include "lrya/common/shared/utilities_shared.h"

#define VERBOSITY 0

using namespace Lrya::Embedded;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  Lrya::SetCoreTechPrintFunctionPtr(mexPrintf);

  const s32 bufferSize = 10000000;
  MemoryStack memory(malloc(bufferSize), bufferSize);
  LryaConditionalErrorAndReturn(memory.IsValid(), "mexBinomialFilter", "Memory could not be allocated");

  LryaConditionalErrorAndReturn(nrhs == 1 && nlhs == 1, "mexBinomialFilter", "Call this function as following: imgFiltered = mexBinomialFilter(img);");

  Array<u8> img = mxArrayToArray<u8>(prhs[0], memory);

  LryaConditionalErrorAndReturn(img.get_buffer() != 0, "mexBinomialFilter", "Could not allocate Array<u8> img");

  Array<u8> imgFiltered(img.get_size(0), img.get_size(1), memory);
  LryaConditionalErrorAndReturn(img.get_buffer() != 0, "mexBinomialFilter", "Could not allocate Array<u8> imgFiltered");

  if(ImageProcessing::BinomialFilter<u8,u32,u8>(img, imgFiltered, memory) != Lrya::RESULT_OK) {
    Lrya::CoreTechPrint("Error: mexBinomialFilter\n");
  }

  plhs[0] = arrayToMxArray<u8>(imgFiltered);

  free(memory.get_buffer());
}
