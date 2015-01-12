#include "fw.h"

namespace fw
{
    void AppendHeader( String& shader )
    {
#if GsOpenGles2
        return;
#elif GsOpenGles3
        shader += "#version 300 es\n";
#else
        shader += "#version 410 core\n";
#endif
    }
    
    void AppendPrecision( String& shader, eShaderPrecision precision )
    {
#if GsOpenGles2 || GsOpenGles3
        switch( precision )
        {
            case eShaderPrecisionLow:
            {
                shader += "lowp ";
                break;
            }
            case eShaderPrecisionMedium:
            {
                shader += "mediump ";
                break;
            }
            case eShaderPrecisionHigh:
            {
                shader += "highp ";
                break;
            }
        }
#endif
    }
    
    void AppendVertexInput( String& shader, const c8* type, const c8* name )
    {
#if GsOpenGles2
        shader += "attribute ";
#else
        shader += "in ";
#endif
        shader += type;
        shader += " ";
        shader += name;
        shader += ";\n";
    }
    
    void AppendVertexOutput( String& shader, const c8* type, const c8* name, eShaderPrecision precision )
    {
#if GsOpenGles2
        shader += "varying ";
#else
        shader += "out ";
#endif
        AppendPrecision( shader, precision );
        shader += type;
        shader += " ";
        shader += name;
        shader += ";\n";
    }

    void AppendVertexUniform( String& shader, const c8* type, const c8* name )
    {
        shader += "uniform ";
        shader += type;
        shader += " ";
        shader += name;
        shader += ";\n";
    }
    
    void AppendFragmentInput( String& shader, const c8* type, const c8* name, eShaderPrecision precision )
    {
#if GsOpenGles2
        shader += "varying ";
#else
        shader += "in ";
#endif
        AppendPrecision( shader, precision );
        shader += type;
        shader += " ";
        shader += name;
        shader += ";\n";
    }
    
    void AppendFragmentOutput( String& shader, s32 index, eShaderPrecision precision )
    {
#if !GsOpenGles2
        shader += "layout(location = ";
        shader = shader + index;
        shader += ") out ";
        AppendPrecision( shader, precision );
        shader += "vec4 output_colour";
        shader = shader + index;
        shader += ";\n";
#endif
    }

    void AppendFragmentOutput( String& shader, eShaderPrecision precision )
    {
#if !GsOpenGles2
        shader += "out ";
        AppendPrecision( shader, precision );
        shader += "vec4 output_colour;\n";
#endif
    }

    void AppendFragmentUniform( String& shader, const c8* type, const c8* name, eShaderPrecision precision )
    {
        shader += "uniform ";
        AppendPrecision( shader, precision );
        shader += type;
        shader += " ";
        shader += name;
        shader += ";\n";
    }
    
    void AppendFunctionBegin( String& shader, const c8* output, const c8* name, const c8* inputs )
    {
        shader += output;
        shader += " ";
        shader += name;
        shader += "(";
        shader += inputs;
        shader += ")";
        shader += "\n{\n";
    }
    
    void AppendFunctionEnd( String& shader )
    {
        shader += "}\n";
    }
    
    void AppendVariable( String& shader, const c8* type, const c8* name, eShaderPrecision precision )
    {
        AppendPrecision( shader, precision );
        shader += type;
        shader += " ";
        shader += name;
        shader += ";\n";
    }
    
    gs::ShaderHandle ShaderMake( eShader shader )
    {
        return ShaderMake( shader, 0, 0 );
    }
    
