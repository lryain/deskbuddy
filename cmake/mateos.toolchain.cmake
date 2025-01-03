cmake_minimum_required(VERSION 3.10.0)

# CMake invokes the toolchain file twice during the first build, but only once
# during subsequent rebuilds. This was causing the various flags to be added
# twice on the first build, and on a rebuild ninja would see only one set of the
# flags and rebuild the world.
# https://github.com/android-ndk/ndk/issues/323
if(MATEOS_TOOLCHAIN_INCLUDED)
    return()
endif(MATEOS_TOOLCHAIN_INCLUDED)

set(MATEOS_TOOLCHAIN_INCLUDED true)

# Touch toolchain variable to suppress "unused variable" warning.
# This happens if CMake is invoked with the same command line the second time.
if(CMAKE_TOOLCHAIN_FILE)
endif()

# We only support clang
set(MATEOS_TOOLCHAIN clang)

set(MATEOS_CPP_FEATURES rtti exceptions)

# Standard cross-compiling stuff.
set(MATEOS TRUE)

set(CMAKE_SYSTEM_NAME Linux)

set(MATEOS_COMPILER_FLAGS)
set(MATEOS_COMPILER_FLAGS_CXX)
set(MATEOS_COMPILER_FLAGS_DEBUG)
set(MATEOS_COMPILER_FLAGS_RELEASE)
set(MATEOS_LINKER_FLAGS)
set(MATEOS_LINKER_FLAGS_EXE)

set(MATEOS_C_COMPILER   "/usr/bin/clang")
set(MATEOS_CXX_COMPILER "/usr/bin/clang++")

# Generic flags.
list(APPEND MATEOS_COMPILER_FLAGS
    -DMATEOS
    -Qunused-arguments
	-ffunction-sections
	-fdata-sections
	-funwind-tables
	-fstack-protector-strong
#  -flto
#  -fvisibility=hidden
#  -fsanitize=cfi
	-no-canonical-prefixes)
list(APPEND MATEOS_COMPILER_FLAGS_CXX
    -Qunused-arguments
	-fno-exceptions
	-fno-rtti)
list(APPEND MATEOS_COMPILER_FLAGS_RELEASE
  -D_FORTIFY_SOURCE=2)
list(APPEND MATEOS_LINKER_FLAGS
	-Wl,--build-id
	#-Wl,--gdb-index
	-Wl,--warn-shared-textrel
	-Wl,--gc-sections)
#    -Wl,-rpath-link,${MATEOS_SDK}/sysroot/lib
#    -Wl,-rpath-link,${MATEOS_SDK}/sysroot/usr/lib)
#	-Wl,--fatal-warnings)
# list(APPEND MATEOS_LINKER_FLAGS_EXE
#    -Wl,-rpath-link,${MATEOS_SDK}/sysroot/lib
#    -Wl,-rpath-link,${MATEOS_SDK}/sysroot/usr/lib
# 	-Wl,-z,nocopyreloc)

# Debug and release flags.
list(APPEND MATEOS_COMPILER_FLAGS_DEBUG
	-O0
    -Winstantiation-after-specialization 
    # -fno-elide-constructors 
    # -Werror=undefined-func-template
    # -fno-limit-debug-info
    )
list(APPEND MATEOS_COMPILER_FLAGS_RELEASE
	-Os
        -DNDEBUG)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND MATEOS_LINKER_FLAGS_EXE
        -Wl,-rpath-link,${CMAKE_SOURCE_DIR}/_build/mateos/Release/lib)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND MATEOS_LINKER_FLAGS_EXE
        -Wl,-rpath-link,${CMAKE_SOURCE_DIR}/_build/mateos/Release/lib)
endif()

# Toolchain and ABI specific flags.
# list(APPEND MATEOS_COMPILER_FLAGS
# 	-march=armv8-a
# 	-mfloat-abi=hard
# 	-mfpu=neon-fp-armv8)
# list(APPEND MATEOS_LINKER_FLAGS
# 	-Wl,--fix-cortex-a8)

# Disable integrated-as for better compatibility.
# list(APPEND ANDROID_COMPILER_FLAGS
#	-fno-integrated-as)

# STL specific flags for libc++
set(MATEOS_STL_PREFIX llvm-libc++)
list(APPEND MATEOS_LINKER_FLAGS
	-Wl,--exclude-libs,libunwind.a
	-lc
	-latomic
	-lpthread)
