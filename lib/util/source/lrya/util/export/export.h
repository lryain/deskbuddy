//
//  export.h
//  BaseStation
//
//  Created by Mark Pauley on 10/9/14.
//  Copyright (c) 2014 Lrya. All rights reserved.
//

#ifndef Util_Export_h_
#define Util_Export_h_

#define LRYA_VISIBLE __attribute__((visibility("default")))

#define LRYA_EXPORT	LRYA_VISIBLE extern
#define LRYA_INTERN __attribute__((visibility("hidden")))  extern

#define LRYA_TRUE 1
#define LRYA_FALSE 0
#define PRINT_EXTERNAL_CALLS 0

#ifdef __cplusplus
#define LRYA_C_BEGIN extern "C" {
#define LRYA_C_END }
#define LRYA_C_EXTERN extern "C"
#else
#define LRYA_C_BEGIN // C
#define LRYA_C_END   // C
#define LRYA_C_EXTERN extern
#endif


#endif
