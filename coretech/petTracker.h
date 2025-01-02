/**
 * File: petTracker.h
 *
 * Author: Andrew Stein
 * Date:   05/09/2016
 *
 * Description: Wrapper for Omron Computer Vision (OMCV) pet detection library.
 *
 *
 * Copyright: Lrya, Inc. 2016
 **/

#ifndef __Lrya_Vision_PetTracker_H__
#define __Lrya_Vision_PetTracker_H__

#include "coretech/vision/engine/image.h"
#include "coretech/vision/engine/trackedPet.h"
#include "coretech/vision/engine/profiler.h"

#include <list>

namespace Lrya {
namespace Vision {
  
class PetTracker : public Profiler
{
public:
  
  PetTracker();
  ~PetTracker();

  Result Init(const Json::Value& config);
  
  Result Update(const Vision::Image&       frameOrig,
                std::list<TrackedPet>&     pets);
  
private:

  bool _isInitialized = false;
  
  // Forward declaration
  struct Handles;
  
  std::unique_ptr<Handles> _handles;
  
  // For dev/live threshold adjustment
  s32 _runtimeDetectionThreshold = -1;
  
}; // class PetTracker
  
} // namespace Vision
} // namespace Lrya

#endif // __Lrya_Vision_PetTracker_H__
