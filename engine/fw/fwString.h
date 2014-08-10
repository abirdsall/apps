#ifndef FW_STRING
#define FW_STRING

namespace fw
{
	class String
	{
	private:
        
		static const u32 kSizeLimit = 4096;

		c8 mData[ kSizeLimit + 1 ]; // +1 to allow space for null when toStr() called

        u32 mSize;

		void Append( const c8* s )
		{
			u32 size = os::strlen( s );
            
			ASSERT( ( mSize + size ) < kSizeLimit );
			
            os::strcpy( mData + mSize, s, size );
			
            mSize += size;
		}
		
	public:
		String() : mSize( 0 )
		{
		}
		
		String( const c8* s ) : mSize( 0 )
		{
			Append( s );
		}
		
		const c8* toStr()
		{
			mData[ mSize ] = '\0';
            
			return mData;
		}
			
		inline String& operator += ( const c8* s )
		{
			Append( s );
            
			return *this;
		}
		
		inline String& operator + ( const c8* s )
		{
			*this += s;
            
			return *this;
		}
		
		inline String& operator + ( s32 value )
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
        
        inline String& operator + ( f32 value )
        {
            c8 chrs[ 256 ];

            snprintf( chrs, 256, "%f", value );
            
            Append( chrs );
            
            return *this;
        }
	};
}
#endif