#include "fw.h"

namespace fw
{
    static const s32 BatchCapacityFilled = 64;
    static const s32 BatchCapacityRadiosity = 64;
    static const s32 BatchCapacityRadiosityMono = 64;
    
    static DrawBatchHandle sBatchFilled = InvalidDrawBatchHandle;
    static DrawBatchHandle sBatchRadiosity = InvalidDrawBatchHandle;
    static DrawBatchHandle sBatchRadiosityMono = InvalidDrawBatchHandle;
    
    static DrawBatchHandle sBatchActive = InvalidDrawBatchHandle;
    
   void InitCubeBatches()
    {
        if(sBatchFilled == InvalidDrawBatchHandle)
        {
            sBatchFilled = DrawBatchNew(BatchCapacityFilled, 36, 24, 3, 3, 4, 0);
        }
        
        if(sBatchRadiosity == InvalidDrawBatchHandle)
        {
            sBatchRadiosity = DrawBatchNew(BatchCapacityRadiosity, 36, 24, 3, 0, 4, 2);
        }
        
        if(sBatchRadiosityMono == InvalidDrawBatchHandle)
        {
            sBatchRadiosityMono = DrawBatchNew(BatchCapacityRadiosityMono, 36, 24, 3, 0, 0, 2);
        }

        sBatchActive = InvalidDrawBatchHandle;
    }
    
    void KillCubeBatches()
    {
        if(sBatchFilled != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( sBatchFilled );
            sBatchFilled = InvalidDrawBatchHandle;
        }
        
        if(sBatchRadiosity != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( sBatchRadiosity );
            sBatchRadiosity = InvalidDrawBatchHandle;
        }
        
        if(sBatchRadiosityMono != InvalidDrawBatchHandle)
        {
            DrawBatchDelete( sBatchRadiosityMono );
            sBatchRadiosityMono = InvalidDrawBatchHandle;
        }
        
        sBatchActive = InvalidDrawBatchHandle;
    }

    DrawBatchHandle BatchFromCubeBatch( CubeBatch batch )
    {
        switch( batch )
        {
            case CubeBatchFilled:
            {
                return sBatchFilled;
            }
                
            case CubeBatchRadiosity:
            {
                return sBatchRadiosity;
            }
                
            case CubeBatchRadiosityMono:
            {
                return sBatchRadiosityMono;
            }
        }
    }
    
    void BatchCubeBegin( CubeBatch batch )
    {
        sBatchActive = BatchFromCubeBatch( batch );
    }
    
    void BatchCube( const v3& position, const v3& radius, const v4& colour )
    {
        ASSERT( sBatchActive == sBatchFilled );
        CubeGenElements( DrawBatchElementPtr( sBatchActive ), DrawBatchVertexCount( sBatchActive ) );
        CubeGenVertices( DrawBatchVertexPtr( sBatchActive ), position, radius, colour );
        DrawBatchIncrement( sBatchActive );
    }

    void BatchCubeRadiosity( const v3& position, const v3& radius, const v4& colour )
    {
        ASSERT( sBatchActive == sBatchRadiosity );
        CubeGenElements( DrawBatchElementPtr( sBatchActive ), DrawBatchVertexCount( sBatchActive ) );
        CubeGenVerticesRadiosity( DrawBatchVertexPtr( sBatchActive ), position, radius, colour );
        DrawBatchIncrement( sBatchActive );
    }
    
    void BatchCubeRadiosity( const v3& position, const v3& radius )
    {
        ASSERT( sBatchActive == sBatchRadiosityMono );
        CubeGenElements( DrawBatchElementPtr( sBatchActive ), DrawBatchVertexCount( sBatchActive ) );
        CubeGenVerticesRadiosityMono( DrawBatchVertexPtr( sBatchActive ), position, radius );
        DrawBatchIncrement( sBatchActive );
    }
    
    void BatchCubeEnd( gs::ePrim primitive )
    {
        DrawBatchFlush( sBatchActive, primitive );
    }
    
    void DrawCube( const v3& position, const v3& radius, const v4& colour )
    {
        BatchCubeBegin( CubeBatchFilled );
        BatchCube( position, radius, colour );
        BatchCubeEnd( gs::ePrimTriangles);
    }
    
    void DrawCubeRadiosity( const v3& position, const v3& radius, const v4& colour )
    {
        BatchCubeBegin( CubeBatchRadiosity );
        BatchCubeRadiosity( position, radius, colour );
        BatchCubeEnd( gs::ePrimTriangles);
    }

    void DrawCubeRadiosity( const v3& position, const v3& radius )
    {
        BatchCubeBegin( CubeBatchRadiosityMono );
        BatchCubeRadiosity( position, radius );
        BatchCubeEnd( gs::ePrimTriangles);
    }
}

