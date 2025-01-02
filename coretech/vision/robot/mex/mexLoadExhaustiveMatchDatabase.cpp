#include "mex.h"

#include "lrya/common/robot/matlabInterface.h"

#include "coretech/vision/robot/fiducialMarkers.h"
#include "coretech/vision/robot/fiducialDetection.h"

#include "lrya/common/matlab/mexWrappers.h"
#include "lrya/common/shared/utilities_shared.h"

#include <string.h>
#include <vector>

using namespace Lrya::Embedded;

// patterns = {'Z:/Box Sync/Cozmo SE/VisionMarkers/lryaLogoMat/unpadded/*.png', 'Z:/Box Sync/Cozmo SE/VisionMarkers/dice/withFiducials/*.png', 'Z:/Box Sync/Cozmo SE/VisionMarkers/letters/withFiducials/*.png', 'Z:/Box Sync/Cozmo SE/VisionMarkers/symbols/withFiducials/*.png'};
// imageFilenames = {};
// for iPattern = 1:length(patterns)
//   files = dir(patterns{iPattern});
//   for iFile = 1:length(files)
//     imageFilenames{end+1} = [strrep(patterns{iPattern}, '*.png', ''), files(iFile).name];
//   end
// end
// [numDatabaseImages, databaseImageHeight, databaseImageWidth, databaseImages, databaseLabelIndexes] = mexLoadExhaustiveMatchDatabase(imageFilenames);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  Lrya::SetCoreTechPrintFunctionPtr(mexPrintf);

  const s32 bufferSize = 100000000;

  LryaConditionalErrorAndReturn(nrhs == 1 && nlhs == 5, "mexLoadExhaustiveMatchDatabase", "Call this function as following: [numDatabaseImages, databaseImageHeight, databaseImageWidth, databaseImages, databaseLabelIndexes] = mexLoadExhaustiveMatchDatabase(imageFilenames);");

  MemoryStack memory(mxMalloc(bufferSize), bufferSize);
  LryaConditionalErrorAndReturn(memory.IsValid(), "mexLoadExhaustiveMatchDatabase", "Memory could not be allocated");

  Array<char *> imageFilenamesArray = mxCellArrayToStringArray(prhs[0], memory);

  LryaConditionalErrorAndReturn(imageFilenamesArray.IsValid(), "mexLoadExhaustiveMatchDatabase", "Could not allocate imageFilenamesArray");

  const s32 numImages = MAX(imageFilenamesArray.get_size(0), imageFilenamesArray.get_size(1));

  FixedLengthList<const char *> imageFilenames(numImages, memory, Flags::Buffer(true, false, true));

  LryaConditionalErrorAndReturn(imageFilenames.IsValid(), "mexLoadExhaustiveMatchDatabase", "Could not allocate imageFilenames");

  for(s32 i=0; i<numImages; i++) {
    imageFilenames[i] = imageFilenamesArray.Element(i);
  }

  VisionMarkerImages vmi(imageFilenames, memory);

  LryaConditionalErrorAndReturn(vmi.IsValid(), "mexLoadExhaustiveMatchDatabase", "Could not init VisionMarkerImages");

  Array<s32> oneOutput(1,1,memory);

  *oneOutput.Pointer(0,0) = vmi.get_numDatabaseImages();
  plhs[0] = arrayToMxArray(oneOutput);

  *oneOutput.Pointer(0,0) = vmi.get_databaseImageHeight();
  plhs[1] = arrayToMxArray(oneOutput);

  *oneOutput.Pointer(0,0) = vmi.get_databaseImageWidth();
  plhs[2] = arrayToMxArray(oneOutput);

  const Array<u8> databaseImages = vmi.get_databaseImages();
  Array<u8> databaseImagesTransposed(databaseImages.get_size(1), databaseImages.get_size(0), memory);
  Matrix::Transpose(databaseImages, databaseImagesTransposed);

  plhs[3] = arrayToMxArray(databaseImagesTransposed);

  FixedLengthList<Lrya::Vision::MarkerType> databaseLabelIndexesList = vmi.get_databaseLabelIndexes();
  const Array<Lrya::Vision::MarkerType> databaseLabelIndexesArray = databaseLabelIndexesList.get_array();
  plhs[4] = arrayToMxArray<s32>(*(reinterpret_cast<const Array<s32>*> (&databaseLabelIndexesArray)));

  mxFree(memory.get_buffer());
}
