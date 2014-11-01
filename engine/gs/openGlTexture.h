#ifndef OPENGL_TEXTURE
#define OPENGL_TEXTURE

namespace gs
{
	struct TextureHw
	{
		GLuint mTexture;
		GLenum mTarget;
		GLint mInternalFormat;
		GLenum _format;
		GLenum _type;
	};
}

#endif
