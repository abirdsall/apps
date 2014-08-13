#include "fw.h"

namespace fw
{
    struct c16 { c8 c[17]; };
    struct c256 { c8 c[257]; };
    struct c4096 { c8 c[4097]; };
    
    static const s32 Pool16Size = 1024; // 136k
    static const s32 Pool256Size = 128; // 267k
    static const s32 Pool4096Size = 16; // 512k
    
    static Pool<c16> sPool16( Pool16Size );
    static Pool<c256> sPool256( Pool256Size );
    static Pool<c4096> sPool4096( Pool4096Size );
    
    String::String( StringCapacity capacity ) : _size( 0 )
    {
        _capacity = capacity;
        
        Alloc();
    }
    
    String::String( const c8* s, StringCapacity capacity ) : _size( 0 )
    {
        _capacity = capacity;

        Alloc();
        
        Append( s );
    }
    
    String::~String()
    {
        Free();
    }

    void String::Alloc()
    {
        switch( _capacity )
        {
            case StringCapacity16:
            {
                _data = (c8*)sPool16.Alloc();
                _dataSize = 16;
                break;
            }
            case StringCapacity256:
            {
                _data = (c8*)sPool256.Alloc();
                _dataSize = 256;
                break;
            }
            case StringCapacity4096:
            {
                _data = (c8*)sPool4096.Alloc();
                _dataSize = 4096;
                break;
            }
        }
    }
    
    void String::Free()
    {
        switch( _capacity )
        {
            case StringCapacity16:
            {
                sPool16.Free( (c16*)_data );
                break;
            }
            case StringCapacity256:
            {
                sPool256.Free( (c256*)_data );
                break;
            }
            case StringCapacity4096:
            {
                sPool4096.Free( (c4096*)_data );
                break;
            }
        }
    }
    
    void String::Append( const c8* s )
    {
        u32 size = os::strlen( s );
        
        ASSERT( ( _size + size ) < _dataSize );
        
        os::strcpy( _data + _size, s, size );
        
        _size += size;
    }
}