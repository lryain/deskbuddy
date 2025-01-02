#
# platform/config/CMakeLists.txt
#
# cmake rules to determine which files get deployed to robot under /lrya/{bin,etc}
#
# Build options
#
# Configure with -DLRYA_RESOURCE_SHIPPING=1 to deploy shipping config files.
# Configure with -DLRYA_RESOURCE_SHIPPING=0 to deploy development config files.
# Default is LRYA_RESOURCE_SHIPPING=0.
#
# Configure with -DLRYA_USER_DEV=1 to deploy the userdev version of config files
# Default is LRYA_USER_DEV=0
#
# Configure with -DLRYA_BETA=1 to deploy the beta version of config files
# Default is LRYA_BETA=0
#
# Note use of add_dependencies() to serialize copy operations. This is done to prevent random build failures
# that occur when parallel processes attempt to create the same directory structure.
#
include(lrya_build_copy_assets)

lrya_build_copy_assets(
    TARGET platform_config_bin
    SRCLIST_DIR ${LRYA_SRCLIST_DIR}
    OUTPUT_DIR "${CMAKE_BINARY_DIR}"
)

lrya_build_copy_assets(
    TARGET platform_config_etc
    SRCLIST_DIR ${LRYA_SRCLIST_DIR}
    OUTPUT_DIR "${CMAKE_BINARY_DIR}"
)

#
# platform_config_etc_development, platform_config_etc_shipping
# platform_config_bin_development, (none for shipping)
# Choose extra config files from shipping or development,
# then copy them into etc
#

if (LRYA_AMAZON_ENDPOINTS_ENABLED)

  if(LRYA_RESOURCE_SHIPPING)
    set(BASE_URL "https://ota.global.lrya-services.com/mate/am736/")
    set(LRYADEV_BASE_URL "https://ota.global.lrya-dev-services.com/mate/am736/lo8awreh23498sf/")
    configure_file(shipping/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_shipping)
  else()
    set(BASE_URL "https://ota.global.lrya-dev-services.com/mate/am736-dev/lo8awreh23498sf/")
    set(BASE_URL_LATEST "")
    configure_file(development/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_development)
    set(platform_config_bin_extra platform_config_bin_development)
  endif()

  if (LRYA_USER_DEV)
    # If this is the special "userdev" build then use its etc files
    set(BASE_URL "https://ota.global.lrya-dev-services.com/mate/am736-userdev/lo8awreh23498sf/")
    configure_file(userdev/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_userdev)
  endif()

  # If this is the special "beta" build then use its etc files
  if (LRYA_BETA)
    set(BASE_URL "https://ota.global.lrya-dev-services.com/mate/am736-beta/lo8awreh23498sf/")
    configure_file(beta/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_beta)
  endif()

else()

  if(LRYA_RESOURCE_SHIPPING AND OSKR_ENDPOINTS)
    set(BASE_URL "https://ota.global.lrya-services.com/mate/oskr/")
    set(LRYADEV_BASE_URL "https://ota.global.lrya-dev-services.com/mate/oskr/")
    configure_file(oskr/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_oskr)
  elseif(LRYA_RESOURCE_SHIPPING AND LRYA_RESOURCE_ESCAPEPOD)
    set(BASE_URL "https://ota.global.lrya-services.com/mate/ep/")
    set(LRYADEV_BASE_URL "https://ota.global.lrya-dev-services.com/mate/ep/")
    configure_file(oskr/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_ep)  
  elseif(LRYA_RESOURCE_SHIPPING AND NOT OSKR_ENDPOINTS AND NOT LRYA_RESOURCE_ESCAPEPOD)
    set(BASE_URL "https://ota-cdn.ddl.io/vector/ota/prod/")
    set(LRYADEV_BASE_URL "https://ota.global.lrya-dev-services.com/mate/master/lo8awreh23498sf/")
    configure_file(shipping/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_shipping)
  elseif(LRYA_RESOURCE_ESCAPEPOD AND NOT LRYA_RESOURCE_SHIPPING)
    set(BASE_URL "https://ota.global.lrya-dev-services.com/mate/master-ep/lo8awreh23498sf/")
    set(BASE_URL_LATEST "https://ota.global.lrya-dev-services.com/mate/master-ep-latest/lo8awreh23498sf/")
    configure_file(development/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_epdev)
    set(platform_config_bin_extra platform_config_bin_epdev)
  else()
    set(BASE_URL "https://ota.pvic.xyz/mate/")
    set(BASE_URL_LATEST "https://ota.pvic.xyz/mate/")
    configure_file(development/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_development)
    set(platform_config_bin_extra platform_config_bin_development)
  endif()

  if (LRYA_USER_DEV)
    set(BASE_URL "https://ota.global.lrya-dev-services.com/mate/master-userdev/lo8awreh23498sf/")
    configure_file(userdev/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_userdev)
  endif()

  if (LRYA_BETA)
    set(BASE_URL "https://ota.global.lrya-dev-services.com/mate/beta/lo8awreh23498sf/")
    configure_file(beta/etc-template/update-engine.env.in ${CMAKE_BINARY_DIR}/etc/update-engine.env)
    set(platform_config_etc_extra platform_config_etc_beta)
  endif()
endif()

lrya_build_copy_assets(
  TARGET ${platform_config_etc_extra}
  SRCLIST_DIR ${LRYA_SRCLIST_DIR}
  OUTPUT_DIR "${CMAKE_BINARY_DIR}"
)

if (NOT LRYA_RESOURCE_SHIPPING)
  lrya_build_copy_assets(
    TARGET ${platform_config_bin_extra}
    SRCLIST_DIR ${LRYA_SRCLIST_DIR}
    OUTPUT_DIR "${CMAKE_BINARY_DIR}"
  )
endif()

add_dependencies(platform_config_etc ${platform_config_etc_extra})
if (NOT LRYA_RESOURCE_SHIPPING)
  add_dependencies(platform_config_bin ${platform_config_bin_extra})
endif()
