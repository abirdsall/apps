#include "gsHw.h"

#if GsOpenGl3 || GsOpenGles2 || GsOpenGles3

namespace gs
{
	struct CanvasHw
	{
		GLuint _id;
	};

    static GLuint _activeBuffer = 0;
    
    static GLuint _backBuffer = 0;
    
	static CanvasHw _canvasesHw[ CanvasLimit ];
	
	static u32 _colorAttachmentMap[ ColorTextureLimit ] =
	{
#if GsOpenGles2
        GL_COLOR_ATTACHMENT0
#else
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
#endif
	};
	
    void LocateBackBuffer()
    {
        if(_activeBuffer == _backBuffer)
        {
            GLint backBuffer;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &backBuffer);
            _activeBuffer = backBuffer;
            _backBuffer = backBuffer;
        }
    }
    
	void CanvasHwNew( CanvasHandle handle )
	{
		glGenFramebuffers( 1, &_canvasesHw[ handle ]._id );
	}
	
	void CanvasHwDelete( CanvasHandle handle )
	{
		glDeleteFramebuffers( 1, &_canvasesHw[ handle ]._id );
	}
	
	void CanvasHwSet( CanvasHandle handle, s32 lod )
	{
        ErrorCheck();

        LocateBackBuffer();

        ErrorCheck();

        if( handle != CanvasInvalid )
        {
            CanvasHw canvasHw = _canvasesHw[ handle ];
            
            ErrorCheck();

            glBindFramebuffer( GL_FRAMEBUFFER, canvasHw._id );

            ErrorCheck();

            _activeBuffer = canvasHw._id;
            
#if !GsOpenGles2
            const Canvas& canvas = CanvasGet( handle );
            
            // todo only reattach textures when changing lod?
            
            for( int i = 0; i < canvas._colorTextureCount; i++ )
            {
                const Texture& texture = TextureGet( canvas._colorTexture[ i ] );
                
                const TextureHw& textureHw = TextureHwGet( canvas._colorTexture[ i ] );
                
                ErrorCheck();
                
                if( texture._sizeZ > 1 )
                {
                    glFramebufferTextureLayer( GL_FRAMEBUFFER, _colorAttachmentMap[ i ], textureHw._id, ( GLint )lod, ( GLint )canvas._colorTextureLayer[ i ] );
                }
                else
                {
                    glFramebufferTexture2D( GL_FRAMEBUFFER, _colorAttachmentMap[ i ], textureHw._target, textureHw._id, ( GLint )lod );
                }
                
                ErrorCheck();
            }

            ErrorCheck();

            glDrawBuffers( canvas._colorTextureCount, _colorAttachmentMap );
            
            ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE );
            
            ErrorCheck();
#endif
        }
        else
        {
            ErrorCheck();

            glBindFramebuffer( GL_FRAMEBUFFER, _backBuffer );

            ErrorCheck();

            _activeBuffer = _backBuffer;
        }
        
        ErrorCheck();

#if 0
        s32 vp[4], sb[4], st;
        glGetIntegerv( GL_SCISSOR_BOX, sb );
        glGetIntegerv( GL_SCISSOR_TEST, &st );
        glGetIntegerv( GL_VIEWPORT, vp );
        printf("vp %d %d %d %d sb %d %d %d %d st %d\n", vp[0], vp[1], vp[2], vp[3], sb[0], sb[1], sb[2], sb[3], st);
#endif
	}
}



#endif