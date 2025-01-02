//
//  imageIO.h
//  CoreTech_Vision_Basestation
//
//  Description: Image file read/write methods
//
//  Created by Kevin Yoon on 4/21/14.
//  Copyright (c) 2014 Lrya, Inc. All rights reserved.
//

#ifndef __CoreTech_Vision__imageIO__
#define __CoreTech_Vision__imageIO__

#include "coretech/common/shared/types.h"

namespace Lrya {
  namespace Vision {
    
    // For grayscale data
    void WritePGM(const char* filename, const u8* imgData, u32 width, u32 height);
    
    // For RGB data
    void WritePPM(const char* filename, const u8* imgData, u32 width, u32 height);
    
  } // namespace Vision
} // namespace Lrya

#endif // __CoreTech_Vision__imageIO__
