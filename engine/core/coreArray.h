namespace core
{
    typedef enum
    {
        eBytePool0,
        eBytePool32,
        eBytePool128,
        eBytePool512,
        eBytePool2048,
        eBytePool8192
    } eBytePool;
    
    u8* bytePoolAlloc( eBytePool bytePool );
    
    void bytePoolFree( eBytePool bytePool, u8* data );
    
    eBytePool bytePoolFind( s32 capacity );
    
    template <class T> class Array
    {
    private:
        
        T* _data;
        
        s32 _count;
        
        s32 _capacity;

    public:
        
        Array()
        {
            _data = Null;
            
            _count = 0;
            
            _capacity = 0;
        }
        
        Array( s32 capacity )
        {
            _data = Null;
            
            _count = 0;
            
            SetCapacity( capacity );
        }
        
        ~Array()
        {
            SetCapacity( 0 );
        }
        
        void SetCount( s32 count )
        {
            if( count > _capacity )
            {
                SetCapacity( count );
            }
            _count = count;
        }
        
        void SetCapacity( s32 capacity )
        {
            if( capacity != _capacity )
            {
                s32 byteCapacityNew = sizeof(T) * capacity;
                
                s32 byteCapacityOld = sizeof(T) * _capacity;
                
                eBytePool bytePoolNew = bytePoolFind( byteCapacityNew );
                
                eBytePool bytePoolOld = bytePoolFind( byteCapacityOld );
                
                if( bytePoolNew != bytePoolOld )
                {
                    u8* dataNew = bytePoolAlloc( bytePoolNew );
                    
                    core::copy( dataNew, ( u8* )_data, min( byteCapacityNew, sizeof(T) * _count ) );
                    
                    bytePoolFree( bytePoolOld, ( u8* )_data );
                    
                    _data = ( T* )dataNew;
                }
                
                _capacity = capacity;
            }
        }
        
        inline void Add( T element )
        {
            s32 count = _count;
            
            SetCount( _count + 1 );
            
            _data[ count ] = element;
        }

        inline T* Data()
        {
            return _data;
        }

        inline s32 Count()
        {
            return _count;
        }

        inline s32 Capacity()
        {
            return _capacity;
        }

        inline T operator [] ( const s32 i )
        {
            ASSERT( i < _count );
            
            return _data[ i ];
        }
    };
}
