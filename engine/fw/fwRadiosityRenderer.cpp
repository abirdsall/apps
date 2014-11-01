#include "fw.h"


namespace fw
{
    using namespace gs;

    void RadiosityRenderer::Init( const aabb& bounds, s32 voxelCountX, s32 voxelCountY, s32 voxelCountZ )
    {
        _bounds = bounds;
        
        _voxelCountX = voxelCountX;
        _voxelCountY = voxelCountY;
        _voxelCountZ = voxelCountZ;
        
        _passCountZ = max( 1, _voxelCountZ / gs::MaxDrawBuffers() );
        _passCountHalfZ = max( 1, _passCountZ / 2 );
        _passCountQuarterZ = max( 1, _passCountZ / 4 );
        
        _voxelCountPerPassZ = min( gs::MaxDrawBuffers(), _voxelCountZ );
        _voxelCountPerPassHalfZ = min( gs::MaxDrawBuffers(), _voxelCountZ / 2 );
        _voxelCountPerPassQuarterZ = min( gs::MaxDrawBuffers(), _voxelCountZ / 4 );
        
        _voxelTextureA = TextureNew( "voxela", TexType3d, TexFormatRGBA8, _voxelCountX, _voxelCountY, _voxelCountZ, TexFlagClamp, Null );
        _voxelTextureB = TextureNew( "voxelb", TexType3d, TexFormatRGBA8, _voxelCountX, _voxelCountY, _voxelCountZ, TexFlagClamp, Null );
        _voxelTextureHalfA = TextureNew( "voxelhalfa", TexType3d, TexFormatRGBA8, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), Null );
        _voxelTextureHalfB = TextureNew( "voxelhalfb", TexType3d, TexFormatRGBA8, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), Null );
        
        for( s32 i = 0; i < _passCountZ; i++)
        {
            _voxelCanvasesA.Add( CanvasNew( "voxelCanvasA" ) );
            _voxelCanvasesB.Add( CanvasNew( "voxelCanvasB" ) );
            _voxelCanvasesHalfA.Add( CanvasNew( "voxelCanvasHalfA" ) );
            _voxelCanvasesHalfB.Add( CanvasNew( "voxelCanvasHalfB" ) );
            
            for( s32 j = 0; j < _voxelCountPerPassZ; j++ )
            {
                s32 z = j + i * _voxelCountPerPassZ;
                
                CanvasAdd( _voxelCanvasesA[ i ], _voxelTextureA, z );
                CanvasAdd( _voxelCanvasesB[ i ], _voxelTextureB, z );
                CanvasAdd( _voxelCanvasesHalfA[ i ], _voxelTextureHalfA, z );
                CanvasAdd( _voxelCanvasesHalfB[ i ], _voxelTextureHalfB, z );
            }
            
            f32 zStep = 1.0f / f32( _voxelCountZ );
            f32 zMin = zStep / 2.0f + f32( i * _voxelCountPerPassZ ) * zStep;
            
            _blurShadersX.Add( MakeBlurShader( 0, _voxelCountPerPassZ, zMin, zStep ) );
            _blurShadersY.Add( MakeBlurShader( 1, _voxelCountPerPassZ, zMin, zStep ) );
            _blurShadersZ.Add( MakeBlurShader( 2, _voxelCountPerPassZ, zMin, zStep ) );
        }

        for( s32 i = 0; i < _passCountHalfZ; i++ )
        {
            _voxelCanvasesQuarterA.Add( CanvasNew( "voxelCanvasQuarterA" ) );
            _voxelCanvasesQuarterB.Add( CanvasNew( "voxelCanvasQuarterB" ) );
            
            for( s32 j = 0; j < _voxelCountPerPassHalfZ; j++ )
            {
                s32 z = j + i * _voxelCountPerPassHalfZ;
                
                CanvasAdd( _voxelCanvasesQuarterA[ i ], _voxelTextureHalfA, z );
                CanvasAdd( _voxelCanvasesQuarterB[ i ], _voxelTextureHalfB, z );
            }
            
            f32 zStep = 1.0f / f32( _voxelCountZ / 2 );
            f32 zMin = zStep / 2.0f + f32( i * _voxelCountPerPassHalfZ ) * zStep;
            
            _blurShadersHalfX.Add( MakeBlurShader( 0, _voxelCountPerPassHalfZ, zMin, zStep ) );
            _blurShadersHalfY.Add( MakeBlurShader( 1, _voxelCountPerPassHalfZ, zMin, zStep ) );
            _blurShadersHalfZ.Add( MakeBlurShader( 2, _voxelCountPerPassHalfZ, zMin, zStep ) );
        }

        for( s32 i = 0; i < _passCountQuarterZ; i++)
        {
            _voxelCanvasesEighthA.Add( CanvasNew( "voxelCanvasEighthA" ) );
            _voxelCanvasesEighthB.Add( CanvasNew( "voxelCanvasEighthB" ) );
            
            for( s32 j = 0; j < _voxelCountPerPassQuarterZ; j++ )
            {
                s32 z = j + i * _voxelCountPerPassQuarterZ;
                
                CanvasAdd( _voxelCanvasesEighthA[ i ], _voxelTextureHalfA, z );
                CanvasAdd( _voxelCanvasesEighthB[ i ], _voxelTextureHalfB, z );
            }
            
            f32 zStep = 1.0f / f32( _voxelCountZ / 4 );
            f32 zMin = zStep / 2.0f + f32( i * _voxelCountPerPassQuarterZ ) * zStep;
            
            _blurShadersQuarterX.Add( MakeBlurShader( 0, _voxelCountPerPassQuarterZ, zMin, zStep ) );
            _blurShadersQuarterY.Add( MakeBlurShader( 1, _voxelCountPerPassQuarterZ, zMin, zStep ) );
            _blurShadersQuarterZ.Add( MakeBlurShader( 2, _voxelCountPerPassQuarterZ, zMin, zStep ) );
        }
        
        _lightTextureColour = TextureNew( "lightcol", TexType3d, TexFormatRGB16F, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ / 2, TexFlagClamp );
        _lightTextureDirection = TextureNew( "lightdir", TexType3d, TexFormatRGB16F, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ / 2, TexFlagClamp );
        
        for( s32 i = 0; i < _voxelCountZ / 2; i++ )
        {
            _lightCanvases.Add( CanvasNew( "lightCanvas" ) );
            
            CanvasAdd( _lightCanvases[ i ], _lightTextureColour, i );
            CanvasAdd( _lightCanvases[ i ], _lightTextureDirection, i );
        }
        
        _fillShader = fw::ShaderMake2d( true, false, 0, _voxelCountPerPassZ - 1 );

        // fragment_tcoord.x = object zmiddle
        // fragment_tcoord.y = object zthickness
