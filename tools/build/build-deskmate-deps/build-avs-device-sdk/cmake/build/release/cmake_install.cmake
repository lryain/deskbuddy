# Install script for directory: /home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/avs-device-sdk

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/ThirdParty/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/AVSCommon/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/ACL/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/ADSL/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/AFML/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/Settings/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/BluetoothImplementations/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/EqualizerImplementations/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/CertifiedSender/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/ContextManager/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/CapabilitiesDelegate/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/PlaylistParser/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/KWD/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/CapabilityAgents/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/Integration/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/ApplicationUtilities/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/MediaPlayer/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/ESP/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/RegistrationManager/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/SampleApp/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/SpeechEncoder/cmake_install.cmake")
  include("/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/Storage/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/pi/deskbuddy/tools/build/build-deskmate-deps/build-avs-device-sdk/cmake/build/release/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
