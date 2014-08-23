#include "fw.h"

namespace fw
{
    static const s32 BatchCapacityFilled = 1;
    static const s32 BatchCapacityTextured = 1;
    static const s32 BatchCapacityTinted = 1;

    static DrawBatchHandle sBatchFilled = InvalidDrawBatchHandle;
    static DrawBatchHandle sBatchTextured = InvalidDrawBatchHandle;
    static DrawBatchHandle sBatchTinted = InvalidDrawBatchHandle;

    static gs::ShaderHandle sShaderFilled = gs::InvalidShaderHandle;
    static gs::ShaderHandle sShaderTextured = gs::InvalidShaderHandle;
    static gs::ShaderHandle sShaderTinted = gs::InvalidShaderHandle;

    void InitQuad2dBatches()
    {
        if(sBatchFilled == InvalidDrawBatchHandle)
        {
            sBatchFilled = DrawBatchNew(BatchCapacityFilled, 6, 4, 2, 0, 4, 0);
        }
        
        if(sBatchTextured == InvalidDrawBatchHandle)
        {
            sBatchTextured = DrawBatchNew(BatchCapacityTextured, 6, 4, 2, 0, 4, 0);
        }
        
        if(sBatchTinted == InvalidDrawBatchHandle)
        {
            sBatchTinted = DrawBatchNew(BatchCapacityTinted, 6, 4, 2, 0, 4, 4);
        }
        
        if(sShaderFilled == gs::InvalidShaderHandle)
        {
            sShaderFilled = ShaderMake2d( true, false, 1 );
        }
        
        if(sShaderTextured == gs::InvalidShaderHandle)
        {
            sShaderTextured = ShaderMake2d( false, true, 1 );
        }
        
        if(sShaderTinted == gs::InvalidShaderHandle)
        {
            sShaderTinted = ShaderMake2d( true, true, 1 );
        }
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
    }
    
    void DrawQuad2d( const Rect& vcoords, const v4& colour )
    {
        gs::ShaderSet( sShaderFilled );
        Quad2dGenElements( DrawBatchElementPtr( sBatchFilled ), DrawBatchVertexCount( sBatchFilled ) );
        Quad2dGenVertices( DrawBatchVertexPtr( sBatchFilled ), vcoords, colour );
        DrawBatchIncrement( sBatchFilled );
        DrawBatchFlush( sBatchFilled, gs::ePrimTriangles );
    }
    
    void DrawQuad2d( const Rect& vcoords, const Rect& tcoords )
    {
        gs::ShaderSet( sShaderTextured );
        Quad2dGenElements( DrawBatchElementPtr( sBatchTextured ), DrawBatchVertexCount( sBatchTextured ) );
        Quad2dGenVertices( DrawBatchVertexPtr( sBatchTextured ), vcoords, tcoords );
        DrawBatchIncrement( sBatchTextured );
        DrawBatchFlush( sBatchTextured, gs::ePrimTriangles );
    }
    
    void DrawQuad2d( const Rect& vcoords, const Rect& tcoords, const v4& colour )
    {
        gs::ShaderSet( sShaderTinted );
        Quad2dGenElements( DrawBatchElementPtr( sBatchTinted ), DrawBatchVertexCount( sBatchTinted ) );
        Quad2dGenVertices( DrawBatchVertexPtr( sBatchTinted ), vcoords, tcoords, colour );
        DrawBatchIncrement( sBatchTinted );
        DrawBatchFlush( sBatchTinted, gs::ePrimTriangles );
    }
}
