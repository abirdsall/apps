#include "fw.h"

using namespace gs;

namespace fw
{
	u32 CanvasViewer::MakeShaderIndex( eChannel outR, eChannel outG, eChannel outB )
	{
		return outR + ( outG * eChannelTypes ) + ( outB * eChannelTypes * eChannelTypes );
	}
	
	ShaderHandle CanvasViewer::MakeShader( eChannel outR, eChannel outG, eChannel outB, bool is2d )
	{
#if kBuildOpenGles2
        
        core::String vShader = "attribute vec2 vertex_position;\n";
        
        vShader += "attribute vec4 vertex_colour;\n";
        vShader += "attribute vec4 vertex_tcoord;\n";
        
        vShader += "varying lowp vec2 fragment_tcoord;\n";
        
        vShader += "varying lowp vec4 fragment_colour;\n";

        vShader += "uniform mat4 viewMatrix;\n";
        vShader += "uniform mat4 projMatrix;\n";
        
        vShader += "void main()\n";
        vShader += "{\n";
        vShader += "\tgl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n";
        vShader += "\tfragment_colour = vertex_colour;\n";
        vShader += "\tfragment_tcoord = vertex_tcoord.xy;\n";
        vShader += "}\n";
        
        core::String fShader = "varying lowp vec4 fragment_colour;\n";
        
        fShader += "varying lowp vec2 fragment_tcoord;\n";
        fShader += "uniform lowp sampler2D texture0;\n";
        fShader += "void main()\n";
        fShader += "{\n";
        fShader += "\tlowp vec4 sample = texture2D(texture0, fragment_tcoord);\n";
        fShader += "\tgl_FragColor = vec4(";
        fShader = fShader + sChannelLookupNames[ outR ] + ",";
        fShader = fShader + sChannelLookupNames[ outG ] + ",";
        fShader = fShader + sChannelLookupNames[ outB ] + ",";
        fShader += "1);\n";
        fShader += "}\n";

#else
        
#if kBuildOpenGl3
        core::String vShader = "#version 150\n";
#else
        core::String vShader = "#version 300 es\n";
#endif
		vShader += "in vec2 vertex_position;\n";
        vShader += "in vec4 vertex_colour;\n";
        vShader += "in vec4 vertex_tcoord;\n";
        vShader += "out vec2 fragment_tcoord;\n";
		vShader += "uniform mat4 viewMatrix;\n";
		vShader += "uniform mat4 projMatrix;\n";
		vShader += "void main()\n";
		vShader += "{\n";
		vShader += "\tgl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n";
        vShader += "\tfragment_tcoord = vertex_tcoord.xy;\n";
		vShader += "}\n";
		
#if kBuildOpenGl3
        core::String fShader = "#version 150\n";
#else
        core::String fShader = "#version 300 es\n";
        fShader = fShader + "precision highp float;\n";
        fShader = fShader + "precision highp sampler3D;\n";
#endif
        fShader += "in vec2 fragment_tcoord;\n";
		if( is2d )
		{
			fShader += "uniform sampler2D texture0;\n";
		}
		else
		{
			fShader += "uniform sampler3D texture0;\n";
		}
		fShader += "uniform float lod;\n";
        if( !is2d )
        {
            fShader += "uniform float layer;\n";
        }
		fShader += "out vec4 output_colour;\n";
		fShader += "void main()\n";
		fShader += "{\n";
        if( is2d )
        {
            fShader += "\tvec4 sample = textureLod(texture0, fragment_tcoord, lod);\n";
        }
        else
        {
            fShader += "\tvec3 tcoord = vec3(fragment_tcoord.x, fragment_tcoord.y, layer);\n";
            fShader += "\tvec4 sample = textureLod(texture0, tcoord, lod);\n";
        }
		fShader += "\toutput_colour = vec4(";
		fShader = fShader + sChannelLookupNames[ outR ] + ",";
		fShader = fShader + sChannelLookupNames[ outG ] + ",";
		fShader = fShader + sChannelLookupNames[ outB ] + ",";
		fShader += "1);\n";
		fShader += "}\n";
#endif
		return ShaderNew( vShader.toStr(), fShader.toStr(), false );
	}
	
	CanvasViewer::CanvasViewer() : mActive( 0 ), mNavLevel( eNavLevelCanvas )
	{
		mNavIndex[ eNavLevelCanvas ] = 0;
	}
	
