#include "core.h"
#include "os.h"
#include "gs.h"

#if DebugOsAssert || DebugOsPanic

namespace core
{
	void Assert( const c8* fileName, s32 lineNumber, bool success, bool fatal )
	{
		if ( !success )
		{
			String reason = "assert\nfile ";
            
            reason += fileName;
            reason +="\nline ";
            reason = reason + lineNumber;
            reason += "\npress c to continue\npress q to quit";
			
            printf( "%s", reason.toStr() );
		}
	}
}

#endif