    gs::ShaderHandle ShaderMake( eShader shader, s32 locationBegin, s32 locationEnd )
    {
        core::String vShader = "";
        
        AppendHeader( vShader );
        
        if( shader & eShaderBits3d )
        {
            AppendVertexInput( vShader, "vec3", "vertex_position" );
        }
        else
        {
            AppendVertexInput( vShader, "vec2", "vertex_position" );
        }
        
        if( shader & eShaderBitsColour )
        {
            AppendVertexInput( vShader, "vec4", "vertex_colour" );
            AppendVertexOutput( vShader, "vec4", "fragment_colour", eShaderPrecisionLow );
        }
        
        if( shader & eShaderBitsTexture2d )
        {
            AppendVertexInput( vShader, "vec2", "vertex_tcoord" );
            AppendVertexOutput( vShader, "vec2", "fragment_tcoord", eShaderPrecisionLow );
        }
        else if( shader & eShaderBitsTexture3d )
        {
            AppendVertexInput( vShader, "vec3", "vertex_tcoord" );
            AppendVertexOutput( vShader, "vec3", "fragment_tcoord", eShaderPrecisionLow );
        }
        
        AppendVertexUniform( vShader, "mat4", "modelViewProjectionMatrix" );
        
        AppendFunctionBegin( vShader, "void", "main", "" );
        
        if( shader & eShaderBits3d )
        {
            vShader += "gl_Position = modelViewProjectionMatrix * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1);\n";
        }
        else
        {
            vShader += "gl_Position = modelViewProjectionMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n";
        }
        
        if( shader & eShaderBitsColour )
        {
            vShader += "fragment_colour = vertex_colour;\n";
        }
        
        if( shader & ( eShaderBitsTexture2d | eShaderBitsTexture3d ) )
        {
            vShader += "fragment_tcoord = vertex_tcoord;\n";
        }
        
        AppendFunctionEnd( vShader );
        
        core::String fShader = "";
        
        AppendHeader( fShader );
        
        if( shader & eShaderBitsColour )
        {
            AppendFragmentInput( fShader, "vec4", "fragment_colour", eShaderPrecisionLow );
        }

        if( shader & eShaderBitsTexture2d )
        {
            AppendFragmentInput( fShader, "vec2", "fragment_tcoord", eShaderPrecisionLow );
            AppendFragmentUniform( fShader, "sampler2D", "texture0", eShaderPrecisionLow );
        }
        else if( shader & eShaderBitsTexture3d )
        {
            AppendFragmentInput( fShader, "vec3", "fragment_tcoord", eShaderPrecisionLow );
            AppendFragmentUniform( fShader, "sampler3D", "texture0", eShaderPrecisionLow );
        }

        if( locationEnd > 0 )
        {
            for( s32 i = locationBegin; i <= locationEnd; i++ )
            {
                AppendFragmentOutput( fShader, i, eShaderPrecisionLow );
            }
        }
        else
        {
            AppendFragmentOutput( fShader, eShaderPrecisionLow );
        }
        
        AppendFunctionBegin( fShader, "void", "main", "" );
        
        const c8* tmp;
        
        if( locationEnd > 0 )
        {
            tmp = "tmp";
            AppendVariable( fShader, "vec4", "tmp", eShaderPrecisionLow );
        }
        else
        {
#if GsOpenGles2
            tmp = "gl_FragColor";
#else
            tmp = "output_colour";
#endif
        }

        switch( shader )
        {
            case eShader2dFill:
            case eShader3dFill:
            {
                fShader = fShader + tmp + " = fragment_colour;\n";
                break;
            }
            case eShader2dTexture2d:
            case eShader2dTexture3d:
            case eShader3dTexture2d:
            case eShader3dTexture3d:
            {
#if GsOpenGles2
                fShader = fShader + tmp + " = texture2D(texture0, fragment_tcoord);\n";
#else
                fShader = fShader + tmp + " = texture(texture0, fragment_tcoord);\n";
#endif
                break;
            }
            case eShader2dTexture2dTinted:
            case eShader2dTexture3dTinted:
            case eShader3dTexture2dTinted:
            case eShader3dTexture3dTinted:
            {
#if GsOpenGles2
                fShader = fShader + tmp + " = texture2D(texture0, fragment_tcoord) * fragment_colour;\n";
#else
                fShader = fShader + tmp + " = texture(texture0, fragment_tcoord) * fragment_colour;\n";
#endif
                break;
            }
        }
        
        if( locationEnd > 0 )
        {
            for( s32 i = locationBegin; i <= locationEnd; i++ )
            {
                fShader += "output_colour";
                fShader = fShader + i;
                fShader = fShader + " = " + tmp + ";\n";
            }
        }
        
        AppendFunctionEnd( fShader );
        
        return gs::ShaderNew( vShader.toStr(), fShader.toStr() );
    }
}
