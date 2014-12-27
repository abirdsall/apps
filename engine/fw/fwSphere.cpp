#include "fw.h"

namespace fw
{
    u16* SphereGenElements( u16* p, u16 offset )
    {
        *p ++=  0 + offset;
        *p ++= 11 + offset;
        *p ++=  5 + offset;
        *p ++=  0 + offset;
        *p ++=  5 + offset;
        *p ++=  1 + offset;
        *p ++=  0 + offset;
        *p ++=  1 + offset;
        *p ++=  7 + offset;
        *p ++=  0 + offset;
        *p ++=  7 + offset;
        *p ++= 10 + offset;
        *p ++=  0 + offset;
        *p ++= 10 + offset;
        *p ++= 11 + offset;

        *p ++=  1 + offset;
        *p ++=  5 + offset;
        *p ++=  9 + offset;
        *p ++=  5 + offset;
        *p ++= 11 + offset;
        *p ++=  4 + offset;
        *p ++= 11 + offset;
        *p ++= 10 + offset;
        *p ++=  2 + offset;
        *p ++= 10 + offset;
        *p ++=  7 + offset;
        *p ++=  6 + offset;
        *p ++=  7 + offset;
        *p ++=  1 + offset;
        *p ++=  8 + offset;
        
        *p ++=  3 + offset;
        *p ++=  9 + offset;
        *p ++=  4 + offset;
        *p ++=  3 + offset;
        *p ++=  4 + offset;
        *p ++=  2 + offset;
        *p ++=  3 + offset;
        *p ++=  2 + offset;
        *p ++=  6 + offset;
        *p ++=  3 + offset;
        *p ++=  6 + offset;
        *p ++=  8 + offset;
        *p ++=  3 + offset;
        *p ++=  8 + offset;
        *p ++=  9 + offset;
        
        *p ++=  4 + offset;
        *p ++=  9 + offset;
        *p ++=  5 + offset;
        *p ++=  2 + offset;
        *p ++=  4 + offset;
        *p ++= 11 + offset;
        *p ++=  6 + offset;
        *p ++=  2 + offset;
        *p ++= 10 + offset;
        *p ++=  8 + offset;
        *p ++=  6 + offset;
        *p ++=  7 + offset;
        *p ++=  9 + offset;
        *p ++=  8 + offset;
        *p ++=  1 + offset;
        
        return p;
    }
    
    f32* SphereGenPositionsNormalsColours( f32* p, const v3& position, const v3& radius, const v4& colour )
    {
        f32 t = ( 1.0f + core::sqrt( 5.0f ) ) / 2.0f;
        
        v3 v[ 12 ] =
        {
            normalise( v3( -1.0f, t, 0.0f ) ),
            normalise( v3( 1.0f, t, 0.0f ) ),
            normalise( v3( -1.0f, -t, 0.0f ) ),
            normalise( v3( 1.0f, -t, 0.0f ) ),
            normalise( v3( 0.0f, -1.0f, t ) ),
            normalise( v3( 0.0f, 1.0f, t ) ),
            normalise( v3( 0.0f, -1.0f, -t ) ),
            normalise( v3( 0.0f, 1.0f, -t ) ),
            normalise( v3( t, 0.0f, -1.0f ) ),
            normalise( v3( t, 0.0f, 1.0f ) ),
            normalise( v3( -t, 0.0f, -1.0f ) ),
            normalise( v3( -t, 0.0f, 1.0f ) )
        };
        
        for( s32 i = 0; i < 12; i++ )
        {
            *p ++= v[ i ].x * radius.x;
            *p ++= v[ i ].y * radius.y;
            *p ++= v[ i ].z * radius.z;
            *p ++= v[ i ].x;
            *p ++= v[ i ].y;
            *p ++= v[ i ].z;
            *p ++= colour.x;
            *p ++= colour.y;
            *p ++= colour.z;
            *p ++= colour.w;
        }
        
        return p;
    }
    
    f32* SphereGenPositionsColours( f32* p, const v3& position, const v3& radius, const v4& colour )
    {
        f32 t = ( 1.0f + core::sqrt( 5.0f ) ) / 2.0f;
        
        v3 v[ 12 ] =
        {
            normalise( v3( -1.0f, t, 0.0f ) ),
            normalise( v3( 1.0f, t, 0.0f ) ),
            normalise( v3( -1.0f, -t, 0.0f ) ),
            normalise( v3( 1.0f, -t, 0.0f ) ),
            normalise( v3( 0.0f, -1.0f, t ) ),
            normalise( v3( 0.0f, 1.0f, t ) ),
            normalise( v3( 0.0f, -1.0f, -t ) ),
            normalise( v3( 0.0f, 1.0f, -t ) ),
            normalise( v3( t, 0.0f, -1.0f ) ),
            normalise( v3( t, 0.0f, 1.0f ) ),
            normalise( v3( -t, 0.0f, -1.0f ) ),
            normalise( v3( -t, 0.0f, 1.0f ) )
        };
        
        for( s32 i = 0; i < 12; i++ )
        {
            *p ++= v[ i ].x * radius.x;
            *p ++= v[ i ].y * radius.y;
            *p ++= v[ i ].z * radius.z;
            *p ++= colour.x;
            *p ++= colour.y;
            *p ++= colour.z;
            *p ++= colour.w;
        }
        
        return p;
    }
    
    f32* SphereGenPositions( f32* p, const v3& position, const v3& radius )
    {
        f32 t = ( 1.0f + core::sqrt( 5.0f ) ) / 2.0f;
        
        v3 v[ 12 ] =
        {
            normalise( v3( -1.0f, t, 0.0f ) ),
            normalise( v3( 1.0f, t, 0.0f ) ),
            normalise( v3( -1.0f, -t, 0.0f ) ),
            normalise( v3( 1.0f, -t, 0.0f ) ),
            normalise( v3( 0.0f, -1.0f, t ) ),
            normalise( v3( 0.0f, 1.0f, t ) ),
            normalise( v3( 0.0f, -1.0f, -t ) ),
            normalise( v3( 0.0f, 1.0f, -t ) ),
            normalise( v3( t, 0.0f, -1.0f ) ),
            normalise( v3( t, 0.0f, 1.0f ) ),
            normalise( v3( -t, 0.0f, -1.0f ) ),
            normalise( v3( -t, 0.0f, 1.0f ) )
        };
        
        for( s32 i = 0; i < 12; i++ )
        {
            *p ++= v[ i ].x * radius.x;
            *p ++= v[ i ].y * radius.y;
            *p ++= v[ i ].z * radius.z;
        }
        
        return p;
    }
}