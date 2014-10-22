#include "gsHw.h"

namespace gs
{
    Array<Canvas> sCanvas;
	//static Canvas sCanvas[ kCanvasLimit ];
	static CanvasHandle sCanvasActive;
	static s32 sCanvasActiveLod;
	static u32 sCanvasActiveCount;

	void InitCanvases()
	{
        sCanvas.SetCount( kCanvasLimit );

		for( u32 i = 0; i < kCanvasLimit; i++ )
		{
			sCanvas[ i ].mActive = false;
		}
		sCanvasActive = kCanvasInvalid;
		sCanvasActiveLod = -1;
		sCanvasActiveCount = 0;
	}
	
	void KillCanvases()
	{
		for( u32 i = 0; i < kCanvasLimit;  i++ )
		{
			CanvasDelete( i );
		}
	}

    CanvasHandle CanvasNew()
    {
        return CanvasNew( "Unknown" );
    }
    
    CanvasHandle CanvasNew( const c8* name )
	{
		for( u32 i = 0; i < kCanvasLimit; i++ )
		{
			if( !sCanvas[ i ].mActive )
			{
				CanvasHandle handle = i;
				Canvas& canvas = sCanvas[ handle ];
                canvas._name = name;
				canvas.mColorTextureCount = 0;
				canvas.mDepthTextureCount = 0;
				canvas.mSizeX = 1;
				canvas.mSizeY = 1;
				canvas.mActive = true;
				sCanvasActiveCount++;
				CanvasHwNew( handle );
				return handle;
			}
		}
		ASSERT(0);
		return kCanvasInvalid;
	}
	
	CanvasHandle CanvasNew( const TextureHandle textureHandle )
	{
		CanvasHandle handle = CanvasNew();
		CanvasAdd( handle, textureHandle );
		return handle;
	}
	
	void CanvasDelete( const CanvasHandle handle )
	{
		if( sCanvas[ handle ].mActive )
		{
			CanvasHwDelete( handle );
			sCanvas[ handle ].mActive = false;
			sCanvasActiveCount--;

		}
	}
	
	void CanvasAdd( const CanvasHandle canvasHandle, const TextureHandle textureHandle, const u32 layer )
	{
		Canvas& canvas = sCanvas[ canvasHandle ];
		const Texture& texture = TextureGet( textureHandle );
		
		canvas.mSizeX = texture.mSizeX;
		canvas.mSizeY = texture.mSizeY;
			
		if( texture.mType == TexTypeDepth )
		{
			ASSERT( canvas.mDepthTextureCount < kDepthTextureLimit );
			canvas.mDepthTexture[ canvas.mDepthTextureCount ] = textureHandle;
            canvas.mDepthTextureLayer[ canvas.mDepthTextureCount ] = layer;
			canvas.mDepthTextureCount++;
		}
		else
		{
			ASSERT( canvas.mColorTextureCount < kColorTextureLimit );
			canvas.mColorTexture[ canvas.mColorTextureCount ] = textureHandle;
            canvas.mColorTextureLayer[ canvas.mColorTextureCount ] = layer;
			canvas.mColorTextureCount++;
		}
	}
	
	void CanvasSet( const CanvasHandle handle, const s32 lod )
	{
        CanvasHwSet( handle, lod );
        
		sCanvasActive = handle;
        
        sCanvasActiveLod = lod;
		
        gs::SetViewport( 0, 0, CanvasSizeX(), CanvasSizeY() );
	}
	
	void CanvasSet( const CanvasHandle handle )
	{
        CanvasSet( handle, 0 );
	}
	
	u32 CanvasSizeX()
	{
		return sCanvasActive != kCanvasInvalid ? sCanvas[ sCanvasActive ].mSizeX >> sCanvasActiveLod : os::WindowSizeX();
	}
	
	u32 CanvasSizeY()
	{
		return sCanvasActive != kCanvasInvalid ? sCanvas[ sCanvasActive ].mSizeY >> sCanvasActiveLod : os::WindowSizeY();
	}
	
	Canvas& CanvasGet( const CanvasHandle handle )
	{
		return sCanvas[ handle ];
	}
	
	u32 CanvasActive() // todo rename to applied?
	{
		return sCanvasActive;
	}
	
	s32 CanvasActiveLod() // todo rename to applied?
	{
		return sCanvasActiveLod;
	}
	
	s32 CanvasActiveCount()
	{
		return sCanvasActiveCount;
	}
	
	s32 CanvasActiveGet( CanvasHandle array[ kCanvasLimit ] )
	{
		s32 count = 0;
		for( int i = 0; i < kCanvasLimit; i++ )
		{
			if( sCanvas[ i ].mActive )
			{
				array[ count ] = i;
				count++;
			}
		}
		return count;
	}
}