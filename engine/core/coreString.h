#ifndef CORE_STRING
#define CORE_STRING

namespace core
{
	c8* strstr( const c8* s1, const c8* s2 );
	u32 strlen( const c8* s );
	s32 strcmp( const c8* s1, const c8* s2 );
	c8* strcpy( c8* s1, const c8* s2, s32 n );
}

#endif