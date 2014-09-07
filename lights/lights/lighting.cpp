/*
 
 current renderer:
 
 clear buffers
 
 set matrices to look down z axis
 
 voxelise half scene

 set matrices to look down z axis

 voxelise half scene
 
 blur voxelised scene
 
 calculate light volume
 
 render final scene with lighting look up
 
 
 generalised:
 
 voxel rep prepare
 
 render voxel contribs
 
 lighting rep generate
 
 render final scene with lighting model
 
 
 
 thoughts:
 
 model
 
 renderer object per material - voxelise pass is a material
 
 models are added to renderers...
 

 have a stat
 
 vertex data? draw call per model? or store vertex data in renderer and switch mvm
 
 
 
 desired renderer:
 
 fast low draw call ios renderer
 
 full fat renderer
 
 
*/
#include "fw.h"
#include "lighting.h"

fw::CanvasViewer mCanvasViewer;
fw::CameraHandle mCameraHandle;

fw::LightHandle _lightHandleA;
fw::LightHandle _lightHandleB;
fw::RadiosityRenderer _renderer;

void lightingTick( f32 dt )
{
    mCanvasViewer.Tick();

    v3 worldSize = _renderer.Bounds().mMax - _renderer.Bounds().mMin;
	v3 lightMove( 0.0f, 0.0f, 0.0f );
    
	if( os::KeyHeld( os::eKeyA ) )
	{
		lightMove.x = -worldSize.x * dt;
	}
	if( os::KeyHeld( os::eKeyD ) )
	{
		lightMove.x = worldSize.x * dt;
	}
	if( os::KeyHeld( os::eKeyS ) )
	{
		lightMove.y = -worldSize.y * dt;
	}
	if( os::KeyHeld( os::eKeyW ) )
	{
		lightMove.y = worldSize.y * dt;
	}
	if( os::KeyHeld( os::eKeyQ ) )
	{
		lightMove.z = -worldSize.z * dt;
	}
	if( os::KeyHeld( os::eKeyE ) )
	{
		lightMove.z = worldSize.z * dt;
	}
	
    
	fw::LightSetPosition( _lightHandleA, clamp( fw::LightPosition( _lightHandleA ) + lightMove, _renderer.Bounds().mMin, _renderer.Bounds().mMax ));
	//fw::LightSetPosition( _lightHandleB, clamp( fw::LightPosition( _lightHandleB ) - lightMove, _renderer.Bounds().mMin, _renderer.Bounds().mMax ));
	
	if( os::KeyUp( os::eKeyZ ) )            
	{
		if( mCanvasViewer.Active() )
		{
			mCanvasViewer.SetActive( false );
		}
		else
		{
			mCanvasViewer.SetActive( true );
		}
	}
}

void lightingDraw()
{
    _renderer.Render();

    gs::Put();
    gs::Set2d();
    
    gs::SetBlend(gs::eBlendRgba);
    
    fw::SystemFontDraw( core::String("0123456789abcdefghijklmnopqrstuvwxyz"), v2( 100, 100 ), v4(1.0f, 1.0f, 1.0f, 1.0f ) );
    
    gs::Pop();

    mCanvasViewer.Draw();
}

void AddCube( const v3& position, const v3& radius, const v4& colour )
{
    //fw::SceneNode* node = fw::SceneNodeNew();
    fw::RadiosityCube* radiosityCube = fw::RadiosityCubeNew();
    radiosityCube->_position = position;
    radiosityCube->_radius = radius;
    radiosityCube->_colour = colour;
    //node->AddComponent( radiosityCube );
    _renderer._scene->AddComponent( radiosityCube );
}

void lightingInit()
{
	fw::Init();
    
    _lightHandleA = fw::LightNew( v3( 5.0f, 5.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ) * 0.0f );
	_lightHandleB = fw::LightNew( v3( 5.0f, 4.5f, 5.0f ), v3( 1.0f, 0.4f, 0.0f ) );

    _renderer.Init( aabb( v3( 0.0f, 0.0f, 0.0f ), v3( 32.0f, 32.0f, 8.0f ) ), 256, 256, 8 );
    _renderer._lights.Add( _lightHandleA );
    _renderer._lights.Add( _lightHandleB );
    _renderer._scene = fw::SceneNodeNew();
    
    AddCube( v3( 16.0f, 16.0f, 0.0f - 0.0f ), v3( 16.0f, 16.0f, 0.05f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    AddCube( v3( 16.0f, 0.0f, 4.0f - 0.0f ), v3( 15.5f, 1.0f, 4.0f ), v4( 1.0f, 0.8f, 0.6f, 1.0f ) );
    AddCube( v3( 0.0f, 16.0f, 4.0f - 0.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    AddCube( v3( 32.0f, 16.0f, 4.0f - 0.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
    AddCube( v3( 4.0f, 4.0f, 7.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 8.0f, 4.0f, 6.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 12.0f, 4.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 16.0f, 4.0f, 4.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 20.0f, 4.0f, 3.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 24.0f, 4.0f, 2.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 28.0f, 4.0f, 1.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );

	mCameraHandle = fw::CameraNew( fw::Rect(0.0f, 0.0f, ( f32 )os::WindowSizeX(), ( f32 )os::WindowSizeY()), &lightingDraw );

	fw::CameraSetFocus( mCameraHandle, v3( 16.0f, 16.0f, 4.0f ) );

	mCanvasViewer.Init();
}
