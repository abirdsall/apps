#ifndef CORE_MATRIX
#define CORE_MATRIX

namespace core
{
    struct m4;
    
    inline v3 multiply33( const m4& m, const v3& v );
    
    struct m4
    {
        v4 rows[ 4 ];
        
        m4()
        {
        }
        
        m4( const v4& r0, const v4& r1, const v4& r2, const v4& r3 )
        {
            rows[ 0 ] = r0;
            rows[ 1 ] = r1;
            rows[ 2 ] = r2;
            rows[ 3 ] = r3;
        }
        
        m4(	f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22, f32 m23, f32 m30, f32 m31, f32 m32, f32 m33 )
        {
            rows[ 0 ] = v4( m00, m01, m02, m03 );
            rows[ 1 ] = v4( m10, m11, m12, m13 );
            rows[ 2 ] = v4( m20, m21, m22, m23 );
            rows[ 3 ] = v4( m30, m31, m32, m33 );
        }
        
        inline void translate( const v3 &v )
        {
            rows[ 0 ].w += dot( rows[ 0 ].xyz(), v );
            rows[ 1 ].w += dot( rows[ 1 ].xyz(), v );
            rows[ 2 ].w += dot( rows[ 2 ].xyz(), v );
            rows[ 3 ].w += dot( rows[ 3 ].xyz(), v );
        }
        
        inline void stabilise()
        {
            ( v3& )rows[ 0 ] = normalise( ( v3& )rows[ 0 ] );
            ( v3& )rows[ 2 ] = normalise( cross( ( v3& )rows[ 0 ], ( v3& )rows[ 1 ] ) );
            ( v3& )rows[ 1 ] = cross( ( v3& )rows[ 2 ], ( v3& )rows[ 0 ] );
        }
        
        v3 getPosition()
        {
            //return rows[ 0 ].w * rows[ 0 ].xyz() + rows[ 1 ].w * rows[ 1 ].xyz() + rows[ 2 ].w * rows[ 2 ].xyz();
            return - rows[ 0 ].w * rows[ 0 ].xyz() - rows[ 1 ].w * rows[ 1 ].xyz() - rows[ 2 ].w * rows[ 2 ].xyz();
        }
        
        void setPosition( const v3& position )
        {
            rows[ 0 ].w = dot( rows[ 0 ].xyz(), position );
            rows[ 1 ].w = dot( rows[ 1 ].xyz(), position );
            rows[ 2 ].w = dot( rows[ 2 ].xyz(), position );
            rows[ 3 ].w = 1.0f;
        }

        void setScale( const v3& scale )
        {
            rows[ 0 ].x = scale.x;
            rows[ 1 ].y = scale.y;
            rows[ 2 ].z = scale.z;
        }
        
        operator f32 *() const
        {
            return ( f32* )&rows[ 0 ].x;
        }
    };
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Remove these
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    struct r4 : m4
    {
        r4( const v3& a, f32 b )
        {
            roll( a, b );
        }
        
        r4( const v3& a, const v3& b, f32 c = 1.0f )
        {
            f32 d = dot( a, b );
            if( d < 1.0f )
            {
                roll( normalise( cross( b, a ) ), core::acos( d ) * c );
            }
            else
            {
                rows[ 0 ] = v4( 1.0f, 0.0f, 0.0f, 0.0f );
                rows[ 1 ] = v4( 0.0f, 1.0f, 0.0f, 0.0f );
                rows[ 2 ] = v4( 0.0f, 0.0f, 1.0f, 0.0f );
                rows[ 3 ] = v4( 0.0f, 0.0f, 0.0f, 1.0f );
            }
        }
        
        inline void roll( const v3& a, const f32 b )
        {
            f32 c = core::cos( b );
            f32 s = core::sin( b );
            f32 v = 1.0f - c;
            f32 vxx = v * a.x * a.x;
            f32 vxy = v * a.x * a.y;
            f32 vxz = v * a.x * a.z;
            f32 vyy = v * a.y * a.y;
            f32 vyz = v * a.y * a.z;
            f32 vzz = v * a.z * a.z;
            f32 sx = s * a.x;
            f32 sy = s * a.y;
            f32 sz = s * a.z;
            
            rows[ 0 ].x = vxx + c;
            rows[ 0 ].y = vxy + sz;
            rows[ 0 ].z = vxz - sy;
            rows[ 0 ].w = 0.0f;
            rows[ 1 ].x = vxy - sz;
            rows[ 1 ].y = vyy + c;
            rows[ 1 ].z = vyz + sx;
            rows[ 1 ].w = 0.0f;
            rows[ 2 ].x = vxz + sy;
            rows[ 2 ].y = vyz - sx;
            rows[ 2 ].z = vzz + c;
            rows[ 2 ].w = 0.0f;
            rows[ 3 ].x = 0.0f;
            rows[ 3 ].y = 0.0f;
            rows[ 3 ].z = 0.0f;
            rows[ 3 ].w = 1.0f;
        }
    };

