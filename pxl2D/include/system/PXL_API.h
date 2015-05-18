#ifndef _PXL_API_H
#define _PXL_API_H

typedef char int8;
typedef unsigned char uint8;

typedef short int16;
typedef unsigned short uint16;

typedef int int32;
typedef unsigned int uint32;

typedef long int64;
typedef unsigned long uint64;

//#define float   float

#define PXL_SHRT_MIN    (-32768)						/* minimum (signed) short value */
#define PXL_SHRT_MAX      32767							/* maximum (signed) short value */
#define PXL_USHRT_MAX     0xffff						/* maximum unsigned short value */
#define PXL_INT_MIN     (-2147483647 - 1)				/* minimum (signed) int value */
#define PXL_INT_MAX       2147483647					/* maximum (signed) int value */
#define PXL_UINT_MAX      0xffffffff					/* maximum unsigned int value */
#define PXL_LONG_MIN    (-2147483647L - 1)				/* minimum (signed) long value */
#define PXL_LONG_MAX      2147483647L					/* maximum (signed) long value */
#define PXL_ULONG_MAX     0xffffffffUL					/* maximum unsigned long value */
#define PXL_LLONG_MAX     9223372036854775807i64		/* maximum signed long long int value */
#define PXL_LLONG_MIN   (-9223372036854775807i64 - 1)	/* minimum signed long long int value */
#define PXL_ULLONG_MAX    0xffffffffffffffffui64		/* maximum unsigned long long int value */

#if defined(__ANDROID__)
	#define PXL_PLATFORM_ANDROID
	#define PXL_PLATFORM_ANDROID_API __ANDROID_API__

#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	#define PXL_PLATFORM_WIN32

#elif defined(__linux__) || defined(linux) || defined(__linux)
	#define PXL_PLATFORM_LINUX

#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	#define PXL_PLATFORM_MAC_OS

#else
	#error Platform not supported (OS macro could not be found (PXL_API.h))
#endif

#if defined(_DEBUG)
	#define PXL_DEBUG
#elif defined (_RELEASE)
	#define PXL_RELEASE
#endif

#if !defined(NULL)
	#define NULL 0
#endif

#endif