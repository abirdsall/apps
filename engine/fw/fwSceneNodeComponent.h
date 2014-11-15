#ifndef FW_SCENE_NODE_COMPONENT
#define FW_SCENE_NODE_COMPONENT

namespace fw
{
    struct SceneNode;
    
    class SceneNodeComponent
    {
    public:
        virtual void Delete()
        {
            
        }
        
        virtual void Tick( float dt, SceneNode& node )
        {
            
        }

        virtual void Render( Renderer& renderer, SceneNode& node )
        {
            
        }
    };
}

#endif