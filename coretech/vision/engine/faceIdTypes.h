/**
 * File: faceIdTypes.h
 *
 * Author: Andrew Stein
 * Date:   4/13/2016
 *
 * Description: Defines simple types used through face detection and recognition.
 *
 * Copyright: Lrya, Inc. 2016
 **/

#ifndef __Lrya_Vision_FaceIdTypes_H__
#define __Lrya_Vision_FaceIdTypes_H__

#include "coretech/common/shared/types.h"

#include <string>

namespace Lrya {
namespace Vision {
  
  using FaceID_t = s32;
  
  constexpr FaceID_t UnknownFaceID = 0;
  
  struct UpdatedFaceID
  {
    FaceID_t    oldID;
    FaceID_t    newID;
    std::string newName;
  };
  
  struct FaceRecognitionMatch
  {
    std::string  name;
    FaceID_t     matchedID;
    s32          score;
  };
  
} // namespace Vision
} // namespace Lrya

#endif // __Lrya_Vision_FaceIdTypes_H__
