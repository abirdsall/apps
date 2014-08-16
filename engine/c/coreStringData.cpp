#include "os.h"

namespace core
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
    
    void StringData16::Alloc()
    {
        _data = (c8*)sPool16.Alloc();
    }
    
    void StringData16::Free()
    {
        sPool16.Free( (c16*)_data );
    }
    
    void StringData256::Alloc()
    {
        _data = (c8*)sPool256.Alloc();
    }
    
    void StringData256::Free()
    {
        sPool256.Free( (c256*)_data );
    }
    
    void StringData4096::Alloc()
    {
        _data = (c8*)sPool4096.Alloc();
    }
    
    void StringData4096::Free()
    {
        sPool4096.Free( (c4096*)_data );
    }
}