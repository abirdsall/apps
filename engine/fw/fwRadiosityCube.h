#ifndef FW_RADIOSITY_CUBE
#define FW_RADIOSITY_CUBE

namespace fw
{
    class RadiosityCube : public SceneNodeComponent
    {
    private:
        
        DrawBatchHandle _batchFilled;
        DrawBatchHandle _batchRadiosity;
        v4 _colour;
        
    public:
        
        RadiosityCube()
        {
            _batchFilled = InvalidDrawBatchHandle;
            _batchRadiosity = InvalidDrawBatchHandle;
        }
        
        void Init( const v4& colour )
        {
            _colour = colour;
            
            if(_batchFilled == InvalidDrawBatchHandle)
            {
                _batchFilled = DrawBatchNew( 1, 36, 24, 3, 3, 4, 0 );
            }
            DrawBatchClear( _batchFilled );
            CubeGenElements( DrawBatchElementPtr( _batchFilled ), DrawBatchVertexCount( _batchFilled ) );
            CubeGenVertices( DrawBatchVertexPtr( _batchFilled ), V3UnitZ, V3One, _colour );
            DrawBatchIncrement( _batchFilled );
            DrawBatchFinalise( _batchFilled );
            
            if(_batchRadiosity == InvalidDrawBatchHandle)
            {
                _batchRadiosity = DrawBatchNew( 1, 36, 24, 3, 0, 4, 2 );
                DrawBatchIncrement( _batchRadiosity );
            }
            DrawBatchClear( _batchRadiosity );
            CubeGenElements( DrawBatchElementPtr( _batchRadiosity ), DrawBatchVertexCount( _batchRadiosity ) );
            CubeGenVerticesRadiosity( DrawBatchVertexPtr( _batchRadiosity ), V3UnitZ, V3One, _colour );
            DrawBatchIncrement( _batchRadiosity );
            DrawBatchFinalise( _batchRadiosity );
        }
        
        void Delete();

        void Render( Renderer& renderer, SceneNode& node, const m4& viewMatrix )
        {
            RadiosityRenderer& radiosityRenderer = (RadiosityRenderer&)renderer;
            
            gs::Put();
            
            gs::SetMatrixM( viewMatrix * node._worldTransform );
            
            if(radiosityRenderer.Voxelising())
            {
                if(_batchRadiosity != InvalidDrawBatchHandle)
                {
                    DrawBatchDraw( _batchRadiosity, gs::PrimitiveTriangles );
                }
                //DrawCubeRadiosity( node._localTransform.getPosition(), node._modelScale, _colour );
            }
            else
            {
                if(_batchFilled != InvalidDrawBatchHandle)
                {
                    DrawBatchDraw( _batchFilled, gs::PrimitiveTriangles );
                }
                //DrawCube( node._localTransform.getPosition(), node._modelScale, _colour );
            }
            
            gs::Pop();
        }
        
        void Tick( f32 dt, SceneNode& node )
        {
            //printf("ffffff\n");
            //node._localTransform = node._localTransform * r4( v3( 0.0f, 0.0f, 1.0f ), 0.001f );
        }
    };
    
    void InitRadiosityCubes();
    void KillRadiosityCubes();
    
    RadiosityCube* RadiosityCubeNew();
    
    void RadiosityCubeDelete( RadiosityCube* cube );
}

#endif