	void CanvasViewer::Init()
	{
		for( u32 outR = 0; outR < eChannelTypes; outR++ )
		{
			for( u32 outG = 0; outG < eChannelTypes; outG++ )
			{
				for( u32 outB = 0; outB < eChannelTypes; outB++ )
				{
					u32 shaderIndex = MakeShaderIndex( ( eChannel )outR, ( eChannel )outG, ( eChannel )outB );
					mTexture2dShaders[ shaderIndex ] = MakeShader( ( eChannel )outR, ( eChannel )outG, ( eChannel )outB, true );
					mTexture3dShaders[ shaderIndex ] = MakeShader( ( eChannel )outR, ( eChannel )outG, ( eChannel )outB, false );
				}
			}
		}
	}
	
	void CanvasViewer::Tick()
	{
		// switch canvas
		// switch lod
		// switch channel?
        
        if( mActive )
        {
            CanvasHandle canvases[ kCanvasLimit ];
            u32 canvasCount = CanvasActiveGet( canvases );

            // todo if leaf navlevel then left right adjust mipmap level
            
            
            if( os::KeyUp( os::eKeyUp ) )
            {
                if( mNavLevel > eNavLevelCanvas )
                {
                    mNavLevel = eNavLevel(s32(mNavLevel) - 1);
                }
            }
            
            if( os::KeyUp( os::eKeyDown ) )
            {
                if( mNavLevel < eNavLevelLayer )
                {
                    mNavLevel = eNavLevel(s32(mNavLevel) + 1);
                    mNavIndex[ mNavLevel ] = 0;
                }
            }
            
            if( os::KeyUp( os::eKeyLeft ) )
            {
                if( mNavIndex[ mNavLevel ] > 0 )
                {
                    mNavIndex[ mNavLevel ]--;
                }
            }
            
            if( os::KeyUp( os::eKeyRight ) )
            {
                switch( mNavLevel )
                {
                    case eNavLevelCanvas:
                    {
                        if( mNavIndex[ mNavLevel ] < ( canvasCount - 1 ) )
                        {
                            mNavIndex[ mNavLevel ]++;
                        }
                        break;
                    }
                    case eNavLevelTexture:
                    {
                        const Canvas& canvas = CanvasGet( canvases[ mNavIndex[ eNavLevelCanvas ] ] );
                        if( mNavIndex[ mNavLevel ] < ( canvas.mColorTextureCount - 1 ) )
                        {
                            mNavIndex[ mNavLevel ]++;
                        }
                        break;
                    }
                    case eNavLevelLayer:
                    {
                        const Canvas& canvas = CanvasGet( canvases[ mNavIndex[ eNavLevelCanvas ] ] );
                        const Texture& texture = TextureGet( canvas.mColorTexture[ mNavIndex[ eNavLevelTexture ] ] );
                        if( mNavIndex[ mNavLevel ] < ( texture.mSizeZ - 1 ) )
                        {
                            mNavIndex[ mNavLevel ]++;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            
            
            
            
            
            
            if( os::KeyUp( os::eKeyUp ) )
            {
                printf("up UP\n");
            }
            if( os::KeyUp( os::eKeyDown ) )
            {
                printf("dn UP\n");
            }
            if( os::KeyUp( os::eKeyLeft ) )
            {
                printf("lf UP\n");
            }
            if( os::KeyUp( os::eKeyRight ) )
            {
                printf("rt UP\n");
            }
            
            if( os::KeyUp( os::eKeyA ) )
            {
                printf("a UP\n");
            }
        }
	}

	void CanvasViewer::Draw()
	{
		if( mActive )
		{
			CanvasHandle canvases[ kCanvasLimit ];
			u32 canvasCount = CanvasActiveGet( canvases );

			switch( mNavLevel )
			{
				case eNavLevelCanvas: // draw first lod of first layer or first texture for all canvases
				{					
					Rect canvasRects[ kCanvasLimit ];
					Rect(0.0f, 0.0f, ( f32 )CanvasSizeX(), ( f32 )CanvasSizeY() ).Subdivide( canvasRects, canvasCount );

					for( int i = 0; i < canvasCount; i++ )
					{
						DrawTexture( canvasRects[ i ], CanvasGet( canvases[ i ] ), 0, 0, 0, i == mNavIndex[ eNavLevelCanvas ] );
					}
					DrawHighlight( canvasRects[ mNavIndex[ eNavLevelCanvas ] ] );
					break;
				}
				case eNavLevelTexture: // draw first lod of first layer for all textures in current canvas
				{
					Rect textureRects[ kColorTextureLimit ];
					const Canvas& canvas = CanvasGet( canvases[ mNavIndex[ eNavLevelCanvas ] ] );
					Rect(0.0f, 0.0f, ( f32 )CanvasSizeX(), ( f32 )CanvasSizeY() ).Subdivide( textureRects, canvas.mColorTextureCount );

					for( int i = 0; i < canvas.mColorTextureCount; i++ )
					{
						DrawTexture( textureRects[ i ], canvas, i, 0, 0, i == mNavIndex[ eNavLevelTexture ] );
					}
					DrawHighlight( textureRects[ mNavIndex[ eNavLevelTexture ] ] );
					break;
				}
				case eNavLevelLayer: // draw first lod for all layers in current textures in current canvas
				{
					const u32 kTextureLayerLimit = 32;
					Rect layerRects[ kTextureLayerLimit ];
					const Canvas& canvas = CanvasGet( canvases[ mNavIndex[ eNavLevelCanvas ] ] );
					const Texture& texture = TextureGet( canvas.mColorTexture[ mNavIndex[ eNavLevelTexture ] ] );
					Rect(0.0f, 0.0f, ( f32 )CanvasSizeX(), ( f32 )CanvasSizeY() ).Subdivide( layerRects, texture.mSizeZ );

					for( int i = 0; i < texture.mSizeZ; i++ )
					{						
						DrawTexture( layerRects[ i ], canvas, mNavIndex[ eNavLevelTexture ], i, mNavIndex[ eNavLevelLayer ], true );
					}
					DrawHighlight( layerRects[ mNavIndex[ eNavLevelLayer ] ] );
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

	void CanvasViewer::DrawHighlight( const Rect& rect )
	{
		Put();
		SetWrite(eWriteRgb);
		SetBlend(eBlendRgb);
		SetDepth(eDepthNone);
		SetCull(eCullNone);
		Set2d();
		FillWireRect( rect, v4( 1.0, 0.0, 1.0, 1.0 ) );
		Pop();
	}
		
	void CanvasViewer::DrawTexture( const Rect& rect, const Canvas& canvas, const s32 textureIndex, const s32 layerIndex, const s32 lodIndex, const bool highlighted )
	{
		ASSERT( textureIndex >= 0 && textureIndex < canvas.mColorTextureCount );
		TextureHandle textureHandle = canvas.mColorTexture[ textureIndex ];
		const Texture& texture = TextureGet( textureHandle );
		u32 shaderIndex = MakeShaderIndex( eChannelR, eChannelG, eChannelB );
		
		Put();
		SetWrite(eWriteRgb);
		SetBlend(eBlendRgb);
		//SetBlend(highlighted?eBlendRgb:eBlendAddRgb);
		SetDepth(eDepthNone);
		SetCull(eCullNone);
		Set2d();
		if( texture.mSizeZ == 1 )
		{
			ShaderHandle shaderHandle = mTexture2dShaders[ shaderIndex ];
			ShaderSet( shaderHandle );
			TextureSet( "texture0", textureHandle );
#if !kBuildOpenGles2
			ShaderSetFloat( "lod", f32( lodIndex ) );///4.0f );
#endif
			DrawRect( rect, Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
		}
		else
		{
			ShaderHandle shaderHandle = mTexture3dShaders[ shaderIndex ];
			ShaderSet( shaderHandle );
			TextureSet( "texture0", textureHandle );
            f32 zStep = 1.0f / f32( texture.mSizeZ );
            f32 zMin = zStep / 2.0f;
			ShaderSetFloat( "lod", f32( lodIndex ) );
            ShaderSetFloat( "layer", zMin + zStep * f32( layerIndex ) );
			DrawRect( rect, Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
		}
		Pop();

        Put();
        Set2d();
        SetWrite(eWriteRgb);
        SetBlend(eBlendRgba);
        SystemFontDraw(core::String("hello HELLO"), rect.Min(), v4(1,0,0,1));
        Pop();
	}

	void CanvasViewer::Kill()
	{
	}
	
	bool CanvasViewer::Active()
	{
		return mActive;
	}
	
	void CanvasViewer::SetActive( bool active )
	{
		mActive = active;
	}
}
