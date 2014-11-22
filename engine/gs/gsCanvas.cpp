#include "gsHw.h"

namespace gs
{
	static Canvas* _canvases;
	static CanvasHandle _canvasActive;
	static s32 _canvasActiveLod;
	static u32 _canvasActiveCount;

	void InitCanvases()
	{
        _canvases = new Canvas[ CanvasLimit ];

		for( u32 i = 0; i < CanvasLimit; i++ )
		{
			_canvases[ i ]._active = false;
		}
		_canvasActive = CanvasInvalid;
		_canvasActiveLod = -1;
		_canvasActiveCount = 0;
	}
	
	void KillCanvases()
	{
		for( u32 i = 0; i < CanvasLimit;  i++ )
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
		for( u32 i = 0; i < CanvasLimit; i++ )
		{
			if( !_canvases[ i ]._active )
			{
				CanvasHandle handle = i;
				Canvas& canvas = _canvases[ handle ];
                canvas._name = name;
				canvas._colorTextureCount = 0;
				canvas._depthTextureCount = 0;
				canvas._sizeX = 1;
				canvas._sizeY = 1;
				canvas._active = true;
				_canvasActiveCount++;
				CanvasHwNew( handle );
				return handle;
			}
		}
		ASSERT(0);
		return CanvasInvalid;
	}
	
	CanvasHandle CanvasNew( TextureHandle textureHandle )
	{
		CanvasHandle handle = CanvasNew();
		CanvasAdd( handle, textureHandle );
		return handle;
	}
	
	void CanvasDelete( CanvasHandle handle )
	{
		if( _canvases[ handle ]._active )
		{
			CanvasHwDelete( handle );
			_canvases[ handle ]._active = false;
			_canvasActiveCount--;

		}
	}
	
	void CanvasAdd( CanvasHandle canvasHandle, TextureHandle textureHandle, u32 layer )
	{
		Canvas& canvas = _canvases[ canvasHandle ];
		const Texture& texture = TextureGet( textureHandle );
		
		canvas._sizeX = texture._sizeX;
		canvas._sizeY = texture._sizeY;
			
		if( texture._type == TexTypeDepth )
		{
			ASSERT( canvas._depthTextureCount < DepthTextureLimit );
			canvas._depthTexture[ canvas._depthTextureCount ] = textureHandle;
            canvas._depthTextureLayer[ canvas._depthTextureCount ] = layer;
			canvas._depthTextureCount++;
		}
		else
		{
			ASSERT( canvas._colorTextureCount < ColorTextureLimit );
			canvas._colorTexture[ canvas._colorTextureCount ] = textureHandle;
            canvas._colorTextureLayer[ canvas._colorTextureCount ] = layer;
			canvas._colorTextureCount++;
		}
	}
	
	void CanvasSet( CanvasHandle handle, s32 lod )
	{
        CanvasHwSet( handle, lod );
        
		_canvasActive = handle;
        
        _canvasActiveLod = lod;
		
        gs::SetViewport( 0, 0, CanvasSizeX(), CanvasSizeY() );
	}
	
	void CanvasSet( CanvasHandle handle )
	{
        CanvasSet( handle, 0 );
	}
	
	u32 CanvasSizeX()
	{
		return _canvasActive != CanvasInvalid ? _canvases[ _canvasActive ]._sizeX >> _canvasActiveLod : os::WindowSizeX();
	}
	
	u32 CanvasSizeY()
	{
		return _canvasActive != CanvasInvalid ? _canvases[ _canvasActive ]._sizeY >> _canvasActiveLod : os::WindowSizeY();
	}
	
	Canvas& CanvasGet( CanvasHandle handle )
	{
		return _canvases[ handle ];
	}
	
	u32 CanvasActive() // todo rename to applied?
	{
		return _canvasActive;
	}
	
	s32 CanvasActiveLod() // todo rename to applied?
	{
		return _canvasActiveLod;
	}
	
	s32 CanvasActiveCount()
	{
		return _canvasActiveCount;
	}
	
	s32 CanvasActiveGet( CanvasHandle array[ CanvasLimit ] )
	{
		s32 count = 0;
		for( int i = 0; i < CanvasLimit; i++ )
		{
			if( _canvases[ i ]._active )
			{
				array[ count ] = i;
				count++;
			}
		}
		return count;
	}
}