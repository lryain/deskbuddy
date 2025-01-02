/**
 * File: stringHelpers
 *
 * Author: Mark Wesley
 * Created: 05/11/16
 *
 * Description: String helpers functions beyond those provided by standard ANSI C
 *
 * Copyright: Lrya, Inc. 2016
 *
 **/


#ifndef __Util_String_StringHelpers_H__
#define __Util_String_StringHelpers_H__

#include <vector>
#include <string>

namespace Lrya {
namespace Util {

// case insensitive version of strcmp as it isn't provided as standard
// in Lrya::Util namespace so won't clash with one provided by some compilers
int stricmp(const char* string1, const char* string2);

std::vector<std::string> SplitString(const std::string& s, char delim);

} // namespace Util
} // namespace Lrya

#endif // __Util_String_StringHelpers_H__
