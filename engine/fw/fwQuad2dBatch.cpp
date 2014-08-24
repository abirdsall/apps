#include "fw.h"

namespace fw
{
    static const s32 BatchCapacityFilled = 256;
    static const s32 BatchCapacityTextured = 256;
    static const s32 BatchCapacityTinted = 256;

    static DrawBatchHandle sBatchFilled = InvalidDrawBatchHandle;
    static DrawBatchHandle sBatchTextured = InvalidDrawBatchHandle;
    static DrawBatchHandle sBatchTinted = InvalidDrawBatchHandle;

    static gs::ShaderHandle sShaderFilled = gs::InvalidShaderHandle;
    static gs::ShaderHandle sShaderTextured = gs::InvalidShaderHandle;
    static gs::ShaderHandle sShaderTinted = gs::InvalidShaderHandle;

    static DrawBatchHandle sBatchActive = InvalidDrawBatchHandle;
    static gs::ShaderHandle sShaderActive = gs::InvalidShaderHandle;

    void InitQuad2dBatches()
    {
        if(sBatchFilled == InvalidDrawBatchHandle)
        {
            sBatchFilled = DrawBatchNew(BatchCapacityFilled, 6, 4, 2, 0, 4, 0);
        }
        
        if(sBatchTextured == InvalidDrawBatchHandle)
        {
            sBatchTextured = DrawBatchNew(BatchCapacityTextured, 6, 4, 2, 0, 0, 2);
        }
        
        if(sBatchTinted == InvalidDrawBatchHandle)
        {
            sBatchTinted = DrawBatchNew(BatchCapacityTinted, 6, 4, 2, 0, 4, 2);
        }
        
        if(sShaderFilled == gs::InvalidShaderHandle)
        {
            sShaderFilled = ShaderMake2d( true, false );
        }
        
        if(sShaderTextured == gs::InvalidShaderHandle)
        {
            sShaderTextured = ShaderMake2d( false, true );
        }
        
        if(sShaderTinted == gs::InvalidShaderHandle)
        {
            sShaderTinted = ShaderMake2d( true, true );
        }
        
        sBatchActive = InvalidDrawBatchHandle;
        sShaderActive = gs::InvalidShaderHandle;
    }
    
    void KillQuad2dBatches()
    {
        if(sBatchFilled != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( sBatchFilled );
            sBatchFilled = InvalidDrawBatchHandle;
        }
        
        if(sBatchTextured != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( sBatchTextured );
            sBatchTextured = InvalidDrawBatchHandle;
        }
        
        if(sBatchTinted != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( sBatchTinted );
            sBatchTinted = InvalidDrawBatchHandle;
        }
        
        if(sShaderFilled != gs::InvalidShaderHandle)
        {
            gs::ShaderDelete( sShaderFilled );
            sShaderFilled = gs::InvalidShaderHandle;
        }
        
        if(sShaderTextured != gs::InvalidShaderHandle)
        {
            gs::ShaderDelete( sShaderTextured );
            sShaderTextured = gs::InvalidShaderHandle;
        }
        
        if(sShaderTinted != gs::InvalidShaderHandle)
        {
            gs::ShaderDelete( sShaderTinted );
            sShaderTinted = gs::InvalidShaderHandle;
        }
        
        sBatchActive = InvalidDrawBatchHandle;
        sShaderActive = gs::InvalidShaderHandle;
    }
    
    DrawBatchHandle BatchFromQuad2dShader( Quad2dShader shader )
    {
        switch( shader )
        {
            case Quad2dShaderFilled:
            case Quad2dShaderFilledCustom:
            {
                return sBatchFilled;
            }
            case Quad2dShaderTextured:
            case Quad2dShaderTexturedCustom:
            {
                return sBatchTextured;
            }
            case Quad2dShaderTinted:
            case Quad2dShaderTintedCustom:
            {
                return sBatchTinted;
            }
        }
    }
    
    gs::ShaderHandle ShaderFromQuad2dShader( Quad2dShader shader )
    {
        switch( shader )
        {
            case Quad2dShaderFilled:
            {
                return sShaderFilled;
            }
            case Quad2dShaderTextured:
            {
                return sShaderTextured;
            }
            case Quad2dShaderTinted:
            {
                return sShaderTinted;
            }
            case Quad2dShaderFilledCustom:
            case Quad2dShaderTexturedCustom:
            case Quad2dShaderTintedCustom:
            {
                return gs::InvalidShaderHandle;
            }
        }
    }
    
    void BatchQuad2dBegin( Quad2dShader shader, gs::TextureHandle texture )
    {
        sBatchActive = BatchFromQuad2dShader( shader );
        sShaderActive = ShaderFromQuad2dShader( shader );
        
        if( sShaderActive != gs::InvalidShaderHandle )
        {
            gs::ShaderSet( sShaderActive );
        }
        
        if( texture != gs::InvalidTextureHandle )
        {
            gs::TextureSet( "texture0", texture );
        }
    }
    
    void BatchQuad2d( const Rect& vcoords, const v4& colour )
    {
        ASSERT( sBatchActive == sBatchFilled );
        Quad2dGenElements( DrawBatchElementPtr( sBatchActive ), DrawBatchVertexCount( sBatchActive ) );
        Quad2dGenVertices( DrawBatchVertexPtr( sBatchActive ), vcoords, colour );
        DrawBatchIncrement( sBatchActive );
    }
    
    void BatchQuad2d( const Rect& vcoords, const Rect& tcoords )
    {
        ASSERT( sBatchActive == sBatchTextured );
        Quad2dGenElements( DrawBatchElementPtr( sBatchActive ), DrawBatchVertexCount( sBatchActive ) );
        Quad2dGenVertices( DrawBatchVertexPtr( sBatchActive ), vcoords, tcoords );
        DrawBatchIncrement( sBatchActive );
    }
    
    void BatchQuad2d( const Rect& vcoords, const Rect& tcoords, const v4& colour )
    {
        ASSERT( sBatchActive == sBatchTinted );
        Quad2dGenElements( DrawBatchElementPtr( sBatchActive ), DrawBatchVertexCount( sBatchActive ) );
        Quad2dGenVertices( DrawBatchVertexPtr( sBatchActive ), vcoords, tcoords, colour );
        DrawBatchIncrement( sBatchActive );
    }
    
    void BatchQuad2dEnd( gs::ePrim primitive )
    {
        DrawBatchFlush( sBatchActive, primitive );
    }

    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const v4& colour, bool wireframe )
    {
        BatchQuad2dBegin( shader, gs::InvalidTextureHandle );
        BatchQuad2d( vcoords, colour );
        BatchQuad2dEnd( wireframe ? gs::ePrimLines : gs::ePrimTriangles);
    }
    
    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const Rect& tcoords )
    {
        BatchQuad2dBegin( shader, gs::InvalidTextureHandle );
        BatchQuad2d( vcoords, tcoords );
        BatchQuad2dEnd( gs::ePrimTriangles);
    }
    
    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const Rect& tcoords, const v4& colour )
    {
        BatchQuad2dBegin( shader, gs::InvalidTextureHandle );
        BatchQuad2d( vcoords, tcoords, colour );
        BatchQuad2dEnd( gs::ePrimTriangles);
    }
}
