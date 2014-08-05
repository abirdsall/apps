#ifndef CUBE_RENDERER
#define CUBE_RENDERER

#include "gs.h"
#include "renderer.h"

class CubeRenderer : public Renderer
{
protected:
	virtual void InitVoxeliser();
	virtual void InitRenderer();
	virtual void AddToVoxeliser( const fw::Component& component );
	virtual void AddToRenderer( const fw::Component& component );
	
	virtual size_t ComponentSize();
};

#endif
