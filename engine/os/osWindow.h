#ifndef OS_WINDOW
#define OS_WINDOW

namespace os
{
    struct WindowFormat
	{
        s32 _sizeX;
        s32 _sizeY;
        
        s32 mBitsR;
        s32 mBitsG;
        s32 mBitsB;
        s32 mBitsA;
        s32 mBitsZ;
        
        bool mFullscreen;
        bool mNative;
        
        WindowFormat()
        {
            Init( 0, 0, 0, 0, 0, 0, 0, false, true );
        }
        
        WindowFormat( s32 sizeX, s32 sizeY, s32 rBits, s32 gBits, s32 bBits, s32 aBits, s32 zBits, bool fullscreen, bool native )
        {
            Init( sizeX, sizeY, rBits, gBits, bBits, aBits, zBits, fullscreen, native );
        }
        
        void Init( s32 sizeX, s32 sizeY, s32 rBits, s32 gBits, s32 bBits, s32 aBits, s32 zBits, bool fullscreen, bool native )
        {
            _sizeX = sizeX;
            _sizeY = sizeY;
            mBitsR = rBits;
            mBitsG = gBits;
            mBitsB = bBits;
            mBitsA = aBits;
            mBitsZ = zBits;
            mFullscreen = fullscreen;
            mNative = native;
        }
	};
    
	void WindowInit();
	void WindowTick();
    
	bool WindowOpen( WindowFormat& windowFormat );
	
    s32 WindowSizeX();
	s32 WindowSizeY();
	f32 WindowPixelSizeX();
	f32 WindowPixelSizeY();
	f32 WindowAspect();
	
    bool WindowOpened();
};

#endif