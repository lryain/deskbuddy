/**
 * File: CConversionHelpers
 *
 * Author: Mark Wesley
 * Created: 10/14/2014
 *
 * Description: Helper functions for more easily copying types out (pure C interface)
 *
 * Copyright: Lrya, Inc. 2014
 *
 **/

#include "util/helpers/cConversionHelpers.h"
#include "util/math/numericCast.h"
#include <string>
#include <cstring>

uint32_t   LryaGetStringLength(const char* inString)
{
  const size_t stringLength = strlen(inString);
  return Lrya::Util::numeric_cast<uint32_t>(stringLength);
}


uint32_t LryaCopyStringIntoOutBuffer(const char* inString, char* outBuffer, uint32_t outBufferLen)
{
  const size_t inStringLen = strlen(inString);
  
  if ((outBuffer != NULL) && (outBufferLen > 0))
  {
    const size_t maxCharsToCopy = (inStringLen < outBufferLen) ? inStringLen : (outBufferLen - 1);
    assert(inStringLen <= maxCharsToCopy);           // otherwise string will be truncated
    
    strncpy(outBuffer, inString, maxCharsToCopy);
    outBuffer[maxCharsToCopy] = 0;                   // ensure string is null terminated even if truncated
    
    return Lrya::Util::numeric_cast<uint32_t>(maxCharsToCopy);
  }
  else
  {
    return Lrya::Util::numeric_cast<uint32_t>(inStringLen);
  }
}



