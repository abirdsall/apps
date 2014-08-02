#ifndef GS
#define GS

#include "os.h"
#include "matrix.h"

#include "gsTexture.h"
#include "gsCanvas.h"
#include "gsShader.h"

#if kBuildIos

#include <OpenGLES/ES2/gl.h>

#elif kBuildMac

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#else

#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include "gshwTexture.h"
#include "gshwCanvas.h"
#include "gshwShader.h"

namespace gs
{	
	enum eAttrib
	{
		eAttribVertex,
		eAttribNormal,
		eAttribColour,
		eAttribTcoord,
		eAttribCount
	};

	enum ePrim
	{
		ePrimPoints,
		ePrimLines,
		ePrimLineLoop,
		ePrimLineStrip,
		ePrimTriangles,
		ePrimTriangleFan,
		ePrimTriangleStrip,
		ePrimCount
	};
	
	enum eBlend
	{
		eBlendNone, // deprecated?
		eBlendRgb,
		eBlendRgba,
		eBlendAddRgb,
		eBlendAddRgba,
		eBlendRgbDstAlpha,
		eBlendMixRgbAddA,
		eBlendMin,
		eBlendMax
	};
	
	enum eDepth
	{
		eDepthNone,
		eDepthLess,
		eDepthEqual,
		eDepthLequal,
		eDepthGreater,
		eDepthGequal,
		eDepthNotEqual
	};
	
	enum eWrite
	{
		eWriteNone = 0,
		eWriteZ = 0x01,
		eWriteR = 0x02,
		eWriteG = 0x04,
		eWriteB = 0x08,
		eWriteA = 0x10,
		eWriteRg = eWriteR | eWriteG,
		eWriteRgb = eWriteRg | eWriteB,
		eWriteRgba = eWriteRgb | eWriteA,
		eWriteRgbz = eWriteRgb | eWriteZ,
		eWriteRgbaz = eWriteRgba | eWriteZ
	};
	
	enum eCull
	{
		eCullNone,
		eCullBack,
		eCullFront
	};
	
	struct state
	{
		eBlend mBlend;
		eDepth mDepth;
		eWrite mWrite;
		eCull mCull;
		s32 mScissor[4];
		s32 mViewport[4];
		m4 mMatrixM;
		m4 mMatrixP;
	};
	
	void ApplyState();
	void Put(void);
	void Pop(void);
	void Set2d(void);
	void SetOrtho(const f32 x1 = 0.0f, const f32 y1 = 0.0f, const f32 x2 = 1.0f, const f32 y2 = 1.0f);	
	void SetBlend(const eBlend blend);
	void SetDepth(const eDepth depth);
	void SetWrite(const eWrite write);
	void SetCull(const eCull cull);
	void SetScissor(const s32 x1, const s32 y1, const s32 x2, const s32 y2);
	void SetViewport(const s32 x1, const s32 y1, const s32 x2, const s32 y2);
	void SetMatrixP(const m4& matrix);
	void SetMatrixM(const m4& matrix);
	
	u32 NewVertexArray();
	u32 NewVertexBuffer();
	u32 NewElementBuffer();
	void SetVertexArray(u32 va);
	void SetVertexBuffer(u32 vb);
	void SetElementBuffer(u32 eb);
	void FillVertexBuffer( const void* data, s32 dataSize, bool dynamic );
	void FillElementBuffer( const void* data, s32 dataSize, bool dynamic );
	void UpdateVertexBuffer( const void* data, s32 dataSize, s32 dataOffset );
	void UpdateElementBuffer( const void* data, s32 dataSize, s32 dataOffset );
	void DeleteVertexArray(u32 va);
	void DeleteVertexBuffer(u32 vb);
	void DeleteElementBuffer(u32 eb);
	void SetArray(const eAttrib attrib, const u32 size, const u32 stride, const void* pointer);
	void UnsetArray(const eAttrib attrib);
	void DrawArray(const ePrim primitive, const u32 num);
	void DrawElements(const ePrim primitive, const u32 num);

	const c8* AttribName( const eAttrib attrib );
	void Clear(const bool colour, const bool depth);
	
	void Init(void);
	void Kill(void);
}

#endif