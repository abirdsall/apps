#ifndef FW_CUBE
#define FW_CUBE

#include "fwObject.h"
#include "os.h"
#include "cMath.h"
#include "vector.h"

namespace fw
{
	class Cube
	{
	public:
		v3 mPosition;
		v3 mRadius;
		
		Cube(){}
		Cube( v3 position, v3 radius );
		
		inline const v3& Position() const
		{
			return mPosition;
		}
		
		inline const v3& Radius() const
		{
			return mRadius;
		}
	};
}

#endif
