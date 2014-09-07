#ifndef CORE_H
#define CORE_H

#include <stdio.h> // printf
#include <stdlib.h> // malloc free
#include <sys/sysctl.h> // gettimeofday

#define OsWindows           ( 0 )
#define OsLinux             ( 0 )
#define OsOsx               ( 1 )
#define OsIos               ( 0 )
#define GsOpenGles2         ( OsIos && 1 )
#define GsOpenGles3         ( OsIos && 0 )
#define GsOpenGl3           ( OsOsx )
#define Architecture32Bit   ( 0 )
#define Architecture64Bit   ( 1 )
#define DebugOsWarn         ( 1 )
#define DebugOsAssert       ( 1 )
#define DebugOsPanic        ( 1 )
#define DebugGs             ( 1 )
#define Null                ( 0	)
#define MaxS32              ( 2147483647 )
#define MaxU32              ( 4294967295UL )
#define MaxF32              ( 3.402823466e+38f )
#define MaxF64              ( 1.7976931348623158e+308 )
#define Pi                  ( 3.1415926535897932f )
#define PiMul2              ( 6.2831853071795865f )
#define PiDiv2              ( 1.5707963267948966f )
#define PiDiv4              ( 0.7853981633974483f )
#define ZNear               ( 0.01f )
#define ZFar                ( 1000.0f )

typedef char c8;
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned char byte_t;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

#if OsWindows

typedef signed __int64 s64;
typedef unsigned __int64 u64;

#elif OsLinux

typedef signed long long s64;
typedef unsigned long long u64;

#elif OsOsx

typedef signed long long s64;
typedef unsigned long long u64;

    #if defined(__GNUC__) && defined(__SIZE_TYPE__)

    typedef __SIZE_TYPE__ size_t;

    #else

    typedef unsigned long size_t;

    #endif

#elif OsIos

typedef signed long long s64;
typedef unsigned long long u64;
typedef unsigned long size_t;

#endif

#if Architecture32Bit
typedef u32 hwInt;
#elif Architecture64Bit
typedef u64 hwInt;
#endif

#undef min
#undef max

#include "coreAssert.h"
#include "coreMath.h"
#include "coreMemory.h"
#include "coreArray.h"
#include "corePool.h"
#include "coreString.h"
#include "coreTime.h"
#include "coreVector.h"
#include "coreMatrix.h"

using namespace core;

#endif