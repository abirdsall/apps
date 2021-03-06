#ifndef FW_RENDERER
#define FW_RENDERER

namespace fw
{
    struct Renderer
    {
        virtual void Render( Camera* camera ) = 0;
        
        Array<LightHandle> _lights;
        
        SceneNode* _scene;
    };
}

#endif