#ifndef CORE_STRING_OBJECT
#define CORE_STRING_OBJECT

namespace core
{
    template <class T> class StringObject
    {
    private:
        
        T _data;
        
        s32 _size;
        
        void Append( const c8* s )
        {
            u32 size = core::strlen( s );
            
            ASSERT( ( _size + size ) < _data.Size() );
            
            core::strcpy( _data.Data() + _size, s, size );
            
            _size += size;
        }
        
    public:
        
        StringObject()
        {
            _size = 0;
            
            _data.Alloc();
        }
        
        StringObject( const c8* s )
        {
            _size = 0;
            
            _data.Alloc();
            
            Append( s );
        }
        
        ~StringObject()
        {
            _data.Free();
        }
        
        s32 Size()
        {
            return _size;
        }
        
        c8 operator [] ( const s32 i )
        {
            return _data.Data()[ i ];
        }
        
        const c8* toStr()
        {
            _data.Data()[ _size ] = '\0';
            
            return _data.Data();
        }
        
        StringObject<T>& operator = ( const c8* s )
        {
            _size = 0;
            
            Append( s );
            
            return *this;
        }
        
        StringObject<T>& operator += ( const c8* s )
        {
            Append( s );
            
            return *this;
        }
        
        StringObject<T>& operator + ( const c8* s )
        {
            *this += s;
            
            return *this;
        }
        
        StringObject<T>& operator + ( s32 value )
        {
            if( value == 0 )
            {
                Append( "0" );
            }
            else
            {
                bool neg = false;
                
                if( value < 0 )
                {
                    neg = true;
                    value = -value;
                }
                
                s32 digits = 0;
                s32 tmp = value;
                
                while( tmp > 0 )
                {
                    digits++;
                    tmp /= 10;
                }
                
                s32 digs[ digits ];
                
                c8 chrs[ digits + 1];
                
                for( tmp = digits; value > 0; tmp--)
                {
                    digs[ tmp ] = value % 10;
                    
                    value /= 10;
                }
                
                s32 i = 0;
                
                if( neg )
                {
                    chrs[ i++ ] = '-';
                }
                
                for( tmp = 1; tmp <= digits; tmp++ )
                {
                    chrs[ i++ ] = ( c8 )(digs[ tmp ] + 48);
                }
                
                chrs[ i ] = '\0';
                
                Append( chrs );
            }
            
            return *this;
        }
        
        StringObject<T>& operator + ( f32 value )
        {
            c8 chrs[ 256 ];
            
            snprintf( chrs, 256, "%f", value );
            
            Append( chrs );
            
            return *this;
        }
    };
    
    typedef StringObject<StringData16> String16;
    typedef StringObject<StringData256> String256;
    typedef StringObject<StringData4096> String4096;
    typedef StringObject<StringData4096> String;
}

#endif