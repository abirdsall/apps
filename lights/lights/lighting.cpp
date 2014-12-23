#include "fw.h"
#include "lighting.h"

fw::LightHandle _lightHandleA;
fw::LightHandle _lightHandleB;
fw::Camera* _camera;
fw::RadiosityRenderer* _renderer;
fw::TextureViewer _textureViewer;
gs::ShaderHandle _shaderFill3d;

void lightingTick( f32 dt )
{
    _textureViewer.Tick();

    v3 worldSize = _renderer->Bounds()._max - _renderer->Bounds()._min;
	v3 lightMove( 0.0f, 0.0f, 0.0f );
    
	if( os::KeyboardHeld( os::KeyA ) )
	{
		lightMove.x = -worldSize.x * dt;
	}
	if( os::KeyboardHeld( os::KeyD ) )
	{
		lightMove.x = worldSize.x * dt;
	}
	if( os::KeyboardHeld( os::KeyS ) )
	{
		lightMove.y = -worldSize.y * dt;
	}
	if( os::KeyboardHeld( os::KeyW ) )
	{
        lightMove.y = worldSize.y * dt;// * 0.1f;
	}
	if( os::KeyboardHeld( os::KeyQ ) )
	{
		lightMove.z = -worldSize.z * dt;
	}
	if( os::KeyboardHeld( os::KeyE ) )
	{
		lightMove.z = worldSize.z * dt;
	}

    //fw::LightSetPosition( _lightHandleA, fw::LightPosition( _lightHandleA ) + lightMove );
	fw::LightSetPosition( _lightHandleA, clamp( fw::LightPosition( _lightHandleA ) + lightMove, _renderer->Bounds()._min, _renderer->Bounds()._max ));
	//fw::LightSetPosition( _lightHandleB, clamp( fw::LightPosition( _lightHandleB ) - lightMove, _renderer->Bounds()._min, _renderer->Bounds()._max ));
	
	if( os::KeyboardUp( os::KeyZ ) )            
	{
		if( _textureViewer.Active() )
		{
			_textureViewer.SetActive( false );
		}
		else
		{
			_textureViewer.SetActive( true );
		}
	}
    
    _renderer->_scene->Tick( dt );
}

void lightingDraw()
{
    _renderer->Render( _camera );

//    gs::Put();
//    gs::ShaderSet( _shaderFill3d );
//    m4 pm = perspective( core::d2r( 55.0f ), 1.0f, 0.1f, 100.0f );
//    gs::SetCull(gs::CullFaceNone);
//    gs::SetMatrixP( pm );
//    gs::SetMatrixV( look( v3( 0.0f, 0.0f, 25.0f ), v3( 0.0f, 0.0f, 0.0f ), V3UnitY ) );
//    fw::DrawCube( v3( 0.0f, 0.0f, 1.0f ), v3( 2.0f, 2.0f, 2.0f ), v4( 0.0f, 1.0f, 0.0f, 1.0f ) );
//    
//    gs::Pop();

    gs::Put();
    gs::Set2d();
    
    gs::SetBlend(gs::BlendModeRgba);
    
    core::String test = core::String("0123456789abcdefghijklmnopqrstuvwxyz");
    
    fw::SystemFontDraw( test, v2( 100, 100 ), v4(1.0f, 1.0f, 1.0f, 1.0f ) );
    
    gs::Pop();

    _textureViewer.Draw();
}

void AddCube( const v3& position, const v3& radius, const v4& colour )
{
    fw::SceneNode* node = fw::SceneNodeNew();
    fw::RadiosityCube* radiosityCube = fw::RadiosityCubeNew();
    if(radius.x == 1.0f && radius.y == 1.0f && radius.z == 1.0f)
    {
        radiosityCube->_spin = true;
    }
    node->SetLocalPosition( position );
    node->SetLocalScale( radius );
    radiosityCube->Init( colour );
    node->AddComponent( radiosityCube );
    _renderer->_scene->AddChild( node );
}

void lightingInit()
{
    _renderer = new fw::RadiosityRenderer();
    
    fw::InitCameras();
    fw::InitSceneNodes();
    fw::InitLights();
    fw::InitDrawBatches();
    fw::InitCubeBatches();
    fw::InitQuad2dBatches();
    fw::InitRadiosityCubes();
    fw::SystemFontInit();
    
    _shaderFill3d = fw::ShaderMake3d( true, false );
    
    _lightHandleA = fw::LightNew( v3( 5.0f, 5.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ) * 0.85f );
	_lightHandleB = fw::LightNew( v3( 5.0f, 4.5f, 5.0f ), v3( 1.0f, 0.5f, 0.0f ) * 0.0f );

    _renderer->Init( aabb( v3( 0.0f, 0.0f, 0.0f ), v3( 32.0f, 32.0f, 8.0f ) ), 256, 256, 32 );
    _renderer->_lights.Add( _lightHandleA );
    _renderer->_lights.Add( _lightHandleB );
    _renderer->_scene = fw::SceneNodeNew();
    
    fw::SceneNode* cameraNode = fw::SceneNodeNew();
    _camera = fw::CameraNew();
    _camera->SetPosition( v3( 16.0f, 16.0f, 32.0f ) );
    _camera->SetTarget( v3( 16.0f, 16.0f, 0.0f ) );
    cameraNode->AddComponent( _camera );
    _renderer->_scene->AddChild( cameraNode );
    
    AddCube( v3( 16.0f, 16.0f, 0.0f ), v3( 16.0f, 16.0f, 0.5f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    AddCube( v3( 16.0f, 0.0f, 4.0f ), v3( 15.5f, 1.0f, 4.0f ), v4( 1.0f, 0.8f, 0.6f, 1.0f ) );
    AddCube( v3( 0.0f, 16.0f, 4.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    AddCube( v3( 32.0f, 16.0f, 4.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 1.0f, 1.0f, 0.0f, 1.0f ) );
    
    AddCube( v3( 4.0f, 4.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 8.0f, 3.0f, 4.5f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 12.0f, 2.0f, 4.25f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 16.0f, 5.0f, 4.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 20.0f, 5.0f, 3.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 24.0f, 5.0f, 2.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 28.0f, 5.0f, 1.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );

    AddCube( v3( 16.0f, 14.0f, 2.0f ), v3( 5.0f, 1.0f, 1.0f ), v4( 0.5f, 0.8f, 1.0f, 1.0f ) );

	_textureViewer.Init();
}