#if GsOpenGl3
        core::String vShader = "#version 410 core\n";
#else
        core::String vShader = "#version 300 es\n";
#endif
        vShader = vShader + "in vec3 vertex_position;\n\
        in vec4 vertex_colour;\n\
        in vec2 vertex_tcoord;\n\
        out vec4 fragment_colour;\n\
        out vec2 fragment_tcoord;\n\
        uniform mat4 viewMatrix;\n\
        uniform mat4 projMatrix;\n\
        void main()\n\
        {\n\
        fragment_colour = vertex_colour; \n\
        fragment_tcoord = vertex_tcoord; \n\
        gl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);\n\
        }";
        
#if GsOpenGl3
        core::String fShader = "#version 410 core\n";
#else
        core::String fShader = "#version 300 es\n";
        fShader = fShader + "precision highp float;\n";
#endif
        fShader = fShader + "uniform float zMin;\n\
        uniform float zStep;\n\
        in vec4 fragment_colour;\n\
        in vec2 fragment_tcoord;\n";
        
        for(s32 i = 0; i < _voxelCountPerPassZ; i++)
        {
            fShader = fShader + "layout(location = " + i + ") out vec4 fs_" + i + ";\n";
        }
        
        fShader = fShader + "void main()\n\
        {\n\
        float zBase = zMin;\n";
        for(s32 i = 0; i < _voxelCountPerPassZ; i++)
        {
            fShader = fShader + "fs_" + i + "= vec4( fragment_colour.x, fragment_colour.y, fragment_colour.z, clamp( ( fragment_tcoord.y - abs( zBase - fragment_tcoord.x ) ) / zStep, 0.0, 1.0 ) );\n\
            zBase += zStep;\n";
        }
        fShader = fShader + "}";
        
        _shaderVoxelise = ShaderNew( vShader.toStr(), fShader.toStr() );
        
