#include "core.h"

namespace core
{
	c8* strstr( const c8* s1, const c8* s2 )
	{
		ASSERT( s1 && s2 );
		
		u32 size1 = strlen( s1 );
		u32 size2 = strlen( s2 );
		
		while ( size1-- >= size2 )
		{
			if ( !core::compare( s1, s2, size2 ) )
			{
				return ( c8* )s1;
			}
			s1++;
		}
		
		return kNull;
	}
	
	u32 strlen( const c8* s )
	{
        if(s == kNull)
        {
            return 0;
        }
        else
        {
            const c8* c = s;
            
            while( *c != '\0' )
            {
                c++;
            }
            
            return ( u32 )( c - s );
        }
	}
	
	s32 strcmp( const c8* s1, const c8* s2 )
	{
		const c8* c1 = s1;
		const c8* c2 = s2;
		
		while( *c1 && *c2 )
		{
			c8 a = *c1++;
			c8 b = *c2++;
			
			if( a > b )
			{
				return 1;
			}
			
			if( a < b )
			{
				return -1;
			}
		}

		if( *c1 )
		{
			return 1;
		}
		
		if( *c2 )
		{
			return -1;
		}
		
		return 0;
	}
	
	c8* strcpy( c8* s1, const c8* s2, s32 n )
	{
		while( *s2 != '\0' && --n >= 0 )
		{
			*s1++ = *s2++;
		}
		
		*s1 = '\0';
		
		return s1;
	}
}
