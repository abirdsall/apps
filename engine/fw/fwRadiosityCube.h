#ifndef FW_RADIOSITY_CUBE
#define FW_RADIOSITY_CUBE

namespace fw
{
    struct RadiosityCube;
    
    void InitRadiosityCubes();
    void KillRadiosityCubes();
    RadiosityCube* RadiosityCubeNew();
    void RadiosityCubeDelete( RadiosityCube* cube );

    struct RadiosityCube : SceneNodeComponent
    {
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
    };
}

#endif