#ifndef FW_XENDERER
#define FW_XENDERER

namespace fw
{
    struct Xenderer
    {
        virtual void Render() = 0;
        
        Array<LightHandle> _lights;
        
        SceneNodeHandle _scene;
    };
}

#endif