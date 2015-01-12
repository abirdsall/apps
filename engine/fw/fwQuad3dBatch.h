#ifndef FW_QUAD3D_BATCH
#define FW_QUAD3D_BATCH

namespace fw
{
    enum Quad3dShader
    {
        Quad3dShaderFilled,
        Quad3dShaderFilledCustom,
        Quad3dShaderTextured,
        Quad3dShaderTexturedCustom,
        Quad3dShaderTextured3d,
        Quad3dShaderTextured3dCustom,
        Quad3dShaderTinted,
        Quad3dShaderTintedCustom
    };
    
    void InitQuad3dBatches();
    void KillQuad3dBatches();
    void TickQuad3dBatches();
    
    void BatchQuad3dBegin( Quad3dShader shader, gs::TextureHandle texture );
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const v4& colour );
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords );
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, f32 tcoordZ );
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, const v4& colour );
    void BatchQuad3dEnd( gs::Primitive primitive );
    
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const v4& colour, bool wireframe = false );
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords );
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, f32 tcoordZ );
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, const v4& colour );
}

#endif