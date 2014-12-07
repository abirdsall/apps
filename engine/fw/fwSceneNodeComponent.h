#ifndef FW_SCENE_NODE_COMPONENT
#define FW_SCENE_NODE_COMPONENT

namespace fw
{
    struct SceneNode;
    
    class SceneNodeComponent
    {
    public:

        f32 testa;
        
        SceneNodeComponent()
        {
            testa = 7.0f;
            testb = 8.0f;
        }

        virtual void Delete()
        {
            
        }

        virtual void Render( Renderer& renderer, SceneNode& node, const m4& viewMatrix )
        {
            
        }

        virtual void Tick( f32 dt, SceneNode& node )
        {
            
        }
        
        f32 testb;
    };
}

#endif