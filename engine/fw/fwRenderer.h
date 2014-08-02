#ifndef FW_RENDERER
#define FW_RENDERER

#include "os.h"
#include "fwMesh.h"
#include "fwComponentFactory.h"

namespace fw
{
	class Renderer : public ComponentFactory
	{
	public:
		virtual void Init( s32 limit );
		virtual ComponentHandle Add( const Component& component );
		virtual void Draw( gs::ePrim primitive );
		virtual void Kill();
		
	protected:
		fw::Mesh mRenderer;

		virtual bool RequiresFinalising();
		virtual void InitRenderer() = 0;
		virtual void AddToRenderer( const Component& component ) = 0;
		
	private:
		bool mFinalised;
	};
}

#endif