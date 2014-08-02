#ifndef GS_HW
#define GS_HW

#include "gs.h"

namespace gs
{
	extern void ErrorCheck();
	
	void InitHw(void);
	void KillHw(void);
	
	void SetBlendHw( const eBlend blend );
	void SetDepthHw( const eDepth depth );
	void SetWriteHw( const eWrite write );
	void SetCullHw( const eCull cull );
	void SetScissorHw( const s32* scissor );
	void SetViewportHw( const s32* viewport );

	void ClearHw( const bool colour, const bool depth );
	u32 NewVertexArrayHw();
	u32 NewVertexBufferHw();
	u32 NewElementBufferHw();
	void SetVertexArrayHw(u32 va);
	void SetVertexBufferHw(u32 vb);
	void SetElementBufferHw(u32 eb);
	void FillVertexBufferHw( const void* data, s32 dataSize, bool dynamic );
	void FillElementBufferHw( const void* data, s32 dataSize, bool dynamic );
	void UpdateVertexBufferHw( const void* data, s32 dataSize, s32 dataOffset );
	void UpdateElementBufferHw( const void* data, s32 dataSize, s32 dataOffset );
	void DeleteVertexArrayHw(u32 va);
	void DeleteVertexBufferHw(u32 vb);
	void DeleteElementBufferHw(u32 eb);
	void SetArrayHw(const eAttrib attrib, const u32 size, const u32 stride, const void* pointer);
	void UnsetArrayHw(const eAttrib attrib);
	void DrawArrayHw(const ePrim primitive, const u32 num);
	void DrawElementsHw(const ePrim primitive, const u32 num);
}

#endif