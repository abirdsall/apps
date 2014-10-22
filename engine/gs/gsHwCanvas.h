#ifndef GS_HW_CANVAS
#define GS_HW_CANVAS

namespace gs
{
	void CanvasHwNew( CanvasHandle handle );
	void CanvasHwDelete( CanvasHandle handle );
	void CanvasHwSet( const CanvasHandle handle, const s32 lod );
}

#endif