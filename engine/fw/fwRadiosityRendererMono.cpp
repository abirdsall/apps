#include "fw.h"

static const s32 RenderTargetLimit = 4;

namespace fw
{
    using namespace gs;

    void RadiosityRendererMono::Init( const aabb& bounds, s32 voxelCountX, s32 voxelCountY, s32 voxelCountZ )
    {
        _bounds = bounds;
        
        _voxelCountX = voxelCountX;
        
        _voxelCountY = voxelCountY;
        
        _voxelCountZ = voxelCountZ;
        
        _passesPacked = max( 1, ( voxelCountZ / 4 ) / RenderTargetLimit );
        _passesUnpacked = max( 1, voxelCountZ / RenderTargetLimit );
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Create sd scratch voxel textures and canvases
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////
/*
        Array<gs::CanvasHandle> _voxelCanvasesHdPacked;
        Array<gs::CanvasHandle> _voxelCanvasesHdPackedScratch;
        Array<gs::CanvasHandle> _voxelCanvasesSdPacked;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedScratch;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod1;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod1Scratch;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod2;
        Array<gs::CanvasHandle> _voxelCanvasesSdPackedLod2Scratch;
        Array<gs::CanvasHandle> _voxelCanvasesHd;
        Array<gs::CanvasHandle> _voxelCanvasesSd;
*/
        _voxelTextureSdScratch = TextureNew( TexType3d, TexFormatRGBA8, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), Null );

        s32 targetCount = 0;
        gs::CanvasHandle canvas = CanvasNew();
        _voxelCanvasesSdPacked.Add( canvas );
        
        for(s32 i = 0; i < _voxelCountZ; i++)
        {
            if( targetCount == RenderTargetLimit )
            {
                targetCount = 0;
                canvas = CanvasNew();
                _voxelCanvasesSdPacked.Add( canvas );
            }
            
            CanvasAdd( canvas, _voxelTextureSdScratch, i );

            targetCount++;
        }
        
        _voxelCanvasSdScratch = CanvasNew();
        
        _voxelCanvasSdLod1Scratch = CanvasNew();
        
        _voxelCanvasSdLod2Scratch = CanvasNew();
        
        for( s32 i = 0; i < _voxelCountZ; i++ )
        {
            CanvasAdd( _voxelCanvasSdScratch, _voxelTextureSdScratch, i );
        }
        
        for(s32 i = 0; i < _voxelCountZ / 2; i++)
        {
            CanvasAdd( _voxelCanvasSdLod1Scratch, _voxelTextureSdScratch, i );
        }
        
        for(s32 i = 0; i < _voxelCountZ / 4; i++)
        {
            CanvasAdd( _voxelCanvasSdLod2Scratch, _voxelTextureSdScratch, i );
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Create sd voxel textures and canvases
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        _voxelTextureSd = TextureNew( TexType3d, TexFormatRGBA8, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), Null );
        
        _voxelCanvasSd = CanvasNew();
        _voxelCanvasSdLod1 = CanvasNew();
        _voxelCanvasSdLod2 = CanvasNew();
        
        for( s32 i = 0; i < _voxelCountZ; i++ )
        {
            CanvasAdd( _voxelCanvasSd, _voxelTextureSd, i );
        }
        
        for( s32 i = 0; i < _voxelCountZ / 2; i++ )
        {
            CanvasAdd( _voxelCanvasSdLod1, _voxelTextureSd, i );
        }
        
