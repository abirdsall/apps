#ifndef FW_OBJECT_FACTORY
#define FW_OBJECT_FACTORY

namespace fw
{	
	class ObjectFactory
	{
	public:
		virtual void Init( s32 limit );
		virtual ObjectHandle Add( const Object& object );
		virtual void Kill();
		
	protected:
		Object* mObjects;
		
		s32 mCount;
		s32 mLimit;

		virtual size_t ObjectSize() = 0;
	};
}

#endif
