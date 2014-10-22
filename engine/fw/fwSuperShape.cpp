#include "fw.h"

namespace fw
{
    static f32 SuperShape(const f32 t, const f32 m, const f32 a, const f32 b, const f32 n1, const f32 n2, const f32 n3)
    {
        return ( f32 )pow( 1e-12f + pow( abs( cos( m * t / 4.0f ) / a ), n2 ) + pow( abs( sin( m * t / 4.0f ) / b ), n3 ), -1.0f / n1 );
    }
/*
    DrawBatchHandle SuperShapeBatch()
    {
        DrawBatchHandle batchHandle = DrawBatchNew(1, 6, 4, 2, 0, 4, 0);
        
        SuperShapeGen(
                      DrawBatchElementPtr( batchHandle ),
                      DrawBatchVertexCount( batchHandle ),
                      DrawBatchVertexPtr( batchHandle ),
        );
        
        DrawBatchIncrement( batchHandle );

        return batchHandle;
    }
    */
    void SuperShapeGen( u16* e, u16 offset, f32* v, u16 detail,
                       const v3& position,
                       const v3& radius,
                       const v4& colour,
                       f32 ss1m, f32 ss1a, f32 ss1b, f32 ss1n1, f32 ss1n2, f32 ss1n3,
                       f32 ss2m, f32 ss2a, f32 ss2b, f32 ss2n1, f32 ss2n2, f32 ss2n3 )
    {
        u16 detailHalf = detail / 2;
        f32 rDetailFloat = 1.0f / ( f32 )detail;
        f32 detailPi = rDetailFloat * PiMul2;
        
        ASSERT(detail >= 4);
        
        //s32 elementCount = detail * ( detail / 2 ) * 6;
        //s32 vertexCount = detail * ( ( detail / 2 ) + 1 );
        
        for( u16 j = 0; j <= detailHalf; j++ )
        {
            f32 t2 = ( f32 )j * detailPi - PiDiv2;
            f32 r2 = SuperShape( t2, ss2m, ss2a, ss2b, ss2n1, ss2n2, ss2n3 );
            
            for( u16 i = 0; i < detail; i++ )
            {
                f32 t1	= ( f32 )i * detailPi - Pi;
                f32 r1	= SuperShape( t1, ss1m, ss1a, ss1b, ss1n1, ss1n2, ss1n3 );
                f32 x = cos( t1 ) * r1 * cos( t2 ) * r2;
                f32 z = sin( t1 ) * r1 * cos( t2 ) * r2;
                f32 y = sin( t2 ) * r2;
                *v ++= position.x + radius.x * x;
                *v ++= position.y + radius.y * y;
                *v ++= position.z + radius.z * z;
                *v ++= ( abs( x ) * 0.5f + 0.5f );
                *v ++= ( abs( y ) * 0.5f + 0.5f );
                *v ++= ( abs( z ) * 0.5f + 0.5f );
                *v ++= colour.x;
                *v ++= colour.y;
                *v ++= colour.z;
                *v ++= colour.w;
                
                if( i < detail && j < detailHalf )
                {
                    u16 in = ( i + 1 == detail ? 0 : i + 1 );
                    u16 jn = j + 1;
                    
                    //a b
                    //c d
                    
                    u16 a = j * detail + i;
                    u16 b = j * detail + in;
                    u16 c = jn * detail + i;
                    u16 d = jn * detail + in;
                    
                    *e ++= d;
                    *e ++= c;
                    *e ++= a;
                    *e ++= a;
                    *e ++= b;
                    *e ++= d;
                }
            }
        }
    }
    
    void SuperShapeGenRadiosity( u16* e, u16 offset, f32* v, u16 detail,
                                const v3& position,
                                const v3& radius,
                                const v4& colour,
                                f32 ss1m, f32 ss1a, f32 ss1b, f32 ss1n1, f32 ss1n2, f32 ss1n3,
                                f32 ss2m, f32 ss2a, f32 ss2b, f32 ss2n1, f32 ss2n2, f32 ss2n3 )
    {
        u16 detailHalf = detail / 2;
        f32 rDetailFloat = 1.0f / ( f32 )detail;
        f32 detailPi = rDetailFloat * PiMul2;
        
        ASSERT(detail >= 4);
        
        for( u16 j = 0; j <= detailHalf; j++ )
        {
            f32 t2 = ( f32 )j * detailPi - PiDiv2;
            f32 r2 = SuperShape( t2, ss2m, ss2a, ss2b, ss2n1, ss2n2, ss2n3 );
            
            for( u16 i = 0; i < detail; i++ )
            {
                f32 t1	= ( f32 )i * detailPi - Pi;
                f32 r1	= SuperShape( t1, ss1m, ss1a, ss1b, ss1n1, ss1n2, ss1n3 );
                f32 x = cos( t1 ) * r1 * cos( t2 ) * r2;
                f32 z = sin( t1 ) * r1 * cos( t2 ) * r2;
                f32 y = sin( t2 ) * r2;
                *v ++= position.x + radius.x * x;
                *v ++= position.y + radius.y * y;
                *v ++= position.z + radius.z * z;
                *v ++= colour.x;
                *v ++= colour.y;
                *v ++= colour.z;
                *v ++= colour.w;
                *v ++= position.z;
                *v ++= radius.z;
                
                if( i < detail && j < detailHalf )
                {
                    u16 in = ( i + 1 == detail ? 0 : i + 1 );
                    u16 jn = j + 1;
                    
                    //a b
                    //c d
                    
                    u16 a = offset + j * detail + i;
                    u16 b = offset + j * detail + in;
                    u16 c = offset + jn * detail + i;
                    u16 d = offset + jn * detail + in;
                    
                    *e ++= d;
                    *e ++= c;
                    *e ++= a;
                    *e ++= a;
                    *e ++= b;
                    *e ++= d;
                }
            }
        }
    }
}