/**
 File: matlabInterface.h
 Author: Andrew Stein
 Created: 11/21/2014
 
 Extends shared Matlab interface to add basestation-specific converters.
 
 Copyright Lrya, Inc. 2014
 For internal use only. No part of this code may be used without a signed non-disclosure agreement with Lrya, inc.
 **/
#ifndef LRYA_CORETECH_COMMON_BASESTATION_MATLAB_INTERFACE_H_
#define LRYA_CORETECH_COMMON_BASESTATION_MATLAB_INTERFACE_H_

#if LRYACORETECH_USE_MATLAB

#include "coretech/common/shared/sharedMatlabInterface.h"
#include "coretech/common/engine/matlabConverters_basestation.h"

namespace Lrya {
  
  class Matlab : public SharedMatlabInterface
  {
  public:
    Matlab(bool clearWorkspace=false);
    
  }; // class Matlab
  
} // namespace Lrya


#endif // LRYACORETECH_USE_MATLAB

#endif // LRYA_CORETECH_COMMON_BASESTATION_MATLAB_INTERFACE_H_

