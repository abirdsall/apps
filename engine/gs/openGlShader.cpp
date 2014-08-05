#include "gsHw.h"

#if kBuildOpenGl3 || kBuildOpenGles2 || kBuildOpenGles3

namespace gs
{
	struct ShaderHw
	{
		GLuint mProgram;
		GLuint mShaderV;
		GLuint mShaderF;
	};
	
	static ShaderHw sShaderHw[ kShaderLimit ];
	
	bool ShaderHwNew( ShaderHandle handle, const c8* vShader, const c8* fShader )
	{
		ShaderHw& shaderHw = sShaderHw[ handle ];
		GLint vSuccess;
		GLint fSuccess;
		GLint lSuccess;
		GLchar logBuffer[ 8192 ];
		
		shaderHw.mProgram = glCreateProgram();
		shaderHw.mShaderV = glCreateShader( GL_VERTEX_SHADER );
		shaderHw.mShaderF = glCreateShader( GL_FRAGMENT_SHADER );
		
		glShaderSource( shaderHw.mShaderV, 1, &vShader, kNull );
		glShaderSource( shaderHw.mShaderF, 1, &fShader, kNull );
		
		glCompileShader( shaderHw.mShaderV );
		glCompileShader( shaderHw.mShaderF );
		
		glGetShaderiv( shaderHw.mShaderV, GL_COMPILE_STATUS, &vSuccess );
		glGetShaderiv( shaderHw.mShaderF, GL_COMPILE_STATUS, &fSuccess );
		
		if( !vSuccess )
		{
			glGetShaderInfoLog( shaderHw.mShaderV, sizeof( logBuffer ), kNull, logBuffer );
			printf( "VS Error\n%s", logBuffer );
		}

		if( !fSuccess )
		{
			glGetShaderInfoLog( shaderHw.mShaderF, sizeof( logBuffer ), kNull, logBuffer );
			printf( "FS Error\n%s", logBuffer );
		}
		
		if( vSuccess && fSuccess )
		{
			glAttachShader( shaderHw.mProgram, shaderHw.mShaderV );
			glAttachShader( shaderHw.mProgram, shaderHw.mShaderF );
			
			glBindAttribLocation( shaderHw.mProgram, eAttribVertex, AttribName( eAttribVertex ) );
			glBindAttribLocation( shaderHw.mProgram, eAttribNormal, AttribName( eAttribNormal ) );
			glBindAttribLocation( shaderHw.mProgram, eAttribColour, AttribName( eAttribColour ) );
			glBindAttribLocation( shaderHw.mProgram, eAttribTcoord, AttribName( eAttribTcoord ) );
			
			glLinkProgram(shaderHw.mProgram);
			glGetProgramiv(shaderHw.mProgram, GL_LINK_STATUS, (int *)&lSuccess);
			if(!lSuccess)
			{
				glGetProgramInfoLog( shaderHw.mProgram, sizeof( logBuffer ), kNull, logBuffer );
				printf( "Link Error\n%s", logBuffer );
				glDetachShader( shaderHw.mProgram, shaderHw.mShaderV );
				glDetachShader( shaderHw.mProgram, shaderHw.mShaderF );
			}
			else
			{
				return true;
			}
		}
		glDeleteProgram( shaderHw.mProgram );
		glDeleteShader( shaderHw.mShaderV );
		glDeleteShader( shaderHw.mShaderF );
		return false;
	}
	
	void ShaderHwDelete( ShaderHandle handle )
	{
		ShaderHw& shaderHw = sShaderHw[ handle ];
		glDetachShader( shaderHw.mProgram, shaderHw.mShaderV );
		glDetachShader( shaderHw.mProgram, shaderHw.mShaderF );
		glDeleteProgram( shaderHw.mProgram );
		glDeleteShader( shaderHw.mShaderV );
		glDeleteShader( shaderHw.mShaderF );
	}
	
	void ShaderHwSet( const ShaderHandle handle )
	{
		glUseProgram( sShaderHw[ handle ].mProgram );
		TextureHwActiveCountReset();
	}
	
	static s32 FindUniform( const ShaderHandle handle, GLchar* name )
	{
		ShaderHw& shaderHw = sShaderHw[ handle ];
		GLint location = ::glGetUniformLocation( shaderHw.mProgram, name );
		ASSERT( location >= 0 );
		return location;
	}
	
	void ShaderHwSetInt( const ShaderHandle handle, const c8* name, const s32 value )
	{
		glUniform1i( FindUniform( handle, ( GLchar* )name ), ( GLint )value );
	}
	
	void ShaderHwSetFloat( const ShaderHandle handle, const c8* name, const f32 value )
	{
		glUniform1f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )value );
	}

	void ShaderHwSetFloatArray( const ShaderHandle handle, const c8* name, f32* array, u32 arraySize )
	{
		glUniform1fv( FindUniform( handle, ( GLchar* )name ), ( GLsizei )arraySize, ( GLfloat* )array );
	}

	void ShaderHwSetVec2( const ShaderHandle handle, const c8* name, const v2& v )
	{
		glUniform2f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )v.x, ( GLfloat )v.y );
	}
	
	void ShaderHwSetVec3( const ShaderHandle handle, const c8* name, const v3& v )
	{
		glUniform3f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )v.x, ( GLfloat )v.y, ( GLfloat )v.z );
	}
	
	void ShaderHwSetVec4( const ShaderHandle handle, const c8* name, const v4& v )
	{
		glUniform4f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )v.x, ( GLfloat )v.y, ( GLfloat )v.z, ( GLfloat )v.w );
	}
			
	void ShaderHwSetMat4( const ShaderHandle handle, const c8* name, const m4& m )
	{
#if kBuildOpenGles2 || kBuildOpenGles3
//        m4 tm = transpose( m );
//        glUniformMatrix4fv( FindUniform( handle, ( GLchar* )name ), 1, false, ( GLfloat* )tm );
        glUniformMatrix4fv( FindUniform( handle, ( GLchar* )name ), 1, true, ( GLfloat* )m );
#else
        glUniformMatrix4fv( FindUniform( handle, ( GLchar* )name ), 1, true, ( GLfloat* )m );
#endif
	}
}

#endif