list(APPEND MATEOS_COMPILER_FLAGS_CXX
	-stdlib=libc++
	-std=c++14)

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES "")

# Add libc++_shared.so
# /usr/lib/arm-linux-gnueabihf/libc++.so.1
set(MATEOS_CXX_STANDARD_LIBRARIES "/usr/lib/arm-linux-gnueabihf/libc++.so.1")

# Add libunwind
set(MATEOS_CXX_STANDARD_LIBRARIES "/usr/lib/arm-linux-gnueabihf/libunwind.a")

set(CMAKE_C_STANDARD_LIBRARIES_INIT "-lm")
set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "${CMAKE_C_STANDARD_LIBRARIES_INIT}")
if(MATEOS_CXX_STANDARD_LIBRARIES)
    string(REPLACE ";" "\" \"" MATEOS_CXX_STANDARD_LIBRARIES "\"${MATEOS_CXX_STANDARD_LIBRARIES}\"")
    set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "${CMAKE_CXX_STANDARD_LIBRARIES_INIT} ${MATEOS_CXX_STANDARD_LIBRARIES}")
endif()
message("CMAKE_CXX_STANDARD_LIBRARIES_INIT: ${CMAKE_CXX_STANDARD_LIBRARIES_INIT} ===========================")
# Configuration specific flags.
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
list(APPEND MATEOS_LINKER_FLAGS_EXE
	-pie
	-fPIE)
if(MATEOS_CPP_FEATURES)
    separate_arguments(MATEOS_CPP_FEATURES)
    foreach(feature ${MATEOS_CPP_FEATURES})
        if(NOT ${feature} MATCHES "^(rtti|exceptions)$")
            message(FATAL_ERROR "Invalid C++ feature: ${feature}.")
        endif()
        list(APPEND MATEOS_COMPILER_FLAGS_CXX -f${feature})
    endforeach()
    string(REPLACE ";" " " MATEOS_CPP_FEATURES "${MATEOS_CPP_FEATURES}")
endif()
if(NOT MATEOS_ALLOW_UNDEFINED_SYMBOLS)
    list(APPEND MATEOS_LINKER_FLAGS
		-Wl,--no-undefined)
endif()

# set thumb mode (use -marm for arm mode)
list(APPEND MATEOS_COMPILER_FLAGS -mthumb)

list(APPEND MATEOS_COMPILER_FLAGS
    -mfpu=neon)
#list(APPEND MATEOS_COMPILER_FLAGS
#    -Wa,--noexecstack)
list(APPEND MATEOS_LINKER_FLAGS
    -Wl,-z,noexecstack)

# CMake automatically forwards all compiler flags to the linker,
# and clang doesn't like having -Wa flags being used for linking.
# To prevent CMake from doing this would require meddling with
# the CMAKE_<LANG>_COMPILE_OBJECT rules, which would get quite messy.
list(APPEND MATEOS_LINKER_FLAGS
    -Qunused-arguments)
list(APPEND MATEOS_LINKER_FLAGS
    -Wl,-z,relro -Wl,-z,now)
list(APPEND MATEOS_COMPILER_FLAGS
    -Wformat -Werror=format-security)

# Convert these lists into strings.
string(REPLACE ";" " " MATEOS_COMPILER_FLAGS         "${MATEOS_COMPILER_FLAGS}")
string(REPLACE ";" " " MATEOS_COMPILER_FLAGS_CXX     "${MATEOS_COMPILER_FLAGS_CXX}")
string(REPLACE ";" " " MATEOS_COMPILER_FLAGS_DEBUG   "${MATEOS_COMPILER_FLAGS_DEBUG}")
string(REPLACE ";" " " MATEOS_COMPILER_FLAGS_RELEASE "${MATEOS_COMPILER_FLAGS_RELEASE}")
string(REPLACE ";" " " MATEOS_LINKER_FLAGS           "${MATEOS_LINKER_FLAGS}")
string(REPLACE ";" " " MATEOS_LINKER_FLAGS_EXE       "${MATEOS_LINKER_FLAGS_EXE}")

