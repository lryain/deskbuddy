#
# resources/CMakeLists.txt
#
# cmake rules to determine which files get deployed to robot
#
# Build options
#
# Configure with -DLRYA_BETA=1 to deploy beta resource files.
# Configure with -DLRYA_RESOURCE_SHIPPING=1 to deploy shipping resource files.
# Default is LRYA_BETA=0 and LRYA_RESOURCE_SHIPPING=0 to deploy developer resource files.
#
# Configure with -DFACTORY_TEST=1 or LRYA_NO_WEBSERVER_ENABLED=1 to prevent deployment of webserver resources.
# Configure with -DFACTORY_TEST=0 and LRYA_NO_WEBSERVER_ENABLED=0 to allow deployment of webserver resources.
# Default is FACTORY_TEST=0 and LRYA_NO_WEBSERVER_ENABLED=0.
#
# Note use of add_dependencies() to serialize copy operations. This is done to prevent random build failures
# that occur when parallel processes attempt to create the same directory structure.
#

include(lrya_build_copy_assets)

# Do we want to deploy webserver resources?
if (FACTORY_TEST OR LRYA_NO_WEBSERVER_ENABLED)
  set(LRYA_ENABLE_WEBSERVER 0)
else()
  set(LRYA_ENABLE_WEBSERVER 1)
endif()

list(APPEND ASSET_OUTPUT_RELATIVE_DSTS "")
list(APPEND ASSET_OUTPUT_FILES "")
list(APPEND ASSET_TARGETS "")

#
# Helper macro to manage ASSET_TARGETS
# Add a bogus dependency to each new target to serialize copy operations.
#
macro(add_asset_target target)
  if (ASSET_TARGETS)
    list(GET ASSET_TARGETS -1 tail)
    add_dependencies(${target} ${tail})
  endif()
  list(APPEND ASSET_TARGETS ${target})
endmacro()

#
# cozmo_resources_assets
#
lrya_build_copy_assets(
    TARGET cozmo_resources_assets
    SRCLIST_DIR ${LRYA_SRCLIST_DIR}
    OUTPUT_DIR "${CMAKE_BINARY_DIR}/data/assets/cozmo_resources"
    RELATIVE_OUTPUT_DIR "cozmo_resources"
    OUT_DSTS ASSET_OUTPUT_FILES
    OUT_RELATIVE_DSTS ASSET_OUTPUT_RELATIVE_DSTS
)

add_asset_target(cozmo_resources_assets)

#
# cozmo_resources_sound
#
# Override sound target for locally generated sound banks!!!!
set(sound_target cozmo_resources_sound)
set(asset_subdirectory "deskmate-audio-assets")
option(USE_LOCAL_AUDIO_ASSETS "Use local audio assets" OFF)
if(USE_LOCAL_AUDIO_ASSETS)
    message("----------------------------------------------------------------------")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/externals/local-audio-assets")
        set(sound_target cozmo_resources_local_sound)
        set(asset_subdirectory "local-audio-assets")
        message("Using locally generated audio assets - EXTERNALS/local-audio-assets")
    else()
        message("WARNING - Locally generated audio assets do NOT exists!")
        message("Reverting back to use DEPS assets")
    endif()
    message("----------------------------------------------------------------------")
endif()

set(sound_platform "deskmate_linux")
if(MACOSX)
    set(sound_platform "dev_mac")
endif()

lrya_build_copydirectory_assets(
    TARGET ${sound_target}
    SRCLIST_DIR "externals/${asset_subdirectory}/deskmate_robot/${sound_platform}"
    OUTPUT_DIR "${CMAKE_BINARY_DIR}/data/assets/cozmo_resources/sound"
    RELATIVE_OUTPUT_DIR "cozmo_resources/sound"
    OUT_DSTS ASSET_OUTPUT_FILES
    OUT_RELATIVE_DSTS ASSET_OUTPUT_RELATIVE_DSTS
)

add_asset_target(${sound_target})

#
# cozmo_resources_config
#
lrya_build_copy_assets(
    TARGET cozmo_resources_config
    SRCLIST_DIR ${LRYA_SRCLIST_DIR}
    OUTPUT_DIR "${CMAKE_BINARY_DIR}/data/assets/cozmo_resources"
    RELATIVE_OUTPUT_DIR "cozmo_resources"
    OUT_DSTS ASSET_OUTPUT_FILES
    OUT_RELATIVE_DSTS ASSET_OUTPUT_RELATIVE_DSTS
)

add_asset_target(cozmo_resources_config)

