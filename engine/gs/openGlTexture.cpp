#include "gsHw.h"

#if GsOpenGl3 || GsOpenGles2 || GsOpenGles3

namespace gs
{
	static TextureHw _texturesHw[ TextureLimit ];
	static GLuint _texturesHwActiveCount;
		
	const GLenum _hwTextureTargetMap[ TexTypeCount ] =
	{
		GL_TEXTURE_2D,//TexType2d,
#if GsOpenGles2
        GL_TEXTURE_2D,//TexType3d,
#else
		GL_TEXTURE_3D,//TexType3d,
#endif
		GL_TEXTURE_2D//TexTypeDepth,
	};
	
	const GLint _hwTextureInternalFormatMap[ TexFormatCount ] =
	{
#if GsOpenGles2
        GL_LUMINANCE,//TexFormatR8,
        GL_LUMINANCE,//TexFormatR16F,
        GL_LUMINANCE,//TexFormatR32F,
        GL_LUMINANCE_ALPHA,//TexFormatRG8,
        GL_LUMINANCE_ALPHA,//TexFormatRG16F,
        GL_LUMINANCE_ALPHA,//TexFormatRG32F,
        GL_RGB,//TexFormatRGB8,
        GL_RGB,//TexFormatRGB16F,
        GL_RGB,//TexFormatRGB32F,
        GL_RGBA,//TexFormatRGBA8,
        GL_RGBA,//TexFormatRGBA16F,
        GL_RGBA,//TexFormatRGBA32F,
        GL_RGBA,//TexFormatRGBA32U,
        GL_DEPTH_COMPONENT//TexFormatDepth,
#else
        GL_R8,//TexFormatR8,
        GL_R16F,//TexFormatR16F,
        GL_R32F,//TexFormatR32F,
        GL_RG8,//TexFormatRG8,
        GL_RG16F,//TexFormatRG16F,
        GL_RG32F,//TexFormatRG32F,
        GL_RGB8,//TexFormatRGB8,
        GL_RGB16F,//TexFormatRGB16F,
        GL_RGB32F,//TexFormatRGB32F,
        GL_RGBA8,//TexFormatRGBA8,
        GL_RGBA16F,//TexFormatRGBA16F,
        GL_RGBA32F,//TexFormatRGBA32F,
        GL_RGBA32UI,//TexFormatRGBA32U,
        GL_DEPTH_COMPONENT//TexFormatDepth,
#endif
	};
	
	const GLint _hwTextureFormatMap[ TexFormatCount ] =
	{
#if GsOpenGles2
        GL_LUMINANCE,//TexFormatR8,
        GL_LUMINANCE,//TexFormatR16F,
        GL_LUMINANCE,//TexFormatR32F,
        GL_LUMINANCE_ALPHA,//TexFormatRG8,
        GL_LUMINANCE_ALPHA,//TexFormatRG16F,
        GL_LUMINANCE_ALPHA,//TexFormatRG32F,
        GL_RGB,//TexFormatRGB8,
        GL_RGB,//TexFormatRGB16F,
        GL_RGB,//TexFormatRGB32F,
        GL_RGBA,//TexFormatRGBA8,
        GL_RGBA,//TexFormatRGBA16F,
        GL_RGBA,//TexFormatRGBA32F,
        GL_RGBA,//TexFormatRGBA32U,
        GL_DEPTH_COMPONENT//TexFormatDepth,
#else
        GL_RED,//TexFormatR8,
        GL_RED,//TexFormatR16F,
        GL_RED,//TexFormatR32F,
        GL_RG,//TexFormatRG8,
        GL_RG,//TexFormatRG16F,
        GL_RG,//TexFormatRG32F,
        GL_RGB,//TexFormatRGB8,
        GL_RGB,//TexFormatRGB16F,
        GL_RGB,//TexFormatRGB32F,
        GL_RGBA,//TexFormatRGBA8,
        GL_RGBA,//TexFormatRGBA16F,
        GL_RGBA,//TexFormatRGBA32F,
        GL_RGBA,//TexFormatRGBA32U,
        GL_DEPTH_COMPONENT//TexFormatDepth,
#endif
	};
	
	const GLint _hwTextureTypeMap[ TexFormatCount ] =
	{
		GL_FLOAT,//TexFormatR8,
		GL_FLOAT,//TexFormatR16F,
		GL_FLOAT,//TexFormatR32F,
		GL_FLOAT,//TexFormatRG8,
		GL_FLOAT,//TexFormatRG16F,
		GL_FLOAT,//TexFormatRG32F,
		GL_FLOAT,//TexFormatRGB8,
		GL_FLOAT,//TexFormatRGB16F,
		GL_FLOAT,//TexFormatRGB32F,
        GL_UNSIGNED_BYTE,//TexFormatRGBA8, // Was GL_FLOAT
		GL_FLOAT,//TexFormatRGBA16F,
		GL_FLOAT,//TexFormatRGBA32F,
		GL_UNSIGNED_INT,//TexFormatRGBA32U,
		GL_UNSIGNED_BYTE//TexFormatDepth,
	};
	
