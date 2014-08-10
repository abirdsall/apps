#ifndef FW_MATERIAL
#define FW_MATERIAL

namespace fw
{
	class Material
	{
	private:
		v4 mColour;

	public:
		Material();
		Material( v4 colour );
		
		inline const v4& Colour() const
		{
			return mColour;
		}
	};
}

#endif
