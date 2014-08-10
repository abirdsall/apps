#include "fw.h"

namespace fw
{
    using namespace gs;
    
	static const s32 kVoxelCountX = 512;
	static const s32 kVoxelCountY = 512;
    static const s32 kVoxelCountZ = 8 ;

	static Light sLight[ kLightLimit ];
	static u32 sLightActiveCount;
	
	static aabb sBounds;
	
	static CanvasHandle sVoxelCanvasHd;
	static CanvasHandle sVoxelCanvasHdScratch;
	static CanvasHandle sVoxelCanvasSd;
	static CanvasHandle sVoxelCanvasSdScratch;
    static CanvasHandle sVoxelCanvasSdLod1;
    static CanvasHandle sVoxelCanvasSdLod1Scratch;
    static CanvasHandle sVoxelCanvasSdLod2;
    static CanvasHandle sVoxelCanvasSdLod2Scratch;
	static CanvasHandle sLightCanvas;
	
	static TextureHandle sVoxelTextureHd;
	static TextureHandle sVoxelTextureHdScratch;
	static TextureHandle sVoxelTextureSd;
	static TextureHandle sVoxelTextureSdScratch;
	static TextureHandle sLightTextureColour;
	static TextureHandle sLightTextureDirection;
    
    //static ShaderHandle sFillShader;

	static ShaderHandle sShaderBlurX[ kVoxelCountZ ];
	static ShaderHandle sShaderBlurY[ kVoxelCountZ ];
	static ShaderHandle sShaderBlurZ[ kVoxelCountZ ];
	static ShaderHandle sShaderVoxelise;
	static ShaderHandle sShaderLuminise;
	static ShaderHandle sShaderForward;

	const aabb& LightGetBounds()
	{
		return sBounds;
	}
	
	LightHandle LightNew( const v3& position, const v3& colour )
	{
		for( u32 i = 0; i < kLightLimit; i++ )
		{
			if( !sLight[ i ].mActive )
			{
				sLight[ i ].mPosition = position;
				sLight[ i ].mColour = colour;
				sLight[ i ].mActive = true;
				sLightActiveCount++;
				return i;
			}
		}
		ASSERT(0);
		return kLightInvalid;
	}
	
	void LightDelete( const LightHandle handle )
	{
		if( sLight[ handle ].mActive )
		{
			sLight[ handle ].mActive = false;
			sLightActiveCount--;
		}
	}
	
	void LightSetPosition( LightHandle handle, const v3& position )
	{
		sLight[ handle ].mPosition = position;
	}
	
	const v3& LightGetPosition( LightHandle handle )
	{
		return sLight[ handle ].mPosition;
	}
	
	const v3& LightGetColour( LightHandle handle )
	{
		return sLight[ handle ].mColour;
	}
	
	s32 LightActiveCount()
	{
		return sLightActiveCount;
	}
	
	s32 LightActiveGet( LightHandle array[ kLightLimit ] )
	{
		s32 count = 0;
		for( int i = 0; i < kLightLimit; i++ )
		{
			if( sLight[ i ].mActive )
			{
				array[ count ] = i;
				count++;
			}
		}
		return count;
	}

