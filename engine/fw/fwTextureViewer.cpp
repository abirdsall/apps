#include "fw.h"

using namespace gs;

namespace fw
{
	u32 TextureViewer::MakeShaderIndex( Channel outR, Channel outG, Channel outB )
	{
		return outR + ( outG * ChannelTypes ) + ( outB * ChannelTypes * ChannelTypes );
	}
	
	ShaderHandle TextureViewer::MakeShader( Channel outR, Channel outG, Channel outB, bool is2d )
	{
#if GsOpenGles2
        
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
        
#if GsOpenGl3
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
		
#if GsOpenGl3
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
	
	TextureViewer::TextureViewer() : _active( 0 ), _navLevel( NavLevelTexture )
	{
		_navIndex[ NavLevelTexture ] = 0;
	}
	
	void TextureViewer::Init()
	{
		for( u32 outR = 0; outR < ChannelTypes; outR++ )
		{
			for( u32 outG = 0; outG < ChannelTypes; outG++ )
			{
				for( u32 outB = 0; outB < ChannelTypes; outB++ )
				{
					u32 shaderIndex = MakeShaderIndex( ( Channel )outR, ( Channel )outG, ( Channel )outB );
					_texture2dShaders[ shaderIndex ] = MakeShader( ( Channel )outR, ( Channel )outG, ( Channel )outB, true );
					_texture3dShaders[ shaderIndex ] = MakeShader( ( Channel )outR, ( Channel )outG, ( Channel )outB, false );
				}
			}
		}
	}
	
	void TextureViewer::Tick()
	{
		// switch canvas
		// switch lod
		// switch channel?
        
        if( _active )
        {
            TextureHandle textures[ TextureLimit ];
            s32 textureCount = TextureActiveGet( textures );

            // todo if leaf navlevel then left right adjust mipmap level
            
            
            if( os::KeyboardUp( os::KeyUp ) )
            {
                if( _navLevel > NavLevelTexture )
                {
                    _navLevel = NavLevel(s32(_navLevel) - 1);
                }
            }
            
            if( os::KeyboardUp( os::KeyDown ) )
            {
                if( _navLevel < NavLevelLayer )
                {
                    _navLevel = NavLevel(s32(_navLevel) + 1);
                    _navIndex[ _navLevel ] = 0;
                }
            }
            
            if( os::KeyboardUp( os::KeyLeft ) )
            {
                if( _navIndex[ _navLevel ] > 0 )
                {
                    _navIndex[ _navLevel ]--;
                }
            }
            
            if( os::KeyboardUp( os::KeyRight ) )
            {
                switch( _navLevel )
                {
                    case NavLevelTexture:
                    {
                        if( _navIndex[ _navLevel ] < ( textureCount - 1 ) )
                        {
                            _navIndex[ _navLevel ]++;
                        }
                        
                        break;
                    }
                    case NavLevelLayer:
                    {
                        const Texture& texture = TextureGet( textures[ _navIndex[ NavLevelTexture ] ] );

                        if( _navIndex[ _navLevel ] < ( texture._sizeZ - 1 ) )
                        {
                            _navIndex[ _navLevel ]++;
                        }
                        
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
	}

	void TextureViewer::Draw()
	{
		if( _active )
		{
            TextureHandle textures[ TextureLimit ];
            s32 textureCount = TextureActiveGet( textures );
            
			switch( _navLevel )
			{
				case NavLevelTexture: // draw first lod of first layer for all textures in current canvas
				{
					Rect textureRects[ TextureLimit ];
                    
					Rect(0.0f, 0.0f, ( f32 )CanvasSizeX(), ( f32 )CanvasSizeY() ).Subdivide( textureRects, textureCount );

					for( s32 i = 0; i < textureCount; i++ )
					{
						DrawTexture( textureRects[ i ], textures[ i ], 0, 0, i == _navIndex[ NavLevelTexture ] );
					}
                    
					DrawHighlight( textureRects[ _navIndex[ NavLevelTexture ] ] );
                    
					break;
				}
				case NavLevelLayer: // draw first lod for all layers in current textures in current canvas
				{
					const u32 TextureLayerLimit = 32;
                    
					Rect layerRects[ TextureLayerLimit ];
                    
                    TextureHandle textureHandle = textures[ _navIndex[ NavLevelTexture ] ];
                    
                    const Texture& texture = TextureGet( textureHandle );
					
                    Rect(0.0f, 0.0f, ( f32 )CanvasSizeX(), ( f32 )CanvasSizeY() ).Subdivide( layerRects, texture._sizeZ );

					for( s32 i = 0; i < texture._sizeZ; i++ )
					{						
						DrawTexture( layerRects[ i ], textureHandle, i, _navIndex[ NavLevelLayer ], true );
					}
					
                    DrawHighlight( layerRects[ _navIndex[ NavLevelLayer ] ] );
                    
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

	void TextureViewer::DrawHighlight( const Rect& rect )
	{
		Put();
		SetWrite(WriteMaskRgb);
		SetBlend(BlendModeRgb);
		SetDepth(DepthTestNone);
		SetCull(CullFaceNone);
		Set2d();
		DrawQuad2d( Quad2dShaderFilled, rect, v4( 1.0, 0.0, 1.0, 1.0 ), true );
		Pop();
	}
		
	void TextureViewer::DrawTexture( const Rect& rect, TextureHandle textureHandle, s32 layerIndex, s32 lodIndex, bool highlighted )
	{
		Texture& texture = TextureGet( textureHandle );
		u32 shaderIndex = MakeShaderIndex( ChannelR, ChannelG, ChannelB );
		
		Put();
		SetWrite(WriteMaskRgb);
		SetBlend(BlendModeRgb);
		//SetBlend(highlighted?BlendModeRgb:BlendModeAddRgb);
		SetDepth(DepthTestNone);
		SetCull(CullFaceNone);
		Set2d();
		if( texture._sizeZ == 1 )
		{
			ShaderHandle shaderHandle = _texture2dShaders[ shaderIndex ];
			ShaderSet( shaderHandle );
			TextureSet( "texture0", textureHandle );
#if !GsOpenGles2
			ShaderSetFloat( "lod", f32( lodIndex ) );///4.0f );
#endif
			DrawQuad2d( Quad2dShaderTexturedCustom, rect, Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
		}
		else
		{
			ShaderHandle shaderHandle = _texture3dShaders[ shaderIndex ];
			ShaderSet( shaderHandle );
			TextureSet( "texture0", textureHandle );
            f32 zStep = 1.0f / f32( texture._sizeZ );
            f32 zMin = zStep / 2.0f;
			ShaderSetFloat( "lod", f32( lodIndex ) );
            ShaderSetFloat( "layer", zMin + zStep * f32( layerIndex ) );
			DrawQuad2d( Quad2dShaderTexturedCustom, rect, Rect( 0.0f, 1.0f, 1.0f, 0.0f ) );
		}
		Pop();

        Put();
        Set2d();
        SetWrite(WriteMaskRgb);
        SetBlend(BlendModeRgba);
        core::String& textureName = texture._name;
        
        SystemFontDraw(textureName, rect.Min(), v4(1,0,0,1));
        Pop();
	}

	void TextureViewer::Kill()
	{
	}
	
	bool TextureViewer::Active()
	{
		return _active;
	}
	
	void TextureViewer::SetActive( bool active )
	{
		_active = active;
	}
}
