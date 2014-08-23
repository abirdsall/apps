#ifndef GS_SHADER
#define GS_SHADER

namespace gs
{
	typedef ::u32 ShaderHandle;

    const ShaderHandle InvalidShaderHandle = kMaxU32;

	const u32 kShaderInvalid = kMaxU32;
	const u32 kShaderLimit = 256 + 64;
	
	struct Shader
	{
		bool mActive;
	};
	
	void InitShaders();
	void KillShaders();
	
	ShaderHandle ShaderNew();
	ShaderHandle ShaderNew( const c8* vShader, const c8* fShader, bool debug = true );
	void ShaderDelete( const ShaderHandle handle );
	
	void ShaderSet( const ShaderHandle handle );
	void ShaderSetInt( const c8* name, const s32 value );
	void ShaderSetFloat( const c8* name, const f32 value );
	void ShaderSetFloatArray( const c8* name, f32* array, u32 arraySize );
	void ShaderSetVec2( const c8* name, const v2& v );
	void ShaderSetVec3( const c8* name, const v3& v );
	void ShaderSetVec4( const c8* name, const v4& v );
	void ShaderSetMat4( const c8* name, const m4& m );

	ShaderHandle ShaderActive();
}

#endif