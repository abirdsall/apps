#ifndef OS_WINDOW
#define OS_WINDOW

namespace os
{
    struct WindowFormat
	{
        s32 _sizeX;
        s32 _sizeY;
        
        s32 _bitsR;
        s32 _bitsG;
        s32 _bitsB;
        s32 _bitsA;
        s32 _bitsZ;
        
        bool _fullscreen;
        bool _native;
        
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
            _bitsR = rBits;
            _bitsG = gBits;
            _bitsB = bBits;
            _bitsA = aBits;
            _bitsZ = zBits;
            _fullscreen = fullscreen;
            _native = native;
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