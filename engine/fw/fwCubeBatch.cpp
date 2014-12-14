#include "fw.h"

namespace fw
{
    static const s32 BatchCapacityFilled = 64;
    static const s32 BatchCapacityRadiosity = 64;
    static const s32 BatchCapacityRadiosityMono = 64;
    
    static DrawBatchHandle _batchFilled = InvalidDrawBatchHandle;
    static DrawBatchHandle _batchRadiosity = InvalidDrawBatchHandle;
    static DrawBatchHandle _batchRadiosityMono = InvalidDrawBatchHandle;
    
    static DrawBatchHandle _batchActive = InvalidDrawBatchHandle;
    
   void InitCubeBatches()
    {
        if(_batchFilled == InvalidDrawBatchHandle)
        {
            _batchFilled = DrawBatchNew(BatchCapacityFilled, 36, 24, 3, 3, 4, 0);
        }
        
        if(_batchRadiosity == InvalidDrawBatchHandle)
        {
            _batchRadiosity = DrawBatchNew(BatchCapacityRadiosity, 36, 24, 3, 0, 4, 1);
        }
        
        if(_batchRadiosityMono == InvalidDrawBatchHandle)
        {
            _batchRadiosityMono = DrawBatchNew(BatchCapacityRadiosityMono, 36, 24, 3, 0, 0, 1);
        }

        _batchActive = InvalidDrawBatchHandle;
    }
    
    void KillCubeBatches()
    {
        if(_batchFilled != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchFilled );
            _batchFilled = InvalidDrawBatchHandle;
        }
        
        if(_batchRadiosity != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchRadiosity );
            _batchRadiosity = InvalidDrawBatchHandle;
        }
        
        if(_batchRadiosityMono != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( _batchRadiosityMono );
            _batchRadiosityMono = InvalidDrawBatchHandle;
        }
        
        _batchActive = InvalidDrawBatchHandle;
    }

    DrawBatchHandle BatchFromCubeBatch( CubeBatch batch )
    {
        switch( batch )
        {
            case CubeBatchFilled:
            {
                return _batchFilled;
            }
                
            case CubeBatchRadiosity:
            {
                return _batchRadiosity;
            }
                
            case CubeBatchRadiosityMono:
            {
                return _batchRadiosityMono;
            }
        }
    }
    
    void BatchCubeBegin( CubeBatch batch )
    {
        _batchActive = BatchFromCubeBatch( batch );
    }
    
    void BatchCube( const v3& position, const v3& radius, const v4& colour )
    {
        ASSERT( _batchActive == _batchFilled );
        CubeGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        CubeGenPositionsNormalsColours( DrawBatchVertexPtr( _batchActive ), position, radius, colour );
        DrawBatchIncrement( _batchActive );
    }

    void BatchCubeRadiosity( const v3& position, const v3& radius, const v4& colour )
    {
        ASSERT( _batchActive == _batchRadiosity );
        CubeGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        CubeGenPositionsColours( DrawBatchVertexPtr( _batchActive ), position, radius, colour );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchCubeRadiosity( const v3& position, const v3& radius )
    {
        ASSERT( _batchActive == _batchRadiosityMono );
        CubeGenElements( DrawBatchElementPtr( _batchActive ), DrawBatchVertexCount( _batchActive ) );
        CubeGenPositions( DrawBatchVertexPtr( _batchActive ), position, radius );
        DrawBatchIncrement( _batchActive );
    }
    
    void BatchCubeEnd( gs::Primitive primitive )
    {
        DrawBatchFlush( _batchActive, primitive );
    }
    
    void DrawCube( const v3& position, const v3& radius, const v4& colour )
    {
        BatchCubeBegin( CubeBatchFilled );
        BatchCube( position, radius, colour );
        BatchCubeEnd( gs::PrimitiveTriangles );
    }
    
    void DrawCubeRadiosity( const v3& position, const v3& radius, const v4& colour )
    {
        BatchCubeBegin( CubeBatchRadiosity );
        BatchCubeRadiosity( position, radius, colour );
        BatchCubeEnd( gs::PrimitiveTriangles );
    }

    void DrawCubeRadiosity( const v3& position, const v3& radius )
    {
        BatchCubeBegin( CubeBatchRadiosityMono );
        BatchCubeRadiosity( position, radius );
        BatchCubeEnd( gs::PrimitiveTriangles );
    }
}

