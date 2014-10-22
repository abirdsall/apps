#include "fw.h"

namespace fw
{
    struct DrawBatch
    {
        Mesh _mesh;
        s32 _size;
        s32 _capacity;
        s32 _elementsPerPrimitive;
        s32 _verticesPerPrimitive;
        s32 _vertexAttribSize;
        s32 _normalAttribSize;
        s32 _colourAttribSize;
        s32 _tcoordAttribSize;
        s32 _attribSize;
    };
 
    static Pool<DrawBatch> _batches;

    void InitDrawBatches()
    {
        _batches.Init( 32 );
    }
    
    void KillDrawBatches()
    {
        
    }
    
    DrawBatchHandle DrawBatchNew(s32 capacity,
                                 s32 elementsPerPrimitive,
                                 s32 verticesPerPrimitive,
                                 s32 vertexAttribSize,
                                 s32 normalAttribSize,
                                 s32 colourAttribSize,
                                 s32 tcoordAttribSize
    )
    {
        DrawBatch* batch = _batches.New();
        
        batch->_size = 0;
        batch->_capacity = capacity;
        batch->_elementsPerPrimitive = elementsPerPrimitive;
        batch->_verticesPerPrimitive = verticesPerPrimitive;
        batch->_vertexAttribSize = vertexAttribSize;
        batch->_normalAttribSize = normalAttribSize;
        batch->_colourAttribSize = colourAttribSize;
        batch->_tcoordAttribSize = tcoordAttribSize;
        batch->_attribSize = batch->_vertexAttribSize + batch->_normalAttribSize + batch->_colourAttribSize + batch->_tcoordAttribSize;
        
        s32 elementLimit = batch->_capacity * elementsPerPrimitive;
        s32 elementSize = sizeof( u16 );
        
        s32 vertexLimit = batch->_capacity * verticesPerPrimitive;
        s32 vertexSize = sizeof( f32 ) * batch->_attribSize;
        
        MeshInit( batch->_mesh );
        
        MeshSetElementData( batch->_mesh, core::alloc( elementSize * elementLimit ), elementSize, 0, elementLimit, true );
        
        MeshSetVertexData( batch->_mesh, core::alloc( vertexSize * vertexLimit ), vertexSize, 0, vertexLimit, true );
        
        MeshSetAttrib( batch->_mesh, gs::eAttribVertex, ( void* )( sizeof( f32 ) * 0 ), batch->_vertexAttribSize );

        if(normalAttribSize > 0)
        {
            MeshSetAttrib( batch->_mesh,
                          gs::eAttribNormal,
                          ( void* )( sizeof( f32 ) * ( batch->_vertexAttribSize ) ),
                          batch->_normalAttribSize );
        }
        
        if(colourAttribSize > 0)
        {
            MeshSetAttrib( batch->_mesh,
                          gs::eAttribColour,
                          ( void* )( sizeof( f32 ) * ( batch->_vertexAttribSize + batch->_normalAttribSize ) ),
                          batch->_colourAttribSize );
        }
        
        if(tcoordAttribSize > 0)
        {
            MeshSetAttrib( batch->_mesh, gs::eAttribTcoord,
                          ( void* )( sizeof( f32 ) * ( batch->_vertexAttribSize + batch->_normalAttribSize + batch->_colourAttribSize ) ),
                          batch->_tcoordAttribSize );
        }
        
        MeshFinalise( batch->_mesh );
        
        return ( DrawBatchHandle )batch;
    }
    
    void DrawBatchDelete( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        MeshKill( batch->_mesh );
        
        _batches.Delete( batch );
    }

    u16* DrawBatchElementPtr( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        return ( u16* )MeshGetElementData( batch->_mesh, batch->_elementsPerPrimitive * batch->_size );
    }

    f32* DrawBatchVertexPtr( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        return ( f32* )MeshGetVertexData( batch->_mesh, batch->_verticesPerPrimitive * batch->_size );
    }
    
    u16 DrawBatchElementCount( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        return batch->_elementsPerPrimitive * batch->_size;
    }
    
    u16 DrawBatchVertexCount( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        return batch->_verticesPerPrimitive * batch->_size;
    }
    
    void DrawBatchIncrement( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        batch->_size++;
    }

    void DrawBatchFinalise( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        MeshSetElementCount( batch->_mesh, batch->_elementsPerPrimitive * batch->_size );
        
        MeshSetVertexCount( batch->_mesh, batch->_verticesPerPrimitive * batch->_size );
    }
    
    void DrawBatchDraw( DrawBatchHandle handle, gs::ePrim prim )
    {
        DrawBatch* batch = ( DrawBatch* )handle;

        if( batch->_size > 0 )
        {
            MeshDraw( batch->_mesh, prim );
        }
    }
    
    void DrawBatchClear( DrawBatchHandle handle )
    {
        DrawBatch* batch = ( DrawBatch* )handle;
        
        batch->_size = 0;
        
        MeshSetElementCount( batch->_mesh, 0 );
        
        MeshSetVertexCount( batch->_mesh, 0 );
    }
    
    void DrawBatchFlush( DrawBatchHandle handle, gs::ePrim prim )
    {
        DrawBatchFinalise( handle );
        DrawBatchDraw( handle, prim );
        DrawBatchClear( handle );
    }
}