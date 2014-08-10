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
#include "gs.h"
#include "intersect.h"
#include "fwDraw.h"
#include "fwMesh.h"
#include "fwRect.h"
#include "fwString.h"
#include "lighting.h"
#include "fwCubeComponent.h"
#include "cubeRenderer.h"

fw::CanvasViewer mCanvasViewer;
fw::CameraHandle mCameraHandle;
fw::LightHandle mLightHandle;
fw::LightHandle mLightHandle2;

CubeRenderer mCubeRenderer;

void lightingTick( f32 dt )
{
    mCanvasViewer.Tick();

    /*
    if( os::KeyUp( os::eKeyUp ) )
    {
        printf("up UP\n");
    }
    if( os::KeyUp( os::eKeyDown ) )
    {
        printf("dn UP\n");
    }
    if( os::KeyUp( os::eKeyLeft ) )
    {
        printf("lf UP\n");
    }
    if( os::KeyUp( os::eKeyRight ) )
    {
        printf("rt UP\n");
    }
    
    if( os::KeyUp( os::eKeyA ) )
    {
        printf("a UP\n");
    }


    if( os::KeyHeld( os::eKeyUp ) )
    {
        printf("up held\n");
    }
    if( os::KeyHeld( os::eKeyDown ) )
    {
        printf("dn held\n");
    }
    if( os::KeyHeld( os::eKeyLeft ) )
    {
        printf("lf held\n");
    }
    if( os::KeyHeld( os::eKeyRight ) )
    {
        printf("rt held\n");
    }
    
    if( os::KeyHeld( os::eKeyA ) )
    {
        printf("a held\n");
    }
    
    */
	v3 worldSize = fw::LightGetBounds().mMax - fw::LightGetBounds().mMin;
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
	
	fw::LightSetPosition( mLightHandle, clamp( fw::LightGetPosition( mLightHandle ) + lightMove, fw::LightGetBounds().mMin, fw::LightGetBounds().mMax ));
	//fw::LightSetPosition( mLightHandle2, clamp( fw::LightGetPosition( mLightHandle2 ) - lightMove, fw::LightGetBounds().mMin, fw::LightGetBounds().mMax ));
	
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

void setStageMatrices( bool tracing )
{
	aabb bounds = fw::LightGetBounds();
	m4 mp = orthogonal(
					   bounds.mMin.x, bounds.mMax.x,
					   bounds.mMax.y, bounds.mMin.y,
					   bounds.mMax.z + 20.0f, bounds.mMin.z - 20.0f );
	gs::SetMatrixP( mp );
	if( tracing )
	{
		m4 mm = identity4();
		mm.rows[ 2 ].z = -1.0f;
		mm.setPosition( v3( 0.0f, 0.0f, 20.0f ) );
		gs::SetMatrixM( mm );
	}
}

void lightingDraw()
{

    //gs::Put();
    
	fw::DrawLights( fw::DrawPhasePreVoxelPass );

	setStageMatrices(true);
	
	mCubeRenderer.Draw( gs::ePrimTriangles, true );

	fw::DrawLights( fw::DrawPhaseMake );
	
	//setStageMatrices( false );
	
	mCubeRenderer.Draw( gs::ePrimTriangles, false );
	
	fw::DrawLights( fw::DrawPhaseFinal );
    
    //gs::Pop();

/*
	// debug
	gs::Put();
    gs::ShaderSet( sFillShader );
	gs::SetCull(gs::eCullNone);
	gs::SetWrite(gs::eWriteRgb);
    gs::SetBlend(gs::eBlendRgba);
	gs::SetDepth(gs::eDepthNone);//Lequal);
    gs::SetMatrixM( identity4() );

	setStageMatrices(false);

    mCubeRenderer.Draw( gs::ePrimLineLoop, false );
    
	gs::Pop();
*/

    mCanvasViewer.Draw();
}

void lightingInit()
{
	fw::DrawInit();
    
	fw::InitLights( aabb( v3( 0.0f, 0.0f, 0.0f ), v3( 32.0f, 32.0f, 8.0f ) ) );
	
    mLightHandle = fw::LightNew( v3( 5.0f, 5.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ) * 0.5f );
	//mLightHandle2 = fw::LightNew( v3( 5.0f, 4.5f, 5.0f ), v3( 1.0f, 0.4f, 0.0f ) );

	mCameraHandle = fw::CameraNew( fw::Rect(0.0f, 0.0f, ( f32 )os::WindowSizeX(), ( f32 )os::WindowSizeY()), &lightingDraw );

	fw::CameraSetFocus( mCameraHandle, v3( 16.0f, 16.0f, 4.0f ) );
	
	mCubeRenderer.Init( 128 );

	mCubeRenderer.Add( fw::CubeComponent( v3( 16.0f, 16.0f, 0.0f - 0.0f ), v3( 16.0f, 16.0f, 0.05f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );
	mCubeRenderer.Add( fw::CubeComponent( v3( 16.0f, 0.0f, 4.0f - 0.0f ), v3( 15.5f, 1.0f, 4.0f ), v4( 1.0f, 0.8f, 0.6f, 1.0f ) ) );
	mCubeRenderer.Add( fw::CubeComponent( v3( 0.0f, 16.0f, 4.0f - 0.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );
	mCubeRenderer.Add( fw::CubeComponent( v3( 32.0f, 16.0f, 4.0f - 0.0f ), v3( 0.5f, 16.0f, 4.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );

//	mCubeRenderer.Add( fw::CubeComponent( v3( 0.0f, 4.0f, 8.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 4.0f, 4.0f, 7.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 8.0f, 4.0f, 6.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 12.0f, 4.0f, 5.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 0.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 16.0f, 4.0f, 4.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 0.0f, 1.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 20.0f, 4.0f, 3.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 24.0f, 4.0f, 2.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 28.0f, 4.0f, 1.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) ) );
//    mCubeRenderer.Add( fw::CubeComponent( v3( 32.0f, 4.0f, 0.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.0f, 0.0f, 1.0f, 1.0f ) ) );

//	mCubeRenderer.Add( fw::CubeComponent( v3( 8.0f, 22.0f, 8.0f - 4.0f ), v3( 1.0f, 3.0f, 1.0f ), v4( 0.6f, 0.8f, 0.7f, 1.0f ) ) );
//	mCubeRenderer.Add( fw::CubeComponent( v3( 12.0f, 22.0f, 7.0f - 4.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 0.6f, 0.7f, 0.9f, 1.0f ) ) );
//	mCubeRenderer.Add( fw::CubeComponent( v3( 16.0f, 22.0f, 6.0f - 4.0f ), v3( 1.0f, 2.5f, 1.0f ), v4( 0.4f, 0.6f, 0.8f, 1.0f ) ) );
//	mCubeRenderer.Add( fw::CubeComponent( v3( 20.0f, 22.0f, 5.0f - 4.0f ), v3( 1.0f, 2.0f, 1.0f ), v4( 0.9f, 0.8f, 0.7f, 1.0f ) ) );
//	mCubeRenderer.Add( fw::CubeComponent( v3( 24.0f, 22.0f, 4.0f - 4.0f ), v3( 1.0f, 1.5f, 1.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );

	mCanvasViewer.Init();
}



/*
 #if kBuildOpenGles3
 fw::String vShader = "#version 300 es\n";
 #else //kBuildOpenGl3
 fw::String vShader = "#version 150\n";
 #endif
 vShader = vShader + "in vec3 vertex_position;\n";
 vShader += "in vec4 vertex_colour;\n";
 vShader += "in vec4 vertex_tcoord;\n";
 vShader += "out vec4 fragment_colour;\n";
 vShader += "uniform mat4 viewMatrix;\n";
 vShader += "uniform mat4 projMatrix;\n";
 vShader += "void main()\n";
 vShader += "{\n";
 vShader = vShader + "\tgl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1);\n";
 vShader += "\tfragment_colour = vertex_colour;\n";
 vShader += "}\n";
 
 #if kBuildOpenGles3
 fw::String fShader = "#version 300 es\n";
 fShader += "precision highp float;\n";
 #else //kBuildOpenGl3
 fw::String fShader = "#version 150\n";
 #endif
 fShader += "in vec4 fragment_colour;\n";
 fShader += "out vec4 output_colour;\n";
 fShader += "void main()\n";
 fShader += "{\n";
 fShader += "\toutput_colour = fragment_colour;\n";
 fShader += "}\n";
 
 sFillShader = gs::ShaderNew( vShader.toStr(), fShader.toStr() );
 */
