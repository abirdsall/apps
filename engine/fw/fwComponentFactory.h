#ifndef FW_COMPONENT_FACTORY
#define FW_COMPONENT_FACTORY

#include "os.h"
#include "fwComponent.h"

namespace fw
{	
	class ComponentFactory
	{
	public:
		virtual void Init( s32 limit );
		virtual ComponentHandle Add( const Component& component );
		virtual void Kill();
		
	protected:
		Component* mComponents;
		
		s32 mCount;
		s32 mLimit;
        
		virtual size_t ComponentSize() = 0;
	};
}

#endif
