#ifndef GS_CANVAS
#define GS_CANVAS

namespace gs
{
	typedef ::u32 CanvasHandle;
	
	const u32 kCanvasInvalid = kMaxU32;
	const u32 kCanvasLimit = 64;
	const u32 kColorTextureLimit = 16;
	const u32 kDepthTextureLimit = 1;
	
	struct Canvas
	{
		u32 mColorTextureCount;
		u32 mDepthTextureCount;
		u32 mColorTexture[ kColorTextureLimit ];
		u32 mDepthTexture[ kDepthTextureLimit ];
		u32 mSizeX;
		u32 mSizeY;
		bool mActive;
	};
	
	void InitCanvases();
	void KillCanvases();
	
	CanvasHandle CanvasNew();
	CanvasHandle CanvasNew( const TextureHandle textureHandle );
	void CanvasDelete( const CanvasHandle handle );
	void CanvasAdd( const CanvasHandle canvasHandle, const TextureHandle textureHandle, const u32 layer = 0 );
	void CanvasSet( const CanvasHandle handle, const s32 layer, const s32 lod );
	void CanvasSet( const CanvasHandle handle );
	u32 CanvasSizeX();
	u32 CanvasSizeY();
	
	const Canvas& CanvasGet( const CanvasHandle handle );
	u32 CanvasActive();
	s32 CanvasActiveLod();
	
	s32 CanvasActiveCount();
	s32 CanvasActiveGet( CanvasHandle array[ kCanvasLimit ] );
}

#endif