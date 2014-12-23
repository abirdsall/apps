#ifndef FW_CAMERA
#define FW_CAMERA

namespace fw
{
    typedef enum
    {
        CameraControl_None,
        CameraControl_Fly,
        CameraControl_Orbit
    } CameraControl;
    
    class Camera : public SceneNodeComponent
    {
    private:
        v3 _position;
        v3 _target;
        CameraControl _control;
        
        void TickFly( f32 dt );
        void TickOrbit( f32 dt );
        
    public:
        m4 _projection;
        m4 _view;
        
        Camera()
        {
            _projection = identity4();
            _view = identity4();
            _control = CameraControl_Orbit;
            
            SetPerspective( core::d2r( 45.0f ), os::WindowAspect(), 0.1f, 1000.0f );
            SetPosition( V3UnitZ );
            SetTarget( V3Zero );
        }
        
        void Tick( f32 dt, SceneNode& node )
        {
            switch( _control )
            {
                case CameraControl_Fly:
                {
                    TickFly( dt );
                    break;
                }
                case CameraControl_Orbit:
                {
                    TickOrbit( dt );
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        
        void SetPerspective( f32 fov, f32 aspect, f32 zNear, f32 zFar )
        {
            _projection = perspective(fov, aspect, zNear, zFar);
        }
        
        void SetPosition( const v3& position )
        {
            _position = position;
            _view = look( _position, _target, V3UnitY );
        }
        
        void SetTarget( const v3& target )
        {
            _target = target;
            _view = look( _position, _target, V3UnitY );
        }
    };
    
    void InitCameras();
    void KillCameras();
    Camera* CameraNew();
    void CameraDelete( Camera* camera );
}

#endif