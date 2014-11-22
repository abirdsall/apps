#ifndef GS_CANVAS
#define GS_CANVAS

namespace gs
{
	typedef ::u32 CanvasHandle;
	
	const u32 CanvasInvalid = MaxU32;
	const u32 CanvasLimit = 256;
	const u32 ColorTextureLimit = 16;
	const u32 DepthTextureLimit = 1;
	
	struct Canvas
	{
        core::String _name;
		u32 _colorTextureCount;
		u32 _depthTextureCount;
		u32 _colorTexture[ ColorTextureLimit ];
		u32 _depthTexture[ DepthTextureLimit ];
        u32 _colorTextureLayer[ ColorTextureLimit ];
        u32 _depthTextureLayer[ DepthTextureLimit ];
		u32 _sizeX;
		u32 _sizeY;
		bool _active;
	};
	
	void InitCanvases();
	void KillCanvases();
	
	CanvasHandle CanvasNew();
    CanvasHandle CanvasNew( const c8* name );
	CanvasHandle CanvasNew( TextureHandle textureHandle );
	void CanvasDelete( CanvasHandle handle );
	void CanvasAdd( CanvasHandle canvasHandle, TextureHandle textureHandle, u32 layer = 0 );
	void CanvasSet( CanvasHandle handle, s32 lod );
	void CanvasSet( CanvasHandle handle );
	u32 CanvasSizeX();
	u32 CanvasSizeY();
	
	Canvas& CanvasGet( CanvasHandle handle );
	u32 CanvasActive();
	s32 CanvasActiveLod();
	
	s32 CanvasActiveCount();
	s32 CanvasActiveGet( CanvasHandle array[ CanvasLimit ] );
}

#endif