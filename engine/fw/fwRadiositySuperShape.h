#ifndef FW_RADIOSITY_SUPERSHAPE
#define FW_RADIOSITY_SUPERSHAPE

namespace fw
{
    struct RadiositySuperShape;
    
    void InitRadiositySuperShapes();
    void KillRadiositySuperShapes();
    RadiositySuperShape* RadiositySuperShapeNew();
    void RadiositySuperShapeDelete( RadiositySuperShape* shape );
    
    struct RadiositySuperShape : SceneNodeComponent
    {
        v4 _colour;
        
        void Delete()
        {
            RadiositySuperShapeDelete( this );
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