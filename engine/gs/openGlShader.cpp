#include "gsHw.h"

#if GsOpenGl3 || GsOpenGles2 || GsOpenGles3

namespace gs
{
	struct ShaderHw
	{
		GLuint _program;
		GLuint _shaderV;
		GLuint _shaderF;
	};
	
	static ShaderHw _shadersHw[ ShaderLimit ];
	
	bool ShaderHwNew( ShaderHandle handle, const c8* vShader, const c8* fShader )
	{
		ShaderHw& shaderHw = _shadersHw[ handle ];
		GLint vSuccess;
		GLint fSuccess;
		GLint lSuccess;
		GLchar logBuffer[ 8192 ];
		
		shaderHw._program = glCreateProgram();
		shaderHw._shaderV = glCreateShader( GL_VERTEX_SHADER );
		shaderHw._shaderF = glCreateShader( GL_FRAGMENT_SHADER );
		
		glShaderSource( shaderHw._shaderV, 1, &vShader, Null );
		glShaderSource( shaderHw._shaderF, 1, &fShader, Null );
		
		glCompileShader( shaderHw._shaderV );
		glCompileShader( shaderHw._shaderF );
		
		glGetShaderiv( shaderHw._shaderV, GL_COMPILE_STATUS, &vSuccess );
		glGetShaderiv( shaderHw._shaderF, GL_COMPILE_STATUS, &fSuccess );
		
		if( !vSuccess )
		{
			glGetShaderInfoLog( shaderHw._shaderV, sizeof( logBuffer ), Null, logBuffer );
			printf( "VS Error\n%s", logBuffer );
		}

		if( !fSuccess )
		{
			glGetShaderInfoLog( shaderHw._shaderF, sizeof( logBuffer ), Null, logBuffer );
			printf( "FS Error\n%s", logBuffer );
		}
		
		if( vSuccess && fSuccess )
		{
			glAttachShader( shaderHw._program, shaderHw._shaderV );
			glAttachShader( shaderHw._program, shaderHw._shaderF );
			
			glBindAttribLocation( shaderHw._program, AttributeVertex, AttributeName( AttributeVertex ) );
			glBindAttribLocation( shaderHw._program, AttributeNormal, AttributeName( AttributeNormal ) );
			glBindAttribLocation( shaderHw._program, AttributeColour, AttributeName( AttributeColour ) );
			glBindAttribLocation( shaderHw._program, AttributeTcoord, AttributeName( AttributeTcoord ) );
			
			glLinkProgram(shaderHw._program);
			glGetProgramiv(shaderHw._program, GL_LINK_STATUS, (int *)&lSuccess);
			if(!lSuccess)
			{
				glGetProgramInfoLog( shaderHw._program, sizeof( logBuffer ), Null, logBuffer );
				printf( "Link Error\n%s", logBuffer );
				glDetachShader( shaderHw._program, shaderHw._shaderV );
				glDetachShader( shaderHw._program, shaderHw._shaderF );
			}
			else
			{
				return true;
			}
		}
		glDeleteProgram( shaderHw._program );
		glDeleteShader( shaderHw._shaderV );
		glDeleteShader( shaderHw._shaderF );
		return false;
	}
	
	void ShaderHwDelete( ShaderHandle handle )
	{
		ShaderHw& shaderHw = _shadersHw[ handle ];
		glDetachShader( shaderHw._program, shaderHw._shaderV );
		glDetachShader( shaderHw._program, shaderHw._shaderF );
		glDeleteProgram( shaderHw._program );
		glDeleteShader( shaderHw._shaderV );
		glDeleteShader( shaderHw._shaderF );
	}
	
	void ShaderHwSet( ShaderHandle handle )
	{
		glUseProgram( _shadersHw[ handle ]._program );
		TextureHwActiveCountReset();
	}
	
	static s32 FindUniform( ShaderHandle handle, GLchar* name )
	{
		ShaderHw& shaderHw = _shadersHw[ handle ];
		GLint location = ::glGetUniformLocation( shaderHw._program, name );
		return location;
	}
	
	void ShaderHwSetInt( ShaderHandle handle, const c8* name, s32 value )
	{
		glUniform1i( FindUniform( handle, ( GLchar* )name ), ( GLint )value );
	}
	
	void ShaderHwSetFloat( ShaderHandle handle, const c8* name, f32 value )
	{
		glUniform1f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )value );
	}

	void ShaderHwSetFloatArray( ShaderHandle handle, const c8* name, f32* array, u32 arraySize )
	{
		glUniform1fv( FindUniform( handle, ( GLchar* )name ), ( GLsizei )arraySize, ( GLfloat* )array );
	}

	void ShaderHwSetVec2( ShaderHandle handle, const c8* name, const v2& v )
	{
		glUniform2f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )v.x, ( GLfloat )v.y );
	}
	
	void ShaderHwSetVec3( ShaderHandle handle, const c8* name, const v3& v )
	{
		glUniform3f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )v.x, ( GLfloat )v.y, ( GLfloat )v.z );
	}
	
	void ShaderHwSetVec4( ShaderHandle handle, const c8* name, const v4& v )
	{
		glUniform4f( FindUniform( handle, ( GLchar* )name ), ( GLfloat )v.x, ( GLfloat )v.y, ( GLfloat )v.z, ( GLfloat )v.w );
	}
			
	void ShaderHwSetMat4( ShaderHandle handle, const c8* name, const m4& m )
	{
        s32 uniform = FindUniform( handle, ( GLchar* )name );
        
        if( uniform >= 0 ) // Assert? 
        {
#if GsOpenGles2
            m4 tm = transpose( m );
            glUniformMatrix4fv( uniform, 1, false, ( GLfloat* )tm );
#else
            glUniformMatrix4fv( uniform, 1, true, ( GLfloat* )m );
#endif
        }
	}
}

#endif