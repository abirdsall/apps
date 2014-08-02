#include "gsHw.h"

#if kBuildOpenGl3

namespace gs
{
	struct CanvasHw
	{
		GLuint mCanvas;
	};
	
	static CanvasHw sCanvasHw[ kCanvasLimit ];
	
	static u32 sColorAttachmentMap[ kColorTextureLimit ] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,
		GL_COLOR_ATTACHMENT7,
		GL_COLOR_ATTACHMENT8,
		GL_COLOR_ATTACHMENT9,
		GL_COLOR_ATTACHMENT10,
		GL_COLOR_ATTACHMENT11,
		GL_COLOR_ATTACHMENT12,
		GL_COLOR_ATTACHMENT13,
		GL_COLOR_ATTACHMENT14,
		GL_COLOR_ATTACHMENT15
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
				glFramebufferTexture3D( GL_FRAMEBUFFER, sColorAttachmentMap[ attachmentIndex ], textureHw.mTarget, textureHw.mTexture, 0, ( GLint )layer );
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
			const Canvas& canvas = CanvasGet( handle );
			CanvasHw canvasHw = sCanvasHw[ handle ];
			glBindFramebuffer( GL_FRAMEBUFFER, canvasHw.mCanvas );
			// todo only reattach textures when changing lod?
			for( int i = 0; i < canvas.mColorTextureCount; i++ )
			{
				const Texture& texture = TextureGet( canvas.mColorTexture[ i ] );
				const TextureHw& textureHw = TextureHwGet( canvas.mColorTexture[ i ] );
				if( texture.mSizeZ > 1 )
				{
					glFramebufferTexture3D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureHw.mTarget, textureHw.mTexture, ( GLint )lod, ( GLint )layer );
				}
				else
				{
					glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureHw.mTarget, textureHw.mTexture, ( GLint )lod );
				}
			}
			glDrawBuffers( canvas.mColorTextureCount, sColorAttachmentMap );
		}
		else
		{
			glBindFramebuffer( GL_FRAMEBUFFER, 0 );
			glDrawBuffer( GL_BACK );
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