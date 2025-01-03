# 获取当前已有的C++编译选项
get_property(OLD_C_FLAGS GLOBAL PROPERTY CMAKE_C_FLAGS)
get_property(OLD_CXX_FLAGS GLOBAL PROPERTY CMAKE_CXX_FLAGS)
# 添加禁用警告的选项，注意使用空格进行连接
set(CMAKE_C_FLAGS "${OLD_C_FLAGS} -Wno-unused-private-field ")
set(CMAKE_CXX_FLAGS "${OLD_CXX_FLAGS} -Wno-unused-private-field -Wno-format -std=c++14")


message(STATUS "Final C compilation flags: ${CMAKE_C_FLAGS}")
message(STATUS "Final C++ compilation flags: ${CMAKE_CXX_FLAGS}")