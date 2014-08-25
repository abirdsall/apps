#ifndef CORE_STRING
#define CORE_STRING

namespace core
{
	c8* strstr( const c8* s1, const c8* s2 );
	u32 strlen( const c8* s );
	s32 strcmp( const c8* s1, const c8* s2 );
	c8* strcpy( c8* s1, const c8* s2, s32 n );
    
    class String
    {
    private:
        
        Array<c8> _data;
        
        void Append( const c8* string )
        {
            u32 additionalCount = core::strlen( string );
            
            s32 nullLocation = _data.Count() - 1;
            
            _data.SetCount( _data.Count() + additionalCount );
            
            core::strcpy( _data.Data() + nullLocation, string, additionalCount );
        }
        
    public:
        
        String()
        {
            _data.SetCount( 1 );
        }
        
        String( const c8* s )
        {
            _data.SetCount( 1 );
            
            Append( s );
        }
        
        s32 Size()
        {
            return _data.Count() - 1;
        }
        
        c8 operator [] ( const s32 i )
        {
            return _data[ i ];
        }
        
        const c8* toStr()
        {
            _data.Data()[ _data.Count() - 1 ] = '\0';
            
            return _data.Data();
        }
        
        String& operator = ( const c8* s )
        {
            _data.SetCount( 1 );
            
            Append( s );
            
            return *this;
        }
        
        String& operator += ( const c8* s )
        {
            Append( s );
            
            return *this;
        }
        
        String& operator + ( const c8* s )
        {
            *this += s;
            
            return *this;
        }
        
        String& operator + ( s32 value )
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
        
        String& operator + ( f32 value )
        {
            c8 chrs[ 256 ];
            
            snprintf( chrs, 256, "%f", value );
            
            Append( chrs );
            
            return *this;
        }
    };
}

#endif