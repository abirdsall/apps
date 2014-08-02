#ifndef OS_MEMORY
#define OS_MEMORY

#include "os.h"

namespace memory
{
	void* alloc(size_t size);
	void free(void* ptr);
	void set( void* dataDst, const s32 dataValue, s32 size );
	void copy( void* dataDst, const void* dataSrc, s32 size );
	void move( void* dataDst, const void* dataSrc, s32 size );
	s32 compare( const void* dataA, const void* dataB, s32 size );
}

#endif