#ifndef CORE_H
#define CORE_H

#include <stdio.h> // printf
#include <stdlib.h> // malloc free
#include <sys/sysctl.h> // gettimeofday

//TODO - make a config file
#define kBuildLibc        ( 1							)

#define kBuildWindows     ( 0							)
#define kBuildLinux       ( 0							)
#define kBuildMac         ( 0                           )
#define kBuildIos         ( 1                           )

#define kBuildOpenGles2   ( kBuildIos && 1              )
#define kBuildOpenGles3   ( kBuildIos && 0              )
#define kBuildOpenGl3     ( kBuildMac					)

#define kBuild32bit       ( 0							)
#define kBuild64bit       ( 1							)

#define kEnableWarn       ( 1							)
#define kEnablePanic      ( 1							)
#define kEnableAssert     ( 1							)

#define kDebug            ( 1							)
#define kGsDebug          ( 1                           )

#define kNull             ( 0							)
#define kKb               ( 1024                        )
#define kMb               ( 1048576                     )
#define kMinS8            (-128                         )
#define kMaxS8            ( 127							)
#define kMinS32           (-2147483648                  )
#define kMaxS32           ( 2147483647					)
#define kMinU32           ( 0							)
#define kMaxU32           ( 4294967295UL				)
#define kMinF32           (-3.402823466e+38f			)
#define kMaxF32           ( 3.402823466e+38f			)
#define kMinF64           (-1.7976931348623158e+308		)
#define kMaxF64           ( 1.7976931348623158e+308		)
#define kPi               ( 3.1415926535897932f			)
#define kPiMul2           ( 6.2831853071795865f			)
#define kPiDiv2           ( 1.5707963267948966f			)
#define kPiDiv4           ( 0.7853981633974483f			)
#define kRad360           ( kPiMul2						)
#define kRad180           ( kPi							)
#define kRad90            ( kPiDiv2						)
#define kRad45            ( kPiDiv4						)
#define kSqrt2            ( 1.4142135623730951f			)
#define k1Div8            ( 0.125f                      )
#define k1Div16           ( 0.0625f                     )
#define k1Div32           ( 0.03125f                    )
#define k1Div64           ( 0.015625f                   )

#define kFullscreen       ( 0							)
#define kBpp              ( 32							)
#define kZNear            ( 0.01f						)
#define kZFar             ( 1000.0f						)

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

#if kBuildWindows
typedef signed __int64 s64;
typedef unsigned __int64 u64;
#elif kBuildLinux
typedef signed long long s64;
typedef unsigned long long u64;
#elif kBuildMac
typedef signed long long s64;
typedef unsigned long long u64;
#if defined(__GNUC__) && defined(__SIZE_TYPE__)
typedef __SIZE_TYPE__ size_t;
#else
typedef unsigned long size_t;
#endif
#elif kBuildIos
typedef signed long long s64;
typedef unsigned long long u64;
typedef unsigned long size_t;
#endif

#if kBuild32bit
typedef u32 hwInt;
#elif kBuild64bit
typedef u64 hwInt;
#endif

#undef min
#undef max

#include "coreAssert.h"
#include "coreMath.h"
#include "coreMemory.h"
#include "corePool.h"
#include "coreString.h"
#include "coreStringData.h"
#include "coreStringObject.h"
#include "coreTime.h"
#include "coreVector.h"
#include "coreMatrix.h"

using namespace core;

#endif