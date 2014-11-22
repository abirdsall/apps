#include "fw.h"

namespace fw
{
    static Pool<Camera> _cameras;
    
    static const f32 FlyRotationalSpeed = 5e2f;
    static const f32 FlyMovementSpeed = 2.0f;
    static const f32 OrbitRotationalSpeed = 120.0f;
    static const f32 OrbitMovementSpeed = 1200.0f;
    
    void InitCameras()
    {
        _cameras.Init( 32 );
    }
    
    void KillCameras()
    {
    }
    
    Camera* CameraNew()
    {
        return _cameras.New();
    }
    
    void CameraDelete( Camera* camera )
    {
        _cameras.Delete( camera );
    }
    
    void Camera::TickFly( f32 dt )
    {
        v3 position = _view.getPosition();
        
        if( os::MouseButtonDown( os::MouseButtonRight ) )
        {
            os::MouseSetVisibile( false );
        }
        
        if( os::MouseButtonUp( os::MouseButtonRight ) )
        {
            os::MouseSetVisibile( true );
        }
        
        if( os::MouseButtonHeld( os::MouseButtonRight ) )
        {
            v2 delta = os::MouseVelocity() * dt * FlyRotationalSpeed;
            
            if( core::abs( delta.x ) > 0.0f || core::abs( delta.y ) > 0.0f)
            {
                _view.setPosition( v3( 0.0f, 0.0f, 0.0f ) );
                
                if( core::abs( delta.x ) > 0.0f )
                {
                    _view = multiply33( _view, r4( V3UnitY, delta.x ) );
                }
                
                if( core::abs( delta.y ) > 0.0f )
                {
                    _view = multiply33( _view, r4( _view.rows[ 0 ].xyz(), -delta.y ) );
                }
                
                _view.stabilise();
            }
        }
        
        if( os::KeyboardHeld( os::KeyA ) )
        {
            position -= _view.rows[ 0 ].xyz() * dt * FlyMovementSpeed;
        }
        
        if( os::KeyboardHeld( os::KeyD ) )
        {
            position += _view.rows[ 0 ].xyz() * dt * FlyMovementSpeed;
        }
        
        if( os::KeyboardHeld( os::KeyS ) )
        {
            position -= _view.rows[ 2 ].xyz() * dt * FlyMovementSpeed;
        }
        
        if( os::KeyboardHeld( os::KeyW ) )
        {
            position += _view.rows[ 2 ].xyz() * dt * FlyMovementSpeed;
        }
        
        _view.setPosition( position );
    }
    
    void Camera::TickOrbit( f32 dt )
    {
        if( os::KeyboardHeld( os::KeySpace ) )
        {
            if( os::MouseButtonHeld( os::MouseButtonLeft ) )
            {
                v2 delta = -os::MouseVelocity() * OrbitRotationalSpeed * dt;
                
                _view = multiply33( _view, r4( V3UnitY, delta.x ) );
                _view = multiply33( _view, r4( _view.rows[ 0 ].xyz(), delta.y ) );
                
                if( dot( _view.rows[ 1 ].xyz(), V3UnitY ) < 0.0f )
                {
                    v3 limit = dot( _view.rows[2].xyz(), V3UnitY ) > 0.0f ? V3UnitY : -V3UnitY;
                    
                    _view = multiply33( _view, r4( limit, _view.rows[2].xyz() ) );
                }
            }
            
            f32 distance = core::distance( _view.getPosition(), _target );
            
            if( os::MouseButtonHeld( os::MouseButtonRight ) )
            {
                distance += os::MouseVelocity().y * OrbitMovementSpeed * dt;
                
                if( distance < 0.0f )
                {
                    distance = 0.0f;
                }
            }
            
            _view.setPosition( _target - _view.rows[ 2 ].xyz() * distance);
        }
    }
}