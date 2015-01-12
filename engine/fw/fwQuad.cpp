#include "fw.h"

namespace fw
{
    // 1 3
    // 0 2
    // 201 //012
    // 132

    u16* QuadGenElements( u16* p, u16 offset )
    {
        *p ++= 2 + offset;
        *p ++= 0 + offset;
        *p ++= 1 + offset;
        *p ++= 1 + offset;
        *p ++= 3 + offset;
        *p ++= 2 + offset;
        return p;
    }
    
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords, const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD )
    {
        *p ++= vcoords.Min().x;
        *p ++= vcoords.Min().y;
        *p ++= colourA.x;
        *p ++= colourA.y;
        *p ++= colourA.z;
        *p ++= colourA.w;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoords.Min().x;
        *p ++= vcoords.Max().y;
        *p ++= colourB.x;
        *p ++= colourB.y;
        *p ++= colourB.z;
        *p ++= colourB.w;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Max().y;
        *p ++= vcoords.Max().x;
        *p ++= vcoords.Min().y;
        *p ++= colourC.x;
        *p ++= colourC.y;
        *p ++= colourC.z;
        *p ++= colourC.w;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoords.Max().x;
        *p ++= vcoords.Max().y;
        *p ++= colourD.x;
        *p ++= colourD.y;
        *p ++= colourD.z;
        *p ++= colourD.w;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Max().y;
        return p;
    }
    
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD )
    {
        *p ++= vcoords.Min().x;
        *p ++= vcoords.Min().y;
        *p ++= colourA.x;
        *p ++= colourA.y;
        *p ++= colourA.z;
        *p ++= colourA.w;
        *p ++= vcoords.Min().x;
        *p ++= vcoords.Max().y;
        *p ++= colourB.x;
        *p ++= colourB.y;
        *p ++= colourB.z;
        *p ++= colourB.w;
        *p ++= vcoords.Max().x;
        *p ++= vcoords.Min().y;
        *p ++= colourC.x;
        *p ++= colourC.y;
        *p ++= colourC.z;
        *p ++= colourC.w;
        *p ++= vcoords.Max().x;
        *p ++= vcoords.Max().y;
        *p ++= colourD.x;
        *p ++= colourD.y;
        *p ++= colourD.z;
        *p ++= colourD.w;
        return p;
    }
    
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords )
    {
        *p ++= vcoords.Min().x;
        *p ++= vcoords.Min().y;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoords.Min().x;
        *p ++= vcoords.Max().y;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Max().y;
        *p ++= vcoords.Max().x;
        *p ++= vcoords.Min().y;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoords.Max().x;
        *p ++= vcoords.Max().y;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Max().y;
        return p;
    }
    
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords, const v4& colour )
    {
        return Quad2dGenVertices( p, vcoords, tcoords, colour, colour, colour, colour );
    }

    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const v4& colour )
    {
        return Quad2dGenVertices( p, vcoords, colour, colour, colour, colour );
    }
    
    f32* Quad3dGenVertices( f32* p,
                           const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD,
                           const fw::Rect& tcoords,
                           const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD )
    {
        *p ++= vcoordA.x;
        *p ++= vcoordA.y;
        *p ++= vcoordA.z;
        *p ++= colourA.x;
        *p ++= colourA.y;
        *p ++= colourA.z;
        *p ++= colourA.w;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoordB.x;
        *p ++= vcoordB.y;
        *p ++= vcoordB.z;
        *p ++= colourB.x;
        *p ++= colourB.y;
        *p ++= colourB.z;
        *p ++= colourB.w;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Max().y;
        *p ++= vcoordC.x;
        *p ++= vcoordC.y;
        *p ++= vcoordC.z;
        *p ++= colourC.x;
        *p ++= colourC.y;
        *p ++= colourC.z;
        *p ++= colourC.w;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoordD.x;
        *p ++= vcoordD.y;
        *p ++= vcoordD.z;
        *p ++= colourD.x;
        *p ++= colourD.y;
        *p ++= colourD.z;
        *p ++= colourD.w;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Max().y;
        return p;
    }
    
    f32* Quad3dGenVertices( f32* p,
                           const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD,
                           const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD )
    {
        *p ++= vcoordA.x;
        *p ++= vcoordA.y;
        *p ++= vcoordA.z;
        *p ++= colourA.x;
        *p ++= colourA.y;
        *p ++= colourA.z;
        *p ++= colourA.w;
        *p ++= vcoordB.x;
        *p ++= vcoordB.y;
        *p ++= vcoordB.z;
        *p ++= colourB.x;
        *p ++= colourB.y;
        *p ++= colourB.z;
        *p ++= colourB.w;
        *p ++= vcoordC.x;
        *p ++= vcoordC.y;
        *p ++= vcoordC.z;
        *p ++= colourC.x;
        *p ++= colourC.y;
        *p ++= colourC.z;
        *p ++= colourC.w;
        *p ++= vcoordD.x;
        *p ++= vcoordD.y;
        *p ++= vcoordD.z;
        *p ++= colourD.x;
        *p ++= colourD.y;
        *p ++= colourD.z;
        *p ++= colourD.w;
        return p;
    }
    
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const fw::Rect& tcoords )
    {
        *p ++= vcoordA.x;
        *p ++= vcoordA.y;
        *p ++= vcoordA.z;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoordB.x;
        *p ++= vcoordB.y;
        *p ++= vcoordB.z;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Max().y;
        *p ++= vcoordC.x;
        *p ++= vcoordC.y;
        *p ++= vcoordC.z;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Min().y;
        *p ++= vcoordD.x;
        *p ++= vcoordD.y;
        *p ++= vcoordD.z;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Max().y;
        return p;
    }
    
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const fw::Rect& tcoords, f32 tcoordZ )
    {
        *p ++= vcoordA.x;
        *p ++= vcoordA.y;
        *p ++= vcoordA.z;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Min().y;
        *p ++= tcoordZ;
        *p ++= vcoordB.x;
        *p ++= vcoordB.y;
        *p ++= vcoordB.z;
        *p ++= tcoords.Min().x;
        *p ++= tcoords.Max().y;
        *p ++= tcoordZ;
        *p ++= vcoordC.x;
        *p ++= vcoordC.y;
        *p ++= vcoordC.z;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Min().y;
        *p ++= tcoordZ;
        *p ++= vcoordD.x;
        *p ++= vcoordD.y;
        *p ++= vcoordD.z;
        *p ++= tcoords.Max().x;
        *p ++= tcoords.Max().y;
        *p ++= tcoordZ;
        return p;
    }
    
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const fw::Rect& tcoords, const v4& colour )
    {
        return Quad3dGenVertices( p, vcoordA, vcoordB, vcoordC, vcoordD, tcoords, colour, colour, colour, colour );
    }
    
    f32* Quad3dGenVertices( f32* p, const v3& vcoordA, const v3& vcoordB, const v3& vcoordC, const v3& vcoordD, const v4& colour )
    {
        return Quad3dGenVertices( p, vcoordA, vcoordB, vcoordC, vcoordD, colour, colour, colour, colour );
    }
}