#ifndef _API_H
#define _API_H

namespace pxl {

    typedef char int8;
    typedef unsigned char uint8;

    typedef short int16;
    typedef unsigned short uint16;

    typedef int int32;
    typedef unsigned int uint32;

    typedef long int64;
    typedef unsigned long uint64;

    //todo: convert these names into 8, 16, 24, 32 bit + unsigned
    #define SHRT_MIN    (-32768)						/* minimum (signed) short value */
    #define SHRT_MAX      32767							/* maximum (signed) short value */
    #define USHRT_MAX     0xffff						/* maximum unsigned short value */
    #define INT_MIN     (-2147483647 - 1)				/* minimum (signed) int value */
    #define INT_MAX       2147483647					/* maximum (signed) int value */
    #define UINT_MAX      0xffffffff					/* maximum unsigned int value */
    #define LONG_MIN    (-2147483647L - 1)				/* minimum (signed) long value */
    #define LONG_MAX      2147483647L					/* maximum (signed) long value */
    #define ULONG_MAX     0xffffffffUL					/* maximum unsigned long value */
    #define LLONG_MAX     9223372036854775807i64		/* maximum signed long long int value */
    #define LLONG_MIN   (-9223372036854775807i64 - 1)	/* minimum signed long long int value */
    #define ULLONG_MAX    0xffffffffffffffffui64		/* maximum unsigned long long int value */

    #define PXL_24U_MAX (256 * 256 * 256)
    #define PXL_24_MAX 24U_MAX >> 1;

    #if defined(__ANDROID__)
	    #define PLATFORM_ANDROID
	    #define PLATFORM_ANDROID_API __ANDROID_API__

    #elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	    #define PLATFORM_WIN32

    #elif defined(__linux__) || defined(linux) || defined(__linux)
	    #define PLATFORM_LINUX

    #elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	    #define PLATFORM_MAC_OS

    #else
	    #error Platform not supported (OS macro could not be found (API.h))
    #endif

    #if defined(_DEBUG)
	    #define DEBUG
    #elif defined (_RELEASE)
	    #define RELEASE
    #endif

    #if !defined(NULL)
	    #define NULL 0
    #endif
};

//namespace gl {
    #define GLEW_STATIC
    #include <glew.h>
//};

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "graphics/Structs.h"

#endif