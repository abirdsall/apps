#ifndef GS
#define GS

#include "os.h"
#include "gsTexture.h"
#include "gsCanvas.h"
#include "gsShader.h"

#if GsOpenGles2

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#elif GsOpenGles3

#include <OpenGLES/ES3/gl.h>

#elif GsOpenGl3

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#else

#include <GL/gl.h>
#include <GL/glu.h>

#endif

#include "gsHwTexture.h"
#include "gsHwCanvas.h"
#include "gsHwShader.h"

namespace gs
{	
	enum Attribute
	{
		AttributeVertex,
		AttributeNormal,
		AttributeColour,
		AttributeTcoord,
		AttributeCount
	};

	enum Primitive
	{
		PrimitivePoints,
		PrimitiveLines,
		PrimitiveLineLoop,
		PrimitiveLineStrip,
		PrimitiveTriangles,
		PrimitiveTriangleFan,
		PrimitiveTriangleStrip,
		PrimitiveCount
	};
	
	enum BlendMode
	{
		BlendModeNone, // deprecated?
		BlendModeRgb,
		BlendModeRgba,
		BlendModeAddRgb,
		BlendModeAddRgba,
		BlendModeRgbDstAlpha,
		BlendModeMixRgbAddA,
		BlendModeMin,
		BlendModeMax
	};
	
	enum DepthTest
	{
		DepthTestNone,
		DepthTestLess,
		DepthTestEqual,
		DepthTestLequal,
		DepthTestGreater,
		DepthTestGequal,
		DepthTestNotEqual
	};
	
	enum WriteMask
	{
		WriteMaskNone = 0,
		WriteMaskZ = 0x01,
		WriteMaskR = 0x02,
		WriteMaskG = 0x04,
		WriteMaskB = 0x08,
		WriteMaskA = 0x10,
		WriteMaskRg = WriteMaskR | WriteMaskG,
		WriteMaskRgb = WriteMaskRg | WriteMaskB,
		WriteMaskRgba = WriteMaskRgb | WriteMaskA,
		WriteMaskRgbz = WriteMaskRgb | WriteMaskZ,
		WriteMaskRgbaz = WriteMaskRgba | WriteMaskZ
	};
	
	enum CullFace
	{
		CullFaceNone,
		CullFaceBack,
		CullFaceFront
	};
	
	struct state
	{
		BlendMode _blend;
		DepthTest _depth;
		WriteMask _write;
		CullFace _cull;
		s32 _scissor[4];
		s32 _viewport[4];
		m4 _matrixP;
        m4 _matrixV;
        m4 _matrixM;
	};
	
	void ApplyState();
	void Put();
	void Pop();
	void Set2d();
	void SetOrtho( f32 x1 = 0.0f, f32 y1 = 0.0f, f32 x2 = 1.0f, f32 y2 = 1.0f );
	void SetBlend( BlendMode blend );
	void SetDepth( DepthTest depth );
	void SetWrite( WriteMask write );
	void SetCull( CullFace cull );
	void SetScissor( s32 x1, s32 y1, s32 x2, s32 y2 );
	void SetViewport( s32 x1, s32 y1, s32 x2, s32 y2 );
	void SetMatrixP( const m4& matrix );
    void SetMatrixV( const m4& matrix );
	void SetMatrixM( const m4& matrix );
	
	u32 NewVertexArray();
	u32 NewVertexBuffer();
	u32 NewElementBuffer();
	void SetVertexArray( u32 va );
	void SetVertexBuffer( u32 vb );
	void SetElementBuffer( u32 eb );
	void FillVertexBuffer( const void* data, s32 dataSize, bool dynamic );
	void FillElementBuffer( const void* data, s32 dataSize, bool dynamic );
	void UpdateVertexBuffer( const void* data, s32 dataSize, s32 dataOffset );
	void UpdateElementBuffer( const void* data, s32 dataSize, s32 dataOffset );
	void DeleteVertexArray( u32 va );
	void DeleteVertexBuffer( u32 vb );
	void DeleteElementBuffer( u32 eb );
	void SetArray( Attribute attrib, u32 size, u32 stride, const void* pointer );
	void UnsetArray( Attribute attrib );
	void DrawArray( Primitive primitive, u32 num );
	void DrawElements( Primitive primitive, u32 num );
    
    s32 MaxDrawBuffers();

	const c8* AttributeName( Attribute attrib );
	void Clear( bool colour, bool depth );
	
	void Init();
	void Kill();
}

#endif