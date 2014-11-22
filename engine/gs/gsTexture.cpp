#include "gsHw.h"

namespace gs
{
	static Texture* _textures;
	
	void InitTextures()
	{
        _textures = new Texture[ TextureLimit ];

		for( u32 i = 0; i < TextureLimit; i++ )
		{
			_textures[ i ]._active = false;
		}
	}
	
	void KillTextures()
	{
		for( u32 i = 0; i < TextureLimit;  i++ )
		{
			TextureDelete( i );
		}
	}

	TextureHandle TextureNew( const c8* name, TexType type, TexFormat format, u32 sizeX, u32 sizeY, u32 sizeZ, const TexFlags flags, void* data )
	{
		for( int i = 0; i < TextureLimit; i++ )
		{
			if( !_textures[ i ]._active )
			{
				TextureHandle handle = i;
				Texture& texture = _textures[ handle ];
                texture._name = name;
				texture._format = format;
				texture._type = type;
				texture._sizeX = sizeX;
				texture._sizeY = sizeY;
				texture._sizeZ = sizeZ;
                texture._lodMax = 0;
				texture._flags = flags;
				texture._data = data;
				texture._active = true;
				
				if( texture._flags & TexFlagMipMap )
				{
					u32 x = sizeX;
					u32 y = sizeY;
					u32 z = sizeZ;
					while( x != 1 || y != 1 || z != 1 )
					{
						if( x > 1 ) x >>= 1;
						if( y > 1 ) y >>= 1;
						if( z > 1 ) z >>= 1;
						texture._lodMax++;
					}
				}

				TextureHwNew( handle );
				return handle;
			}
		}
		ASSERT(0);
		return TextureInvalid;
	}
			
	TextureHandle TextureNew3d( const c8* name, TexFormat format, u32 sizeX, u32 sizeY, u32 sizeZ, void* data )
	{
		return TextureNew( name, TexType3d, format, sizeX, sizeY, sizeZ, TexFlagNone, data );
	}

	TextureHandle TextureNew2d( const c8* name, TexFormat format, u32 sizeX, u32 sizeY, void* data )
	{
		return TextureNew( name, TexType2d, format, sizeX, sizeY, 1, TexFlagNone, data );
	}
	
	TextureHandle TextureNewDepth( const c8* name, TexFormat format, u32 sizeX, u32 sizeY )
	{
		return TextureNew( name, TexTypeDepth, format, sizeX, sizeY, 1, TexFlagNone, Null );
	}
	
	void TextureDelete( TextureHandle handle )
	{
		if( _textures[ handle ]._active )
		{
			TextureHwDelete( handle );
			_textures[ handle ]._active = false;
		}
	}
	
	void TextureSet( const c8* shader, TextureHandle handle )
	{
		TextureHwSet( shader, handle );
	}
	
	Texture& TextureGet( TextureHandle handle )
	{
		return _textures[ handle ];
	}
    
    s32 TextureActiveGet( TextureHandle array[ TextureLimit ] )
    {
        s32 count = 0;
        for( int i = 0; i < TextureLimit; i++ )
        {
            if( _textures[ i ]._active )
            {
                array[ count ] = i;
                count++;
            }
        }
        return count;
    }
}