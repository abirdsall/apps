#ifndef FW_POOL
#define FW_POOL

namespace fw
{
    template <class T> class Pool
    {
    private:
        T* _data;
        T** _free;
        s32 _dataSize;
        s32 _freeSize;
        
    public:
        Pool( s32 size )
        {
            _dataSize = size;
            _freeSize = size;
            _data = (T*)memory::alloc(sizeof(T) * _dataSize);
            _free = (T**)memory::alloc(sizeof(void*) * _dataSize);
            
            for( s32 i = 0; i < _dataSize; i++ )
            {
                _free[ i ] = &(_data[ i ]);
            }
        }
        
        T* Alloc()
        {
            ASSERT( _freeSize > 0);
            return _free[ --_freeSize ];
        }
        
        void Free( T* object )
        {
            ASSERT( _freeSize < _dataSize );
            ASSERT( hwInt( object ) >= hwInt( _data ) && hwInt( object ) <= hwInt( &_data[_dataSize - 1] ));
            _free[ _freeSize++ ] = object;
        }
    };
}

#endif