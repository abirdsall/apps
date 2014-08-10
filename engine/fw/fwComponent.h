#ifndef FW_COMPONENT
#define FW_COMPONENT

namespace fw
{
    enum eComponent
	{
		eComponentRender,
		eComponentLimit
	};

    typedef u32 ComponentHandle;
	const u32 kComponentInvalid = kMaxU32;
    
	class Component
	{
	};
}

#endif