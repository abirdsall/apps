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
        m4 _projection;
        m4 _view;
        v3 _target;
        CameraControl _control;
        
        void TickFly( f32 dt );
        void TickOrbit( f32 dt );
        
    public:
        
        Camera()
        {
            _projection = identity4();
            _view = identity4();
            _control = CameraControl_Orbit;
            
            SetPerspective( 85.0f, os::WindowAspect(), 0.01f, 1000.0f );
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
            _projection = projection(fov, aspect, zNear, zFar);
        }
        
        void SetPosition( const v3& position )
        {
            _view.setPosition( position );
        }
        
        void SetTarget( const v3& target )
        {
            _target = target;
            _view = look( _view.getPosition(), target, V3UnitY );
        }
    };
    
    void InitCameras();
    void KillCameras();
    Camera* CameraNew();
    void CameraDelete( Camera* camera );
}

#endif