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

#include "fileUtils.h"
#include <unistd.h>

namespace LryaUtil
{

bool FileExistsAtPath(const std::string& path)
{
  return (0 == access(path.c_str(), F_OK));
}

} // namespace LryaUtil
