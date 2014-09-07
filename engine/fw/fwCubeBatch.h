#ifndef FW_CUBE_BATCH
#define FW_CUBE_BATCH

namespace fw
{
    typedef enum
    {
        CubeBatchFilled,
        CubeBatchRadiosity,
        CubeBatchRadiosityMono
    } CubeBatch;
    
    void InitCubeBatches();
    void KillCubeBatches();
    
    void BatchCubeBegin();
    void BatchCube( const v3& position, const v3& radius, const v4& colour );
    void BatchCubeRadiosity( const v3& position, const v3& radius, const v4& colour );
    void BatchCubeRadiosity( const v3& position, const v3& radius );
    void BatchCubeEnd( gs::ePrim primitive );
    
    void DrawCube( const v3& position, const v3& radius, const v4& colour );
    void DrawCubeRadiosity( const v3& position, const v3& radius, const v4& colour );
    void DrawCubeRadiosity( const v3& position, const v3& radius );
}

#endif