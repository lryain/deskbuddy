/**
 * File: imageCacheSizes.h
 *
 * Author:  Al Chaussee
 * Created: 10/04/2018
 *
 * Description: Defines the various image sizes that are supported by ImageCache
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/


#ifndef __Lrya_Cozmo_Basestation_ImageCacheSizes_H__
#define __Lrya_Cozmo_Basestation_ImageCacheSizes_H__

namespace Lrya {
namespace Vision {

// For requesting different sizes
enum class ImageCacheSize : u8
{
  // ImageCache is reset at this size (resolution of camera)
  // For Vector this is 1280x720
  Full,
  Half,
  Quarter,
  Eighth,
};

// Convert size to a scaleFactor
inline f32 ImageCacheSizeToScaleFactor(ImageCacheSize size)
{
  switch(size)
  {
    case ImageCacheSize::Full:
      return 1.f;
      
    case ImageCacheSize::Half:
      return 0.5f;
      
    case ImageCacheSize::Quarter:
      return 0.25f;

    case ImageCacheSize::Eighth:
      return 0.125f;
  }
}

  
}
}

#endif
