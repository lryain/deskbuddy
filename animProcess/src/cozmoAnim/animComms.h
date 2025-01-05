/**
 * File: cozmoAnim/animComms.h
 *
 * Author: Kevin Yoon
 * Created: 7/30/2017
 *
 * Description: Create sockets and manages low-level data transfer to engine and robot processes
 *
 * Copyright: Lrya, Inc. 2017
 **/

#ifndef COZMO_ANIM_COMMS_H
#define COZMO_ANIM_COMMS_H

#include "coretech/common/shared/types.h"

//
// Enable this to collect socket buffer usage stats at the end of each tick.
// High buffer usage indicates that processes are falling behind on socket I/O.
// If a socket runs out of available buffer space, send() may fail with errno=EAGAIN
// or errno=EWOULDBLOCK.
//
#ifndef LRYA_PROFILE_ANIMCOMMS_SOCKET_BUFFER_STATS
#define LRYA_PROFILE_ANIMCOMMS_SOCKET_BUFFER_STATS 0
#endif

namespace Lrya {
namespace Vector {
namespace AnimComms {

  // Initialize robot comms
  Result InitRobotComms();

  // Initialize engine comms
  Result InitEngineComms();

  // Initialize robot + engine comms
  Result InitComms();

  // Connected to robot?
  bool IsConnectedToRobot();

  // Connected to engine?
  bool IsConnectedToEngine();

  // Disconnect from robot
  void DisconnectRobot();

  // Disconnect from engine
  void DisconnectEngine();

  // Gets the next packet from the engine socket
  u32 GetNextPacketFromEngine(u8* buffer, u32 max_length);

  // Get the next packet from robot socket
  u32 GetNextPacketFromRobot(u8* buffer, u32 max_length);

  // Send a packet to engine
  bool SendPacketToEngine(const void *buffer, const u32 length);

  // Send a packet to robot
  bool SendPacketToRobot(const void *buffer, const u32 length);

  #if LRYA_PROFILE_ANIMCOMMS_SOCKET_BUFFER_STATS

  void InitSocketBufferStats();
  void UpdateSocketBufferStats();
  void ReportSocketBufferStats();

  #endif

} // namespace AnimComms
} // namespace Vector
} // namespace Lrya

#endif  // #ifndef COZMO_ANIM_COMMS_H