	void TextureHwNew( TextureHandle handle )
	{
		const Texture& texture = TextureGet( handle );
		TextureHw& textureHw = _texturesHw[ handle ];
		textureHw._target = _hwTextureTargetMap[ texture._type ];
		textureHw._internalFormat = _hwTextureInternalFormatMap[ texture._format ];
		textureHw._format = _hwTextureFormatMap[ texture._format ];
		textureHw._type = _hwTextureTypeMap[ texture._format ];
		glGenTextures( 1, &textureHw._id );
		glBindTexture( textureHw._target,textureHw._id );
        glTexParameteri( textureHw._target, GL_TEXTURE_BASE_LEVEL, 0 );
        glTexParameteri( textureHw._target, GL_TEXTURE_MAX_LEVEL, texture._lodMax );

		if( texture._sizeZ > 1 )
		{
#if GsOpenGles2
            ASSERT(false);
#else
            glTexImage3D( textureHw._target, 0, textureHw._internalFormat, texture._sizeX, texture._sizeY, texture._sizeZ, 0, textureHw._format, textureHw._type, texture._data );

            if( texture._flags & TexFlagMipMap )
            {
                if( texture._data == Null )
                {
                    s32 sizeX = texture._sizeX;
                    s32 sizeY = texture._sizeY;
                    s32 sizeZ = texture._sizeZ;
                    
                    for( s32 lod = 1; lod <= texture._lodMax; lod++ )
                    {
                        if( sizeX > 1 ) sizeX >>= 1;
                        if( sizeY > 1 ) sizeY >>= 1;
                        if( sizeZ > 1 ) sizeZ >>= 1;
                        
                        glTexImage3D( textureHw._target, lod, textureHw._internalFormat, sizeX, sizeY, sizeZ, 0, textureHw._format, textureHw._type, texture._data );
                    }
                }
                else
                {
                    glGenerateMipmap( textureHw._target );
                }
            }
#endif
		}
		else
		{
            glTexImage2D( textureHw._target, 0, textureHw._internalFormat, texture._sizeX, texture._sizeY, 0, textureHw._format, textureHw._type, texture._data );

            if( texture._flags & TexFlagMipMap )
            {
                if( texture._data == Null )
                {
                    s32 sizeX = texture._sizeX;
                    s32 sizeY = texture._sizeY;
                    
                    for( s32 lod = 1; lod <= texture._lodMax; lod++ )
                    {
                        if( sizeX > 1 ) sizeX >>= 1;
                        if( sizeY > 1 ) sizeY >>= 1;
                        
                        glTexImage2D( textureHw._target, lod, textureHw._internalFormat, sizeX, sizeY, 0, textureHw._format, textureHw._type, texture._data );
                    }
                }
                else
                {
                    glGenerateMipmap( textureHw._target );
                }
            }
		}
        
		glTexParameteri( textureHw._target, GL_TEXTURE_WRAP_S, texture._flags & TexFlagClampS ? GL_CLAMP_TO_EDGE : GL_REPEAT );
		glTexParameteri( textureHw._target, GL_TEXTURE_WRAP_T, texture._flags & TexFlagClampT ? GL_CLAMP_TO_EDGE : GL_REPEAT );
        
		if( texture._sizeZ > 1 )
		{
#if GsOpenGles2
            ASSERT(false);
#else
            glTexParameteri( textureHw._target, GL_TEXTURE_WRAP_R, texture._flags & TexFlagClampR ? GL_CLAMP_TO_EDGE : GL_REPEAT );
#endif
		}
        
		glTexParameteri( textureHw._target, GL_TEXTURE_MAG_FILTER, texture._flags & TexFlagNearest ? GL_NEAREST : GL_LINEAR);
		
        if( texture._flags & TexFlagMipMap )
		{
			glTexParameteri( textureHw._target, GL_TEXTURE_MIN_FILTER, texture._flags & TexFlagNearest ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR );
		}
		else
		{
			glTexParameteri( textureHw._target, GL_TEXTURE_MIN_FILTER, texture._flags & TexFlagNearest ? GL_NEAREST : GL_LINEAR );
		}

        glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void TextureHwDelete( TextureHandle handle )
	{
		glDeleteTextures( 1, &_texturesHw[ handle ]._id );
	}

	void TextureHwSet( const c8* name, TextureHandle handle )
	{
		gs::ShaderSetInt( name, _texturesHwActiveCount );
		glActiveTexture( GL_TEXTURE0 + _texturesHwActiveCount );
		glBindTexture( _texturesHw[ handle ]._target, _texturesHw[ handle ]._id );
		_texturesHwActiveCount++;
	}
	
	const TextureHw& TextureHwGet( TextureHandle handle )
	{
		return _texturesHw[ handle ];
	}

	void TextureHwActiveCountReset()
	{
		_texturesHwActiveCount = 0;
	}
}

#endif