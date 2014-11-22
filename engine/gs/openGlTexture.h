#ifndef OPENGL_TEXTURE
#define OPENGL_TEXTURE

namespace gs
{
	struct TextureHw
	{
		GLuint _id;
		GLenum _target;
		GLint _internalFormat;
		GLenum _format;
		GLenum _type;
	};
}

#endif
