#ifndef CORE_POOL
#define CORE_POOL

#include <new>

namespace core
{
    template <class T> class Pool
    {
    private:
        T* _data;
        T** _free;
        s32 _dataSize;
        s32 _freeSize;
        
    public:
        Pool( s32 size = 0 )
        {
            _data = Null;
            _free = Null;

            if( size > 0)
            {
                Init( size );
            }
        }
        
        void Init( s32 size )
        {
            ASSERT( _data == Null );
            
            _dataSize = size;
            _freeSize = size;
            
            _data = (T*)core::alloc(sizeof(T) * _dataSize);
            _free = (T**)core::alloc(sizeof(void*) * _dataSize);
            
            for( s32 i = 0; i < _dataSize; i++ )
            {
                _free[ i ] = &(_data[ i ]);
            }
        }
        
        T* New()
        {
            ASSERT( _freeSize > 0);
            return new( _free[ --_freeSize ] )T();
        }
        
        void Delete( T* object )
        {
            ASSERT( _freeSize < _dataSize );
            ASSERT( hwInt( object ) >= hwInt( _data ) && hwInt( object ) <= hwInt( &_data[_dataSize - 1] ));
            object->~T();
            _free[ _freeSize++ ] = object;
        }
    };
}

#endif