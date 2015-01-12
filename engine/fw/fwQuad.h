#ifndef FW_QUAD2D
#define FW_QUAD2D

namespace fw
{
    u16* QuadGenElements( u16* p, u16 offset );
    
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords, const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords, const v4& colour );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const v4& colour );
    
    f32* Quad3dGenVertices( f32* p,
                           const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD,
                           const fw::Rect& tcoords,
                           const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD );
    f32* Quad3dGenVertices( f32* p,
                           const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD,
                           const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD );
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const fw::Rect& tcoords );
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const fw::Rect& tcoords, f32 tcoordZ );
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const fw::Rect& tcoords, const v4& colour );
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const v4& colour );
}

#endif