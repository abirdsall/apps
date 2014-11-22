#ifndef GS_HW
#define GS_HW

#include "gs.h"

namespace gs
{
#ifdef DebugGs
	extern void ErrorCheck();
#else    
#define ErrorCheck
#endif
    
	void InitHw();
	void KillHw();
	
	void SetBlendHw( BlendMode blend );
	void SetDepthHw( DepthTest depth );
	void SetWriteHw( WriteMask write );
	void SetCullHw( CullFace cull );
	void SetScissorHw( const s32* scissor );
	void SetViewportHw( const s32* viewport );

	void ClearHw( bool colour, bool depth );
	u32 NewVertexArrayHw();
	u32 NewVertexBufferHw();
	u32 NewElementBufferHw();
	void SetVertexArrayHw( u32 va );
	void SetVertexBufferHw( u32 vb );
	void SetElementBufferHw( u32 eb );
	void FillVertexBufferHw( const void* data, s32 dataSize, bool dynamic );
	void FillElementBufferHw( const void* data, s32 dataSize, bool dynamic );
	void UpdateVertexBufferHw( const void* data, s32 dataSize, s32 dataOffset );
	void UpdateElementBufferHw( const void* data, s32 dataSize, s32 dataOffset );
	void DeleteVertexArrayHw( u32 va );
	void DeleteVertexBufferHw( u32 vb );
	void DeleteElementBufferHw( u32 eb );
	void SetArrayHw( Attribute attrib, u32 size, u32 stride, const void* pointer );
	void UnsetArrayHw( Attribute attrib );
	void DrawArrayHw( Primitive primitive, u32 num );
	void DrawElementsHw( Primitive primitive, u32 num );
}

#endif