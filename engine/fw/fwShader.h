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
    
    gs::ShaderHandle ShaderMake2d( bool colour, bool tcoords );
    gs::ShaderHandle ShaderMake2d( bool colour, bool tcoords, s32 locationBegin, s32 locationEnd );
    gs::ShaderHandle ShaderMake3d( bool colour, bool tcoords );
    gs::ShaderHandle ShaderMake3d( bool colour, bool tcoords, s32 locationBegin, s32 locationEnd );
}

#endif