        for( s32 i = 0; i < _voxelCountZ / 4; i++ )
        {
            CanvasAdd( _voxelCanvasSdLod2, _voxelTextureSd, i );
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Create light texture and canvas
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        _lightTextureColour = TextureNew( TexType3d, TexFormatRGB16F, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ / 2, TexFlagClamp );
        
        _lightTextureDirection = TextureNew( TexType3d, TexFormatRGB16F, _voxelCountX / 2, _voxelCountY / 2, _voxelCountZ / 2, TexFlagClamp );
        
        _lightCanvas = CanvasNew();
        
        CanvasAdd( _lightCanvas, _lightTextureColour );
        
        CanvasAdd( _lightCanvas, _lightTextureDirection );
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Create hd voxel texture and canvas
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        
        _voxelTextureHd = TextureNew( TexType3d, TexFormatRGBA8, _voxelCountX, _voxelCountY, _voxelCountZ / 4, TexFlagClamp, Null );
        
        _voxelCanvasHd = CanvasNew();
        
        for( s32 i = 0; i < _voxelCountZ / 4; i++ )
        {
            CanvasAdd( _voxelCanvasHd, _voxelTextureHd, i );
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Create hd scratch voxel texture and canvas
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        _voxelTextureHdScratch = TextureNew( TexType3d, TexFormatRGBA8, _voxelCountX, _voxelCountY, _voxelCountZ / 4, TexFlagClamp, Null );
        
        _voxelCanvasHdScratch = CanvasNew();
        
        for( s32 i = 0; i < _voxelCountZ / 4; i++ )
        {
            CanvasAdd( _voxelCanvasHdScratch, _voxelTextureHdScratch, i );
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Create shaders
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        
        _shaderFillZ = fw::ShaderMake2d( true, false, 0, 7 );
        
        _shaderBlurX = MakeBlurShader( 0, _voxelCountZ );
        _shaderBlurY = MakeBlurShader( 1, _voxelCountZ );
        _shaderBlurZ = MakeBlurShader( 2, _voxelCountZ );
        
        _shaderBlurXHalf = MakeBlurShader( 0, _voxelCountZ / 2 );
        _shaderBlurYHalf = MakeBlurShader( 1, _voxelCountZ / 2 );
        _shaderBlurZHalf = MakeBlurShader( 2, _voxelCountZ / 2 );
        
        _shaderBlurXQuarter = MakeBlurShader( 0, _voxelCountZ / 4 );
        _shaderBlurYQuarter = MakeBlurShader( 1, _voxelCountZ / 4 );
        _shaderBlurZQuarter = MakeBlurShader( 2, _voxelCountZ / 4 );
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Voxelise shader
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // fragment_tcoord.x = object zmiddle
        // fragment_tcoord.y = object zthickness
#if GsOpenGl3
        core::String vShader = "#version 410 core\n";
#else
        core::String vShader = "#version 300 es\n";
#endif
        vShader = vShader + "in vec3 vertex_position;\n\
        in vec2 vertex_tcoord;\n\
        out vec2 fragment_tcoord;\n\
        uniform mat4 viewMatrix;\n\
        uniform mat4 projMatrix;\n\
        void main()\n\
        {\n\
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
        in vec2 fragment_tcoord;\n";
        for(s32 i = 0; i < _voxelCountZ / 4; i++)
        {
            fShader = fShader + "layout(location = " + i + ") out vec4 fs_" + i + ";\n";
        }
        
        fShader = fShader + "void main()\n\
        {\n\
        float zBase = zMin;\n";
        for(s32 i = 0; i < _voxelCountZ / 4; i++)
        {
            fShader = fShader + "fs_" + i + "= clamp( ( fragment_tcoord.y - abs( vec4( zBase, zBase + zStep, zBase + zStep * 2.0, zBase + zStep * 3.0 ) - fragment_tcoord.x ) ) / zStep, 0.0, 1.0 );\n\
            zBase += ( zStep * 4.0 );\n";
        }
        fShader = fShader + "}";

        _shaderVoxelise = ShaderNew( vShader.toStr(), fShader.toStr() );
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Unswizzle shader
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        
#if GsOpenGl3
        vShader = "#version 410 core\n";
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
        fragment_tcoord = vertex_tcoord; \n\
        gl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);\n\
        }";
        
#if GsOpenGl3
        fShader = "#version 410 core\n";
#else
        fShader = "#version 300 es\n";
        fShader = fShader + "precision highp float;\n";
#endif
        fShader = fShader + "uniform sampler2D texture0;\n\
        in vec2 fragment_tcoord;\n";
        
        for(s32 i = 0; i < _voxelCountZ / 4; i++)
        {
            fShader = fShader + "layout(location = " + i + ") out vec4 fs_" + i + ";\n";
        }
        
        fShader = fShader + "void main()\n\
        {\n\
        vec4 sample = texture( texture0, fragment_tcoord );\n";
        for(s32 i = 0; i < _voxelCountZ / 4; i++)
        {
            fShader = fShader + "fs_" + i + ".r = sample[" + i + "];\n";
        }
        fShader = fShader + "}";
        
        _shaderUnswizzle = ShaderNew( vShader.toStr(), fShader.toStr() );
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Luminise shader
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////

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
        float attenuate = clamp( 1.0 - ( lightDistance / 32.0 ), 0.0, 1.0 );\n\
        vec3 aopos = vec3( fragment_tcoord.x, fragment_tcoord.y, ( zWorld - worldMin.z ) / worldSize.z );\n\
        vec3 aolightpos = ( lightPos - worldMin ) / worldSize;\n\
        vec3 ff = aolightpos - aopos;\n\
        float occ = 0.0;\n\
        float l = length( ff );\n\
        const float max_length = 0.875;\n\
        const int samples = 32;\n\
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
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Forward shader
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////

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
        //aonormal.y += 0.75;\n\
        vec3 worldPos2 = fragment_worldPos.xyz + aonormal.xyz * 1.0;\n\
        vec3 texturePos2 = ( worldPos2 - worldMin ) / worldSize;\n\
        vec3 worldPos3 = fragment_worldPos.xyz + aonormal.xyz * 1.0;\n\
        vec3 texturePos3 = ( worldPos3 - worldMin ) / worldSize;\n\
        vec3 worldPos4 = fragment_worldPos.xyz + aonormal.xyz * 1.0;\n\
        vec3 texturePos4 = ( worldPos4 - worldMin ) / worldSize;\n\
        vec3 worldPos5 = fragment_worldPos.xyz + aonormal.xyz * 1.0;\n\
        vec3 texturePos5 = ( worldPos5 - worldMin ) / worldSize;\n\
        vec3 col = texture( texture0, texturePos ).xyz;\n\
        vec4 aocol0 = texture( texture2, texturePos2 );\n\
        vec4 aocol1 = textureLod( texture3, texturePos3, 0.0 );\n\
        vec4 aocol2 = textureLod( texture3, texturePos4, 1.0 );\n\
        vec4 aocol3 = textureLod( texture3, texturePos5, 2.0 );\n\
        aocol0.a*=1.0;\n\
        aocol1.a*=1.0;\n\
        aocol2.a*=1.0;\n\
        aocol3.a*=1.0;\n\
        float ao = 0.0;\n\
        ao += aocol0.a;\n\
        ao += aocol1.a;\n\
        ao += aocol2.a;\n\
        ao += aocol3.a;\n\
        ao = 1.0 - clamp( 0.25 * ao, 0.0, 1.0 );\n\
        //aocol0.rgb = mix( vec3( 1.0, 1.0, 1.0 ), aocol0.rgb, aocol0.a );\n\
        //aocol1.rgb = mix( vec3( 1.0, 1.0, 1.0 ), aocol1.rgb, aocol1.a );\n\
        //aocol2.rgb = mix( vec3( 1.0, 1.0, 1.0 ), aocol2.rgb, aocol2.a );\n\
        //aocol3.rgb = mix( vec3( 1.0, 1.0, 1.0 ), aocol3.rgb, aocol3.a );\n\
        vec3 amb = aocol0.rgb*aocol1.rgb*aocol2.rgb*aocol3.rgb;\n\
        //amb = amb*amb*amb*amb*amb*amb*amb;\n\
        //output_colour = vec4( amb, 1.0)*ao;\n\
        //output_colour = clamp(fragment_colour, 0.0, 1.0) * vec4( amb, 1.0) * ao;\n\
        output_colour = fragment_colour * ( vec4( amb.xyz * 0.5 * ao + 1.0 * ( col * attenuation ), 1.0 ) + vec4(0.1,0.0,0.0,1.0));\n\
        }";
        _shaderForward = ShaderNew( vShader.toStr(), fShader.toStr() );
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Blur shader
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ShaderHandle RadiosityRendererMono::MakeBlurShader( s32 axis, s32 layerCount )
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
        for(s32 i = 0; i < layerCount; i++)
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
        
