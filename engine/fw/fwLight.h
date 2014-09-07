#ifndef FWX_LIGHT
#define FWX_LIGHT

namespace fw
{
    typedef ::hwInt LightHandle;
    
    static const ::hwInt InvalidLightHandle = ( LightHandle )Null;

    struct Light
    {
        v3 _position;
        v3 _colour;
        bool _active;
    };
    
    void InitLights();
    void KillLights();
    
    LightHandle LightNew( const v3& position, const v3& colour );
    
    void LightDelete( LightHandle handle );
    
    const v3& LightPosition( LightHandle handle );
    void LightSetPosition( LightHandle handle, const v3& position );
}

#endif