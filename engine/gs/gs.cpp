#include "gsHw.h"

namespace gs
{
	static const c8* kAttribNames[ eAttribCount ] =
	{
		"vertex_position",
		"vertex_normal",
		"vertex_colour",
		"vertex_tcoord"
	};

	static const u32 kStateLimit = 8;
	static state sState[ kStateLimit ];
	static state sStateApplied;
	static u32 sStateActive = 0;

	static void InitStates()
	{
		SetBlend( eBlendRgba );
		SetDepth( eDepthLequal );
		SetWrite( eWriteRgbz );
		SetCull( eCullBack );
		SetScissor( 0, 0, CanvasSizeX(), CanvasSizeY() );
		SetViewport( 0, 0, CanvasSizeX(), CanvasSizeY() );
		SetMatrixP( identity4() );
		SetMatrixM( identity4() );
		
		state& stateActive = sState[ sStateActive ];
		
		SetBlendHw( stateActive.mBlend );
		SetDepthHw( stateActive.mDepth );
		SetWriteHw( stateActive.mWrite );
		SetCullHw( stateActive.mCull );
		SetScissorHw( stateActive.mScissor );
		SetViewportHw( stateActive.mViewport );
		
		memory::copy( &sStateApplied, &sState[ sStateActive ], sizeof( state ) );
	}
	
	void ApplyState()
	{
		state& stateActive = sState[ sStateActive ];
		
		if( stateActive.mBlend != sStateApplied.mBlend )
		{
			SetBlendHw( stateActive.mBlend );
		}
		
		if( stateActive.mDepth != sStateApplied.mDepth )
		{
			SetDepthHw( stateActive.mDepth );
		}
		
		if( stateActive.mWrite != sStateApplied.mWrite )
		{
			SetWriteHw( stateActive.mWrite );
		}
		
		if( stateActive.mCull != sStateApplied.mCull )
		{
			SetCullHw( stateActive.mCull );
		}
		
		if( stateActive.mScissor != sStateApplied.mScissor )
		{
			SetScissorHw( stateActive.mScissor );
		}
		
		if( stateActive.mViewport != sStateApplied.mViewport )
		{
			SetViewportHw( stateActive.mViewport );
		}
		
		if( ShaderActive() != kShaderInvalid )
		{
			ShaderSetMat4( "viewMatrix", stateActive.mMatrixM );
			ShaderSetMat4( "projMatrix", stateActive.mMatrixP );
		}

		memory::copy( &sStateApplied, &sState[ sStateActive ], sizeof( state ) );
	}
	
	void Put()
	{
		sStateActive++;
		ASSERT( sStateActive < kStateLimit );
		memory::copy( &sState[ sStateActive ], &sState[ sStateActive - 1 ], sizeof( state ) );
	}
	
	void Pop()
	{
		ASSERT( sStateActive > 0 );
		sStateActive--;
	}
		
	void SetBlend(const eBlend blend)
	{
		sState[ sStateActive ].mBlend = blend;
	}
	
	void SetDepth(const eDepth depth)
	{
		sState[ sStateActive ].mDepth = depth;
	}
	
	void SetWrite(const eWrite write)
	{
		sState[ sStateActive ].mWrite = write;
	}
	
	void SetCull(const eCull cull)
	{
		sState[ sStateActive ].mCull = cull;
	}
	
	void SetScissor(const s32 x1, const s32 y1, const s32 x2, const s32 y2)
	{
		state& activeState = sState[ sStateActive ];
		
		activeState.mScissor[ 0 ] = x1;
		activeState.mScissor[ 1 ] = y1;
		activeState.mScissor[ 2 ] = x2;
		activeState.mScissor[ 3 ] = y2;
		
		SetScissorHw( activeState.mScissor );
		
		sStateApplied.mScissor[ 0 ] = x1;
		sStateApplied.mScissor[ 1 ] = y1;
		sStateApplied.mScissor[ 2 ] = x2;
		sStateApplied.mScissor[ 3 ] = y2;
	}
	
