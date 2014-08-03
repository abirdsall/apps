#include "gsHw.h"

#if kBuildOpenGl3 || kBuildOpenGles2 || kBuildOpenGles3

namespace gs
{
	static GLenum sPrimMap[ ePrimCount ] =
	{
		GL_POINTS,			// ePrimPoints
		GL_LINES,			// ePrimLines
		GL_LINE_LOOP,		// ePrimLineLoop
		GL_LINE_STRIP,		// ePrimLineStrip
		GL_TRIANGLES,		// ePrimTriangles
		GL_TRIANGLE_FAN,	// ePrimTriangleFan
		GL_TRIANGLE_STRIP,	// ePrimTriangleStrip
	};
	
	void ErrorCheck()
	{
		u32 error = glGetError();
		switch(error)
		{
			case GL_NO_ERROR: break;
			case GL_INVALID_ENUM: ASSERT(0);break;
			case GL_INVALID_VALUE: ASSERT(0);break;
			case GL_INVALID_OPERATION: ASSERT(0);break;
			case GL_OUT_OF_MEMORY: ASSERT(0);break;
			default: ASSERT(0);break;
		}
	}
	
	void InitHw(void)
	{
		glFrontFace(GL_CCW);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_SCISSOR_TEST);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
#if kBuildOpenGl3
        glDisable(GL_LINE_SMOOTH); // This causes shaders to run in software
		//glHint(GL_POLYGON_SMOOTH, GL_NICEST);
		//glEnable(GL_POLYGON_SMOOTH);
#endif
	}
	
	void KillHw(void)
	{
	}
	
	void SetBlendHw( const eBlend blend )
	{
		ErrorCheck();
		if ( blend == eBlendMin )
		{
#if kBuildOpenGles2
            ASSERT(false);
#else
            glBlendFunc( GL_ONE, GL_ONE );
            glBlendEquation( GL_MIN );
#endif
		}
		else if ( blend == eBlendMax )
		{
#if kBuildOpenGles2
            ASSERT(false);
#else
            glBlendFunc( GL_ONE, GL_ONE );
            glBlendEquation( GL_MAX );
#endif
		}
		else
		{
			glBlendEquation( GL_FUNC_ADD );
		}
		
		//void glBlendFuncSeparate(GLenum  srcRGB,  GLenum  dstRGB,  GLenum  srcAlpha,  GLenum  dstAlpha);
		//void glBlendFuncSeparatei(GLuint  buf,  GLenum  srcRGB,  GLenum  dstRGB,  GLenum  srcAlpha,  GLenum  dstAlpha); //buf is drawbuffer
		switch( blend )
		{
			case eBlendNone : glDisable( GL_BLEND ); return;
			case eBlendRgb : glBlendFunc( GL_ONE, GL_ZERO ); break;
			case eBlendRgba : glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); break;
			case eBlendAddRgb : glBlendFunc( GL_ONE, GL_ONE ); break;
			case eBlendAddRgba : glBlendFunc( GL_SRC_ALPHA, GL_ONE ); break;
			case eBlendRgbDstAlpha : glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA ); break;
			case eBlendMixRgbAddA : glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE); break;
			default: break;
		}
		
		glEnable( GL_BLEND );
	}
	
	void SetDepthHw( const eDepth depth )
	{
		switch( depth )
		{
			case eDepthNone : glDisable( GL_DEPTH_TEST ); return;
			case eDepthLess : glDepthFunc( GL_LESS ); break;
			case eDepthLequal : glDepthFunc( GL_LEQUAL ); break;
			case eDepthGequal : glDepthFunc( GL_GEQUAL ); break;
			case eDepthGreater : glDepthFunc( GL_GREATER ); break;
			case eDepthNotEqual : glDepthFunc( GL_NOTEQUAL );	break;
			case eDepthEqual : glDepthFunc( GL_EQUAL ); break;
		}
		
		glEnable( GL_DEPTH_TEST );

	}
	
	void SetWriteHw( const eWrite write )
	{
		glDepthMask( ( write & eWriteZ ) ? GL_TRUE : GL_FALSE );
		glColorMask( ( ( write & eWriteR ) >> 1 ) ? GL_TRUE : GL_FALSE,
					( ( write & eWriteG ) >> 2 ) ? GL_TRUE : GL_FALSE,
					( ( write & eWriteB ) >> 3 ) ? GL_TRUE : GL_FALSE,
					( ( write & eWriteA ) >> 4 ) ? GL_TRUE : GL_FALSE );
	}
	
	void SetCullHw( const eCull cull )
	{
		switch( cull )
		{
			case eCullNone : glDisable( GL_CULL_FACE ); return;
			case eCullBack : glCullFace( GL_BACK ); break;
			case eCullFront : glCullFace( GL_FRONT ); break;
		}
		
		glEnable( GL_CULL_FACE );

	}
	
	void SetScissorHw( const s32* scissor )
	{
		s32 x1 = scissor[ 0 ];
		s32 y1 = scissor[ 1 ];
		s32 x2 = scissor[ 2 ];
		s32 y2 = scissor[ 3 ];
		s32 yo = ( s32 )CanvasSizeY() - y1 - ( y2 - y1 );
		glScissor( ( GLint )x1, ( GLint )yo, ( GLsizei )( x2 - x1 ), ( GLsizei )( y2 - y1 ) );
	}
	
	void SetViewportHw( const s32* viewport )
	{
		s32 x1 = viewport[ 0 ];
		s32 y1 = viewport[ 1 ];
		s32 x2 = viewport[ 2 ];
		s32 y2 = viewport[ 3 ];
		glViewport( ( GLint )x1, ( GLint )y1, ( GLsizei )( x2 - x1 ), ( GLsizei )( y2 - y1 ) );
		
		// TODO : tidy up
		s32 yo = ( s32 )CanvasSizeY() - y1 - ( y2 - y1 );
		glScissor( ( GLint )x1, ( GLint )yo, ( GLsizei )( x2 - x1 ), ( GLsizei )( y2 - y1 ) );
	}
	
	void ClearHw( const bool colour, const bool depth )
	{
		ApplyState();
		glClear( ( colour ? GL_COLOR_BUFFER_BIT : 0 ) | ( depth ? GL_DEPTH_BUFFER_BIT : 0 ) );
	}
	
	u32 NewVertexArrayHw()
	{
		GLuint va;
#if kBuildOpenGles2
        glGenVertexArraysOES( 1, &va );
#else
        glGenVertexArrays( 1, &va );
#endif
		return ( u32 )va;
	}
	
	u32 NewVertexBufferHw()
	{
		GLuint vb;
		glGenBuffers( 1, &vb );
		return ( u32 )vb;
	}
	
	u32 NewElementBufferHw()
	{
		GLuint eb;
		glGenBuffers( 1, &eb );
		return ( u32 )eb;
	}
	
	void SetVertexArrayHw( u32 va )
	{
#if kBuildOpenGles2
        glBindVertexArrayOES( ( GLuint )va );
#else
        glBindVertexArray( ( GLuint )va );
#endif
	}
	
	void SetVertexBufferHw( u32 vb )
	{
		glBindBuffer( GL_ARRAY_BUFFER, ( GLuint )vb );
	}

	void SetElementBufferHw( u32 eb )
	{ 
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ( GLuint )eb );
	}

	void FillVertexBufferHw( const void* data, s32 dataSize, bool dynamic )
	{
		glBufferData( GL_ARRAY_BUFFER, ( GLsizeiptr )dataSize, ( const GLvoid* )data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
	}

	void FillElementBufferHw( const void* data, s32 dataSize, bool dynamic )
	{
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, ( GLsizeiptr )dataSize, ( const GLvoid* )data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
	}

	void UpdateVertexBufferHw( const void* data, s32 dataSize, s32 dataOffset )
	{
		glBufferSubData( GL_ARRAY_BUFFER, ( GLintptr )dataOffset, ( GLsizeiptr )dataSize, ( const GLvoid* )data );
	}

	void UpdateElementBufferHw( const void* data, s32 dataSize, s32 dataOffset )
	{
		glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, ( GLintptr )dataOffset, ( GLsizeiptr )dataSize, ( const GLvoid* )data );
	}
	
	void DeleteVertexArrayHw( u32 va )
	{
#if kBuildOpenGles2
        glDeleteVertexArraysOES( 1, ( GLuint* )&va );
#else
        glDeleteVertexArrays( 1, ( GLuint* )&va );
#endif
	}
	
	void DeleteVertexBufferHw( u32 vb )
	{
		glDeleteBuffers( 1, ( GLuint* )&vb );
	}
	
	void DeleteElementBufferHw( u32 eb )
	{
		glDeleteBuffers( 1, ( GLuint* )&eb );
	}
	
	void SetArrayHw( const eAttrib attrib, const u32 size, const u32 stride, const void* pointer )
	{
		glEnableVertexAttribArray( attrib );
		glVertexAttribPointer( ( GLuint )attrib, ( GLint )size, GL_FLOAT, GL_FALSE, ( GLsizei )stride, ( const GLvoid* )pointer );
	}
	
	void UnsetArrayHw( const eAttrib attrib)
	{
		glDisableVertexAttribArray( ( GLuint )attrib );
	}
	
	void DrawArrayHw( const ePrim primitive, const u32 num )
	{
		ApplyState();
		glDrawArrays( sPrimMap[ primitive ], 0, ( GLsizei )num );
	}
	
	void DrawElementsHw( const ePrim primitive, const u32 num )
	{
		ApplyState();
		glDrawElements( sPrimMap[ primitive ], ( GLsizei )num, GL_UNSIGNED_SHORT, 0 );
	}

}

#endif