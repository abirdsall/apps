#ifndef FW_RECT
#define FW_RECT

namespace fw
{
	class Rect
	{
	private:
		f32 mXMin;
		f32 mYMin;
		f32 mXMax;
		f32 mYMax;

	public:
		Rect(){}
		Rect( f32 xMin, f32 yMin, f32 xMax, f32 yMax );
		v2 Min() const;
		v2 Mid() const;
		v2 Max() const;
		v2 Size() const;
		v2 Fraction( const v2& position ) const;
		Rect Fix() const;
        v4 v4() const;
		void Subdivide( Rect* rects, u32 count ) const;
		
		inline const f32& MinX() const
		{
			return mXMin;
		}
		
		inline const f32& MinY() const
		{
			return mYMin;
		}
		
		inline const f32& MaxX() const
		{
			return mXMax;
		}
		
		inline const f32& MaxY() const
		{
			return mYMax;
		}
	};
}

#endif
