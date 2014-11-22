#ifndef GS_HW_TEXTURE
#define GS_HW_TEXTURE

#include "os.h"

#if GsOpenGl3 || GsOpenGles2 || GsOpenGles3

#include "openGlTexture.h"

#endif

namespace gs
{
	void TextureHwNew( TextureHandle handle );
	void TextureHwDelete( TextureHandle handle );
	void TextureHwSet( const c8* shader, TextureHandle handle );
	const TextureHw& TextureHwGet( TextureHandle handle );
	void TextureHwActiveCountReset();
}

#endif