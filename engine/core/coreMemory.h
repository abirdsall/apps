#ifndef CORE_MEMORY
#define CORE_MEMORY

namespace core
{
	void* alloc(size_t size);
	void xfree(void* ptr);
	void set( void* dataDst, s32 dataValue, s32 size );
	void copy( void* dataDst, const void* dataSrc, s32 size );
	void move( void* dataDst, const void* dataSrc, s32 size );
	s32 compare( const void* dataA, const void* dataB, s32 size );
}

#endif