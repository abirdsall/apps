#ifndef MAC
#define MAC

#define MAC_MAX_PATH_LENGTH  8192

#if defined(__OBJC__)
#import <Cocoa/Cocoa.h>
#else
#include <ApplicationServices/ApplicationServices.h>
typedef void* id;
#endif

#endif
