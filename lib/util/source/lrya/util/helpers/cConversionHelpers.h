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

#ifndef __Util_Helpers_CConversionHelpers_H__
#define __Util_Helpers_CConversionHelpers_H__


#include "util/export/export.h"
#include "stdint.h"
#include "stddef.h"


LRYA_C_BEGIN


LRYA_EXPORT uint32_t   LryaGetStringLength(const char* inString);
LRYA_EXPORT uint32_t   LryaCopyStringIntoOutBuffer(const char* inString, char* outBuffer, uint32_t outBufferLen);


LRYA_C_END



#endif // #ifndef __Util_Helpers_CConversionHelpers_H__

