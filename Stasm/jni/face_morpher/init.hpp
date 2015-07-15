/*
 * __init__.cpp
 *
 *  Created on: 2015年6月23日
 *      Author: ns
 */

#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <math.h>
#include <android/log.h>
#include <stdio.h>

#include "stasm/stasm_lib.h"


#define len(array)  (sizeof(array) / sizeof((array)[0]))

//#define printf(format,args...) __android_log_print(ANDROID_LOG_INFO, "MFLib", format,##args)
#define aprintf(format,args...) __android_log_print(ANDROID_LOG_INFO, "MFLib", format,##args)
//#define sprintf(tag,format,args...) __android_log_print(ANDROID_LOG_INFO, tag, format,##args)
#define farrayprintf(array) aprintf("array %f %f %f %f %f %f",array[0],array[1],array[2],array[3],array[4],array[5])
#define matprintf(mat) aprintf("mat cols : %i rows : %i data : %i ptr : %i",mat->cols,mat->rows,mat->data,mat)
