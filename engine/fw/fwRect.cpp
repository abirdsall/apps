#include "fw.h"

namespace fw
{
	Rect::Rect( f32 xMin, f32 yMin, f32 xMax, f32 yMax )
	{
		mXMin = xMin;
		mYMin = yMin;
		mXMax = xMax;
		mYMax = yMax;
	}
		
	v2 Rect::Min() const
	{
		return v2( mXMin, mYMin );
	}
	
	v2 Rect::Mid() const
	{
		return ( Min() + Max() ) * 0.5f;
	}
	
	v2 Rect::Max() const
	{
		return v2( mXMax, mYMax );
	}
	
	v2 Rect::Size() const
	{
		return Max() - Min();
	}
	
	v2 Rect::Fraction( const v2& position ) const
	{
		f32 fx = position.x - mXMin;
		f32 fy = position.y - mYMin;
		f32 sx = mXMax - mXMin;
		f32 sy = mYMax - mYMin;
		return v2(sx > 0.0f ? fx / sx : 0.0f, sy > 0.0f ? fy / sy : 0.0f);
	}
	
	Rect Rect::Fix() const
	{
		return Rect( core::min( mXMin, mXMax ), core::min( mYMin, mYMax ), core::max( mYMin, mYMax ), core::max( mXMin, mXMax ) );
	}
    
    v4 Rect::v4() const
    {
        return ::v4( mXMin, mYMin, mXMax, mYMax );
    }

	void Rect::Subdivide( Rect* rects, u32 count ) const
	{
		ASSERT( count > 0 );
		u32 xCount = core::sqrt( count );
		u32 yCount = xCount;
		if( ( xCount * xCount ) < count )
		{
			xCount++;
			yCount++;
		}
		f32 xSize = ( mXMax - mXMin ) / xCount;
		f32 ySize = ( mYMax - mYMin ) / yCount;
		f32 y = mYMin;
		u32 index = 0;
		for( u32 i = 0; i < yCount; i++ )
		{
			f32 x = mXMin;
			for( u32 j = 0; j < xCount; j++ )
			{
				Rect& rect = rects[ index ];
				rect.mXMin = x;
				rect.mYMin = y;
				rect.mXMax = x + xSize;
				rect.mYMax = y + ySize;
				x += xSize;
				index++;
			}
			y += ySize;
		}
	}
}