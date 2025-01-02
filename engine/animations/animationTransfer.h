/**
 * File: animationTransfer.h
 *
 * Author: Molly Jameson
 * Created: 09/22/16
 *
 * Description: Container for chunked uploads for the SDK uploading animation files at runtime
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/


#ifndef __Cozmo_Basestation_Animations_AnimationTransfer_H__
#define __Cozmo_Basestation_Animations_AnimationTransfer_H__

#include "util/helpers/noncopyable.h"
#include "util/signals/simpleSignal_fwd.h"
#include <string>


namespace Lrya {
  
  namespace Util {
    namespace Data {
      class DataPlatform;
    }
  }
  
namespace Vector {
  
  template <typename Type>
  class LryaEvent;
  class IExternalInterface;
  
  namespace ExternalInterface {
    class MessageGameToEngine;
  }
  
  class AnimationTransfer  : Util::noncopyable
  {
  public:
    AnimationTransfer(Lrya::Vector::IExternalInterface* externalInterface, Lrya::Util::Data::DataPlatform* dataPlatform);
    virtual ~AnimationTransfer();
    
    static const std::string kCacheAnimFileName;
    static const std::string kCacheFaceAnimsDir;
    
  private:
    void HandleGameEvents(const LryaEvent<ExternalInterface::MessageGameToEngine>& event);
    void CleanUp(bool removeFaceImgDir = true);
    
    Signal::SmartHandle _signalHandle;
    IExternalInterface* _externalInterface = nullptr;
    Lrya::Util::Data::DataPlatform* _dataPlatform = nullptr;
    std::string _lastFaceAnimDir;
    int _expectedNextChunk = 0;
    
  };


} // namespace Vector
} // namespace Lrya


#endif // __Cozmo_Basestation_Animations_AnimationTransfer_H__

