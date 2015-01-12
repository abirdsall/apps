#include "fw.h"

namespace fw
{
    static const s32 BatchCapacityFilled = 256;
    static const s32 BatchCapacityTextured = 256;
    static const s32 BatchCapacityTextured3d = 256;
    static const s32 BatchCapacityTinted = 256;
    
    static DrawBatchHandle _batchFilled = InvalidDrawBatchHandle;
    static DrawBatchHandle _batchTextured = InvalidDrawBatchHandle;
    static DrawBatchHandle _batchTextured3d = InvalidDrawBatchHandle;
    static DrawBatchHandle _batchTinted = InvalidDrawBatchHandle;
    
    static gs::ShaderHandle _shaderFilled = gs::ShaderInvalid;
    static gs::ShaderHandle _shaderTextured = gs::ShaderInvalid;
    static gs::ShaderHandle _shaderTextured3d = gs::ShaderInvalid;
    static gs::ShaderHandle _shaderTinted = gs::ShaderInvalid;
    
    static DrawBatchHandle _batchActive = InvalidDrawBatchHandle;
    static gs::ShaderHandle _shaderActive = gs::ShaderInvalid;
    
    void InitQuad3dBatches()
    {
        if(_batchFilled == InvalidDrawBatchHandle)
        {
            _batchFilled = DrawBatchNew(BatchCapacityFilled, 6, 4, 3, 0, 4, 0);
        }
        
        if(_batchTextured == InvalidDrawBatchHandle)
        {
            _batchTextured = DrawBatchNew(BatchCapacityTextured, 6, 4, 3, 0, 0, 2);
        }
        
        if(_batchTextured3d == InvalidDrawBatchHandle)
        {
            _batchTextured3d = DrawBatchNew(BatchCapacityTextured3d, 6, 4, 3, 0, 0, 3);
        }
        
        if(_batchTinted == InvalidDrawBatchHandle)
        {
            _batchTinted = DrawBatchNew(BatchCapacityTinted, 6, 4, 3, 0, 4, 2);
        }
        
        if(_shaderFilled == gs::ShaderInvalid)
        {
            _shaderFilled = ShaderMake( eShader3dFill );
        }
        
        if(_shaderTextured == gs::ShaderInvalid)
        {
            _shaderTextured = ShaderMake( eShader3dTexture2d );
        }

        if(_shaderTextured3d == gs::ShaderInvalid)
        {
            _shaderTextured3d = ShaderMake( eShader3dTexture3d );
        }

        if(_shaderTinted == gs::ShaderInvalid)
        {
            _shaderTinted = ShaderMake( eShader3dTexture2dTinted );
        }
        
        _batchActive = InvalidDrawBatchHandle;
        _shaderActive = gs::ShaderInvalid;
    }
    
    void KillQuad3dBatches()
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

        if(_batchTextured3d != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchTextured3d );
            _batchTextured3d = InvalidDrawBatchHandle;
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

        if(_shaderTextured3d != gs::ShaderInvalid)
        {
            gs::ShaderDelete( _shaderTextured3d );
            _shaderTextured3d = gs::ShaderInvalid;
        }

        if(_shaderTinted != gs::ShaderInvalid)
        {
            gs::ShaderDelete( _shaderTinted );
            _shaderTinted = gs::ShaderInvalid;
        }
        
        _batchActive = InvalidDrawBatchHandle;
        _shaderActive = gs::ShaderInvalid;
    }
    
    DrawBatchHandle BatchFromQuad3dShader( Quad3dShader shader )
    {
        switch( shader )
        {
            case Quad3dShaderFilled:
            case Quad3dShaderFilledCustom:
            {
                return _batchFilled;
            }
            case Quad3dShaderTextured:
            case Quad3dShaderTexturedCustom:
            {
                return _batchTextured;
            }
            case Quad3dShaderTextured3d:
            case Quad3dShaderTextured3dCustom:
            {
                return _batchTextured3d;
            }
            case Quad3dShaderTinted:
            case Quad3dShaderTintedCustom:
            {
                return _batchTinted;
            }
        }
    }
    
    gs::ShaderHandle ShaderFromQuad3dShader( Quad3dShader shader )
    {
        switch( shader )
        {
            case Quad3dShaderFilled:
            {
                return _shaderFilled;
            }
            case Quad3dShaderTextured:
            {
                return _shaderTextured;
            }
            case Quad3dShaderTextured3d:
            {
                return _shaderTextured3d;
            }
            case Quad3dShaderTinted:
            {
                return _shaderTinted;
            }
            case Quad3dShaderFilledCustom:
            case Quad3dShaderTexturedCustom:
            case Quad3dShaderTextured3dCustom:
            case Quad3dShaderTintedCustom:
            {
                return gs::ShaderInvalid;
            }
        }
    }
    
    void BatchQuad3dBegin( Quad3dShader shader, gs::TextureHandle texture )
    {
        _batchActive = BatchFromQuad3dShader( shader );
        _shaderActive = ShaderFromQuad3dShader( shader );
        
        if( _shaderActive != gs::ShaderInvalid )
        {
            gs::ShaderSet( _shaderActive );
        }
        
        if( texture != gs::TextureInvalid )
        {
            gs::TextureSet( "texture0", texture );
        }
    }
    
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const v4& colour )
    {
        ASSERT( _batchActive == _batchFilled );
        QuadGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        Quad3dGenVertices( DrawBatchVertexPtr( _batchActive ), v0, v1, v2, v3, colour );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords )
    {
        ASSERT( _batchActive == _batchTextured );
        QuadGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        Quad3dGenVertices( DrawBatchVertexPtr( _batchActive ), v0, v1, v2, v3, tcoords );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, f32 tcoordZ )
    {
        ASSERT( _batchActive == _batchTextured3d );
        QuadGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        Quad3dGenVertices( DrawBatchVertexPtr( _batchActive ), v0, v1, v2, v3, tcoords, tcoordZ );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchQuad3d( const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, const v4& colour )
    {
        ASSERT( _batchActive == _batchTinted );
        QuadGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        Quad3dGenVertices( DrawBatchVertexPtr( _batchActive ), v0, v1, v2, v3, tcoords, colour );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchQuad3dEnd( gs::Primitive primitive )
    {
        DrawBatchFlush( _batchActive, primitive );
    }
    
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const v4& colour, bool wireframe )
    {
        BatchQuad3dBegin( shader, gs::TextureInvalid );
        BatchQuad3d( v0, v1, v2, v3, colour );
        BatchQuad3dEnd( wireframe ? gs::PrimitiveLineStrip : gs::PrimitiveTriangles);
    }
    
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords )
    {
        BatchQuad3dBegin( shader, gs::TextureInvalid );
        BatchQuad3d( v0, v1, v2, v3, tcoords );
        BatchQuad3dEnd( gs::PrimitiveTriangles);
    }
    
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, f32 tcoordZ )
    {
        BatchQuad3dBegin( shader, gs::TextureInvalid );
        BatchQuad3d( v0, v1, v2, v3, tcoords, tcoordZ );
        BatchQuad3dEnd( gs::PrimitiveTriangles);
    }
    
    void DrawQuad3d( Quad3dShader shader, const v3& v0, const v3& v1, const v3& v2, const v3& v3, const Rect& tcoords, const v4& colour )
    {
        BatchQuad3dBegin( shader, gs::TextureInvalid );
        BatchQuad3d( v0, v1, v2, v3, tcoords, colour );
        BatchQuad3dEnd( gs::PrimitiveTriangles);
    }
}
