#ifndef FW_OBJECT
#define FW_OBJECT

#include "os.h"
#include "fwComponent.h"

namespace fw
{
    typedef u32 ObjectHandle;
	const u32 kObjectInvalid = kMaxU32;
    
 	class Object
	{
    public:
        Object()
        {
            for(s32 i = 0; i < eComponentLimit; i++)
            {
                mComponents[ i ] = kComponentInvalid;
            }
        }
        
    private:
        ComponentHandle mComponents[ eComponentLimit ];
	};
}

#endif