	static void BlurLayers(TextureHandle textureX, CanvasHandle canvasX,
						   TextureHandle textureY, CanvasHandle canvasY,
						   TextureHandle textureZ, CanvasHandle canvasZ, s32 srcLod, s32 dstLod, u32 dstSizeX, u32 dstSizeY, u32 dstSizeZ, f32 zBlur )
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
        ShaderSet( sShaderBlurX[ dstSizeZ ] );
        TextureSet( "texture0", textureX );
        ShaderSetFloat( "lod", f32( srcLod ) );
        ShaderSetFloatArray( "offset" , xOffsets, 5 );
        ShaderSetFloatArray( "weight" , weights, 5 );
        fw::DrawRect( fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ), 0.0f );
        
        CanvasSet( canvasY, -1, dstLod );
        Set2d();
        ShaderSet( sShaderBlurY[ dstSizeZ ] );
        TextureSet( "texture0", textureY );
        ShaderSetFloat( "lod", f32( dstLod ) );
        ShaderSetFloatArray( "offset" , yOffsets, 5 );
        ShaderSetFloatArray( "weight" , weights, 5 );
        fw::DrawRect( fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ), 0.0f );
        
        CanvasSet( canvasZ, -1, dstLod );
        Set2d();
        ShaderSet( sShaderBlurZ[ dstSizeZ ] );
        TextureSet( "texture0", textureZ );
        ShaderSetFloat( "lod", f32( dstLod ) );
        ShaderSetFloatArray( "offset" , zOffsets, 5 );
        ShaderSetFloatArray( "weight" , weights, 5 );
        fw::DrawRect( fw::Rect( 0, 0, f32( dstSizeX ), f32( dstSizeY ) ), fw::Rect( 0.0f, 1.0f, 1.0f, 0.0f ), 0.0f );

        Pop();
	}
	
	void DrawLights( DrawPhase phase )
	{
		switch( phase )
		{
			case DrawPhasePreVoxelPass:
			{
                // Clear 
				Put();
                
				CanvasSet( sVoxelCanvasHdScratch );
				
                Set2d();
				SetWrite( eWriteRgba );
				SetBlend( eBlendNone );
				fw::Fill4Rect( fw::Rect( 0.0f, 0.0f, f32( kVoxelCountX ), f32( kVoxelCountY ) ), v4( 1.0f, 1.0f, 1.0f, 0.0f ) );
                
                Pop();
                
                
				f32 zStep = ( sBounds.mMax.z - sBounds.mMin.z ) / f32( kVoxelCountZ );
				f32 zMin = sBounds.mMin.z + ( zStep / 2.0f );
                
                
				Put();
				
                CanvasSet( sVoxelCanvasHdScratch );
				
                SetDepth( eDepthNone );
				
                ShaderSet( sShaderVoxelise );
				ShaderSetFloat( "zMin", zMin );
				ShaderSetFloat( "zStep", zStep );
				
                SetWrite( eWriteRgba );
				SetBlend( eBlendMixRgbAddA );
				SetCull( eCullBack );
				
                break;
			}
                
			case DrawPhaseMake:
			{
				Pop();
                
				BlurLayers(
                           sVoxelTextureHdScratch, //textureX
                           
                           sVoxelCanvasHd,//canvasX
                           
                           sVoxelTextureHd,//textureY
                           
                           sVoxelCanvasHdScratch,//canvasY
                        
                           sVoxelTextureHdScratch,//textureZ
                           
                           sVoxelCanvasHd,//canvasZ
                           
                           0,//srcLod
                           
                           0,//dstLod
                           
                           kVoxelCountX,//dstSizeX
                           
                           kVoxelCountY,//dstSizeY
                           
                           kVoxelCountZ,//dstSizeZ
                           
                           0.5f//zBlur
                           
                           );
				BlurLayers(
                           sVoxelTextureHd, //textureX
                           
                           sVoxelCanvasSd, //canvasX
                           
                           sVoxelTextureSd, //textureY
                           
                           sVoxelCanvasSdScratch, //canvasY
							
                           sVoxelTextureSdScratch, //textureZ
                           
                           sVoxelCanvasSd, //canvasZ
                           
                           0,
                           
                           0,
                           
                           kVoxelCountX / 2, //dstSizeX
                           
                           kVoxelCountY / 2, //dstSizeY
                           
                           kVoxelCountZ, //dstSizeZ
                           
                           1.0f//zBlur
                           
                           );
                    
				BlurLayers(
                           sVoxelTextureSd,
                           
                           sVoxelCanvasSdLod1Scratch,
                           
                           sVoxelTextureSdScratch,
                           
                           sVoxelCanvasSdLod1,
                        
                           sVoxelTextureSd,
                           
                           sVoxelCanvasSdLod1Scratch,
                           
                           0,
                           
                           1,
                           
                           kVoxelCountX / 4,
                           
                           kVoxelCountY / 4,
                           
                           kVoxelCountZ / 2,
                           
                           1.0f
                           
                           );
                    
				BlurLayers(
                           sVoxelTextureSdScratch,
                           
                           sVoxelCanvasSdLod2,
                           
                           sVoxelTextureSd,
                           
                           sVoxelCanvasSdLod2Scratch,
						   
                           sVoxelTextureSdScratch,
                           
                           sVoxelCanvasSdLod2,
                           
                           1,
                           
                           2,
                           
                           kVoxelCountX / 8,
                           
                           kVoxelCountY / 8,
                           
                           kVoxelCountZ / 4,
                           
                           1.0f
                           
                           );
				Put();
				SetWrite( eWriteRgb );
				SetBlend( eBlendNone );
				SetDepth( eDepthNone );
				SetCull( eCullNone );
				f32 zStep = ( sBounds.mMax.z - sBounds.mMin.z ) / f32( kVoxelCountZ / 2 );
				f32 zMin = sBounds.mMin.z + ( zStep / 2.0f );
				for( int l = 0; l < kLightLimit; l++ )
				{
					Light light = sLight[ l ];
					if( light.mActive )
					{
						for( int i = 0; i < ( kVoxelCountZ / 2 ); i++ )
						{
							CanvasSet( sLightCanvas, i, 0 );
							Set2d();
							ShaderSet( sShaderLuminise );
							TextureSet("texture0", sVoxelTextureHd );
							ShaderSetVec3( "lightPos", light.mPosition );
							ShaderSetVec3( "lightCol", light.mColour );
							ShaderSetVec3( "worldMin", sBounds.mMin );
							ShaderSetVec3( "worldSize", sBounds.mMax - sBounds.mMin );
							ShaderSetFloat( "zWorld", zMin + zStep * f32( i ) );
							fw::DrawRect( fw::Rect( 0, 0, f32( kVoxelCountX / 2 ), f32( kVoxelCountY / 2 ) ), fw::Rect(0.0f, 1.0f, 1.0f, 0.0f) );
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
				ShaderSet( sShaderForward );
				TextureSet("texture0", sLightTextureColour );
				TextureSet("texture1", sLightTextureDirection );
				TextureSet("texture2", sVoxelTextureHd );
				TextureSet("texture3", sVoxelTextureSd );
				ShaderSetVec3( "worldMin", sBounds.mMin );
				ShaderSetVec3( "worldSize", sBounds.mMax - sBounds.mMin );
				break;
			}
			case DrawPhaseFinal:
			{
				Pop();
				break;
			}
		}
	}
	
	static ShaderHandle MakeBlurShader( s32 axis, s32 layerCount )
	{
		const c8* lookup = axis == 0 ? "offset[ j ], 0.0, 0.0": axis == 1 ? "0.0, offset[ j ], 0.0" : "0.0, 0.0, offset[ j ]";
#if kBuildOpenGl3
		fw::String vShader = "#version 410\n";
#else
        fw::String vShader = "#version 300 es\n";
#endif
		vShader = vShader + "in vec2 vertex_position;\n\
        in vec4 vertex_colour;\n\
		in vec4 vertex_tcoord;\n\
		out vec3 fragment_tcoord;\n\
		uniform mat4 viewMatrix;\n\
		uniform mat4 projMatrix;\n\
		void main()\n\
		{\n\
			gl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n\
			fragment_tcoord = vertex_tcoord.xyz;\n\
		}";
#if kBuildOpenGl3
        fw::String fShader = "#version 410\n";
#else
        fw::String fShader = "#version 300 es\n";
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
        in vec3 fragment_tcoord;\n\
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
	
	static void InitShaders()
	{
        for( s32 i = 0; i < kVoxelCountZ; i++ )
        {
            sShaderBlurX[ i + 1 ] = MakeBlurShader( 0, i + 1 );
            sShaderBlurY[ i + 1 ] = MakeBlurShader( 1, i + 1 );
            sShaderBlurZ[ i + 1 ] = MakeBlurShader( 2, i + 1 );
        }
		
		// fragment_tcoord.x = object zmiddle
		// fragment_tcoord.y = object zthickness
#if kBuildOpenGl3
        fw::String vShader = "#version 410 core\n";
#else
        fw::String vShader = "#version 300 es\n";
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
        
#if kBuildOpenGl3
        fw::String fShader = "#version 410 core\n";
#else
        fw::String fShader = "#version 300 es\n";
        fShader = fShader + "precision highp float;\n";
#endif
        fShader = fShader + "uniform float zMin;\n\
        uniform float zStep;\n\
        in vec4 fragment_colour;\n\
        in vec2 fragment_tcoord;\n";
        
        for(s32 i = 0; i < kVoxelCountZ; i++)
        {
            fShader = fShader + "layout(location = " + i + ") out vec4 fs_" + i + ";\n";
        }
        
        fShader = fShader + "void main()\n\
        {\n\
        float zBase = zMin;\n";
        for(s32 i = 0; i < kVoxelCountZ; i++)
        {
            fShader = fShader + "fs_" + i + "= vec4( fragment_colour.x, fragment_colour.y, fragment_colour.z, clamp( ( fragment_tcoord.y - abs( zBase - fragment_tcoord.x ) ) / zStep, 0.0, 1.0 ) );\n\
            zBase += zStep;\n";
        }
        fShader = fShader + "}";

		sShaderVoxelise = ShaderNew( vShader.toStr(), fShader.toStr() );
		
#if kBuildOpenGl3
        vShader = "#version 150\n";
#else
        vShader = "#version 300 es\n";
#endif
        vShader = vShader + "in vec2 vertex_position;\n\
        in vec4 vertex_colour;\n\
		in vec4 vertex_tcoord;\n\
		out vec2 fragment_tcoord;\n\
		uniform mat4 viewMatrix;\n\
		uniform mat4 projMatrix;\n\
		void main()\n\
		{\n\
		gl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);\n\
		fragment_tcoord = vertex_tcoord.xy;\n\
		}";
#if kBuildOpenGl3
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
#if kBuildOpenGl3
        fShader = fShader + "\
        output_colour[0].rgb = lightCol * attenuate;\n\
        output_colour[1].rgb = lightDir;\n}";
#else
        fShader = fShader + "\
        fs_0.rgb = lightCol * attenuate;\n\
        fs_1.rgb = lightDir;\n}";
#endif
        
		// need to kill light dir if lightCol zero
		sShaderLuminise = ShaderNew( vShader.toStr(), fShader.toStr() );
#if kBuildOpenGl3
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
#if kBuildOpenGl3
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
		sShaderForward = ShaderNew( vShader.toStr(), fShader.toStr() );
	}

	void InitLights( const aabb& bounds )
	{
		sBounds = bounds;
        
		for( u32 i = 0; i < kLightLimit; i++ )
		{
			sLight[ i ].mActive = false;
		}
		
        sLightActiveCount = 0;
        
        
        
#if 1
        sVoxelTextureSdScratch = TextureNew( TexType3d, TexFormatRGBA8, kVoxelCountX / 2, kVoxelCountY / 2, kVoxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), kNull );
        
        sVoxelCanvasSdScratch = CanvasNew();
        sVoxelCanvasSdLod1Scratch = CanvasNew();
        sVoxelCanvasSdLod2Scratch = CanvasNew();
        
        for(s32 i = 0; i < kVoxelCountZ; i++)
        {
            CanvasAdd( sVoxelCanvasSdScratch, sVoxelTextureSdScratch, i );
        }

        for(s32 i = 0; i < kVoxelCountZ / 2; i++)
        {
            CanvasAdd( sVoxelCanvasSdLod1Scratch, sVoxelTextureSdScratch, i );
        }

        for(s32 i = 0; i < kVoxelCountZ / 4; i++)
        {
            CanvasAdd( sVoxelCanvasSdLod2Scratch, sVoxelTextureSdScratch, i );
        }

        sVoxelTextureSd = TextureNew( TexType3d, TexFormatRGBA8, kVoxelCountX / 2, kVoxelCountY / 2, kVoxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), kNull );
        
        sVoxelCanvasSd = CanvasNew();
        sVoxelCanvasSdLod1 = CanvasNew();
        sVoxelCanvasSdLod2 = CanvasNew();
        
        for(s32 i = 0; i < kVoxelCountZ; i++)
        {
            CanvasAdd( sVoxelCanvasSd, sVoxelTextureSd, i );
        }

        for(s32 i = 0; i < kVoxelCountZ / 2; i++)
        {
            CanvasAdd( sVoxelCanvasSdLod1, sVoxelTextureSd, i );
        }

        for(s32 i = 0; i < kVoxelCountZ / 4; i++)
        {
            CanvasAdd( sVoxelCanvasSdLod2, sVoxelTextureSd, i );
        }

#else
		sVoxelTextureSdScratch = TextureNew( TexType3d, TexFormatRGBA8, kVoxelCountX / 2, kVoxelCountY / 2, kVoxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), kNull );
		
        sVoxelCanvasSdScratch = CanvasNew( sVoxelTextureSdScratch );
		
        
        sVoxelTextureSd = TextureNew( TexType3d, TexFormatRGBA8, kVoxelCountX / 2, kVoxelCountY / 2, kVoxelCountZ, TexFlags( TexFlagClamp | TexFlagMipMap ), kNull );
		
        sVoxelCanvasSd = CanvasNew( sVoxelTextureSd );
#endif
        
        
        sLightTextureColour = TextureNew( TexType3d, TexFormatRGB16F, kVoxelCountX / 2, kVoxelCountY / 2, kVoxelCountZ / 2, TexFlagClamp );
		
        sLightTextureDirection = TextureNew( TexType3d, TexFormatRGB16F, kVoxelCountX / 2, kVoxelCountY / 2, kVoxelCountZ / 2, TexFlagClamp );
		
        sLightCanvas = CanvasNew();
		
        CanvasAdd( sLightCanvas, sLightTextureColour );
		
        CanvasAdd( sLightCanvas, sLightTextureDirection );
        
        
		
        sVoxelTextureHd = TextureNew( TexType3d, TexFormatRGBA8, kVoxelCountX, kVoxelCountY, kVoxelCountZ, TexFlagClamp, kNull );
		
        sVoxelCanvasHd = CanvasNew();
		
        for(s32 i = 0; i < kVoxelCountZ; i++)
        {
            CanvasAdd( sVoxelCanvasHd, sVoxelTextureHd, i );
        }

        
        sVoxelTextureHdScratch = TextureNew( TexType3d, TexFormatRGBA8, kVoxelCountX, kVoxelCountY, kVoxelCountZ, TexFlagClamp, kNull );
		
        sVoxelCanvasHdScratch = CanvasNew();
        
        for(s32 i = 0; i < kVoxelCountZ; i++)
        {
            CanvasAdd( sVoxelCanvasHdScratch, sVoxelTextureHdScratch, i );
        }
		
        InitShaders();
        
        
        /*
#if kBuildOpenGles3
        fw::String vShader = "#version 300 es\n";
#else //kBuildOpenGl3
        fw::String vShader = "#version 150\n";
#endif
        vShader = vShader + "in vec3 vertex_position;\n";
        vShader += "in vec4 vertex_colour;\n";
        vShader += "in vec4 vertex_tcoord;\n";
        vShader += "out vec4 fragment_colour;\n";
        vShader += "uniform mat4 viewMatrix;\n";
        vShader += "uniform mat4 projMatrix;\n";
        vShader += "void main()\n";
        vShader += "{\n";
        vShader = vShader + "\tgl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1);\n";
        vShader += "\tfragment_colour = vertex_colour;\n";
        vShader += "}\n";
        
#if kBuildOpenGles3
        fw::String fShader = "#version 300 es\n";
        fShader += "precision highp float;\n";
#else //kBuildOpenGl3
        fw::String fShader = "#version 150\n";
#endif
        fShader += "in vec4 fragment_colour;\n";
        fShader += "out vec4 output_colour;\n";
        fShader += "void main()\n";
        fShader += "{\n";
        fShader += "\toutput_colour = fragment_colour;\n";
        fShader += "}\n";
        
        sFillShader = gs::ShaderNew( vShader.toStr(), fShader.toStr() );
         */
	}
	
	void KillLights()
	{
		for( u32 i = 0; i < kLightLimit;  i++ )
		{
			LightDelete( i );
		}
	}
}
