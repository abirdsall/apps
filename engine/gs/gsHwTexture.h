#ifndef GS_HW_TEXTURE
#define GS_HW_TEXTURE

#include "os.h"

#if kBuildOpenGl3 || kBuildOpenGles2 || kBuildOpenGles3

#include "openGlTexture.h"

#endif

namespace gs
{
	void TextureHwNew( const TextureHandle handle );
	void TextureHwDelete( const TextureHandle handle );
	void TextureHwSet( const c8* shader, const TextureHandle handle );
	const TextureHw& TextureHwGet( const TextureHandle handle );
	void TextureHwActiveCountReset();
}

#endif