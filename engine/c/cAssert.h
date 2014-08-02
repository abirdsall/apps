#ifndef OS_ASSERT
#define OS_ASSERT

namespace os
{
#if kEnableAssert || kEnablePanic
	
	void Assert( const c8* fileName, s32 lineNumber, bool success, bool fatal );
	
#endif
}
	
#if kEnableAssert
	
#define ASSERT( success ) os::Assert( __FILE__, __LINE__, ( success ), false )
	
#else
	
#define ASSERT( success )
	
#endif
	
	
#if kEnablePanic
	
#define PANIC( success ) os::Assert( __FILE__, __LINE__, ( success ), true )
	
#else
	
#define PANIC( success )
	
#endif

#endif