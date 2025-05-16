#ifndef PROPERTIES_HELPER_H
#define PROPERTIES_HELPER_H

#include <cstdint>
// #include <sys/system_properties.h>
#define PROP_NAME_MAX   32
#define PROP_VALUE_MAX  92

// 定义一个类似PROPERTY_VALUE_MAX的宏，用于限制读取值的长度，可根据实际情况调整
#define PROPERTY_KEY_MAX   PROP_NAME_MAX
#define PROPERTY_VALUE_MAX  PROP_VALUE_MAX

// 声明函数，与之前在树莓派上模拟实现的cpp文件中的函数原型匹配，用于在其他源文件中调用

// 模拟实现的property_get函数，从模拟属性存储中获取指定属性的值，若获取失败可使用默认值填充
int property_get(const char *key, char *value, const char *default_value);

// 模拟实现的property_get_bool函数，将获取到的属性值转换为布尔值（按特定规则），若转换失败返回默认布尔值
int8_t property_get_bool(const char *key, int8_t default_value);

// 模拟实现的property_get_int64函数，将获取到的属性值转换为int64_t类型整数，若转换失败返回默认整数值
int64_t property_get_int64(const char *key, int64_t default_value);

// 模拟实现的property_get_int32函数，将获取到的属性值转换为int32_t类型整数，若转换失败返回默认整数值
int32_t property_get_int32(const char *key, int32_t default_value);

// 模拟实现的property_set函数，用于设置属性（简单模拟写入到文件等方式），成功返回0，失败返回-1
int property_set(const char *key, const char *value);

// 模拟实现的property_list函数，遍历所有模拟属性，并针对每个属性执行传入的回调函数，成功返回0，失败返回-1
int property_list(void (*propfn)(const char *key, const char *value, void *cookie), void *cookie);

#endif  // PROPERTIES_HELPER_H