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

fw::LightHandle _lightHandleA;
fw::LightHandle _lightHandleB;
fw::Camera* _camera;
fw::RadiosityRenderer* _renderer;
fw::TextureViewer _textureViewer;

void lightingTick( f32 dt )
{
    _textureViewer.Tick();

    v3 worldSize = _renderer->Bounds().mMax - _renderer->Bounds().mMin;
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
        lightMove.y = worldSize.y * dt;// * 0.1f;
	}
	if( os::KeyHeld( os::eKeyQ ) )
	{
		lightMove.z = -worldSize.z * dt;
	}
	if( os::KeyHeld( os::eKeyE ) )
	{
		lightMove.z = worldSize.z * dt;
	}

    //fw::LightSetPosition( _lightHandleA, fw::LightPosition( _lightHandleA ) + lightMove );
	fw::LightSetPosition( _lightHandleA, clamp( fw::LightPosition( _lightHandleA ) + lightMove, _renderer->Bounds().mMin, _renderer->Bounds().mMax ));
	//fw::LightSetPosition( _lightHandleB, clamp( fw::LightPosition( _lightHandleB ) - lightMove, _renderer->Bounds().mMin, _renderer->Bounds().mMax ));
	
	if( os::KeyUp( os::eKeyZ ) )            
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
    _renderer->Render();

    gs::Put();
    gs::Set2d();
    
    gs::SetBlend(gs::eBlendRgba);
    
    core::String test = core::String("0123456789abcdefghijklmnopqrstuvwxyz");
    
    fw::SystemFontDraw( test, v2( 100, 100 ), v4(1.0f, 1.0f, 1.0f, 1.0f ) );
    
    gs::Pop();

    _textureViewer.Draw();
}

void AddCube( const v3& position, const v3& radius, const v4& colour )
{
    fw::SceneNode* node = fw::SceneNodeNew();
    fw::RadiosityCube* radiosityCube = fw::RadiosityCubeNew();
    node->_localTransform = identity4();
    node->_localTransform.setPosition( position );
    //v3 test = node->_localTransform.getPosition();
    node->_modelScale = radius;
    radiosityCube->_colour = colour;
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
    
    _lightHandleA = fw::LightNew( v3( 5.0f, 5.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ) * 0.5f );
	_lightHandleB = fw::LightNew( v3( 5.0f, 4.5f, 5.0f ), v3( 1.0f, 0.5f, 0.0f ) * 0.0f );

    _renderer->Init( aabb( v3( 0.0f, 0.0f, 0.0f ), v3( 32.0f, 32.0f, 8.0f ) ), 256, 256, 16 );
    _renderer->_lights.Add( _lightHandleA );
    _renderer->_lights.Add( _lightHandleB );
    _renderer->_scene = fw::SceneNodeNew();
    
    fw::SceneNode* cameraNode = fw::SceneNodeNew();
    _camera = fw::CameraNew();
    cameraNode->AddComponent( _camera );
    _renderer->_scene->AddChild( cameraNode );
    
    AddCube( v3( 16.0f, 16.0f, 0.0f - 0.0f ), v3( 16.0f, 16.0f, 0.5f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    AddCube( v3( 16.0f, 0.0f, 4.0f - 0.0f ), v3( 15.5f, 1.0f, 4.0f ), v4( 1.0f, 0.8f, 0.6f, 1.0f ) );
    AddCube( v3( 0.0f, 16.0f, 4.0f - 0.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    AddCube( v3( 32.0f, 16.0f, 4.0f - 0.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 1.0f, 1.0f, 0.0f, 1.0f ) );
    
    AddCube( v3( 4.0f, 3.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 8.0f, 2.0f, 4.5f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 12.0f, 1.0f, 4.25f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    AddCube( v3( 16.0f, 4.0f, 4.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 20.0f, 4.0f, 3.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 24.0f, 4.0f, 2.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );
    AddCube( v3( 28.0f, 4.0f, 1.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) );

    AddCube( v3( 16.0f, 14.0f, 2.0f ), v3( 5.0f, 1.0f, 1.0f ), v4( 0.5f, 0.8f, 1.0f, 1.0f ) );

	_textureViewer.Init();
}
