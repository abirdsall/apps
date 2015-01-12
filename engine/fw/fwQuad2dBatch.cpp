#include "fw.h"

namespace fw
{
    static const s32 BatchCapacityFilled = 256;
    static const s32 BatchCapacityTextured = 256;
    static const s32 BatchCapacityTinted = 256;

    static DrawBatchHandle _batchFilled = InvalidDrawBatchHandle;
    static DrawBatchHandle _batchTextured = InvalidDrawBatchHandle;
    static DrawBatchHandle _batchTinted = InvalidDrawBatchHandle;

    static gs::ShaderHandle _shaderFilled = gs::ShaderInvalid;
    static gs::ShaderHandle _shaderTextured = gs::ShaderInvalid;
    static gs::ShaderHandle _shaderTinted = gs::ShaderInvalid;

    static DrawBatchHandle _batchActive = InvalidDrawBatchHandle;
    static gs::ShaderHandle _shaderActive = gs::ShaderInvalid;

    void InitQuad2dBatches()
    {
        if(_batchFilled == InvalidDrawBatchHandle)
        {
            _batchFilled = DrawBatchNew( BatchCapacityFilled, 6, 4, 2, 0, 4, 0 );
        }
        
        if(_batchTextured == InvalidDrawBatchHandle)
        {
            _batchTextured = DrawBatchNew( BatchCapacityTextured, 6, 4, 2, 0, 0, 2 );
        }
        
        if(_batchTinted == InvalidDrawBatchHandle)
        {
            _batchTinted = DrawBatchNew( BatchCapacityTinted, 6, 4, 2, 0, 4, 2 );
        }
        
        if(_shaderFilled == gs::ShaderInvalid)
        {
            _shaderFilled = ShaderMake( eShader2dFill );
        }
        
        if(_shaderTextured == gs::ShaderInvalid)
        {
            _shaderTextured = ShaderMake( eShader2dTexture2d );
        }
        
        if(_shaderTinted == gs::ShaderInvalid)
        {
            _shaderTinted = ShaderMake( eShader2dTexture2dTinted );
        }
        
        _batchActive = InvalidDrawBatchHandle;
        _shaderActive = gs::ShaderInvalid;
    }
    
    void KillQuad2dBatches()
    {
        if(_batchFilled != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchFilled );
            _batchFilled = InvalidDrawBatchHandle;
        }
        
        if(_batchTextured != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchTextured );
            _batchTextured = InvalidDrawBatchHandle;
        }
        
        if(_batchTinted != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchTinted );
            _batchTinted = InvalidDrawBatchHandle;
        }
        
        if(_shaderFilled != gs::ShaderInvalid)
        {
            gs::ShaderDelete( _shaderFilled );
            _shaderFilled = gs::ShaderInvalid;
        }
        
        if(_shaderTextured != gs::ShaderInvalid)
        {
            gs::ShaderDelete( _shaderTextured );
            _shaderTextured = gs::ShaderInvalid;
        }
        
        if(_shaderTinted != gs::ShaderInvalid)
        {
            gs::ShaderDelete( _shaderTinted );
            _shaderTinted = gs::ShaderInvalid;
        }
        
        _batchActive = InvalidDrawBatchHandle;
        _shaderActive = gs::ShaderInvalid;
    }
    
    DrawBatchHandle BatchFromQuad2dShader( Quad2dShader shader )
    {
        switch( shader )
        {
            case Quad2dShaderFilled:
            case Quad2dShaderFilledCustom:
            {
                return _batchFilled;
            }
            case Quad2dShaderTextured:
            case Quad2dShaderTexturedCustom:
            {
                return _batchTextured;
            }
            case Quad2dShaderTinted:
            case Quad2dShaderTintedCustom:
            {
                return _batchTinted;
            }
        }
    }
    
    gs::ShaderHandle ShaderFromQuad2dShader( Quad2dShader shader )
    {
        switch( shader )
        {
            case Quad2dShaderFilled:
            {
                return _shaderFilled;
            }
            case Quad2dShaderTextured:
            {
                return _shaderTextured;
            }
            case Quad2dShaderTinted:
            {
                return _shaderTinted;
            }
            case Quad2dShaderFilledCustom:
            case Quad2dShaderTexturedCustom:
            case Quad2dShaderTintedCustom:
            {
                return gs::ShaderInvalid;
            }
        }
    }
    
    void BatchQuad2dBegin( Quad2dShader shader, gs::TextureHandle texture )
    {
        _batchActive = BatchFromQuad2dShader( shader );
        _shaderActive = ShaderFromQuad2dShader( shader );
        
        if( _shaderActive != gs::ShaderInvalid )
        {
            gs::ShaderSet( _shaderActive );
        }
        
        if( texture != gs::TextureInvalid )
        {
            gs::TextureSet( "texture0", texture );
        }
    }
    
    void BatchQuad2d( const Rect& vcoords, const v4& colour )
    {
        ASSERT( _batchActive == _batchFilled );
        QuadGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        Quad2dGenVertices( DrawBatchVertexPtr( _batchActive ), vcoords, colour );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchQuad2d( const Rect& vcoords, const Rect& tcoords )
    {
        ASSERT( _batchActive == _batchTextured );
        QuadGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        Quad2dGenVertices( DrawBatchVertexPtr( _batchActive ), vcoords, tcoords );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchQuad2d( const Rect& vcoords, const Rect& tcoords, const v4& colour )
    {
        ASSERT( _batchActive == _batchTinted );
        QuadGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        Quad2dGenVertices( DrawBatchVertexPtr( _batchActive ), vcoords, tcoords, colour );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchQuad2dEnd( gs::Primitive primitive )
    {
        DrawBatchFlush( _batchActive, primitive );
    }

    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const v4& colour, bool wireframe )
    {
        BatchQuad2dBegin( shader, gs::TextureInvalid );
        BatchQuad2d( vcoords, colour );
        BatchQuad2dEnd( wireframe ? gs::PrimitiveLineStrip : gs::PrimitiveTriangles);
    }
    
    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const Rect& tcoords )
    {
        BatchQuad2dBegin( shader, gs::TextureInvalid );
        BatchQuad2d( vcoords, tcoords );
        BatchQuad2dEnd( gs::PrimitiveTriangles);
    }
    
    void DrawQuad2d( Quad2dShader shader, const Rect& vcoords, const Rect& tcoords, const v4& colour )
    {
        BatchQuad2dBegin( shader, gs::TextureInvalid );
        BatchQuad2d( vcoords, tcoords, colour );
        BatchQuad2dEnd( gs::PrimitiveTriangles);
    }
}
