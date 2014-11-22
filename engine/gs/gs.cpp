#include "gsHw.h"

namespace gs
{
	static const c8* AttributeNames[ AttributeCount ] =
	{
		"vertex_position",
		"vertex_normal",
		"vertex_colour",
		"vertex_tcoord"
	};

	static const u32 StateLimit = 8;
	static state _state[ StateLimit ];
	static state _stateApplied;
	static u32 _stateActive = 0;

	static void InitStates()
	{
		SetBlend( BlendModeRgba );
		SetDepth( DepthTestLequal );
		SetWrite( WriteMaskRgbz );
		SetCull( CullFaceBack );
		SetScissor( 0, 0, CanvasSizeX(), CanvasSizeY() );
		SetViewport( 0, 0, CanvasSizeX(), CanvasSizeY() );
		SetMatrixP( identity4() );
		SetMatrixM( identity4() );
		
		state& stateActive = _state[ _stateActive ];
		
		SetBlendHw( stateActive._blend );
		SetDepthHw( stateActive._depth );
		SetWriteHw( stateActive._write );
		SetCullHw( stateActive._cull );
		SetScissorHw( stateActive._scissor );
		SetViewportHw( stateActive._viewport );
		
		core::copy( &_stateApplied, &_state[ _stateActive ], sizeof( state ) );
	}
	
	void ApplyState()
	{
		state& stateActive = _state[ _stateActive ];
		
		if( stateActive._blend != _stateApplied._blend )
		{
			SetBlendHw( stateActive._blend );
		}
		
		if( stateActive._depth != _stateApplied._depth )
		{
			SetDepthHw( stateActive._depth );
		}
		
		if( stateActive._write != _stateApplied._write )
		{
			SetWriteHw( stateActive._write );
		}
		
		if( stateActive._cull != _stateApplied._cull )
		{
			SetCullHw( stateActive._cull );
		}
		
		if( stateActive._scissor != _stateApplied._scissor )
		{
			SetScissorHw( stateActive._scissor );
		}
		
		if( stateActive._viewport != _stateApplied._viewport )
		{
			SetViewportHw( stateActive._viewport );
		}
		
		if( ShaderActive() != ShaderInvalid )
		{
			ShaderSetMat4( "viewMatrix", stateActive._matrixM );
			ShaderSetMat4( "projMatrix", stateActive._matrixP );
		}

		core::copy( &_stateApplied, &_state[ _stateActive ], sizeof( state ) );
	}
	
	void Put()
	{
		_stateActive++;
		ASSERT( _stateActive < StateLimit );
		core::copy( &_state[ _stateActive ], &_state[ _stateActive - 1 ], sizeof( state ) );
	}
	
	void Pop()
	{
		ASSERT( _stateActive > 0 );
		_stateActive--;
	}
		
	void SetBlend( BlendMode blend )
	{
		_state[ _stateActive ]._blend = blend;
	}
	
	void SetDepth( DepthTest depth )
	{
		_state[ _stateActive ]._depth = depth;
	}
	
	void SetWrite( WriteMask write )
	{
		_state[ _stateActive ]._write = write;
	}
	
	void SetCull(CullFace cull)
	{
		_state[ _stateActive ]._cull = cull;
	}
	
	void SetScissor( s32 x1, s32 y1, s32 x2, s32 y2 )
	{
		state& activeState = _state[ _stateActive ];
		
		activeState._scissor[ 0 ] = x1;
		activeState._scissor[ 1 ] = y1;
		activeState._scissor[ 2 ] = x2;
		activeState._scissor[ 3 ] = y2;
		
		SetScissorHw( activeState._scissor );
		
		_stateApplied._scissor[ 0 ] = x1;
		_stateApplied._scissor[ 1 ] = y1;
		_stateApplied._scissor[ 2 ] = x2;
		_stateApplied._scissor[ 3 ] = y2;
	}
	
	void SetViewport( s32 x1, s32 y1, s32 x2, s32 y2 )
	{
		state& activeState = _state[ _stateActive ];
		
		activeState._viewport[ 0 ] = x1;
		activeState._viewport[ 1 ] = y1;
		activeState._viewport[ 2 ] = x2;
		activeState._viewport[ 3 ] = y2;
		
		SetViewportHw( activeState._viewport );
		
		_stateApplied._viewport[ 0 ] = x1;
		_stateApplied._viewport[ 1 ] = y1;
		_stateApplied._viewport[ 2 ] = x2;
		_stateApplied._viewport[ 3 ] = y2;
	}
	
	void SetMatrixP( const m4& matrix )
	{
		_state[ _stateActive ]._matrixP = matrix;
	}
	
	void SetMatrixM( const m4& matrix )
	{
		_state[ _stateActive ]._matrixM = matrix;
	}
	
	void Set2d()
	{
		s32* v = _stateApplied._viewport;
		SetOrtho( v[ 0 ], v[ 1 ], v[ 2 ], v[ 3 ] );
	}
	
	void SetOrtho( f32 x1, f32 y1, f32 x2, f32 y2 )
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
	
	void DeleteVertexArray( u32 va )
	{
		DeleteVertexArray( va );
	}
	
	void DeleteVertexBuffer( u32 vb )
	{
		DeleteVertexBufferHw( vb );
	}
	
	void DeleteElementBuffer( u32 eb )
	{
		DeleteElementBufferHw( eb );
	}

	void SetArray( Attribute attrib, u32 size, u32 stride, const void* pointer)
	{
		SetArrayHw( attrib, size, stride, pointer );
	}
	
	void UnsetArray( Attribute attrib)
	{
		UnsetArrayHw( attrib );
	}
	
	void DrawArray( Primitive primitive, u32 num)
	{
		DrawArrayHw( primitive, num );
	}
	
	void DrawElements( Primitive primitive, u32 num )
	{
		DrawElementsHw( primitive, num );
	}

	const c8* AttributeName( Attribute attrib )
	{
		return AttributeNames[ attrib ];
	}
	
	void Clear( bool colour, bool depth )
	{
		ClearHw( colour, depth );
	}
	
	void Init()
	{
		InitHw();
		InitCanvases();
		InitShaders();
		InitTextures();
		InitStates();
	}
	
	void Kill()
	{
		KillCanvases();
		KillShaders();
		KillTextures();
		KillHw();
	}
}