#ifndef FW_DRAW_BATCH
#define FW_DRAW_BATCH

namespace fw
{
    typedef ::hwInt DrawBatchHandle;
    
    static const ::hwInt InvalidDrawBatchHandle = (DrawBatchHandle)Null;
    
    void InitDrawBatches();
    void KillDrawBatches();
    
    DrawBatchHandle DrawBatchNew(s32 capacity,
                                 s32 elementsPerPrimitive,
                                 s32 verticesPerPrimitive,
                                 s32 vertexAttribSize,
                                 s32 normalAttribSize,
                                 s32 colourAttribSize,
                                 s32 tcoordAttribSize );
    
    void DrawBatchDelete( DrawBatchHandle handle );
    
    u16* DrawBatchElementPtr( DrawBatchHandle handle );
    f32* DrawBatchVertexPtr( DrawBatchHandle handle );
    
    u16 DrawBatchElementCount( DrawBatchHandle handle );
    u16 DrawBatchVertexCount( DrawBatchHandle handle );
    
    void DrawBatchIncrement( DrawBatchHandle handle );
    void DrawBatchFinalise( DrawBatchHandle handle );
    void DrawBatchDraw( DrawBatchHandle handle, gs::Primitive prim );
    void DrawBatchClear( DrawBatchHandle handle );
    void DrawBatchFlush( DrawBatchHandle handle, gs::Primitive prim );
}

#endif