#include "gsHw.h"

namespace gs
{
	static Shader sShader[ kShaderLimit ];
	static ShaderHandle sShaderActive = kShaderInvalid;
	
	void InitShaders()
	{
		for( u32 i = 0; i < kShaderLimit; i++ )
		{
			sShader[ i ].mActive = false;
		}
		sShaderActive = kShaderInvalid;
	}
	
	void KillShaders()
	{
		for( u32 i = 0; i < kShaderLimit;  i++ )
		{
			ShaderDelete( i );
		}
	}
	
	ShaderHandle ShaderNew()
	{
		for( u32 i = 0; i < kShaderLimit; i++ )
		{
			if( !sShader[ i ].mActive )
			{
				sShader[ i ].mActive = true;
				return i;
			}
		}
		ASSERT(0);
		return kShaderInvalid;
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
		if( handle != kShaderInvalid )
		{
			Shader& shader = sShader[ handle ];
			if( ShaderHwNew( handle, vShader, fShader ) )
			{
				return handle;
			}
			else
			{
				shader.mActive = false;
			}
		}
		ASSERT(0);
		return kShaderInvalid;
	}
		
	void ShaderDelete( const ShaderHandle handle )
	{
		Shader& shader = sShader[ handle ];
		if( shader.mActive )
		{
			ShaderHwDelete( handle );
			shader.mActive = false;
		}
	}
	
	void ShaderSet( const ShaderHandle handle )
	{
		ShaderHwSet( handle );
		sShaderActive = handle;
	}
	
	void ShaderSetInt( const c8* name, const s32 value )
	{
		ShaderHwSetInt( sShaderActive, name, value );
	}
	
	void ShaderSetFloat( const c8* name, const f32 value )
	{
		ShaderHwSetFloat( sShaderActive, name, value );
	}
	
	void ShaderSetFloatArray( const c8* name, f32* array, u32 arraySize )
	{
		ShaderHwSetFloatArray( sShaderActive, name, array, arraySize );
	}
	
	void ShaderSetVec2( const c8* name, const v2& v )
	{
		ShaderHwSetVec2( sShaderActive, name, v );
	}

	void ShaderSetVec3( const c8* name, const v3& v )
	{
		ShaderHwSetVec3( sShaderActive, name, v );
	}
	
	void ShaderSetVec4( const c8* name, const v4& v )
	{
		ShaderHwSetVec4( sShaderActive, name, v );
	}
	
	void ShaderSetMat4( const c8* name, const m4& m )
	{
		ShaderHwSetMat4( sShaderActive, name, m );
	}
	
	ShaderHandle ShaderActive()
	{
		return sShaderActive;
	}
}