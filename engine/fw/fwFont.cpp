#include "fw.h"

namespace fw
{
    static const s32 SystemCharSizeX = 5;
    static const s32 SystemCharSizeY = 5;
    
    static const s32 SystemFontSourceDataSizeX = SystemCharSizeX * (10 + 26);
    static const s32 SystemFontSourceDataSizeY = SystemCharSizeY;
    
    static const s32 SystemFontTextureDataSizeX = 256;
    static const s32 SystemFontTextureDataSizeY = 8;
    
    static const c8* SystemFontSourceData =
    "00000111  22222333334    5555566666777778888899999aaaaabbbbbccccddddddeeeeefffffgggggh   hiiiii    jk   kl    mmmmmn   nooooopppppqqqqqrrrrrssssstttttu   uv   vw w wx   xy   yzzzzz"\
    "0   0  1      2    34 4  5    6        78   89   9a   ab   bc    d   de    f    g    h   h  i      jk  k l    m m mnn  no   op   pq   qr   rs      t  u   uv   vw w w x x y   y   z "\
    "0   0  1  2222233333444445555566666   7 8888899999aaaaabbbbbc    d   deeeeefffffg   ghhhhh  i      jkkk  l    m m mn n no   opppppq   qrrrrrsssss  t  u   uv   vw w w  x   y y   z  "\
    "0   0  1  2        3  4      56   6  7  8   8    9a   ab   bc    d   de    f    g   gh   h  i      jk  k l    m m mn  nno   op    q q qr  r     s  t  u   u v v w w w x x   y   z   "\
    "00000111112222233333  4  5555566666 7   88888    9a   abbbbbcccccdddddeeeeef    gggggh   hiiiiijjjjjk   klllllm m mn   nooooop    qqqqqr   rsssss  t  uuuuu  v  wwwwwx   x  y  zzzzz";

    static gs::TextureHandle _systemFontTexture;
    static f32 _systemCharFractionX;
    static f32 _systemCharFractionY;
    
    void SystemFontInit()
    {
        u32 systemFontTextureData[ SystemFontTextureDataSizeX * SystemFontTextureDataSizeY ];
        
        for( s32 i = 0; i < SystemFontTextureDataSizeX; i++ )
        {
            for( s32 j = 0; j < SystemFontTextureDataSizeY; j++ )
            {
                s32 textureDataIndex = j * SystemFontTextureDataSizeX + i;

                if(i < SystemFontSourceDataSizeX && j < SystemFontSourceDataSizeY)
                {
                    s32 sourceDataIndex = j * SystemFontSourceDataSizeX + i;
                    
                    systemFontTextureData[ textureDataIndex ] = SystemFontSourceData[ sourceDataIndex ] == ' ' ? 0x00000000 : 0xFFFFFFFF;
                }
                else
                {
                    systemFontTextureData[ textureDataIndex ] = 0x00000000;
                }
            }
        }
        
        _systemCharFractionX = f32( SystemCharSizeX ) / f32( SystemFontTextureDataSizeX );
        _systemCharFractionY = f32( SystemCharSizeY ) / f32( SystemFontTextureDataSizeY );
        _systemFontTexture = gs::TextureNew( gs::TexType2d, gs::TexFormatRGBA8, SystemFontTextureDataSizeX, SystemFontTextureDataSizeY, 1, gs::TexFlags( gs::TexFlagClamp | gs::TexFlagNearest ), ( void* )systemFontTextureData );
    }
    
    s32 SystemCharIndex( c8 c )
    {
        if( c >= 'a' && c <= 'z' )
        {
            return 10 + c - 'a';
        }
        else if( c >= 'A' && c <= 'Z' )
        {
            return 10 + c - 'A';
        }
        else if( c >= '0' && c <= '9' )
        {
            return c - '0';
        }
        else
        {
            return 0;
        }
    }
    
    Rect SystemCharTexCoords( c8 c )
    {
        s32 index = SystemCharIndex( c );
        f32 x1 = f32( index ) * _systemCharFractionX;
        f32 x2 = f32( index + 1 ) * _systemCharFractionX;
        return Rect( x1, 0.0f, x2, _systemCharFractionY );
    }
    
    void SystemFontDraw( core::String text, v2 position, v4 colour )
    {
        BatchBegin( true );

        gs::TextureSet( "texture0", _systemFontTexture );

        f32 x1 = position.x;
        f32 y1 = position.y;
        f32 x2 = position.x + f32( SystemCharSizeX * 1 );
        f32 y2 = position.y + f32( SystemCharSizeY * 1 );
        
        for( s32 i = 0; i < text.Size(); i++ )
        {
            if( text[ i ] != ' ' )
            {
                Rect vertices = Rect( x1, y1, x2, y2 );
                
                Rect tcoords = SystemCharTexCoords( text[ i ] );
                
                TextureRect( vertices, tcoords, colour );
            }
            
            x1 += f32( SystemCharSizeX * 1 ) + 1;
            x2 += f32( SystemCharSizeX * 1 ) + 1;
        }
        
        BatchEnd( gs::ePrimTriangles );
    }
}