    inline m4 multiply33( const m4& m, const m4& n )
    {
        return m4(
                  m.rows[ 0 ].x * n.rows[ 0 ].x + m.rows[ 0 ].y * n.rows[ 1 ].x + m.rows[ 0 ].z * n.rows[ 2 ].x,
                  m.rows[ 0 ].x * n.rows[ 0 ].y + m.rows[ 0 ].y * n.rows[ 1 ].y + m.rows[ 0 ].z * n.rows[ 2 ].y,
                  m.rows[ 0 ].x * n.rows[ 0 ].z + m.rows[ 0 ].y * n.rows[ 1 ].z + m.rows[ 0 ].z * n.rows[ 2 ].z,
                  0.0f,
                  m.rows[ 1 ].x * n.rows[ 0 ].x + m.rows[ 1 ].y * n.rows[ 1 ].x + m.rows[ 1 ].z * n.rows[ 2 ].x,
                  m.rows[ 1 ].x * n.rows[ 0 ].y + m.rows[ 1 ].y * n.rows[ 1 ].y + m.rows[ 1 ].z * n.rows[ 2 ].y,
                  m.rows[ 1 ].x * n.rows[ 0 ].z + m.rows[ 1 ].y * n.rows[ 1 ].z + m.rows[ 1 ].z * n.rows[ 2 ].z,
                  0.0f,
                  m.rows[ 2 ].x * n.rows[ 0 ].x + m.rows[ 2 ].y * n.rows[ 1 ].x + m.rows[ 2 ].z * n.rows[ 2 ].x,
                  m.rows[ 2 ].x * n.rows[ 0 ].y + m.rows[ 2 ].y * n.rows[ 1 ].y + m.rows[ 2 ].z * n.rows[ 2 ].y,
                  m.rows[ 2 ].x * n.rows[ 0 ].z + m.rows[ 2 ].y * n.rows[ 1 ].z + m.rows[ 2 ].z * n.rows[ 2 ].z,
                  0.0f,
                  m.rows[ 3 ].x * n.rows[ 0 ].x + m.rows[ 3 ].y * n.rows[ 1 ].x + m.rows[ 3 ].z * n.rows[ 2 ].x,
                  m.rows[ 3 ].x * n.rows[ 0 ].y + m.rows[ 3 ].y * n.rows[ 1 ].y + m.rows[ 3 ].z * n.rows[ 2 ].y,
                  m.rows[ 3 ].x * n.rows[ 0 ].z + m.rows[ 3 ].y * n.rows[ 1 ].z + m.rows[ 3 ].z * n.rows[ 2 ].z,
                  1.0f);
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Final
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    inline m4 identity4()
    {
        return m4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    inline m4 translate( const v3 &v )
    {
        return m4( 1.0f, 0.0f, 0.0f, v.x, 0.0f, 1.0f, 0.0f, v.y, 0.0f, 0.0f, 1.0f, v.z, 0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline m4 scale( const v3 &v )
    {
        return m4( v.x, 0.0f, 0.0f, 0.0f, 0.0f, v.y, 0.0f, 0.0f, 0.0f, 0.0f, v.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline m4 rotateX( const f32 angle )
    {
        f32 cosA = core::cos( angle );
        f32 sinA = core::sin( angle );
        
        return m4( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosA, -sinA, 0.0f, 0.0f, sinA, cosA, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline m4 rotateY( const f32 angle )
    {
        f32 cosA = core::cos( angle );
        f32 sinA = core::sin( angle );
        
        return m4( cosA, 0.0f, -sinA, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinA, 0.0f, cosA, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline m4 rotateZ( const f32 angle )
    {
        f32 cosA = core::cos( angle );
        f32 sinA = core::sin( angle );
        
        return m4( cosA, -sinA, 0.0f, 0.0f, sinA, cosA, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline m4 rotateXY( const f32 angleX, const f32 angleY )
    {
        f32 cosX = core::cos( angleX );
        f32 sinX = core::sin( angleX );
        f32 cosY = core::cos( angleY );
        f32 sinY = core::sin( angleY );
        
        return m4( cosY, 0.0f, -sinY, 0.0f, -sinX * sinY, cosX, -sinX * cosY, 0.0f, cosX * sinY, sinX, cosX * cosY, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline m4 rotateZXY( const f32 angleX, const f32 angleY, const f32 angleZ )
    {
        f32 cosX = core::cos( angleX );
        f32 sinX = core::sin( angleX );
        f32 cosY = core::cos( angleY );
        f32 sinY = core::sin( angleY );
        f32 cosZ = core::cos( angleZ );
        f32 sinZ = core::sin( angleZ );
        return m4(
                  cosY * cosZ + sinX * sinY * sinZ,
                  -cosX * sinZ,
                  sinX * cosY * sinZ - sinY * cosZ,
                  0.0f,
                  cosY * sinZ - sinX * sinY * cosZ,
                  cosX * cosZ,
                  -sinY * sinZ - sinX * cosY * cosZ,
                  0.0f,
                  cosX * sinY,
                  sinX,
                  cosX * cosY,
                  0.0f,
                  0.0f,
                  0.0f,
                  0.0f,
                  1.0f );
    }
    
    inline m4 perspective( const f32 fovX, const f32 aspect, const f32 n, const f32 f )
    {
        f32 x = core::cos( 0.5f * fovX ) / core::sin( 0.5f * fovX );
        f32 y = x / aspect;
        f32 fn = 1.0f / ( f - n );
        
        return m4( x, 0.0f, 0.0f, 0.0f, 0.0f, y, 0.0f, 0.0f, 0.0f, 0.0f, ( f + n ) * fn, -2.0f * f * n * fn, 0.0f, 0.0f, 1.0f, 0.0f );
    }
    
    inline m4 orthogonal( const f32 l, const f32 r, const f32 t, const f32 b, const f32 n, const f32 f )
    {
        f32 rl = 1.0f / ( r - l );
        f32 tb = 1.0f / ( t - b );
        f32 fn = 1.0f / ( f - n );
        
        return m4( 2.0f * rl, 0.0f, 0.0f, -( r + l ) * rl, 0.0f, 2.0f * tb, 0.0f, -( t + b ) * tb, 0.0f, 0.0f, -2.0f * fn, -( f + n ) * fn, 0.0f, 0.0f, 0.0f, 1.0f );
    }
    
    inline m4 look( const v3& from, const v3& to, const v3& up )
    {
        v3 z = normalise( to - from );
        v3 x = -normalise( cross( up, z ) );
        v3 y = -cross( z, x );
        m4 m( x, y, z, v4( V3Zero ) );
        
        m.setPosition( -from );
        
        return m;
    }

    inline m4 transpose( const m4& m )
    {
        return m4(
                  m.rows[ 0 ].x, m.rows[ 1 ].x, m.rows[ 2 ].x, m.rows[ 3 ].x,
                  m.rows[ 0 ].y, m.rows[ 1 ].y, m.rows[ 2 ].y, m.rows[ 3 ].y,
                  m.rows[ 0 ].z, m.rows[ 1 ].z, m.rows[ 2 ].z, m.rows[ 3 ].z,
                  m.rows[ 0 ].w, m.rows[ 1 ].w, m.rows[ 2 ].w, m.rows[ 3 ].w );
    }
    
    inline v4 operator * ( const m4& m, const v4& v )
    {
        return v4( dot( m.rows[ 0 ], v ), dot( m.rows[ 1 ], v ), dot( m.rows[ 2 ], v ), dot( m.rows[ 3 ], v ) );
    }
    
    inline m4 operator * ( const m4& m, const f32 x )
    {
        return m4( m.rows[ 0 ] * x, m.rows[ 1 ] * x, m.rows[ 2 ] * x, m.rows[ 3 ] * x );
    }
    
    inline m4 operator * ( const m4& m, const m4& n )
    {
        return m4(
                  m.rows[ 0 ].x * n.rows[ 0 ][ 0 ] + m.rows[ 0 ].y * n.rows[ 1 ][ 0 ] + m.rows[ 0 ].z * n.rows[ 2 ][ 0 ] + m.rows[ 0 ].w * n.rows[ 3 ][ 0 ],
                  m.rows[ 0 ].x * n.rows[ 0 ][ 1 ] + m.rows[ 0 ].y * n.rows[ 1 ][ 1 ] + m.rows[ 0 ].z * n.rows[ 2 ][ 1 ] + m.rows[ 0 ].w * n.rows[ 3 ][ 1 ],
                  m.rows[ 0 ].x * n.rows[ 0 ][ 2 ] + m.rows[ 0 ].y * n.rows[ 1 ][ 2 ] + m.rows[ 0 ].z * n.rows[ 2 ][ 2 ] + m.rows[ 0 ].w * n.rows[ 3 ][ 2 ],
                  m.rows[ 0 ].x * n.rows[ 0 ][ 3 ] + m.rows[ 0 ].y * n.rows[ 1 ][ 3 ] + m.rows[ 0 ].z * n.rows[ 2 ][ 3 ] + m.rows[ 0 ].w * n.rows[ 3 ][ 3 ],
                  m.rows[ 1 ].x * n.rows[ 0 ][ 0 ] + m.rows[ 1 ].y * n.rows[ 1 ][ 0 ] + m.rows[ 1 ].z * n.rows[ 2 ][ 0 ] + m.rows[ 1 ].w * n.rows[ 3 ][ 0 ],
                  m.rows[ 1 ].x * n.rows[ 0 ][ 1 ] + m.rows[ 1 ].y * n.rows[ 1 ][ 1 ] + m.rows[ 1 ].z * n.rows[ 2 ][ 1 ] + m.rows[ 1 ].w * n.rows[ 3 ][ 1 ],
                  m.rows[ 1 ].x * n.rows[ 0 ][ 2 ] + m.rows[ 1 ].y * n.rows[ 1 ][ 2 ] + m.rows[ 1 ].z * n.rows[ 2 ][ 2 ] + m.rows[ 1 ].w * n.rows[ 3 ][ 2 ],
                  m.rows[ 1 ].x * n.rows[ 0 ][ 3 ] + m.rows[ 1 ].y * n.rows[ 1 ][ 3 ] + m.rows[ 1 ].z * n.rows[ 2 ][ 3 ] + m.rows[ 1 ].w * n.rows[ 3 ][ 3 ],
                  m.rows[ 2 ].x * n.rows[ 0 ][ 0 ] + m.rows[ 2 ].y * n.rows[ 1 ][ 0 ] + m.rows[ 2 ].z * n.rows[ 2 ][ 0 ] + m.rows[ 2 ].w * n.rows[ 3 ][ 0 ],
                  m.rows[ 2 ].x * n.rows[ 0 ][ 1 ] + m.rows[ 2 ].y * n.rows[ 1 ][ 1 ] + m.rows[ 2 ].z * n.rows[ 2 ][ 1 ] + m.rows[ 2 ].w * n.rows[ 3 ][ 1 ],
                  m.rows[ 2 ].x * n.rows[ 0 ][ 2 ] + m.rows[ 2 ].y * n.rows[ 1 ][ 2 ] + m.rows[ 2 ].z * n.rows[ 2 ][ 2 ] + m.rows[ 2 ].w * n.rows[ 3 ][ 2 ],
                  m.rows[ 2 ].x * n.rows[ 0 ][ 3 ] + m.rows[ 2 ].y * n.rows[ 1 ][ 3 ] + m.rows[ 2 ].z * n.rows[ 2 ][ 3 ] + m.rows[ 2 ].w * n.rows[ 3 ][ 3 ],
                  m.rows[ 3 ].x * n.rows[ 0 ][ 0 ] + m.rows[ 3 ].y * n.rows[ 1 ][ 0 ] + m.rows[ 3 ].z * n.rows[ 2 ][ 0 ] + m.rows[ 3 ].w * n.rows[ 3 ][ 0 ],
                  m.rows[ 3 ].x * n.rows[ 0 ][ 1 ] + m.rows[ 3 ].y * n.rows[ 1 ][ 1 ] + m.rows[ 3 ].z * n.rows[ 2 ][ 1 ] + m.rows[ 3 ].w * n.rows[ 3 ][ 1 ],
                  m.rows[ 3 ].x * n.rows[ 0 ][ 2 ] + m.rows[ 3 ].y * n.rows[ 1 ][ 2 ] + m.rows[ 3 ].z * n.rows[ 2 ][ 2 ] + m.rows[ 3 ].w * n.rows[ 3 ][ 2 ],
                  m.rows[ 3 ].x * n.rows[ 0 ][ 3 ] + m.rows[ 3 ].y * n.rows[ 1 ][ 3 ] + m.rows[ 3 ].z * n.rows[ 2 ][ 3 ] + m.rows[ 3 ].w * n.rows[ 3 ][ 3 ] );
    }
    
    inline m4 operator ! ( const m4& m )
    {
        m4 n;
        
        f32 p00 = m.rows[ 2 ][ 2 ] * m.rows[ 3 ][ 3 ];
        f32 p01 = m.rows[ 3 ][ 2 ] * m.rows[ 2 ][ 3 ];
        f32 p02 = m.rows[ 1 ][ 2 ] * m.rows[ 3 ][ 3 ];
        f32 p03 = m.rows[ 3 ][ 2 ] * m.rows[ 1 ][ 3 ];
        f32 p04 = m.rows[ 1 ][ 2 ] * m.rows[ 2 ][ 3 ];
        f32 p05 = m.rows[ 2 ][ 2 ] * m.rows[ 1 ][ 3 ];
        f32 p06 = m.rows[ 0 ][ 2 ] * m.rows[ 3 ][ 3 ];
        f32 p07 = m.rows[ 3 ][ 2 ] * m.rows[ 0 ][ 3 ];
        f32 p08 = m.rows[ 0 ][ 2 ] * m.rows[ 2 ][ 3 ];
        f32 p09 = m.rows[ 2 ][ 2 ] * m.rows[ 0 ][ 3 ];
        f32 p10 = m.rows[ 0 ][ 2 ] * m.rows[ 1 ][ 3 ];
        f32 p11 = m.rows[ 1 ][ 2 ] * m.rows[ 0 ][ 3 ];
        
        n.rows[ 0 ][ 0 ] = ( p00 * m.rows[ 1 ][ 1 ] + p03 * m.rows[ 2 ][ 1 ] + p04 * m.rows[ 3 ][ 1 ] ) - ( p01 * m.rows[ 1 ][ 1 ] + p02 * m.rows[ 2 ][ 1 ] + p05 * m.rows[ 3 ][ 1 ] );
        n.rows[ 0 ][ 1 ] = ( p01 * m.rows[ 0 ][ 1 ] + p06 * m.rows[ 2 ][ 1 ] + p09 * m.rows[ 3 ][ 1 ] ) - ( p00 * m.rows[ 0 ][ 1 ] + p07 * m.rows[ 2 ][ 1 ] + p08 * m.rows[ 3 ][ 1 ] );
        n.rows[ 0 ][ 2 ] = ( p02 * m.rows[ 0 ][ 1 ] + p07 * m.rows[ 1 ][ 1 ] + p10 * m.rows[ 3 ][ 1 ] ) - ( p03 * m.rows[ 0 ][ 1 ] + p06 * m.rows[ 1 ][ 1 ] + p11 * m.rows[ 3 ][ 1 ] );
        n.rows[ 0 ][ 3 ] = ( p05 * m.rows[ 0 ][ 1 ] + p08 * m.rows[ 1 ][ 1 ] + p11 * m.rows[ 2 ][ 1 ] ) - ( p04 * m.rows[ 0 ][ 1 ] + p09 * m.rows[ 1 ][ 1 ] + p10 * m.rows[ 2 ][ 1 ] );
        n.rows[ 1 ][ 0 ] = ( p01 * m.rows[ 1 ][ 0 ] + p02 * m.rows[ 2 ][ 0 ] + p05 * m.rows[ 3 ][ 0 ] ) - ( p00 * m.rows[ 1 ][ 0 ] + p03 * m.rows[ 2 ][ 0 ] + p04 * m.rows[ 3 ][ 0 ] );
        n.rows[ 1 ][ 1 ] = ( p00 * m.rows[ 0 ][ 0 ] + p07 * m.rows[ 2 ][ 0 ] + p08 * m.rows[ 3 ][ 0 ] ) - ( p01 * m.rows[ 0 ][ 0 ] + p06 * m.rows[ 2 ][ 0 ] + p09 * m.rows[ 3 ][ 0 ] );
        n.rows[ 1 ][ 2 ] = ( p03 * m.rows[ 0 ][ 0 ] + p06 * m.rows[ 1 ][ 0 ] + p11 * m.rows[ 3 ][ 0 ] ) - ( p02 * m.rows[ 0 ][ 0 ] + p07 * m.rows[ 1 ][ 0 ] + p10 * m.rows[ 3 ][ 0 ] );
        n.rows[ 1 ][ 3 ] = ( p04 * m.rows[ 0 ][ 0 ] + p09 * m.rows[ 1 ][ 0 ] + p10 * m.rows[ 2 ][ 0 ] ) - ( p05 * m.rows[ 0 ][ 0 ] + p08 * m.rows[ 1 ][ 0 ] + p11 * m.rows[ 2 ][ 0 ] );
        
        f32 q00 = m.rows[ 2 ][ 0 ] * m.rows[ 3 ][ 1 ];
        f32 q01 = m.rows[ 3 ][ 0 ] * m.rows[ 2 ][ 1 ];
        f32 q02 = m.rows[ 1 ][ 0 ] * m.rows[ 3 ][ 1 ];
        f32 q03 = m.rows[ 3 ][ 0 ] * m.rows[ 1 ][ 1 ];
        f32 q04 = m.rows[ 1 ][ 0 ] * m.rows[ 2 ][ 1 ];
        f32 q05 = m.rows[ 2 ][ 0 ] * m.rows[ 1 ][ 1 ];
        f32 q06 = m.rows[ 0 ][ 0 ] * m.rows[ 3 ][ 1 ];
        f32 q07 = m.rows[ 3 ][ 0 ] * m.rows[ 0 ][ 1 ];
        f32 q08 = m.rows[ 0 ][ 0 ] * m.rows[ 2 ][ 1 ];
        f32 q09 = m.rows[ 2 ][ 0 ] * m.rows[ 0 ][ 1 ];
        f32 q10 = m.rows[ 0 ][ 0 ] * m.rows[ 1 ][ 1 ];
        f32 q11 = m.rows[ 1 ][ 0 ] * m.rows[ 0 ][ 1 ];
        
        n.rows[ 2 ][ 0 ] = ( q00 * m.rows[ 1 ][ 3 ] + q03 * m.rows[ 2 ][ 3 ] + q04 * m.rows[ 3 ][ 3 ] ) - ( q01 * m.rows[ 1 ][ 3 ] + q02 * m.rows[ 2 ][ 3 ] + q05 * m.rows[ 3 ][3 ] );
        n.rows[ 2 ][ 1 ] = ( q01 * m.rows[ 0 ][ 3 ] + q06 * m.rows[ 2 ][ 3 ] + q09 * m.rows[ 3 ][ 3 ] ) - ( q00 * m.rows[ 0 ][ 3 ] + q07 * m.rows[ 2 ][ 3 ] + q08 * m.rows[ 3 ][3 ] );
        n.rows[ 2 ][ 2 ] = ( q02 * m.rows[ 0 ][ 3 ] + q07 * m.rows[ 1 ][ 3 ] + q10 * m.rows[ 3 ][ 3 ] ) - ( q03 * m.rows[ 0 ][ 3 ] + q06 * m.rows[ 1 ][ 3 ] + q11 * m.rows[ 3 ][3 ] );
        n.rows[ 2 ][ 3 ] = ( q05 * m.rows[ 0 ][ 3 ] + q08 * m.rows[ 1 ][ 3 ] + q11 * m.rows[ 2 ][ 3 ] ) - ( q04 * m.rows[ 0 ][ 3 ] + q09 * m.rows[ 1 ][ 3 ] + q10 * m.rows[ 2 ][3 ] );
        n.rows[ 3 ][ 0 ] = ( q02 * m.rows[ 2 ][ 2 ] + q05 * m.rows[ 3 ][ 2 ] + q01 * m.rows[ 1 ][ 2 ] ) - ( q04 * m.rows[ 3 ][ 2 ] + q00 * m.rows[ 1 ][ 2 ] + q03 * m.rows[ 2 ][2 ] );
        n.rows[ 3 ][ 1 ] = ( q08 * m.rows[ 3 ][ 2 ] + q00 * m.rows[ 0 ][ 2 ] + q07 * m.rows[ 2 ][ 2 ] ) - ( q06 * m.rows[ 2 ][ 2 ] + q09 * m.rows[ 3 ][ 2 ] + q01 * m.rows[ 0 ][2 ] );
        n.rows[ 3 ][ 2 ] = ( q06 * m.rows[ 1 ][ 2 ] + q11 * m.rows[ 3 ][ 2 ] + q03 * m.rows[ 0 ][ 2 ] ) - ( q10 * m.rows[ 3 ][ 2 ] + q02 * m.rows[ 0 ][ 2 ] + q07 * m.rows[ 1 ][2 ] );
        n.rows[ 3 ][ 3 ] = ( q10 * m.rows[ 2 ][ 2 ] + q04 * m.rows[ 0 ][ 2 ] + q09 * m.rows[ 1 ][ 2 ] ) - ( q08 * m.rows[ 1 ][ 2 ] + q11 * m.rows[ 2 ][ 2 ] + q05 * m.rows[ 0 ][2 ] );
        
        return n * ( 1.0f / ( m.rows[ 0 ][ 0 ] * n.rows[ 0 ][ 0 ] + m.rows[ 1 ][ 0 ] * n.rows[ 0 ][ 1 ] + m.rows[ 2 ][ 0 ] * n.rows[ 0 ][ 2 ] + m.rows[ 3 ][ 0 ] * n.rows[ 0 ][ 3 ] ) );
    }
}

#endif