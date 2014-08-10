#ifndef RENDERER
#define RENDERER

class Renderer : public fw::Renderer
{
public:
	fw::Mesh mVoxeliser;
	
	void Init( s32 limit );
	fw::ComponentHandle Add( const fw::Component& component );
	void Draw( gs::ePrim primitive, bool voxelise );
	void Kill();
	
protected:
	virtual void InitVoxeliser() = 0;
	virtual void AddToVoxeliser( const fw::Component& component ) = 0;
};

#endif
