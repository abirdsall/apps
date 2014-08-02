#ifndef OPENGL_TEXTURE
#define OPENGL_TEXTURE

namespace gs
{
	struct TextureHw
	{
		GLuint mTexture;
		GLenum mTarget;
		GLint mInternalFormat;
		GLenum mFormat;
		GLenum mType;
	};
}

#endif