#if GsOpenGl3
        vShader = "#version 150\n";
#else
        vShader = "#version 300 es\n";
#endif
        vShader = vShader + "in vec2 vertex_position;\n\
        in vec2 vertex_tcoord;\n\
        out vec2 fragment_tcoord;\n\
        uniform mat4 viewMatrix;\n\
        uniform mat4 projMatrix;\n\
        void main()\n\
        {\n\
        gl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);\n\
        fragment_tcoord = vertex_tcoord;\n\
        }";
#if GsOpenGl3
        fShader = "#version 150\n";
        fShader = fShader + "out vec4 output_colour[2];\n";
#else
        fShader = "#version 300 es\n";
        fShader = fShader + "precision highp float;\n";
        fShader = fShader + "precision highp sampler3D;\n";
        fShader = fShader + "\
        layout(location = 0) out vec4 fs_0;\n\
        layout(location = 1) out vec4 fs_1;\n";
#endif
        fShader = fShader + "uniform sampler3D texture0;\n\
        uniform vec3 lightPos;\n\
        uniform vec3 lightCol;\n\
        uniform vec3 worldMin;\n\
        uniform vec3 worldSize;\n\
        uniform float zWorld;\n\
        in vec2 fragment_tcoord;\n\
        void main()\n\
        {\n\
        vec3 worldPos = vec3( worldMin.x + fragment_tcoord.x * worldSize.x, worldMin.y + fragment_tcoord.y * worldSize.y, zWorld );\n\
        vec3 lightVec = worldPos - lightPos;\n\
        float lightDistance = length( lightVec );\n\
        vec3 lightDir = normalize( lightVec );\n\
        float attenuate = clamp( 1.0 - ( lightDistance / 24.0 ), 0.0, 1.0 );\n\
        vec3 aopos = vec3( fragment_tcoord.x, fragment_tcoord.y, ( zWorld - worldMin.z ) / worldSize.z );\n\
        vec3 aolightpos = ( lightPos - worldMin ) / worldSize;\n\
        vec3 ff = aolightpos - aopos;\n\
        float occ = 0.0;\n\
        float l = length( ff );\n\
        const float max_length = 0.875;\n\
        const int samples = 24;\n\
        float lscale = min( max_length, max_length / l );\n\
        ff *= lscale;\n\
        //float last = 0.0;\n\
        for( int i = samples; i > 1; i--)\n\
        {\n\
        float samp = texture( texture0, aopos + ff.xyz * ( float(i) / float(samples) ) ).a;\n\
        occ += clamp( samp, 0.0, 1.0);\n\
        //occ += clamp( samp-last, 0.0, 1.0);\n\
        //last = samp;\n\
        }\n\
        attenuate *= clamp( 1.0 - occ*0.75, 0.0, 1.0);\n";
#if GsOpenGl3
        fShader = fShader + "\
        output_colour[0].rgb = lightCol * attenuate;\n\
        output_colour[1].rgb = lightDir;\n}";
#else
        fShader = fShader + "\
        fs_0.rgb = lightCol * attenuate;\n\
        fs_1.rgb = lightDir;\n}";
#endif
        
        // need to kill light dir if lightCol zero
        _shaderLuminise = ShaderNew( vShader.toStr(), fShader.toStr() );
        
#if GsOpenGl3
        vShader = "#version 150\n";
#else
        vShader = "#version 300 es\n";
