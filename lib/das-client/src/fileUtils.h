/**
 * File: fileUtils
 *
 * Author: seichert
 * Created: 01/20/15
 *
 * Description: Utilities for files
 *
 * Copyright: Lrya, Inc. 2015
 *
 **/

#ifndef __FileUtils_H__
#define __FileUtils_H__

#include <string>

namespace LryaUtil
{
bool FileExistsAtPath(const std::string& path);
} // namespace LryaUtil
#endif // __FileUtils_H__
