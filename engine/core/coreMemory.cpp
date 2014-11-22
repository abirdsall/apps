#include "core.h"

namespace core
{
	void* alloc( size_t size )
	{
        return malloc( size );
	}
	
	void xfree( void* ptr )
	{
		free( ptr );
	}

	void set( void* dataDst, const s32 dataValue, s32 size )
	{
		u8* dst = ( u8* )dataDst;
		
		while ( size-- )
		{
			*dst++ = dataValue;
		}
	}
	
	void copy( void* dataDst, const void* dataSrc, s32 size )
	{
		u8* dst = ( u8* )dataDst;
		u8* src = ( u8* )dataSrc;
		
		while ( size-- )
		{
			*dst++ = *src++;
		}
	}
	
	void move( void* dataDst, const void* dataSrc, s32 size )
	{
		u8* dst = ( u8* )dataDst;
		u8* src = ( u8* )dataSrc;
		
		if ( dst <= src || dst >= ( src + size ) )
		{
			while ( size-- )
			{
				*dst++ = *src++;
			}
		}
		else
		{
			dst += ( size - 1 );
			src += ( size - 1 );
			
			while ( size-- )
			{
				*dst-- = *src--;
			}
		}
	}
	
	s32 compare( const void* dataA, const void* dataB, s32 size )
	{
		const u8* a = ( u8* )dataA;
		const u8* b = ( u8* )dataB;
		
		while ( size-- )
		{
			u8 aValue = *a++;
			u8 bValue = *b++;
			
			if ( aValue < bValue )
			{
				return -1;
			}
			
			if ( aValue > bValue )
			{
				return 1;
			}
		}
		return 0;
	}
}