#endif
        vShader = vShader + "in vec3 vertex_position;\n\
        in vec3 vertex_normal;\n\
        in vec4 vertex_colour;\n\
        out vec4 fragment_normal;\n\
        out vec4 fragment_colour;\n\
        out vec4 fragment_worldPos;\n\
        uniform mat4 viewMatrix;\n\
        uniform mat4 projMatrix;\n\
        void main()\n\
        {\n\
        gl_Position = projMatrix * viewMatrix * vec4( vertex_position, 1.0 );\n\
        fragment_worldPos = vec4( vertex_position, 1.0 );\n\
        fragment_normal = vec4( vertex_normal, 0.0 );\n\
        fragment_colour = vertex_colour;\n\
        }";
#if GsOpenGl3
        fShader = "#version 150\n";
#else
        fShader = "#version 300 es\n";
        fShader = fShader + "precision highp float;\n";
        fShader = fShader + "precision highp sampler3D;\n";
#endif
        fShader = fShader + "uniform sampler3D texture0;\n\
        uniform sampler3D texture1;\n\
        uniform sampler3D texture2;\n\
        uniform sampler3D texture3;\n\
        uniform vec3 worldMin;\n\
        uniform vec3 worldSize;\n\
        in vec4 fragment_worldPos;\n\
        in vec4 fragment_normal;\n\
        in vec4 fragment_colour;\n\
        out vec4 output_colour;\n\
        void main()\n\
        {\n\
        vec3 worldPos = fragment_worldPos.xyz;// + fragment_normal.xyz;\n\
        vec3 texturePos = ( worldPos - worldMin ) / worldSize;\n\
        vec3 lightDir = texture( texture1, texturePos ).rgb;\n\
        float attenuation = 0.0f + 1.0f * max(0.0, -dot( fragment_normal.xyz, lightDir ) );\n\
        vec4 aonormal = fragment_normal;\n\
        aonormal.y += 0.75;\n\
        vec3 worldPos2 = fragment_worldPos.xyz + aonormal.xyz * 1.0 * 0.4;\n\
        vec3 worldPos3 = fragment_worldPos.xyz + aonormal.xyz * 2.0 * 0.4;\n\
        vec3 worldPos4 = fragment_worldPos.xyz + aonormal.xyz * 4.0 * 0.4;\n\
        vec3 worldPos5 = fragment_worldPos.xyz + aonormal.xyz * 8.0 * 0.4;\n\
        vec3 texturePos2 = ( worldPos2 - worldMin ) / worldSize;\n\
        vec3 texturePos3 = ( worldPos3 - worldMin ) / worldSize;\n\
        vec3 texturePos4 = ( worldPos4 - worldMin ) / worldSize;\n\
        vec3 texturePos5 = ( worldPos5 - worldMin ) / worldSize;\n\
        vec3 col = texture( texture0, texturePos ).xyz;\n\
        vec4 aocol0 = texture( texture2, texturePos2 );\n\
        vec4 aocol1 = textureLod( texture3, texturePos3, 0.0 );\n\
        vec4 aocol2 = textureLod( texture3, texturePos4, 1.0 );\n\
        vec4 aocol3 = textureLod( texture3, texturePos5, 2.0 );\n\
        float ao = 0.0;\n\
        ao += aocol0.a;\n\
        ao += aocol1.a;\n\
        ao += clamp( aocol2.a * 1.0, 0.0, 1.0 );\n\
        ao += clamp( aocol3.a * 1.0, 0.0, 1.0 );\n\
        ao = 1.0 - clamp( 0.25 * ao, 0.0, 1.0 );\n\
        vec3 amb = aocol0.rgb * aocol1.rgb * aocol2.rgb * aocol3.rgb;\n\
        output_colour = fragment_colour * ( vec4( amb.xyz * 0.5 * ao + 1.0 * ( col * attenuation ), 1.0 ) + vec4(0.1,0.0,0.0,1.0));\n\
        }";
        _shaderForward = ShaderNew( vShader.toStr(), fShader.toStr() );
    }
    
    ShaderHandle RadiosityRenderer::MakeBlurShader( s32 axis, s32 targetCount, f32 zMin, f32 zStep )
    {
        const c8* lookup = axis == 0 ? "offset[ j ], 0.0, 0.0": axis == 1 ? "0.0, offset[ j ], 0.0" : "0.0, 0.0, offset[ j ]";
#if GsOpenGl3
        core::String vShader = "#version 410\n";
#else
        core::String vShader = "#version 300 es\n";
#endif
        vShader = vShader + "in vec2 vertex_position;\n\
        in vec2 vertex_tcoord;\n\
        out vec2 fragment_tcoord;\n\
        uniform mat4 viewMatrix;\n\
        uniform mat4 projMatrix;\n\
        void main()\n\
        {\n\
        gl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n\
        fragment_tcoord = vertex_tcoord;\n\
        }";
#if GsOpenGl3
        core::String fShader = "#version 410\n";
#else
        core::String fShader = "#version 300 es\n";
        fShader = fShader + "precision highp float;\n";
        fShader = fShader + "precision highp sampler3D;\n";
#endif
        for(s32 i = 0; i < targetCount; i++)
        {
            fShader = fShader + "layout(location = " + i + ") out vec4 fs_" + i + ";\n";
        }
        
        fShader = fShader + "uniform sampler3D texture0;\n\
        uniform float offset[ 5 ];\n\
        uniform float weight[ 5 ];\n\
        uniform float lod;\n\
        in vec2 fragment_tcoord;\n\
        void main()\n\
        {\n";
        
        f32 tz = zMin;
        
        for(s32 i = 0; i < targetCount; i++)
        {
            fShader = fShader + "fs_" + i + " = textureLod( texture0, vec3( fragment_tcoord.x, fragment_tcoord.y, " + tz + "), lod ) * weight[ 0 ];\n";
            
            tz += zStep;
        }
        
        fShader = fShader + "for( int j = 1; j < 5; j++ )\n\
        {\n";
        
        tz = zMin;
        
        for(s32 i = 0; i < targetCount; i++)
        {
            fShader = fShader + "\
            fs_" + i + " += textureLod( texture0, ( vec3( fragment_tcoord.x, fragment_tcoord.y, " + tz + ") + vec3( " + lookup + " ) ), lod ) * weight[ j ];\n\
            fs_" + i + " += textureLod( texture0, ( vec3( fragment_tcoord.x, fragment_tcoord.y, " + tz + ") - vec3( " + lookup + " ) ), lod ) * weight[ j ];\n";
            
            tz += zStep;
        }
        
        fShader = fShader + "}\n";
        
        fShader = fShader + "}";
        
        return ShaderNew( vShader.toStr(), fShader.toStr() );
    }

    void RadiosityRenderer::Render()
    {
        // Clear buffers and voxelise scene
        
        for( s32 i = 0; i < _passCountZ; i++ )
        {
            Put();
            CanvasSet( _voxelCanvasesB[ i ] );

            Put();
            Set2d();
            SetWrite( eWriteRgba );
            SetBlend( eBlendNone );
            ShaderSet( _fillShader );
            DrawQuad2d( Quad2dShaderFilledCustom, fw::Rect( 0.0f, 0.0f, f32( _voxelCountX ), f32( _voxelCountY ) ), v4( 1.0f, 1.0f, 1.0f, 0.0f ) );
            Pop();

            f32 zStep = ( _bounds.mMax.z - _bounds.mMin.z ) / f32( _voxelCountZ );
            f32 zMin = _bounds.mMin.z + ( zStep / 2.0f ) + (( _bounds.mMax.z - _bounds.mMin.z ) / f32( _passCountZ )) * f32( i );
            Put();
            SetDepth( eDepthNone );
            ShaderSet( _shaderVoxelise );
            ShaderSetFloat( "zMin", zMin );
            ShaderSetFloat( "zStep", zStep );
            SetWrite( eWriteRgba );
            SetBlend( eBlendMixRgbAddA );
            SetCull( eCullBack );
            SetStageMatrices( true );
            _voxelising = true;
            _scene->Render( *this );
            Pop();
            
            Pop();
        }

        // Blur voxelised scene - note how we simultaneously read/write the same texture at different mip levels to ensure the result of the last blur for each mip level always ends up in TextureA

        BlurLayers( _voxelTextureB, _voxelTextureA, _voxelTextureB, _voxelCanvasesA, _voxelCanvasesB, _voxelCanvasesA, _blurShadersX, _blurShadersY, _blurShadersZ, 0, 0, _voxelCountX, _voxelCountY, _voxelCountZ, 0.0f );
        BlurLayers( _voxelTextureA, _voxelTextureHalfA, _voxelTextureHalfB, _voxelCanvasesHalfA, _voxelCanvasesHalfB, _voxelCanvasesHalfA, _blurShadersX, _blurShadersY, _blurShadersZ, 0, 0, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ / 2, 0.0f );
        BlurLayers( _voxelTextureHalfA, _voxelTextureHalfA, _voxelTextureHalfB, _voxelCanvasesQuarterA, _voxelCanvasesQuarterB, _voxelCanvasesQuarterA, _blurShadersHalfX, _blurShadersHalfY, _blurShadersHalfZ, 0, 1, _voxelCountX / 4, _voxelCountY / 4, _voxelCountZ / 2, 1.0f );
        BlurLayers( _voxelTextureHalfA, _voxelTextureHalfA, _voxelTextureHalfB, _voxelCanvasesEighthA, _voxelCanvasesEighthB, _voxelCanvasesEighthA, _blurShadersQuarterX, _blurShadersQuarterY, _blurShadersQuarterZ, 1, 2, _voxelCountX / 8, _voxelCountY / 8, _voxelCountZ / 4, 1.0f );

        // Create light field

        Put();
        SetWrite( eWriteRgb );
        SetBlend( eBlendNone );
        SetDepth( eDepthNone );
        SetCull( eCullNone );
        
        f32 zStep = ( _bounds.mMax.z - _bounds.mMin.z ) / f32( _voxelCountZ / 2 );
        f32 zMin = _bounds.mMin.z + ( zStep / 2.0f );
        
        for( s32 l = 0; l < _lights.Count(); l++ )
        {
            Light* light = (Light*)_lights[ l ];
            if( light->_active )
            {
                for( s32 i = 0; i < ( _voxelCountZ / 2 ); i++ )
                {
                    CanvasSet( _lightCanvases[ i ], 0 );
                    Set2d();
                    ShaderSet( _shaderLuminise );
                    TextureSet("texture0", _voxelTextureA );
                    ShaderSetVec3( "lightPos", light->_position );
                    ShaderSetVec3( "lightCol", light->_colour );
                    ShaderSetVec3( "worldMin", _bounds.mMin );
                    ShaderSetVec3( "worldSize", _bounds.mMax - _bounds.mMin );
                    ShaderSetFloat( "zWorld", zMin + zStep * f32( i ) );
                    DrawQuad2d( Quad2dShaderTexturedCustom, fw::Rect( 0, 0, f32( _voxelCountX / 2 ), f32( _voxelCountY / 2 ) ), fw::Rect(0.0f, 1.0f, 1.0f, 0.0f) );
                }
                SetBlend( eBlendAddRgb ); // set to add rgb for subsequent lights
                //break;
            }
        }
        Pop();
        
        // Render scene

        CanvasSet( kCanvasInvalid );
        
        Put();
        SetCull(eCullBack);
        SetBlend(eBlendNone);
        SetWrite(eWriteRgbz);
        SetDepth(eDepthLequal);
        ShaderSet( _shaderForward );
        TextureSet("texture0", _lightTextureColour );
        TextureSet("texture1", _lightTextureDirection );
        TextureSet("texture2", _voxelTextureA );
        TextureSet("texture3", _voxelTextureHalfA );
        ShaderSetVec3( "worldMin", _bounds.mMin );
        ShaderSetVec3( "worldSize", _bounds.mMax - _bounds.mMin );
        
        //SetStageMatrices( true );

        _voxelising = false;
        _scene->Render( *this );
        
        Pop();
    }
    
    void RadiosityRenderer::BlurLayers(TextureHandle textureX,
                                       TextureHandle textureY,
                                       TextureHandle textureZ,
                                       Array<CanvasHandle>& canvasesX,
                                       Array<CanvasHandle>& canvasesY,
                                       Array<CanvasHandle>& canvasesZ,
                                       Array<ShaderHandle>& shadersX,
                                       Array<ShaderHandle>& shadersY,
                                       Array<ShaderHandle>& shadersZ,
                                       s32 srcLod,
                                       s32 dstLod,
                                       u32 dstSizeX,
                                       u32 dstSizeY,
                                       u32 dstSizeZ,
                                       f32 zBlur )
    {
        float rTexelCountX = 1.0f / f32( dstSizeX );
        float rTexelCountY = 1.0f / f32( dstSizeY );
        float rTexelCountZ = 1.0f / f32( dstSizeZ );
        float xOffsets[ 5 ] = { 0.0f, rTexelCountX, rTexelCountX * 2.0f, rTexelCountX * 3.0f, rTexelCountX * 4.0f };
        float yOffsets[ 5 ] = { 0.0f, rTexelCountY, rTexelCountY * 2.0f, rTexelCountY * 3.0f, rTexelCountY * 4.0f };
        float zOffsets[ 5 ] = { 0.0f, rTexelCountZ, rTexelCountZ * 2.0f, rTexelCountZ * 3.0f, rTexelCountZ * 4.0f };
        float weights[ 5 ] = { 0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f };
        
        for( s32 i = 0; i < 5; i++ )
        {
            zOffsets[ i ] *= zBlur;
        }
        
        Put();
        SetWrite( eWriteRgba );
        SetBlend( eBlendNone );
        SetDepth( eDepthNone );
        SetCull( eCullNone );
        
        for( s32 i = 0; i < shadersX.Count(); i++ )
        {
            CanvasSet( canvasesX[ i ], dstLod );
            Set2d();
            ShaderSet( shadersX[ i ] );
            TextureSet( "texture0", textureX );
            ShaderSetFloat( "lod", f32( srcLod ) );
            ShaderSetFloatArray( "offset" , xOffsets, 5 );
            ShaderSetFloatArray( "weight" , weights, 5 );
            fw::DrawQuad2d( Quad2dShaderTexturedCustom, fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
        }

        for( s32 i = 0; i < shadersY.Count(); i++ )
        {
            CanvasSet( canvasesY[ i ], dstLod );
            Set2d();
            ShaderSet( shadersY[ i ] );
            TextureSet( "texture0", textureY );
            ShaderSetFloat( "lod", f32( dstLod ) );
            ShaderSetFloatArray( "offset" , yOffsets, 5 );
            ShaderSetFloatArray( "weight" , weights, 5 );
            fw::DrawQuad2d( Quad2dShaderTexturedCustom, fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
        }
        
        for( s32 i = 0; i < shadersZ.Count(); i++ )
        {
            CanvasSet( canvasesZ[ i ], dstLod );
            Set2d();
            ShaderSet( shadersZ[ i ] );
            TextureSet( "texture0", textureZ );
            ShaderSetFloat( "lod", f32( dstLod ) );
            ShaderSetFloatArray( "offset" , zOffsets, 5 );
            ShaderSetFloatArray( "weight" , weights, 5 );
            fw::DrawQuad2d( Quad2dShaderTexturedCustom, fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
        }
        
        Pop();
    }
    
    void RadiosityRenderer::SetStageMatrices( bool tracing )
    {
        m4 mp = orthogonal(
                           _bounds.mMin.x, _bounds.mMax.x,
                           _bounds.mMax.y, _bounds.mMin.y,
                           _bounds.mMax.z + 20.0f, _bounds.mMin.z - 20.0f );
        
        gs::SetMatrixP( mp );
        
        if( tracing )
        {
            m4 mm = identity4();
            mm.rows[ 2 ].z = -1.0f;
            mm.setPosition( v3( 0.0f, 0.0f, 20.0f ) );
            gs::SetMatrixM( mm );
        }
    }
}