	void SetViewport(const s32 x1, const s32 y1, const s32 x2, const s32 y2)
	{
		state& activeState = sState[ sStateActive ];
		
		activeState.mViewport[ 0 ] = x1;
		activeState.mViewport[ 1 ] = y1;
		activeState.mViewport[ 2 ] = x2;
		activeState.mViewport[ 3 ] = y2;
		
		SetViewportHw( activeState.mViewport );
		
		sStateApplied.mViewport[ 0 ] = x1;
		sStateApplied.mViewport[ 1 ] = y1;
		sStateApplied.mViewport[ 2 ] = x2;
		sStateApplied.mViewport[ 3 ] = y2;
	}
	
	void SetMatrixP(const m4& matrix)
	{
		sState[ sStateActive ].mMatrixP = matrix;
	}
	
	void SetMatrixM(const m4& matrix)
	{
		sState[ sStateActive ].mMatrixM = matrix;
	}
	
	void Set2d(void)
	{
		s32* v = sStateApplied.mViewport;
		SetOrtho( v[ 0 ], v[ 1 ], v[ 2 ], v[ 3 ] );
	}
	
	void SetOrtho( const f32 x1, const f32 y1, const f32 x2, const f32 y2 )
	{
		SetMatrixP( orthogonal( x1, x2, y1, y2, -1.0f, 1.0f ) );
		SetMatrixM( identity4() );
	}
	
	u32 NewVertexArray()
	{
		return NewVertexArrayHw();
	}
	
	u32 NewVertexBuffer()
	{
		return NewVertexBufferHw();
	}
	
	u32 NewElementBuffer()
	{
		return NewElementBufferHw();
	}

	void SetVertexArray(u32 va)
	{
		SetVertexArrayHw( va );
	}
	
	void SetVertexBuffer(u32 vb)
	{
		SetVertexBufferHw( vb );
	}

	void SetElementBuffer(u32 eb)
	{
		SetElementBufferHw( eb );
	}

	void FillVertexBuffer( const void* data, s32 dataSize, bool dynamic )
	{
		FillVertexBufferHw( data, dataSize, dynamic );
	}
	
	void FillElementBuffer( const void* data, s32 dataSize, bool dynamic )
	{
		FillElementBufferHw( data, dataSize, dynamic );
	}
	
	void UpdateVertexBuffer( const void* data, s32 dataSize, s32 dataOffset )
	{
		UpdateVertexBufferHw( data, dataSize, dataOffset );
	}
	
	void UpdateElementBuffer( const void* data, s32 dataSize, s32 dataOffset )
	{
		UpdateElementBufferHw( data, dataSize, dataOffset );
	}
	
	void DeleteVertexArray(u32 va)
	{
		DeleteVertexArray( va );
	}
	
	void DeleteVertexBuffer(u32 vb)
	{
		DeleteVertexBufferHw( vb );
	}
	
	void DeleteElementBuffer(u32 eb)
	{
		DeleteElementBufferHw( eb );
	}

	void SetArray(const eAttrib attrib, const u32 size, const u32 stride, const void* pointer)
	{
		SetArrayHw( attrib, size, stride, pointer );
	}
	
	void UnsetArray(const eAttrib attrib)
	{
		UnsetArrayHw( attrib );
	}
	
	void DrawArray(const ePrim primitive, const u32 num)
	{
		DrawArrayHw( primitive, num );
	}
	
	void DrawElements( const ePrim primitive, const u32 num )
	{
		DrawElementsHw( primitive, num );
	}

	const c8* AttribName( const eAttrib attrib )
	{
		return kAttribNames[ attrib ];
	}
	
	void Clear( const bool colour, const bool depth )
	{
		ClearHw( colour, depth );
	}
	
	void Init(void)
	{
		InitHw();
		InitCanvases();
		InitShaders();
		InitTextures();
		
		InitStates();
	}
	
	void Kill(void)
	{
		KillCanvases();
		KillShaders();
		KillTextures();
		KillHw();
	}
}