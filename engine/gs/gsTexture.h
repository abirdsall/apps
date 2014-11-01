#ifndef GS_TEXTURE
#define GS_TEXTURE

namespace gs
{
	enum TexType
	{
		TexType2d,
		TexType3d,
		TexTypeDepth,
		TexTypeCount
	};
	
	enum TexFormat
	{
		TexFormatR8,
		TexFormatR16F,
		TexFormatR32F,
		TexFormatRG8,
		TexFormatRG16F,
		TexFormatRG32F,
		TexFormatRGB8,
		TexFormatRGB16F,
		TexFormatRGB32F,
		TexFormatRGBA8,
		TexFormatRGBA16F,
		TexFormatRGBA32F,
		TexFormatRGBA32U,
		TexFormatDepth,
		TexFormatCount
	};
	
	enum TexFlags
	{
		TexFlagNone		= 0x0000,
		TexFlagMipMap	= 0x0001,
		TexFlagNearest	= 0x0002,
		TexFlagClampS	= 0x0004,
		TexFlagClampT	= 0x0008,
		TexFlagClampR	= 0x0010,
		TexFlagClamp	= TexFlagClampS | TexFlagClampT | TexFlagClampR,
	};

	struct Texture
	{
		TexType _type;
		TexFormat _format;
		u32 _sizeX;
        //Why corrupts add member here?
		u32 _sizeY;
		u32 _sizeZ;
		u32 _lodMax;
		TexFlags _flags;
		void* _data;
		bool _active;
        core::String _name;
	};

	typedef ::u32 TextureHandle;
    
    const TextureHandle InvalidTextureHandle = MaxU32;
	
	const u32 kTextureInvalid = MaxU32;
	const int kTextureLimit = 256;
	
	void InitTextures();
	void KillTextures();
	
	TextureHandle TextureNew( const c8* name, const TexType type, const TexFormat format, const u32 sizeX, const u32 sizeY, const u32 sizeZ, const TexFlags flags, void* data = Null );
	TextureHandle TextureNew3d( const c8* name, const TexFormat format, const u32 sizeX, const u32 sizeY, const u32 sizeZ, void* data = Null );
	TextureHandle TextureNew2d( const c8* name, const TexFormat format, const u32 sizeX, const u32 sizeY, void* data = Null );
	TextureHandle TextureNewDepth( const c8* name, const TexFormat format, const u32 sizeX, const u32 sizeY );

	void TextureDelete( const TextureHandle handle );
	void TextureSet( const c8* shader, const TextureHandle handle );
	Texture& TextureGet( const TextureHandle handle );
    
    s32 TextureActiveGet( TextureHandle array[ kTextureLimit ] );
}

#endif