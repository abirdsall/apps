#include "fwCubeComponent.h"

namespace fw
{
	CubeComponent::CubeComponent( v3 position, v3 radius, v4 colour )
	{
		mCube = fw::Cube( position, radius );
		mMaterial = fw::Material( colour );
	}
	
	u16* CubeComponent::GenerateElements( u16* p, u16 offset ) const
	{
		*p ++=  0 + offset; *p ++=  6 + offset; *p ++= 18 + offset; *p ++= 18 + offset; *p ++= 12 + offset; *p ++=  0 + offset;
		*p ++=  3 + offset; *p ++= 15 + offset; *p ++= 21 + offset; *p ++= 21 + offset; *p ++=  9 + offset; *p ++=  3 + offset;
		*p ++=  1 + offset; *p ++=  4 + offset; *p ++= 10 + offset; *p ++= 10 + offset; *p ++=  7 + offset; *p ++=  1 + offset;
		*p ++= 13 + offset; *p ++= 19 + offset; *p ++= 22 + offset; *p ++= 22 + offset; *p ++= 16 + offset; *p ++= 13 + offset;
		*p ++=  2 + offset; *p ++= 14 + offset; *p ++= 17 + offset; *p ++= 17 + offset; *p ++=  5 + offset; *p ++=  2 + offset;
		*p ++=  8 + offset; *p ++= 11 + offset; *p ++= 23 + offset; *p ++= 23 + offset; *p ++= 20 + offset; *p ++=  8 + offset;
		return p;
	}
	
	f32* CubeComponent::GenerateVertices( f32* p, u32 stride ) const
	{
		const v3& position = mCube.Position();
		const v3& radius = mCube.Radius();

		stride -= 3;
		*p ++= position.x - radius.x; *p ++= position.y - radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y - radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y - radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y - radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y - radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y - radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y - radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y - radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y - radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y - radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y - radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y - radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y + radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y + radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y + radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y + radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y + radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y + radius.y; *p ++= position.z - radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y + radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y + radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x - radius.x; *p ++= position.y + radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y + radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y + radius.y; *p ++= position.z + radius.z; p += stride;
		*p ++= position.x + radius.x; *p ++= position.y + radius.y; *p ++= position.z + radius.z; p += stride;
		return p;
	}
	
	f32* CubeComponent::GenerateNormals( f32* p, u32 stride ) const
	{
		stride -= 3;
		*p ++=-1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++=-1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++=-1.0f; p += stride;
		*p ++= 1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++=-1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++=-1.0f; p += stride;
		*p ++=-1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++=-1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++= 1.0f; p += stride;
		*p ++= 1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++=-1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++= 1.0f; p += stride;
		*p ++=-1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++=-1.0f; p += stride;
		*p ++= 1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++=-1.0f; p += stride;
		*p ++=-1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++= 1.0f; p += stride;
		*p ++= 1.0f; *p ++= 0.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 1.0f; *p ++= 0.0f; p += stride;
		*p ++= 0.0f; *p ++= 0.0f; *p ++= 1.0f; p += stride;
		return p;
	}
    
	f32* CubeComponent::GenerateUniforms( f32* p, s32 stride, const v2& u ) const
	{
		stride -= 2;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		*p ++= u.x; *p ++= u.y; p += stride;
		return p;
	}
	
	f32* CubeComponent::GenerateUniforms( f32* p, s32 stride, const v4& u ) const
	{
		stride -= 4;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		return p;
	}
}
// y
// 4 5
//  6 7
// 0 1 x
//  2 3
//   z