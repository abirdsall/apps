#ifndef FW_RADIOSITY_SPHERE
#define FW_RADIOSITY_SPHERE

namespace fw
{
    class RadiositySphere : public SceneNodeComponent
    {
    private:
        
        DrawBatchHandle _batchFilled;
        DrawBatchHandle _batchRadiosity;
        v4 _colour;
        
    public:
        bool _spin;
        
        RadiositySphere()
        {
            _batchFilled = InvalidDrawBatchHandle;
            _batchRadiosity = InvalidDrawBatchHandle;
        }
        
        void Init( const v4& colour )
        {
            _colour = colour;
            
            if(_batchFilled == InvalidDrawBatchHandle)
            {
                _batchFilled = DrawBatchNew( 1, 60, 12, 3, 3, 4, 0 );
            }
            DrawBatchClear( _batchFilled );
            SphereGenElements( DrawBatchElementPtr( _batchFilled ), DrawBatchVertexCount( _batchFilled ) );
            SphereGenPositionsNormalsColours( DrawBatchVertexPtr( _batchFilled ), V3Zero, V3One, _colour );
            DrawBatchIncrement( _batchFilled );
            DrawBatchFinalise( _batchFilled );
            
            if(_batchRadiosity == InvalidDrawBatchHandle)
            {
                _batchRadiosity = DrawBatchNew( 1, 60, 12, 3, 0, 4, 0 );
                DrawBatchIncrement( _batchRadiosity );
            }
            DrawBatchClear( _batchRadiosity );
            SphereGenElements( DrawBatchElementPtr( _batchRadiosity ), DrawBatchVertexCount( _batchRadiosity ) );
            SphereGenPositionsColours( DrawBatchVertexPtr( _batchRadiosity ), V3Zero, V3One, _colour );
            DrawBatchIncrement( _batchRadiosity );
            DrawBatchFinalise( _batchRadiosity );
        }
        
        void Delete();
        
        void Render( Renderer& renderer, SceneNode& node )
        {
            RadiosityRenderer& radiosityRenderer = (RadiosityRenderer&)renderer;
            
            gs::Put();
            
            gs::SetMatrixM( node._worldTransform );
            
            if(radiosityRenderer.Voxelising())
            {
                if(_batchRadiosity != InvalidDrawBatchHandle)
                {
                    DrawBatchDraw( _batchRadiosity, gs::PrimitiveTriangles );
                }
            }
            else
            {
                if(_batchFilled != InvalidDrawBatchHandle)
                {
                    DrawBatchDraw( _batchFilled, gs::PrimitiveTriangles );
                }
            }
            
            gs::Pop();
        }
        
        void Tick( f32 dt, SceneNode& node )
        {
            if( _spin )
            {
                node._localTransform = node._localTransform * rotateZXY( 0.01f, 0.01f, 0.01f );
            }
        }
    };
    
    void InitRadiositySpheres();
    void KillRadiositySpheres();
    
    RadiositySphere* RadiositySphereNew();
    
    void RadiositySphereDelete( RadiositySphere* sphere );
}

#endif