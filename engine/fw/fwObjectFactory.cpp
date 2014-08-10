#include "fw.h"

namespace fw
{
	void ObjectFactory::Init( s32 limit )
	{
		mCount = 0;
		mLimit = limit;
		
		mObjects = ( Object* )memory::alloc( ObjectSize() * limit );
	}
	
	ObjectHandle ObjectFactory::Add( const Object& object )
	{
		ObjectHandle handle = kObjectInvalid;
		
		if( mCount < mLimit )
		{
			handle = mCount++;
		}
		return handle;
	}
		
	void ObjectFactory::Kill()
	{
		if( mObjects )
		{
			memory::free( mObjects );
			mObjects = kNull;
		}
	}
	
	size_t ObjectFactory::ObjectSize()
	{
		return sizeof( Object );
	}
}
