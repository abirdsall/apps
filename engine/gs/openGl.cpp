#include "gsHw.h"

#if GsOpenGl3 || GsOpenGles2 || GsOpenGles3

namespace gs
{
	static GLenum _primitiveMap[ PrimitiveCount ] =
	{
		GL_POINTS,			// PrimitivePoints
		GL_LINES,			// PrimitiveLines
		GL_LINE_LOOP,		// PrimitiveLineLoop
		GL_LINE_STRIP,		// PrimitiveLineStrip
		GL_TRIANGLES,		// PrimitiveTriangles
		GL_TRIANGLE_FAN,	// PrimitiveTriangleFan
		GL_TRIANGLE_STRIP,	// PrimitiveTriangleStrip
	};

#ifdef DebugGs
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
#endif
    
	void InitHw()
	{
		glFrontFace(GL_CCW);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_SCISSOR_TEST);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

#if GsOpenGl3
        glDisable(GL_LINE_SMOOTH); // This causes shaders to run in software
		//glHint(GL_POLYGON_SMOOTH, GL_NICEST);
		//glEnable(GL_POLYGON_SMOOTH);
#endif
        
        ErrorCheck();
	}
	
	void KillHw()
	{
	}
	
	void SetBlendHw( BlendMode blend )
	{
		ErrorCheck();
        
		if ( blend == BlendModeMin )
		{
#if GsOpenGles2
            ASSERT(false);
#else
            glBlendFunc( GL_ONE, GL_ONE );
            glBlendEquation( GL_MIN );
#endif
		}
		else if ( blend == BlendModeMax )
		{
#if GsOpenGles2
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
        
        ErrorCheck();
		
		//void glBlendFuncSeparate(GLenum  srcRGB,  GLenum  dstRGB,  GLenum  srcAlpha,  GLenum  dstAlpha);
		//void glBlendFuncSeparatei(GLuint  buf,  GLenum  srcRGB,  GLenum  dstRGB,  GLenum  srcAlpha,  GLenum  dstAlpha); //buf is drawbuffer
		switch( blend )
		{
			case BlendModeNone : glDisable( GL_BLEND ); return;
			case BlendModeRgb : glBlendFunc( GL_ONE, GL_ZERO ); break;
			case BlendModeRgba : glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); break;
			case BlendModeAddRgb : glBlendFunc( GL_ONE, GL_ONE ); break;
			case BlendModeAddRgba : glBlendFunc( GL_SRC_ALPHA, GL_ONE ); break;
			case BlendModeRgbDstAlpha : glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA ); break;
			case BlendModeMixRgbAddA : glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE); break;
			default: break;
		}
		
        ErrorCheck();

		glEnable( GL_BLEND );
        
        ErrorCheck();
	}
	
	void SetDepthHw( DepthTest depth )
	{
        ErrorCheck();

		switch( depth )
		{
			case DepthTestNone : glDisable( GL_DEPTH_TEST ); return;
			case DepthTestLess : glDepthFunc( GL_LESS ); break;
			case DepthTestLequal : glDepthFunc( GL_LEQUAL ); break;
			case DepthTestGequal : glDepthFunc( GL_GEQUAL ); break;
			case DepthTestGreater : glDepthFunc( GL_GREATER ); break;
			case DepthTestNotEqual : glDepthFunc( GL_NOTEQUAL );	break;
			case DepthTestEqual : glDepthFunc( GL_EQUAL ); break;
		}
		
        ErrorCheck();

		glEnable( GL_DEPTH_TEST );

        ErrorCheck();
	}
	
	void SetWriteHw( WriteMask write )
	{
        ErrorCheck();

		glDepthMask( ( write & WriteMaskZ ) ? GL_TRUE : GL_FALSE );
        
        ErrorCheck();

		glColorMask( ( ( write & WriteMaskR ) >> 1 ) ? GL_TRUE : GL_FALSE,
					( ( write & WriteMaskG ) >> 2 ) ? GL_TRUE : GL_FALSE,
					( ( write & WriteMaskB ) >> 3 ) ? GL_TRUE : GL_FALSE,
					( ( write & WriteMaskA ) >> 4 ) ? GL_TRUE : GL_FALSE );
        
        ErrorCheck();
	}
	
	void SetCullHw( CullFace cull )
	{
        ErrorCheck();

		switch( cull )
		{
			case CullFaceNone : glDisable( GL_CULL_FACE ); return;
			case CullFaceBack : glCullFace( GL_BACK ); break;
			case CullFaceFront : glCullFace( GL_FRONT ); break;
		}

        ErrorCheck();

		glEnable( GL_CULL_FACE );

        ErrorCheck();
	}
	
	void SetScissorHw( const s32* scissor )
	{
		s32 x1 = scissor[ 0 ];
		s32 y1 = scissor[ 1 ];
		s32 x2 = scissor[ 2 ];
		s32 y2 = scissor[ 3 ];
		s32 yo = ( s32 )CanvasSizeY() - y1 - ( y2 - y1 );
        
        ErrorCheck();

		glScissor( ( GLint )x1, ( GLint )yo, ( GLsizei )( x2 - x1 ), ( GLsizei )( y2 - y1 ) );
        
        ErrorCheck();
	}
	
	void SetViewportHw( const s32* viewport )
	{
        ErrorCheck();
        
		s32 x1 = viewport[ 0 ];
		s32 y1 = viewport[ 1 ];
		s32 x2 = viewport[ 2 ];
		s32 y2 = viewport[ 3 ];
		glViewport( ( GLint )x1, ( GLint )y1, ( GLsizei )( x2 - x1 ), ( GLsizei )( y2 - y1 ) );
        
        ErrorCheck();
		
		// TODO : tidy up
		s32 yo = ( s32 )CanvasSizeY() - y1 - ( y2 - y1 );
		glScissor( ( GLint )x1, ( GLint )yo, ( GLsizei )( x2 - x1 ), ( GLsizei )( y2 - y1 ) );
        
        ErrorCheck();
	}
	
	void ClearHw( bool colour, bool depth )
	{
        ErrorCheck();

		ApplyState();

        ErrorCheck();

		glClear( ( colour ? GL_COLOR_BUFFER_BIT : 0 ) | ( depth ? GL_DEPTH_BUFFER_BIT : 0 ) );
        
        ErrorCheck();
	}
	
	u32 NewVertexArrayHw()
	{
		GLuint va;
        
        ErrorCheck();

#if GsOpenGles2
        glGenVertexArraysOES( 1, &va );
#else
        glGenVertexArrays( 1, &va );
#endif
        
        ErrorCheck();

		return ( u32 )va;
	}
	
	u32 NewVertexBufferHw()
	{
		GLuint vb;
        
        ErrorCheck();

		glGenBuffers( 1, &vb );
        
        ErrorCheck();

		return ( u32 )vb;
	}
	
	u32 NewElementBufferHw()
	{
		GLuint eb;
        
        ErrorCheck();

		glGenBuffers( 1, &eb );
        
        ErrorCheck();
        
		return ( u32 )eb;
	}
	
	void SetVertexArrayHw( u32 va )
	{
        ErrorCheck();

#if GsOpenGles2
        glBindVertexArrayOES( ( GLuint )va );
#else
        glBindVertexArray( ( GLuint )va );
#endif
        ErrorCheck();
	}
	
	void SetVertexBufferHw( u32 vb )
	{
        ErrorCheck();
        
		glBindBuffer( GL_ARRAY_BUFFER, ( GLuint )vb );
        
        ErrorCheck();
	}

	void SetElementBufferHw( u32 eb )
	{
        ErrorCheck();

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ( GLuint )eb );
        
        ErrorCheck();
	}

	void FillVertexBufferHw( const void* data, s32 dataSize, bool dynamic )
	{
        ErrorCheck();

		glBufferData( GL_ARRAY_BUFFER, ( GLsizeiptr )dataSize, ( const GLvoid* )data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
        
        ErrorCheck();
	}

	void FillElementBufferHw( const void* data, s32 dataSize, bool dynamic )
	{
        ErrorCheck();

		glBufferData( GL_ELEMENT_ARRAY_BUFFER, ( GLsizeiptr )dataSize, ( const GLvoid* )data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
        
        ErrorCheck();
	}

	void UpdateVertexBufferHw( const void* data, s32 dataSize, s32 dataOffset )
	{
        ErrorCheck();

		glBufferSubData( GL_ARRAY_BUFFER, ( GLintptr )dataOffset, ( GLsizeiptr )dataSize, ( const GLvoid* )data );
        
        ErrorCheck();
	}

	void UpdateElementBufferHw( const void* data, s32 dataSize, s32 dataOffset )
	{
        ErrorCheck();

		glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, ( GLintptr )dataOffset, ( GLsizeiptr )dataSize, ( const GLvoid* )data );

        ErrorCheck();
	}
	
	void DeleteVertexArrayHw( u32 va )
	{
        ErrorCheck();

#if GsOpenGles2
        glDeleteVertexArraysOES( 1, ( GLuint* )&va );
#else
        glDeleteVertexArrays( 1, ( GLuint* )&va );
#endif
        
        ErrorCheck();
	}
	
	void DeleteVertexBufferHw( u32 vb )
	{
        ErrorCheck();

		glDeleteBuffers( 1, ( GLuint* )&vb );
        
        ErrorCheck();
	}
	
	void DeleteElementBufferHw( u32 eb )
	{
        ErrorCheck();

		glDeleteBuffers( 1, ( GLuint* )&eb );
        
        ErrorCheck();
	}
	
	void SetArrayHw( Attribute attrib, u32 size, u32 stride, const void* pointer )
	{
        ErrorCheck();

		glEnableVertexAttribArray( attrib );

        ErrorCheck();

		glVertexAttribPointer( ( GLuint )attrib, ( GLint )size, GL_FLOAT, GL_FALSE, ( GLsizei )stride, ( const GLvoid* )pointer );
        
        ErrorCheck();
	}
	
	void UnsetArrayHw( Attribute attrib)
	{
        ErrorCheck();

		glDisableVertexAttribArray( ( GLuint )attrib );
        
        ErrorCheck();
	}
	
	void DrawArrayHw( Primitive primitive, u32 num )
	{
        ErrorCheck();

		ApplyState();
        
        ErrorCheck();

		glDrawArrays( _primitiveMap[ primitive ], 0, ( GLsizei )num );
        
        ErrorCheck();
	}
	
	void DrawElementsHw( Primitive primitive, u32 num )
	{
        ErrorCheck();

		ApplyState();
        
        ErrorCheck();

        //u16* ebuf = (u16*)glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY );
        //f32* vbuf = (f32*)glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );

        glDrawElements( _primitiveMap[ primitive ], ( GLsizei )num, GL_UNSIGNED_SHORT, Null);// ( const GLvoid* )0 );
        
        //glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );
        //glUnmapBuffer( GL_ARRAY_BUFFER );

        ErrorCheck();
	}

    s32 MaxDrawBuffers()
    {
        GLint maxDrawBuffers = 1;
        
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);
        
        return s32( maxDrawBuffers );
    }
}

#endif