        for(s32 i = 0; i < layerCount; i++)
        {
            f32 tz = ( f32( i ) + 0.5f ) / f32( layerCount );
            
            fShader = fShader + "fs_" + i + " = textureLod( texture0, vec3( fragment_tcoord.x, fragment_tcoord.y, " + tz + "), lod ) * weight[ 0 ];\n";
        }
        
        fShader = fShader + "for( int j = 1; j < 5; j++ )\n\
        {\n";
        
        for(s32 i = 0; i < layerCount; i++)
        {
            f32 tz = ( f32( i ) + 0.5f ) / f32( layerCount );
            
            fShader = fShader + "\
            fs_" + i + " += textureLod( texture0, ( vec3( fragment_tcoord.x, fragment_tcoord.y, " + tz + ") + vec3( " + lookup + " ) ), lod ) * weight[ j ];\n\
            fs_" + i + " += textureLod( texture0, ( vec3( fragment_tcoord.x, fragment_tcoord.y, " + tz + ") - vec3( " + lookup + " ) ), lod ) * weight[ j ];\n";
        }
        
        fShader = fShader + "}\n";
        
        fShader = fShader + "}";
        
        return ShaderNew( vShader.toStr(), fShader.toStr() );
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Render
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void RadiosityRendererMono::Render()
    {
        // Clear
        Put();
        CanvasSet( _voxelCanvasHdScratch );
        Set2d();
        SetWrite( eWriteRgba );
        SetBlend( eBlendNone );
        ShaderSet( _shaderFillZ );
        DrawQuad2d( Quad2dShaderFilledCustom, fw::Rect( 0.0f, 0.0f, f32( _voxelCountX ), f32( _voxelCountY ) ), v4( 1.0f, 1.0f, 1.0f, 0.0f ) );
        Pop();
        
        // Voxelise
        f32 zStep = ( _bounds.mMax.z - _bounds.mMin.z ) / f32( _voxelCountZ );
        f32 zMin = _bounds.mMin.z + ( zStep / 2.0f );
        Put();
        CanvasSet( _voxelCanvasHdScratch );
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
        
        // Make
        Pop();
        
        BlurLayers(
                   _voxelTextureHdScratch, //textureX
                   _voxelCanvasHd,//canvasX
                   _voxelTextureHd,//textureY
                   _voxelCanvasHdScratch,//canvasY
                   _voxelTextureHdScratch,//textureZ
                   _voxelCanvasHd,//canvasZ
                   _shaderBlurX,
                   _shaderBlurY,
                   _shaderBlurZ,
                   0,//srcLod
                   0,//dstLod
                   _voxelCountX,//dstSizeX
                   _voxelCountY,//dstSizeY
                   _voxelCountZ,//dstSizeZ
                   0.5f//zBlur
                   );
        BlurLayers(
                   _voxelTextureHd, //textureX
                   _voxelCanvasSd, //canvasX
                   _voxelTextureSd, //textureY
                   _voxelCanvasSdScratch, //canvasY
                   _voxelTextureSdScratch, //textureZ
                   _voxelCanvasSd, //canvasZ
                   _shaderBlurX,
                   _shaderBlurY,
                   _shaderBlurZ,
                   0,
                   0,
                   _voxelCountX / 2, //dstSizeX
                   _voxelCountY / 2, //dstSizeY
                   _voxelCountZ, //dstSizeZ
                   1.0f//zBlur
                   );
        BlurLayers(
                   _voxelTextureSd,
                   _voxelCanvasSdLod1Scratch,
                   _voxelTextureSdScratch,
                   _voxelCanvasSdLod1,
                   _voxelTextureSd,
                   _voxelCanvasSdLod1Scratch,
                   _shaderBlurXHalf,
                   _shaderBlurYHalf,
                   _shaderBlurZHalf,
                   0,
                   1,
                   _voxelCountX / 4,
                   _voxelCountY / 4,
                   _voxelCountZ / 2,
                   1.0f
                   );
        BlurLayers(
                   _voxelTextureSdScratch,
                   _voxelCanvasSdLod2,
                   _voxelTextureSd,
                   _voxelCanvasSdLod2Scratch,
                   _voxelTextureSdScratch,
                   _voxelCanvasSdLod2,
                   _shaderBlurXQuarter,
                   _shaderBlurYQuarter,
                   _shaderBlurZQuarter,
                   1,
                   2,
                   _voxelCountX / 8,
                   _voxelCountY / 8,
                   _voxelCountZ / 4,
                   1.0f
                   );
        Put();
        SetWrite( eWriteRgb );
        SetBlend( eBlendNone );
        SetDepth( eDepthNone );
        SetCull( eCullNone );
        
        zStep = ( _bounds.mMax.z - _bounds.mMin.z ) / f32( _voxelCountZ / 2 );
        zMin = _bounds.mMin.z + ( zStep / 2.0f );
        
        for( s32 l = 0; l < _lights.Count(); l++ )
        {
            Light* light = (Light*)_lights[ l ];
            if( light->_active )
            {
                for( int i = 0; i < ( _voxelCountZ / 2 ); i++ )
                {
                    CanvasSet( _lightCanvas, i, 0 );
                    Set2d();
                    ShaderSet( _shaderLuminise );
                    TextureSet("texture0", _voxelTextureHd );
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
        
        CanvasSet( kCanvasInvalid );
        
        Put();
        SetCull(eCullBack);
        SetBlend(eBlendNone);
        SetWrite(eWriteRgbz);
        SetDepth(eDepthLequal);
        ShaderSet( _shaderForward );
        TextureSet("texture0", _lightTextureColour );
        TextureSet("texture1", _lightTextureDirection );
        TextureSet("texture2", _voxelTextureHd );
        TextureSet("texture3", _voxelTextureSd );
        ShaderSetVec3( "worldMin", _bounds.mMin );
        ShaderSetVec3( "worldSize", _bounds.mMax - _bounds.mMin );
        
        //SetStageMatrices( false );
        
        _voxelising = false;
        _scene->Render( *this );
        
        Pop();
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Blur
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void RadiosityRendererMono::BlurLayers(TextureHandle textureX,
                                       CanvasHandle canvasX,
                                       TextureHandle textureY,
                                       CanvasHandle canvasY,
                                       TextureHandle textureZ,
                                       CanvasHandle canvasZ,
                                       ShaderHandle shaderX,
                                       ShaderHandle shaderY,
                                       ShaderHandle shaderZ,
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
        for( u32 i = 0; i < 5; i++ )
        {
            zOffsets[ i ] *= zBlur;
        }
        Put();
        SetWrite( eWriteRgba );
        SetBlend( eBlendNone );
        SetDepth( eDepthNone );
        SetCull( eCullNone );
        
        CanvasSet( canvasX, -1, dstLod );
        Set2d();
        ShaderSet( shaderX );
        TextureSet( "texture0", textureX );
        ShaderSetFloat( "lod", f32( srcLod ) );
        ShaderSetFloatArray( "offset" , xOffsets, 5 );
        ShaderSetFloatArray( "weight" , weights, 5 );
        fw::DrawQuad2d( Quad2dShaderTexturedCustom, fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
        
        CanvasSet( canvasY, -1, dstLod );
        Set2d();
        ShaderSet( shaderY );
        TextureSet( "texture0", textureY );
        ShaderSetFloat( "lod", f32( dstLod ) );
        ShaderSetFloatArray( "offset" , yOffsets, 5 );
        ShaderSetFloatArray( "weight" , weights, 5 );
        fw::DrawQuad2d( Quad2dShaderTexturedCustom, fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
        
        CanvasSet( canvasZ, -1, dstLod );
        Set2d();
        ShaderSet( shaderZ );
        TextureSet( "texture0", textureZ );
        ShaderSetFloat( "lod", f32( dstLod ) );
        ShaderSetFloatArray( "offset" , zOffsets, 5 );
        ShaderSetFloatArray( "weight" , weights, 5 );
        fw::DrawQuad2d( Quad2dShaderTexturedCustom, fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
        
        Pop();
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Matrices
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void RadiosityRendererMono::SetStageMatrices( bool tracing )
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