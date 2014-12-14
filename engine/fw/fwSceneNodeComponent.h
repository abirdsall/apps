#ifndef FW_SCENE_NODE_COMPONENT
#define FW_SCENE_NODE_COMPONENT

namespace fw
{
    struct SceneNode;
    
    class SceneNodeComponent
    {
    public:

        SceneNodeComponent()
        {
        }

        virtual void Delete()
        {
            
        }

        virtual void Render( Renderer& renderer, SceneNode& node )
        {
            
        }

        virtual void Tick( f32 dt, SceneNode& node )
        {
            
        }
    };
}

#endif