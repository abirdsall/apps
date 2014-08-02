#ifndef GS_HW_SHADER
#define GS_HW_SHADER

namespace gs
{
	bool ShaderHwNew( ShaderHandle handle, const c8* vShader, const c8* fShader );
	void ShaderHwDelete( ShaderHandle handle );
	void ShaderHwSet( const ShaderHandle handle );	
	void ShaderHwSetInt( const ShaderHandle handle, const c8* name, const s32 value );
	void ShaderHwSetFloat( const ShaderHandle handle, const c8* name, const f32 value );
	void ShaderHwSetFloatArray( const ShaderHandle handle, const c8* name, f32* array, u32 arraySize );
	void ShaderHwSetVec2( const ShaderHandle handle, const c8* name, const v2& v );
	void ShaderHwSetVec3( const ShaderHandle handle, const c8* name, const v3& v );
	void ShaderHwSetVec4( const ShaderHandle handle, const c8* name, const v4& v );
	void ShaderHwSetMat4( const ShaderHandle handle, const c8* name, const m4& m );
}

#endif