# 获取当前已有的C++编译选项
get_property(OLD_C_FLAGS GLOBAL PROPERTY CMAKE_C_FLAGS)
get_property(OLD_CXX_FLAGS GLOBAL PROPERTY CMAKE_CXX_FLAGS)
# 添加禁用警告的选项，注意使用空格进行连接
set(CMAKE_C_FLAGS "${OLD_C_FLAGS} ")
set(CMAKE_CXX_FLAGS "${OLD_CXX_FLAGS} -Wno-unused-private-field -Wno-format -Wl,  -stdlib=libc++ -std=c++14")
# -std=c++14 -fpermissive -Wno-delete-non-virtual-dtor -Wno-explicit-specialization-after-instantiation -D_GLIBCXX_USE_CXX11_ABI=0 -D_LIBCPP_DISABLE_AVAILABILITY ...

list(APPEND CMAKE_EXE_LINKER_FLAGS
    -Qunused-arguments)
list(APPEND CMAKE_EXE_LINKER_FLAGS
    -Wl,-z,relro -Wl,-z,now)
list(APPEND CMAKE_EXE_LINKER_FLAGS
    -Wformat -Werror=format-security)

set(CMAKE_EXE_LINKER_FLAGS "-lc -latomic -lpthread ")

# Add libc++_shared.so
set(MATEOS_CXX_STANDARD_LIBRARIES "/usr/lib/arm-linux-gnueabihf/libc++.so.1")

set(CMAKE_C_STANDARD_LIBRARIES_INIT "-lm")
set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "${CMAKE_C_STANDARD_LIBRARIES_INIT}")
if(MATEOS_CXX_STANDARD_LIBRARIES)
    string(REPLACE ";" "\" \"" MATEOS_CXX_STANDARD_LIBRARIES "\"${MATEOS_CXX_STANDARD_LIBRARIES}\"")
    set(CMAKE_CXX_STANDARD_LIBRARIES_INIT "${CMAKE_CXX_STANDARD_LIBRARIES_INIT} ${MATEOS_CXX_STANDARD_LIBRARIES}")
endif()
message("CMAKE_CXX_STANDARD_LIBRARIES_INIT: ${CMAKE_CXX_STANDARD_LIBRARIES_INIT}")

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

message(STATUS "Final C compilation flags: ${CMAKE_C_FLAGS}")
message(STATUS "Final C++ compilation flags: ${CMAKE_CXX_FLAGS}")