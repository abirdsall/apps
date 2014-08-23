namespace fw
{
    u16* Quad2dGenElements( u16* p, u16 offset );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords, const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const v4& colourA, const v4& colourB, const v4& colourC, const v4& colourD );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const fw::Rect& tcoords, const v4& colour );
    f32* Quad2dGenVertices( f32* p, const fw::Rect& vcoords, const v4& colour );
}