#include "fw.h"

namespace fw
{
    void AppendHeader( String& shader )
    {
#if kBuildOpenGles2
        return;
#elif kBuildOpenGles3
        shader += "#version 300 es\n";
#else
        shader += "#version 410 core\n";
#endif
    }
    
    void AppendPrecision( String& shader, eShaderPrecision precision )
    {
#if kBuildOpenGles2 || kBuildOpenGles3
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
#if kBuildOpenGles2
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
#if kBuildOpenGles2
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
#if kBuildOpenGles2
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
#if !kBuildOpenGles2
        shader += "layout(location = ";
        shader = shader + index;
        shader += ") ";
        AppendPrecision( shader, precision );
        shader += "out vec4 output_colour";
        shader = shader + index;
        shader += ";\n";
#endif
    }

    void AppendFragmentOutput( String& shader, eShaderPrecision precision )
    {
#if !kBuildOpenGles2
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

    gs::ShaderHandle ShaderMake2d( bool colour, bool tcoords )
    {
        return ShaderMake2d( colour, tcoords, 0, 0 );
    }
    
    gs::ShaderHandle ShaderMake2d( bool colour, bool tcoords, s32 locationBegin, s32 locationEnd )
    {
        core::String vShader = "";
        
        AppendHeader( vShader );
        
        AppendVertexInput( vShader, "vec2", "vertex_position" );
        
        if( colour )
        {
            AppendVertexInput( vShader, "vec4", "vertex_colour" );
            AppendVertexOutput( vShader, "vec4", "fragment_colour", eShaderPrecisionLow );
        }
        
        if( tcoords )
        {
            AppendVertexInput( vShader, "vec2", "vertex_tcoord" );
            AppendVertexOutput( vShader, "vec2", "fragment_tcoord", eShaderPrecisionLow );
        }
        
        AppendVertexUniform( vShader, "mat4", "viewMatrix" );
        AppendVertexUniform( vShader, "mat4", "projMatrix" );
        
        AppendFunctionBegin( vShader, "void", "main", "" );
        
        vShader += "gl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n";
        
        if( colour )
        {
            vShader += "fragment_colour = vertex_colour;\n";
        }
        
        if( tcoords )
        {
            vShader += "fragment_tcoord = vertex_tcoord;\n";
        }
        
        AppendFunctionEnd( vShader );
        
        core::String fShader = "";
        
        AppendHeader( fShader );
        
        if( colour )
        {
            AppendFragmentInput( fShader, "vec4", "fragment_colour", eShaderPrecisionLow );
        }

        if( tcoords )
        {
            AppendFragmentInput( fShader, "vec2", "fragment_tcoord", eShaderPrecisionLow );
            AppendFragmentUniform( fShader, "sampler2D", "texture0", eShaderPrecisionLow );
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
#if kBuildOpenGles2
            tmp = "gl_FragColor";
#else
            tmp = "output_colour";
#endif
        }
        
        if( colour && !tcoords )
        {
            fShader = fShader + tmp + " = fragment_colour;\n";
        }
        else if( !colour && tcoords )
        {
#if kBuildOpenGles2
            fShader = fShader + tmp + " = texture2D(texture0, fragment_tcoord);\n";
#else
            fShader = fShader + tmp + " = texture(texture0, fragment_tcoord);\n";
#endif
        }
        else if( colour && tcoords )
        {
#if kBuildOpenGles2
            fShader = fShader + tmp + " = texture2D(texture0, fragment_tcoord) * fragment_colour;\n";
#else
            fShader = fShader + tmp + " = texture(texture0, fragment_tcoord) * fragment_colour;\n";
#endif
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
