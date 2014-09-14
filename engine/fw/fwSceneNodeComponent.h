#ifndef FW_SCENE_NODE_COMPONENT
#define FW_SCENE_NODE_COMPONENT

namespace fw
{
    struct SceneNode;
    
    struct SceneNodeComponent
    {
        virtual void Delete()
        {
            
        }
        
        virtual void Tick( float dt )
        {
            
        }

        virtual void Render( Renderer& renderer, SceneNode& node )
        {
            
        }
    };
}

#endif