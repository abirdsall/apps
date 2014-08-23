namespace fw
{
    typedef enum
    {
        eShaderPrecisionLow,
        eShaderPrecisionMedium,
        eShaderPrecisionHigh
    } eShaderPrecision;
    
    gs::ShaderHandle ShaderMake2d( bool colour, bool tcoords, s32 outputCount );
}
