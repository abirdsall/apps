#ifndef FW_CUBE_COMPONENT
#define FW_CUBE_COMPONENT

#include "fwMaterial.h"
#include "fwComponent.h"
#include "fwCube.h"

namespace fw
{
	class CubeComponent : public Component
	{
	private:
		Cube mCube;
		Material mMaterial;
		
	public:
		CubeComponent(){}
		CubeComponent( v3 position, v3 radius, v4 colour );
		u16* GenerateElements( u16* p, u16 offset ) const;
		f32* GenerateVertices( f32* p, u32 stride ) const;
		f32* GenerateNormals( f32* p, u32 stride ) const;
        f32* GenerateUniforms( f32* p, s32 stride, const v2& u ) const;
        f32* GenerateUniforms( f32* p, s32 stride, const v4& u ) const;
        
		inline const Cube& Cube() const
		{
			return mCube;
		}
		
		inline const Material& Material() const
		{
			return mMaterial;
		}
	};
}

#endif