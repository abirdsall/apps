#include "fw.h"

namespace fw
{
	void ComponentFactory::Init( s32 limit )
	{
		mCount = 0;
		mLimit = limit;
		
		mComponents = ( Component* )core::alloc( ComponentSize() * limit );
	}
	
	ComponentHandle ComponentFactory::Add( const Component& component )
	{
		ComponentHandle handle = kComponentInvalid;
		
		if( mCount < mLimit )
		{
			handle = mCount++;
		}
		return handle;
	}
    
	void ComponentFactory::Kill()
	{
		if( mComponents )
		{
			core::free( mComponents );
			mComponents = kNull;
		}
	}
	
	size_t ComponentFactory::ComponentSize()
	{
		return sizeof( Component );
	}
}
