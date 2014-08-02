#include "fwRenderer.h"

namespace fw
{
	void Renderer::Init( s32 limit )
	{
		ComponentFactory::Init( limit );
		
		InitRenderer();
	}
	
	ComponentHandle Renderer::Add( const Component& component )
	{
		ComponentHandle handle = ComponentFactory::Add( component );
		
		if( handle != kComponentInvalid )
		{
			AddToRenderer( component );
		}
		
		return handle;
	}
	
	void Renderer::Draw( gs::ePrim primitive )
	{
		RequiresFinalising();

		fw::MeshDraw( mRenderer, primitive );
	}
	
	void Renderer::Kill()
	{
		fw::MeshKill( mRenderer );
		
		ComponentFactory::Kill();
	}
	
	bool Renderer::RequiresFinalising()
	{
		if( !mFinalised )
		{
			fw::MeshFinalise( mRenderer );
			
			mFinalised = true;
			
			return true;
		}
		return false;
	}
}