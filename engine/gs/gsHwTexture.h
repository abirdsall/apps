#ifndef GS_HW_TEXTURE
#define GS_HW_TEXTURE

#include "os.h"

#if kBuildDirectX

#elif kBuildOpenGles2

#include "openGlTexture.h"

#elif kBuildOpenGl3

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