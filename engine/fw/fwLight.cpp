#include "fw.h"

namespace fw
{
    static Pool<Light> _lights;
    
    void InitLights()
    {
        _lights.Init( 64 );
    }
    
    void KillLights()
    {
        
    }
    
    LightHandle LightNew( const v3& position, const v3& colour )
    {
        Light* light = _lights.New();
        
        light->_position = position;
        light->_colour = colour;
        light->_active = true;
        
        return ( LightHandle )light;
    }
    
    void LightDelete( LightHandle handle )
    {
        Light* light = ( Light* )handle;
        
        _lights.Delete( light );
    }
    
    const v3& LightPosition( LightHandle handle )
    {
        Light* light = ( Light* )handle;
        
        return light->_position;
    }
    
    void LightSetPosition( LightHandle handle, const v3& position )
    {
        Light* light = ( Light* )handle;
        
        light->_position = position;
    }
}
