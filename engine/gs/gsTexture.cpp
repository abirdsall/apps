#include "gsHw.h"

namespace gs
{
	static Texture sTexture[ kTextureLimit ];
	
	void InitTextures()
	{
		for( u32 i = 0; i < kTextureLimit; i++ )
		{
			sTexture[ i ].mActive = false;
		}
	}
	
	void KillTextures()
	{
		for( u32 i = 0; i < kTextureLimit;  i++ )
		{
			TextureDelete( i );
		}
	}

	TextureHandle TextureNew( const TexType type, const TexFormat format, const u32 sizeX, const u32 sizeY, const u32 sizeZ, const TexFlags flags, void* data )
	{
		for( int i = 0; i < kTextureLimit; i++ )
		{
			if( !sTexture[ i ].mActive )
			{
				TextureHandle handle = i;
				Texture& texture = sTexture[ handle ];
				texture.mFormat = format;
				texture.mType = type;
				texture.mSizeX = sizeX;
				texture.mSizeY = sizeY;
				texture.mSizeZ = sizeZ;
				texture.mFlags = flags;
				texture.mData = data;
				texture.mActive = true;
				
				if( texture.mFlags & TexFlagMipMap )
				{
					u32 x = sizeX;
					u32 y = sizeY;
					u32 z = sizeZ;
					while( x != 1 || y != 1 || z != 1 )
					{
						if( x > 1 ) x >>= 1;
						if( y > 1 ) y >>= 1;
						if( z > 1 ) z >>= 1;
						texture.mLodMax++;
					}
				}

				TextureHwNew( handle );
				return handle;
			}
		}
		ASSERT(0);
		return kTextureInvalid;
	}
			
	TextureHandle TextureNew3d( const TexFormat format, const u32 sizeX, const u32 sizeY, const u32 sizeZ, void* data )
	{
		return TextureNew( TexType3d, format, sizeX, sizeY, sizeZ, TexFlagNone, data );
	}

	TextureHandle TextureNew2d( const TexFormat format, const u32 sizeX, const u32 sizeY, void* data )
	{
		return TextureNew( TexType2d, format, sizeX, sizeY, 1, TexFlagNone, data );
	}
	
	TextureHandle TextureNewDepth( const TexFormat format, const u32 sizeX, const u32 sizeY )
	{
		return TextureNew( TexTypeDepth, format, sizeX, sizeY, 1, TexFlagNone, kNull );
	}
	
	void TextureDelete( const TextureHandle handle )
	{
		if( sTexture[ handle ].mActive )
		{
			TextureHwDelete( handle );
			sTexture[ handle ].mActive = false;
		}
	}
	
	void TextureSet( const c8* shader, const TextureHandle handle )
	{
		TextureHwSet( shader, handle );
	}
	
	const Texture& TextureGet( const TextureHandle handle )
	{
		return sTexture[ handle ];
	}
}