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
		TexType mType;
		TexFormat mFormat;
		u32 mSizeX;
		u32 mSizeY;
		u32 mSizeZ;
		u32 mLodMax;
		TexFlags mFlags;
		void* mData;
		bool mActive;
	};

	typedef ::u32 TextureHandle;
	
	const u32 kTextureInvalid = kMaxU32;
	const int kTextureLimit = 256;
	
	void InitTextures();
	void KillTextures();
	
	TextureHandle TextureNew( const TexType type, const TexFormat format, const u32 sizeX, const u32 sizeY, const u32 sizeZ, const TexFlags flags, void* data = kNull );
	TextureHandle TextureNew3d( const TexFormat format, const u32 sizeX, const u32 sizeY, const u32 sizeZ, void* data = kNull );
	TextureHandle TextureNew2d( const TexFormat format, const u32 sizeX, const u32 sizeY, void* data = kNull );
	TextureHandle TextureNewDepth( const TexFormat format, const u32 sizeX, const u32 sizeY );

	void TextureDelete( const TextureHandle handle );
	void TextureSet( const c8* shader, const TextureHandle handle );
	const Texture& TextureGet( const TextureHandle handle );
}

#endif