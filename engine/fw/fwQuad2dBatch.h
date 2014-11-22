#ifndef FW_QUAD2D_BATCH
#define FW_QUAD2D_BATCH

namespace fw
{
    enum Quad2dShader
    {
        Quad2dShaderFilled,
        Quad2dShaderFilledCustom,
        Quad2dShaderTextured,
        Quad2dShaderTexturedCustom,
        Quad2dShaderTinted,
        Quad2dShaderTintedCustom
    };
    
    void InitQuad2dBatches();
    void KillQuad2dBatches();
    void TickQuad2dBatches();
    
    void BatchQuad2dBegin( Quad2dShader shader, gs::TextureHandle texture );
    void BatchQuad2d( const Rect& vcoords, const v4& colour );
    void BatchQuad2d( const Rect& vcoords, const Rect& tcoords );
    void BatchQuad2d( const Rect& vcoords, const Rect& tcoords, const v4& colour );
    void BatchQuad2dEnd( gs::Primitive primitive );
    
    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const v4& colour, bool wireframe = false );
    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const Rect& tcoords );
    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const Rect& tcoords, const v4& colour );
}

#endif