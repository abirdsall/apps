#include "fw.h"

namespace fw
{
    u16* CubeGenElements( u16* p, u16 offset )
    {
        *p ++=  0 + offset;
        *p ++=  6 + offset;
        *p ++= 18 + offset;
        *p ++= 18 + offset;
        *p ++= 12 + offset;
        *p ++=  0 + offset;
        
        *p ++=  3 + offset;
        *p ++= 15 + offset;
        *p ++= 21 + offset;
        *p ++= 21 + offset;
        *p ++=  9 + offset;
        *p ++=  3 + offset;
        
        *p ++=  1 + offset;
        *p ++=  4 + offset;
        *p ++= 10 + offset;
        *p ++= 10 + offset;
        *p ++=  7 + offset;
        *p ++=  1 + offset;
        
        *p ++= 13 + offset;
        *p ++= 19 + offset;
        *p ++= 22 + offset;
        *p ++= 22 + offset;
        *p ++= 16 + offset;
        *p ++= 13 + offset;
        
        *p ++=  2 + offset;
        *p ++= 14 + offset;
        *p ++= 17 + offset;
        *p ++= 17 + offset;
        *p ++=  5 + offset;
        *p ++=  2 + offset;
        
        *p ++=  8 + offset;
        *p ++= 11 + offset;
        *p ++= 23 + offset;
        *p ++= 23 + offset;
        *p ++= 20 + offset;
        *p ++=  8 + offset;
        
        return p;
    }

    f32* CubeGenPositionsNormalsColours( f32* p, const v3& position, const v3& radius, const v4& colour )
    {
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++=-1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++=-1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= 0.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= 0.0f;
        *p ++= 0.0f;
        *p ++= 1.0f;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        return p;
    }
    
    f32* CubeGenPositionsColours( f32* p, const v3& position, const v3& radius, const v4& colour )
    {
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= colour.x;
        *p ++= colour.y;
        *p ++= colour.z;
        *p ++= colour.w;
        
        return p;
    }
    
    f32* CubeGenPositions( f32* p, const v3& position, const v3& radius )
    {
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y - radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z - radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x - radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        *p ++= position.x + radius.x;
        *p ++= position.y + radius.y;
        *p ++= position.z + radius.z;
        
        return p;
    }
}