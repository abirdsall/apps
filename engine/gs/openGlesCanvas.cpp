#include "gsHw.h"

#if kBuildOpenGles2

namespace gs
{
	struct CanvasHw
	{
		GLuint mCanvas;
	};
	
	static CanvasHw sCanvasHw[ kCanvasLimit ];
	
	static u32 sColorAttachmentMap[ kColorTextureLimit ] =
	{
		GL_COLOR_ATTACHMENT0
	};
	
	void CanvasHwNew( CanvasHandle handle )
	{
		glGenFramebuffers( 1, &sCanvasHw[ handle ].mCanvas );
	}
	
	void CanvasHwDelete( CanvasHandle handle )
	{
		glDeleteFramebuffers( 1, &sCanvasHw[ handle ].mCanvas );
	}

	void CanvasHwAdd( const CanvasHandle canvasHandle, const TextureHandle textureHandle, const u32 attachmentIndex, const u32 layer )
	{
		CanvasHw canvasHw = sCanvasHw[ canvasHandle ];
		const Texture& texture = TextureGet( textureHandle );
		const TextureHw& textureHw = TextureHwGet( textureHandle );
		
		glBindFramebuffer( GL_FRAMEBUFFER, canvasHw.mCanvas );
		glBindTexture( textureHw.mTarget, textureHw.mTexture );
		if( texture.mType != TexTypeDepth )
		{
			if( texture.mSizeZ > 1 )
			{
				ASSERT(false);
			}
			else
			{
				glFramebufferTexture2D( GL_FRAMEBUFFER, sColorAttachmentMap[ attachmentIndex ], textureHw.mTarget, textureHw.mTexture, 0 );
			}
		}
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	}
	
	void CanvasHwSet( const CanvasHandle handle, const u32 layer, const s32 lod )
	{
		if( handle != kCanvasInvalid )
		{
			glBindFramebuffer( GL_FRAMEBUFFER, sCanvasHw[ handle ].mCanvas );
		}
		else
		{
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		}
	}
}


#if 0
s32 vp[4], sb[4], st;
glGetIntegerv( GL_SCISSOR_BOX, sb );
glGetIntegerv( GL_SCISSOR_TEST, &st );
glGetIntegerv( GL_VIEWPORT, vp );
printf("vp %d %d %d %d sb %d %d %d %d st %d\n", vp[0], vp[1], vp[2], vp[3], sb[0], sb[1], sb[2], sb[3], st);
#endif

#endif