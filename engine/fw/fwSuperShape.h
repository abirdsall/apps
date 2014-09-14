#ifndef FW_SUPERSHAPE
#define FW_SUPERSHAPE

namespace fw
{
    void SuperShapeGen( u16* e, u16 offset, f32* v, u16 detail,
                       const v3& position,
                       const v3& radius,
                       const v4& colour,
                       f32 ss1m, f32 ss1a, f32 ss1b, f32 ss1n1, f32 ss1n2, f32 ss1n3,
                       f32 ss2m, f32 ss2a, f32 ss2b, f32 ss2n1, f32 ss2n2, f32 ss2n3 );
    
    void SuperShapeGenRadiosity( u16* e, u16 offset, f32* v, u16 detail,
                                const v3& position,
                                const v3& radius,
                                const v4& colour,
                                f32 ss1m, f32 ss1a, f32 ss1b, f32 ss1n1, f32 ss1n2, f32 ss1n3,
                                f32 ss2m, f32 ss2a, f32 ss2b, f32 ss2n1, f32 ss2n2, f32 ss2n3 );
}

#endif
