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
        v3 _position;
        v3 _radius;
        v4 _colour;
        
        void Delete()
        {
            RadiosityCubeDelete( this );
        }

        void Render( Renderer& renderer )
        {
            RadiosityRenderer& radiosityRenderer = (RadiosityRenderer&)renderer;
            
            if(radiosityRenderer.Voxelising())
            {
                DrawCubeRadiosity( _position, _radius, _colour );
            }
            else
            {
                DrawCube( _position, _radius, _colour );
            }
        }
    };
}

#endif