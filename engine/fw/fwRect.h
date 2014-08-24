#ifndef FW_RECT
#define FW_RECT

namespace fw
{
	class Rect
	{
	private:
        v2 _min;
        v2 _max;

	public:
        Rect()
        {
            _min = v2( 0.0f, 0.0f );
            _max = v2( 1.0f, 1.0f );
        }
        
        Rect( f32 x1, f32 y1, f32 x2, f32 y2 )
        {
            _min = v2( x1, y1 );
            _max = v2( x2, y2 );
        }

        Rect( const v2& min, const v2& max )
        {
            _min = min;
            _max = max;
        }

        inline const v2& Min() const
        {
            return _min;
        }
        
        inline const v2& Max() const
        {
            return _max;
        }

        inline v2 Mid() const
        {
            return _min + _max * 0.5f;
        }

        inline v2 Size() const
        {
            return _max - _min;
        }
        
        inline v2 Fraction( const v2& v ) const
        {
            v2 size = Size();
            
            return v2(size.x > 0.0f ? ( v.x - _min.x ) / size.x : 0.0f,
                      size.y > 0.0f ? ( v.y - _min.y ) / size.y : 0.0f );
        }
        
        inline v4 v4() const
        {
            return core::v4( _min.x, _min.y, _max.x, _max.y );
        }
        
        inline Rect Corrected() const
        {
            return Rect( core::min( _min, _max ), core::max( _min, _max ) );
        }
        
        inline void Subdivide( Rect* rects, u32 count ) const
        {
            ASSERT( count > 0 );
            
            u32 xCount = core::sqrt( count );
            u32 yCount = xCount;
            
            if( ( xCount * xCount ) < count )
            {
                xCount++;
                yCount++;
            }
            
            f32 xSize = ( _max.x - _min.x ) / xCount;
            f32 ySize = ( _max.y - _min.y ) / yCount;
            f32 y = _min.y;
            
            u32 index = 0;
            
            for( u32 i = 0; i < yCount; i++ )
            {
                f32 x = _min.x;
            
                for( u32 j = 0; j < xCount; j++ )
                {
                    Rect& rect = rects[ index ];
                 
                    rect._min.x = x;
                    rect._min.y = y;
                    rect._max.x = x + xSize;
                    rect._max.y = y + ySize;
                    
                    x += xSize;
                    
                    index++;
                }
                y += ySize;
            }
        }
	};
}

#endif
