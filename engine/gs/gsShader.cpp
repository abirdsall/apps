#include "gsHw.h"

namespace gs
{
	static Shader _shaders[ ShaderLimit ];
	static ShaderHandle _shaderActive = ShaderInvalid;
	
	void InitShaders()
	{
		for( u32 i = 0; i < ShaderLimit; i++ )
		{
			_shaders[ i ]._active = false;
		}
		_shaderActive = ShaderInvalid;
	}
	
	void KillShaders()
	{
		for( u32 i = 0; i < ShaderLimit;  i++ )
		{
			ShaderDelete( i );
		}
	}
	
	ShaderHandle ShaderNew()
	{
		for( u32 i = 0; i < ShaderLimit; i++ )
		{
			if( !_shaders[ i ]._active )
			{
				_shaders[ i ]._active = true;
				return i;
			}
		}
		ASSERT(0);
		return ShaderInvalid;
	}
	
	ShaderHandle ShaderNew( const c8* vShader, const c8* fShader, bool debug )
	{
		if( debug )
		{
			printf( "//////////////////////////////////////////////////\n//\n" );
			printf( "vs\n%sfs\n%s", vShader, fShader );
			printf( "//\n//////////////////////////////////////////////////\n\n" );
		}
		ShaderHandle handle = ShaderNew();
		if( handle != ShaderInvalid )
		{
			Shader& shader = _shaders[ handle ];
			if( ShaderHwNew( handle, vShader, fShader ) )
			{
				return handle;
			}
			else
			{
				shader._active = false;
			}
		}
		ASSERT(0);
		return ShaderInvalid;
	}
		
	void ShaderDelete( ShaderHandle handle )
	{
		Shader& shader = _shaders[ handle ];
		if( shader._active )
		{
			ShaderHwDelete( handle );
			shader._active = false;
		}
	}
	
	void ShaderSet( ShaderHandle handle )
	{
		ShaderHwSet( handle );
		_shaderActive = handle;
	}
	
	void ShaderSetInt( const c8* name, s32 value )
	{
		ShaderHwSetInt( _shaderActive, name, value );
	}
	
	void ShaderSetFloat( const c8* name, f32 value )
	{
		ShaderHwSetFloat( _shaderActive, name, value );
	}
	
	void ShaderSetFloatArray( const c8* name, f32* array, u32 arraySize )
	{
		ShaderHwSetFloatArray( _shaderActive, name, array, arraySize );
	}
	
	void ShaderSetVec2( const c8* name, const v2& v )
	{
		ShaderHwSetVec2( _shaderActive, name, v );
	}

	void ShaderSetVec3( const c8* name, const v3& v )
	{
		ShaderHwSetVec3( _shaderActive, name, v );
	}
	
	void ShaderSetVec4( const c8* name, const v4& v )
	{
		ShaderHwSetVec4( _shaderActive, name, v );
	}
	
	void ShaderSetMat4( const c8* name, const m4& m )
	{
		ShaderHwSetMat4( _shaderActive, name, m );
	}
	
	ShaderHandle ShaderActive()
	{
		return _shaderActive;
	}
}