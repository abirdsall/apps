#ifndef FW_SHADER
#define FW_SHADER

namespace fw
{
    typedef enum
    {
        eShaderPrecisionLow,
        eShaderPrecisionMedium,
        eShaderPrecisionHigh
    } eShaderPrecision;

    typedef enum
    {
        eShaderBits3d = 0x01,
        eShaderBitsColour = 0x02,
        eShaderBitsTexture2d = 0x04,
        eShaderBitsTexture3d = 0x08
    } eShaderBits;
    
    typedef enum
    {
        eShader2dFill = eShaderBitsColour,
        eShader2dTexture2d = eShaderBitsTexture2d,
        eShader2dTexture3d = eShaderBitsTexture3d,
        eShader2dTexture2dTinted = eShaderBitsTexture2d | eShaderBitsColour,
        eShader2dTexture3dTinted = eShaderBitsTexture3d | eShaderBitsColour,
        eShader3dFill = eShaderBitsColour | eShaderBits3d,
        eShader3dTexture2d = eShaderBits3d | eShaderBitsTexture2d,
        eShader3dTexture3d = eShaderBits3d | eShaderBitsTexture3d,
        eShader3dTexture2dTinted = eShaderBits3d | eShaderBitsTexture2d | eShaderBitsColour,
        eShader3dTexture3dTinted = eShaderBits3d | eShaderBitsTexture3d | eShaderBitsColour
    } eShader;

    gs::ShaderHandle ShaderMake( eShader shader );
    gs::ShaderHandle ShaderMake( eShader shader, s32 locationBegin, s32 locationEnd );
}

#endif