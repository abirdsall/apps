#include "core.h"

namespace core
{
    //0
    //1 256k
    //2 128k
    //4 64k
    //8 32768
    
    //16 16384
    //32 8192
    //64 4096
    //128 2048
    
    //256 1024
    //512 512
    //1024 256
    //2048 128
    
    //4096 64
    //8192 32

    struct Bx32 { u8 _bytes[ 32 ]; };
    struct Bx128 { u8 _bytes[ 128 ]; };
    struct Bx512 { u8 _bytes[ 512 ]; };
    struct Bx2048 { u8 _bytes[ 2048 ]; };
    struct Bx8192 { u8 _bytes[ 8192 ]; };
    
    static Pool<Bx32> _bytePool32( 8192 );
    static Pool<Bx128> _bytePool128( 2048 );
    static Pool<Bx512> _bytePool512( 512 );
    static Pool<Bx2048> _bytePool2048( 128 );
    static Pool<Bx8192> _bytePool8192( 32 );
    
    u8* bytePoolAlloc( eBytePool bytePool )
    {
        switch( bytePool )
        {
            case eBytePool0:
            {
                return kNull;
            }
            case eBytePool32:
            {
                return ( u8* )_bytePool32.New();
            }
            case eBytePool128:
            {
                return ( u8* )_bytePool128.New();
            }
            case eBytePool512:
            {
                return ( u8* )_bytePool512.New();
            }
            case eBytePool2048:
            {
                return ( u8* )_bytePool2048.New();
            }
            case eBytePool8192:
            {
                return ( u8* )_bytePool8192.New();
            }
        }
    }

    void bytePoolFree( eBytePool bytePool, u8* data )
    {
        switch( bytePool )
        {
            case eBytePool0:
            {
                break;
            }
            case eBytePool32:
            {
                _bytePool32.Delete( ( Bx32* )data );
                break;
            }
            case eBytePool128:
            {
                _bytePool128.Delete( ( Bx128* )data );
                break;
            }
            case eBytePool512:
            {
                _bytePool512.Delete( ( Bx512* )data );
                break;
            }
            case eBytePool2048:
            {
                _bytePool2048.Delete( ( Bx2048* )data );
                break;
            }
            case eBytePool8192:
            {
                _bytePool8192.Delete( ( Bx8192* )data );
                break;
            }
        }
    }
    
    eBytePool bytePoolFind( s32 capacity )
    {
        if( capacity <= 0 )
        {
            return eBytePool0;
        }
        else if( capacity <= 32 )
        {
            return eBytePool32;
        }
        else if( capacity <= 128 )
        {
            return eBytePool128;
        }
        else if( capacity <= 512 )
        {
            return eBytePool512;
        }
        else if( capacity <= 2048 )
        {
            return eBytePool2048;
        }
        else if( capacity <= 8192 )
        {
            return eBytePool8192;
        }
        else
        {
            ASSERT( false );
            return eBytePool0;
        }
    }
}