if(MATEOS_CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER   "${MATEOS_CCACHE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${MATEOS_CCACHE}")
endif()
# set(CMAKE_C_COMPILER        "${MATEOS_C_COMPILER}")
# set(CMAKE_CXX_COMPILER      "${MATEOS_CXX_COMPILER}")
set(CMAKE_C_COMPILER        "/usr/bin/clang")
set(CMAKE_CXX_COMPILER      "/usr/bin/clang++")
# message("0000000000000000000000000000-> CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
# message("0000000000000000000000000000-> CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

# Run the compiler ID checks before we set flags.
# When passed the `-march=` flag, Clang can fail to compile if CMake doesn't
# correctly supply the target and external toolchain, but to do so, CMake
# needs to already know that the compiler is Clang. Tell CMake to run the
# compiler ID detection step before we set Android flags.
# See https://gitlab.kitware.com/cmake/cmake/issues/16587
include(CMakeDetermineCCompiler)
include(CMakeDetermineCXXCompiler)

# Set or retrieve the cached flags.
# This is necessary in case the user sets/changes flags in subsequent
# configures. If we included the Android flags in here, they would get
# overwritten.
set(CMAKE_C_FLAGS ""
	CACHE STRING "Flags used by the compiler during all build types.")
set(CMAKE_CXX_FLAGS ""
	CACHE STRING "Flags used by the compiler during all build types.")
set(CMAKE_C_FLAGS_DEBUG ""
	CACHE STRING "Flags used by the compiler during debug builds.")
set(CMAKE_CXX_FLAGS_DEBUG ""
	CACHE STRING "Flags used by the compiler during debug builds.")
set(CMAKE_C_FLAGS_RELEASE ""
	CACHE STRING "Flags used by the compiler during release builds.")
set(CMAKE_CXX_FLAGS_RELEASE ""
	CACHE STRING "Flags used by the compiler during release builds.")
set(CMAKE_MODULE_LINKER_FLAGS ""
	CACHE STRING "Flags used by the linker during the creation of modules.")
set(CMAKE_SHARED_LINKER_FLAGS ""
	CACHE STRING "Flags used by the linker during the creation of dll's.")
set(CMAKE_EXE_LINKER_FLAGS ""
	CACHE STRING "Flags used by the linker.")

set(CMAKE_C_FLAGS             "${MATEOS_COMPILER_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS           "${MATEOS_COMPILER_FLAGS} ${MATEOS_COMPILER_FLAGS_CXX} ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG       "${MATEOS_COMPILER_FLAGS_DEBUG} ${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG     "${MATEOS_COMPILER_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE     "${MATEOS_COMPILER_FLAGS_RELEASE} ${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE   "${MATEOS_COMPILER_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS "${MATEOS_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${MATEOS_LINKER_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS    "${MATEOS_LINKER_FLAGS} ${MATEOS_LINKER_FLAGS_EXE} ${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# <lrya>
# Work around missing CMAKE_SIZEOF_VOID_P
#
# Resolves build error
#   FindWinSock.cmake:77 (if):
#   if given arguments:
#     "x86_64" "STREQUAL" "AMD64" "AND" "EQUAL" "4"
# </lrya>
set(CMAKE_SIZEOF_VOID_P 4)

# Debug.
message(STATUS "CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES=${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}")

option(USE_LRYAASAN "Enable address sanitizer" OFF)
if (USE_LRYAASAN)
  # Depends on -shared-libasan, convert to OPTION
  set(ASAN_CXX_FLAGS           PUBLIC
                               -fsanitize=address
                               -fno-omit-frame-pointer
  )

  set(ASAN_LINKER_FLAGS        PUBLIC
                               -fsanitize=address
  )

#   set(ASAN_SHARED_LINKER_FLAGS PUBLIC
#                                -fsanitize=address
#                                # requires SDK support -shared-libasan
#                                -ldl
#                                -lrt
#                                # sysroot/usr/local/clang_6.0.1/lib/clang/6.0.1/lib/linux
#                                -l${MATEOS_SDK}/sysroot/usr/local/clang_6.0.1/lib/clang/6.0.1/lib/linux/libclang_rt.asan-aarch64.a
#   )

#   set(ASAN_EXE_LINKER_FLAGS    PUBLIC
#                                -fsanitize=address
#                                -ldl
#                                -lrt
#                                -l${MATEOS_SDK}/sysroot/usr/local/clang_6.0.1/lib/clang/6.0.1/lib/linux/libclang_rt.asan-aarch64.a
#   )
endif()
