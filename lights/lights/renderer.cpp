#include "fw.h"
#include "renderer.h"

void Renderer::Init( s32 limit )
{
	fw::Renderer::Init( limit );
	
	InitVoxeliser();
}

fw::ComponentHandle Renderer::Add( const fw::Component& component )
{
	fw::ComponentHandle handle = fw::Renderer::Add( component );
	
	if( handle != fw::kComponentInvalid )
	{
		AddToVoxeliser( component );
	}
	
	return handle;
}

void Renderer::Draw( gs::ePrim primitive, bool voxelise )
{
	if( voxelise )
	{
		if( fw::Renderer::RequiresFinalising() )
		{
			fw::MeshFinalise( mVoxeliser );
		}

		fw::MeshDraw( mVoxeliser, primitive );
	}
	else
	{
		fw::Renderer::Draw( primitive );
	}
}

void Renderer::Kill()
{
	fw::MeshKill( mVoxeliser );
	
	fw::Renderer::Kill();
}
