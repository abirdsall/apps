#ifndef CORE_ASSERT
#define CORE_ASSERT

namespace core
{
#if DebugOsAssert || DebugOsPanic
	
	void Assert( const c8* fileName, s32 lineNumber, bool success, bool fatal );
	
#endif
}
	
#if DebugOsAssert
	
#define ASSERT( success ) core::Assert( __FILE__, __LINE__, ( success ), false )
	
#else
	
#define ASSERT( success )
	
#endif
	
	
#if DebugOsPanic
	
#define PANIC( success ) core::Assert( __FILE__, __LINE__, ( success ), true )
	
#else
	
#define PANIC( success )
	
#endif

#endif