#
# cozmo_resources_webserver
#
if (${LRYA_ENABLE_WEBSERVER})
  lrya_build_copy_assets(
      TARGET cozmo_resources_webserver
      SRCLIST_DIR ${LRYA_SRCLIST_DIR}
      OUTPUT_DIR "${CMAKE_BINARY_DIR}/data/assets/cozmo_resources"
      RELATIVE_OUTPUT_DIR "cozmo_resources"
      OUT_DSTS ASSET_OUTPUT_FILES
      OUT_RELATIVE_DSTS ASSET_OUTPUT_RELATIVE_DSTS
  )
  add_asset_target(cozmo_resources_webserver)
endif()

#
# cozmo_resources_tts
#
lrya_build_copy_assets(
  TARGET        cozmo_resources_tts
  SRCLIST_DIR   ${LRYA_SRCLIST_DIR}
  OUTPUT_DIR   "${CMAKE_BINARY_DIR}/data/assets/cozmo_resources"
  RELATIVE_OUTPUT_DIR "cozmo_resources"
  OUT_DSTS ASSET_OUTPUT_FILES
  OUT_RELATIVE_DSTS ASSET_OUTPUT_RELATIVE_DSTS
)

add_asset_target(cozmo_resources_tts)

#
# cozmo_resources_beta, cozmo_resources_shipping, cozmo_resources_development
# Choose extra resource files from shipping or development,
# then copy them into resources.
#
if (LRYA_BETA)
  set(cozmo_resources_extra cozmo_resources_beta)
elseif (LRYA_RESOURCE_ESCAPEPOD)
  set(cozmo_resources_extra cozmo_resources_escapepod)
elseif (LRYA_RESOURCE_SHIPPING)
  set(cozmo_resources_extra cozmo_resources_shipping)
elseif (OSKR_ENDPOINTS)
  set(cozmo_resources_extra cozmo_resources_shipping)
else()
  set(cozmo_resources_extra cozmo_resources_development)
endif()

lrya_build_copy_assets(
  TARGET ${cozmo_resources_extra}
  SRCLIST_DIR ${LRYA_SRCLIST_DIR}
  OUTPUT_DIR "${CMAKE_BINARY_DIR}/data/assets/cozmo_resources"
  RELATIVE_OUTPUT_DIR "cozmo_resources"
  OUT_DSTS ASSET_OUTPUT_FILES
  OUT_RELATIVE_DSTS ASSET_OUTPUT_RELATIVE_DSTS
)

add_asset_target(${cozmo_resources_extra})

#
# cozmo_resources_test
# Bundle test assets on OSX
#
if (MACOSX)
    lrya_build_copy_assets(
        TARGET cozmo_resources_test
        SRCLIST_DIR ${LRYA_SRCLIST_DIR}
        OUTPUT_DIR "${CMAKE_BINARY_DIR}/data/assets/cozmo_resources"
        RELATIVE_OUTPUT_DIR "cozmo_resources"
        OUT_DSTS ASSET_OUTPUT_FILES
        OUT_RELATIVE_DSTS ASSET_OUTPUT_RELATIVE_DSTS
    )
    add_asset_target(cozmo_resources_test)
endif(MACOSX)

# Uncomment when debugging asset list aggregation
# list(LENGTH ASSET_OUTPUT_RELATIVE_DSTS __COUNT)
# message(STATUS "ASSET_OUTPUT_DSTS COUNT : ${__COUNT}")

set (ASSET_METADATA_BASENAME "${CMAKE_BINARY_DIR}/asset-build")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/asset-build.manifest.in
    "${ASSET_METADATA_BASENAME}.manifest"
)

add_custom_command(
    OUTPUT "${ASSET_METADATA_BASENAME}.gc.stamp"
    COMMAND ${CMAKE_SOURCE_DIR}/project/deskmate/scripts/prune-dir.sh -c
        -o ${ASSET_METADATA_BASENAME}.gc.stamp
        -m ${ASSET_METADATA_BASENAME}.manifest
        ${CMAKE_BINARY_DIR}/data/assets
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${ASSET_METADATA_BASENAME}.manifest ${ASSET_OUTPUT_FILES}
    COMMENT "rm stale asset files"
)
add_custom_target(prune_files ALL DEPENDS
    "${ASSET_METADATA_BASENAME}.gc.stamp"
)

add_dependencies(cozmo_engine
    ${ASSET_TARGETS}
    prune_files
)

add_dependencies(deskmate_anim
    ${ASSET_TARGETS}
    prune_files
)
