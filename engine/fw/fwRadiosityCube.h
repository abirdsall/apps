#ifndef FW_RADIOSITY_CUBE
#define FW_RADIOSITY_CUBE

namespace fw
{
    class RadiosityCube;
    
    void InitRadiosityCubes();
    void KillRadiosityCubes();
    RadiosityCube* RadiosityCubeNew();
    void RadiosityCubeDelete( RadiosityCube* cube );

    class RadiosityCube : public SceneNodeComponent
    {
    public:
        v4 _colour;
        
        void Delete()
        {
            RadiosityCubeDelete( this );
        }

        void Render( Renderer& renderer, SceneNode& node )
        {
            RadiosityRenderer& radiosityRenderer = (RadiosityRenderer&)renderer;
            
            if(radiosityRenderer.Voxelising())
            {
                DrawCubeRadiosity( node._localTransform.getPosition(), node._modelScale, _colour );
            }
            else
            {
                DrawCube( node._localTransform.getPosition(), node._modelScale, _colour );
            }
        }
        
        void Tick( float dt, SceneNode& node )
        {
            node._localTransform = node._localTransform * r4( v3( 1.0f, 1.0f, 0.0f ), 33.3f );
        }
    };
}

#endif