#ifndef GS_HW_CANVAS
#define GS_HW_CANVAS

namespace gs
{
	void CanvasHwNew( CanvasHandle handle );
	void CanvasHwDelete( CanvasHandle handle );
	void CanvasHwAdd( const CanvasHandle canvasHandle, const TextureHandle textureHandle, const u32 attachmentIndex, const u32 layer );
	void CanvasHwSet( const CanvasHandle handle, const s32 layer, const s32 lod );
}

#endif