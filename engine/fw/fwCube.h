#ifndef FW_CUBE
#define FW_CUBE

namespace fw
{
    u16* CubeGenElements( u16* p, u16 offset );
    f32* CubeGenPositionsNormalsColours( f32* p, const v3& position, const v3& radius, const v4& colour );
    f32* CubeGenPositionsColours( f32* p, const v3& position, const v3& radius, const v4& colour );
    f32* CubeGenPositions( f32* p, const v3& position, const v3& radius );
}

#endif
