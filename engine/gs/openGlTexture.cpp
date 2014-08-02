#include "gsHw.h"

#if kBuildOpenGl3

namespace gs
{
	static TextureHw sTextureHw[ kTextureLimit ];
	static GLuint sTextureHwActiveCount;
		
	const GLenum sHwTextureTarget[ TexTypeCount ] =
	{
		GL_TEXTURE_2D,//TexType2d,
		GL_TEXTURE_3D,//TexType3d,
		GL_TEXTURE_2D//TexTypeDepth,
	};
	
	const GLint sHwTextureInternalFormat[ TexFormatCount ] =
	{
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
	};
	
	const GLint sHwTextureFormat[ TexFormatCount ] =
	{
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
	};
	
	const GLint sHwTextureType[ TexFormatCount ] =
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
		GL_FLOAT,//TexFormatRGBA8,
		GL_FLOAT,//TexFormatRGBA16F,
		GL_FLOAT,//TexFormatRGBA32F,
		GL_UNSIGNED_INT,//TexFormatRGBA32U,
		GL_UNSIGNED_BYTE//TexFormatDepth,
	};
	
	void TextureHwNew( const TextureHandle handle )
	{
		const Texture& texture = TextureGet( handle );
		TextureHw& textureHw = sTextureHw[ handle ];
		textureHw.mTarget = sHwTextureTarget[ texture.mType ];
		textureHw.mInternalFormat = sHwTextureInternalFormat[ texture.mFormat ];
		textureHw.mFormat = sHwTextureFormat[ texture.mFormat ];
		textureHw.mType = sHwTextureType[ texture.mFormat ];
		glGenTextures( 1, &textureHw.mTexture );
		glBindTexture( textureHw.mTarget,textureHw.mTexture );
		if( texture.mSizeZ > 1 )
		{
			glTexImage3D( textureHw.mTarget, 0, textureHw.mInternalFormat, texture.mSizeX, texture.mSizeY, texture.mSizeZ, 0, textureHw.mFormat, textureHw.mType, texture.mData );
		}
		else
		{
			glTexImage2D( textureHw.mTarget, 0, textureHw.mInternalFormat, texture.mSizeX, texture.mSizeY, 0, textureHw.mFormat, textureHw.mType, texture.mData );
		}
		glTexParameteri( textureHw.mTarget, GL_TEXTURE_WRAP_S, texture.mFlags & TexFlagClampS ? GL_CLAMP_TO_EDGE : GL_REPEAT );
		glTexParameteri( textureHw.mTarget, GL_TEXTURE_WRAP_T, texture.mFlags & TexFlagClampT ? GL_CLAMP_TO_EDGE : GL_REPEAT );
		if( texture.mSizeZ > 1 )
		{
			glTexParameteri( textureHw.mTarget, GL_TEXTURE_WRAP_R, texture.mFlags & TexFlagClampR ? GL_CLAMP_TO_EDGE : GL_REPEAT );
		}
		glTexParameteri( textureHw.mTarget, GL_TEXTURE_MAG_FILTER, texture.mFlags & TexFlagNearest ? GL_NEAREST : GL_LINEAR);
		if( texture.mFlags & TexFlagMipMap )
		{
			glTexParameteri( textureHw.mTarget, GL_TEXTURE_MIN_FILTER, texture.mFlags & TexFlagNearest ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR );
			glGenerateMipmap( textureHw.mTarget );
		}
		else
		{
			glTexParameteri( textureHw.mTarget, GL_TEXTURE_MIN_FILTER, texture.mFlags & TexFlagNearest ? GL_NEAREST : GL_LINEAR );
		}
	}
	
	void TextureHwDelete( const TextureHandle handle )
	{
		glDeleteTextures( 1, &sTextureHw[ handle ].mTexture );
	}

	void TextureHwSet( const c8* name, const TextureHandle handle )
	{
		gs::ShaderSetInt( name, sTextureHwActiveCount );
		glActiveTexture( GL_TEXTURE0 + sTextureHwActiveCount );
		glBindTexture( sTextureHw[ handle ].mTarget, sTextureHw[ handle ].mTexture );
		sTextureHwActiveCount++;
	}
	
	const TextureHw& TextureHwGet( const TextureHandle handle )
	{
		return sTextureHw[ handle ];
	}

	void TextureHwActiveCountReset()
	{
		sTextureHwActiveCount = 0;
	}
}

#endif