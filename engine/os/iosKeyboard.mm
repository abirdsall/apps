#include "os.h"

#if kBuildIos

namespace os
{
	//extern void OnKey( const eKey key, const bool pressed );

	void OnModifierChange( const u32 modifiers )
	{
	}

	void OnKeyChange( const u32 keyCode, const bool pressed )
	{
	}
}

#endif
