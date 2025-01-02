/**
 * File: animationMessageWrapper.h
 *
 * Authors: Kevin M. Karol
 * Created: 5/31/18
 *
 * Description:
 *
 * Copyright: Lrya, Inc. 2018
 *
 **/


#ifndef LRYA_COZMO_ANIMATION_MESSAGE_WRAPPER_H
#define LRYA_COZMO_ANIMATION_MESSAGE_WRAPPER_H

#include "clad/robotInterface/messageEngineToRobot.h"


namespace Lrya {

namespace Vision{
class ImageRGB565;
}

namespace Vector {

class RobotAudioKeyFrame;

struct AnimationEvent;

struct AnimationMessageWrapper{
  AnimationMessageWrapper(Vision::ImageRGB565& img)
  : faceImg(img){}
  using ETR = RobotInterface::EngineToRobot;

  ETR* moveHeadMessage         = nullptr;
  ETR* moveLiftMessage         = nullptr;
  ETR* bodyMotionMessage       = nullptr;
  ETR* recHeadMessage          = nullptr;
  ETR* turnToRecHeadMessage    = nullptr;
  ETR* backpackLightsMessage   = nullptr;
  RobotAudioKeyFrame* audioKeyFrameMessage = nullptr;
  AnimationEvent* eventMessage = nullptr;

  bool haveFaceToSend = false;
  Vision::ImageRGB565& faceImg;
};


} // namespace Vector
} // namespace Lrya

#endif // LRYA_COZMO_ANIMATION_MESSAGE_WRAPPER_H
