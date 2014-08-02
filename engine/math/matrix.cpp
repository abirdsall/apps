#include "os.h"

void m4::translate(const v3 &v)
{
	rows[0].w += dot(rows[0].xyz(), v);
	rows[1].w += dot(rows[1].xyz(), v);
	rows[2].w += dot(rows[2].xyz(), v);
	rows[3].w += dot(rows[3].xyz(), v);
}

void m4::stabilise(void) // stabilise
{
	(v3&)rows[0] = normalise((v3&)rows[0]);
	(v3&)rows[2] = normalise(cross((v3&)rows[0], (v3&)rows[1]));
	(v3&)rows[1] = cross((v3&)rows[2], (v3&)rows[0]);
}
	
m4 translate(const v3 &v)
{
	return m4(	1.0f, 0.0f, 0.0f, v.x,
				0.0f, 1.0f, 0.0f, v.y,
				0.0f, 0.0f, 1.0f, v.z,
				0.0f, 0.0f, 0.0f, 1.0f);
}

m4 translate(const f32 x, const f32 y, const f32 z)
{
	return m4(	1.0f, 0.0f, 0.0f, x,
				0.0f, 1.0f, 0.0f, y,
				0.0f, 0.0f, 1.0f, z,
				0.0f, 0.0f, 0.0f, 1.0f);
}

m4 scale(const f32 x, const f32 y, const f32 z)
{
	return m4(	   x, 0.0f, 0.0f, 0.0f,
				0.0f,    y, 0.0f, 0.0f,
				0.0f, 0.0f,    z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

m4 rotateX(const f32 angle)
{
	f32 cosA = math::cos(angle);
	f32 sinA = math::sin(angle);
	return m4(	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cosA,-sinA, 0.0f,
				0.0f, sinA, cosA, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

m4 rotateY(const f32 angle)
{
	f32 cosA = math::cos(angle);
	f32 sinA = math::sin(angle);
	return m4(	cosA, 0.0f,-sinA, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				sinA, 0.0f, cosA, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

m4 rotateZ(const f32 angle)
{
	f32 cosA = math::cos(angle);
	f32 sinA = math::sin(angle);
	return m4(	cosA,-sinA, 0.0f, 0.0f,
				sinA, cosA, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

m4 rotateXY(const f32 angleX, const f32 angleY)
{
	f32 cosX = math::cos(angleX), sinX = math::sin(angleX);
	f32 cosY = math::cos(angleY), sinY = math::sin(angleY);
	return m4(			cosY, 0.0f,		   -sinY, 0.0f,
				-sinX * sinY, cosX, -sinX * cosY, 0.0f,
				 cosX * sinY, sinX,  cosX * cosY, 0.0f,
						0.0f, 0.0f,			0.0f, 1.0f);
}

m4 rotateZXY(const f32 angleX, const f32 angleY, const f32 angleZ)
{
	f32 cosX = math::cos(angleX);
	f32 sinX = math::sin(angleX);
	f32 cosY = math::cos(angleY);
	f32 sinY = math::sin(angleY);
	f32 cosZ = math::cos(angleZ);
	f32 sinZ = math::sin(angleZ);
	return m4(
		cosY * cosZ + sinX * sinY * sinZ,   -cosX * sinZ,    sinX * cosY * sinZ - sinY * cosZ,  0,
		cosY * sinZ - sinX * sinY * cosZ,    cosX * cosZ,   -sinY * sinZ - sinX * cosY * cosZ,  0,
		cosX * sinY,                         sinX,           cosX * cosY,                       0,
		0,                                   0,              0,                                 1);
}

m4 identity4(void)
{
	return m4(	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}


m4 projection(const f32 fovX, const f32 zNear, const f32 zFar)
{
	f32 x = math::cos(0.5f * fovX) / math::sin(0.5f * fovX);
	f32 y = x / os::WindowAspect();

	//f32 x = 1.0f / fovX;
	//f32 y = 1.0f / (fovX * osWindow().aspect());
	return m4(	   x, 0.0f, 0.0f, 0.0f,
				0.0f,    y, 0.0f, 0.0f,
				0.0f, 0.0f, (zFar + zNear) / (zFar - zNear), -(2.0f * zFar * zNear) / (zFar - zNear),
				0.0f, 0.0f, 1.0f, 0.0f);
}
	
m4 orthogonal(const f32 left, const f32 right, const f32 top, const f32 bottom, const f32 zNear, const f32 zFar)
{
	f32 rl = right - left;
	f32 tb = top - bottom;
	f32 fn = zFar - zNear;
	return m4(	2.0f / rl,      0.0f,       0.0f, -(right + left) / rl,
				     0.0f, 2.0f / tb,       0.0f, -(top + bottom) / tb,
				     0.0f,      0.0f, -2.0f / fn, -(zFar + zNear) / fn,
				     0.0f,      0.0f,       0.0f,                 1.0f);
}

#define rcDot4(r, c) (m.rows[r].x * n.rows[0][c] + m.rows[r].y * n.rows[1][c] + m.rows[r].z * n.rows[2][c] + m.rows[r].w * n.rows[3][c])

m4 operator * (const m4& m, const m4& n)
{
	return m4(	rcDot4(0, 0), rcDot4(0, 1), rcDot4(0, 2), rcDot4(0, 3),
				rcDot4(1, 0), rcDot4(1, 1), rcDot4(1, 2), rcDot4(1, 3),
				rcDot4(2, 0), rcDot4(2, 1), rcDot4(2, 2), rcDot4(2, 3),
				rcDot4(3, 0), rcDot4(3, 1), rcDot4(3, 2), rcDot4(3, 3));
}

v4 operator * (const m4& m, const v4& v)
{
	return v4(dot(m.rows[0], v), dot(m.rows[1], v), dot(m.rows[2], v), dot(m.rows[3], v));
}

m4 operator * (const m4& m, const f32 x)
{
	return m4(m.rows[0] * x, m.rows[1] * x, m.rows[2] * x, m.rows[3] * x);
}

m4 operator ! (const m4& m)
{
	m4 n;

	f32 p00 = m.rows[2][2] * m.rows[3][3];
	f32 p01 = m.rows[3][2] * m.rows[2][3];
	f32 p02 = m.rows[1][2] * m.rows[3][3];
	f32 p03 = m.rows[3][2] * m.rows[1][3];
	f32 p04 = m.rows[1][2] * m.rows[2][3];
	f32 p05 = m.rows[2][2] * m.rows[1][3];
	f32 p06 = m.rows[0][2] * m.rows[3][3];
	f32 p07 = m.rows[3][2] * m.rows[0][3];
	f32 p08 = m.rows[0][2] * m.rows[2][3];
	f32 p09 = m.rows[2][2] * m.rows[0][3];
	f32 p10 = m.rows[0][2] * m.rows[1][3];
	f32 p11 = m.rows[1][2] * m.rows[0][3];

	n.rows[0][0] = (p00 * m.rows[1][1] + p03 * m.rows[2][1] + p04 * m.rows[3][1]) - (p01 * m.rows[1][1] + p02 * m.rows[2][1] + p05 * m.rows[3][1]);
	n.rows[0][1] = (p01 * m.rows[0][1] + p06 * m.rows[2][1] + p09 * m.rows[3][1]) - (p00 * m.rows[0][1] + p07 * m.rows[2][1] + p08 * m.rows[3][1]);
	n.rows[0][2] = (p02 * m.rows[0][1] + p07 * m.rows[1][1] + p10 * m.rows[3][1]) - (p03 * m.rows[0][1] + p06 * m.rows[1][1] + p11 * m.rows[3][1]);
	n.rows[0][3] = (p05 * m.rows[0][1] + p08 * m.rows[1][1] + p11 * m.rows[2][1]) - (p04 * m.rows[0][1] + p09 * m.rows[1][1] + p10 * m.rows[2][1]);
	n.rows[1][0] = (p01 * m.rows[1][0] + p02 * m.rows[2][0] + p05 * m.rows[3][0]) - (p00 * m.rows[1][0] + p03 * m.rows[2][0] + p04 * m.rows[3][0]);
	n.rows[1][1] = (p00 * m.rows[0][0] + p07 * m.rows[2][0] + p08 * m.rows[3][0]) - (p01 * m.rows[0][0] + p06 * m.rows[2][0] + p09 * m.rows[3][0]);
	n.rows[1][2] = (p03 * m.rows[0][0] + p06 * m.rows[1][0] + p11 * m.rows[3][0]) - (p02 * m.rows[0][0] + p07 * m.rows[1][0] + p10 * m.rows[3][0]);
	n.rows[1][3] = (p04 * m.rows[0][0] + p09 * m.rows[1][0] + p10 * m.rows[2][0]) - (p05 * m.rows[0][0] + p08 * m.rows[1][0] + p11 * m.rows[2][0]);

	f32 q00 = m.rows[2][0] * m.rows[3][1];
	f32 q01 = m.rows[3][0] * m.rows[2][1];
	f32 q02 = m.rows[1][0] * m.rows[3][1];
	f32 q03 = m.rows[3][0] * m.rows[1][1];
	f32 q04 = m.rows[1][0] * m.rows[2][1];
	f32 q05 = m.rows[2][0] * m.rows[1][1];
	f32 q06 = m.rows[0][0] * m.rows[3][1];
	f32 q07 = m.rows[3][0] * m.rows[0][1];
	f32 q08 = m.rows[0][0] * m.rows[2][1];
	f32 q09 = m.rows[2][0] * m.rows[0][1];
	f32 q10 = m.rows[0][0] * m.rows[1][1];
	f32 q11 = m.rows[1][0] * m.rows[0][1];

	n.rows[2][0] = (q00 * m.rows[1][3] + q03 * m.rows[2][3] + q04 * m.rows[3][3]) - (q01 * m.rows[1][3] + q02 * m.rows[2][3] + q05 * m.rows[3][3]);
	n.rows[2][1] = (q01 * m.rows[0][3] + q06 * m.rows[2][3] + q09 * m.rows[3][3]) - (q00 * m.rows[0][3] + q07 * m.rows[2][3] + q08 * m.rows[3][3]);
	n.rows[2][2] = (q02 * m.rows[0][3] + q07 * m.rows[1][3] + q10 * m.rows[3][3]) - (q03 * m.rows[0][3] + q06 * m.rows[1][3] + q11 * m.rows[3][3]);
	n.rows[2][3] = (q05 * m.rows[0][3] + q08 * m.rows[1][3] + q11 * m.rows[2][3]) - (q04 * m.rows[0][3] + q09 * m.rows[1][3] + q10 * m.rows[2][3]);
	n.rows[3][0] = (q02 * m.rows[2][2] + q05 * m.rows[3][2] + q01 * m.rows[1][2]) - (q04 * m.rows[3][2] + q00 * m.rows[1][2] + q03 * m.rows[2][2]);
	n.rows[3][1] = (q08 * m.rows[3][2] + q00 * m.rows[0][2] + q07 * m.rows[2][2]) - (q06 * m.rows[2][2] + q09 * m.rows[3][2] + q01 * m.rows[0][2]);
	n.rows[3][2] = (q06 * m.rows[1][2] + q11 * m.rows[3][2] + q03 * m.rows[0][2]) - (q10 * m.rows[3][2] + q02 * m.rows[0][2] + q07 * m.rows[1][2]);
	n.rows[3][3] = (q10 * m.rows[2][2] + q04 * m.rows[0][2] + q09 * m.rows[1][2]) - (q08 * m.rows[1][2] + q11 * m.rows[2][2] + q05 * m.rows[0][2]);

	return n * (1.0f / (m.rows[0][0] * n.rows[0][0] + m.rows[1][0] * n.rows[0][1] + m.rows[2][0] * n.rows[0][2] + m.rows[3][0] * n.rows[0][3]));
}

m4 transpose(const m4& m)
{
	return m4(	m.rows[0].x, m.rows[1].x, m.rows[2].x, m.rows[3].x,
				m.rows[0].y, m.rows[1].y, m.rows[2].y, m.rows[3].y,
				m.rows[0].z, m.rows[1].z, m.rows[2].z, m.rows[3].z,
				m.rows[0].w, m.rows[1].w, m.rows[2].w, m.rows[3].w);
}
/*
#define rcDot3(r, c) (m.rows[r].x * n.rows[0][c] + m.rows[r].y * n.rows[1][c] + m.rows[r].z * n.rows[2][c])

m4 multiply33(const m4& m, const m4& n)
{
	return m4(	rcDot3(0, 0), rcDot3(0, 1), rcDot3(0, 2), 0.0f,
				rcDot3(1, 0), rcDot3(1, 1), rcDot3(1, 2), 0.0f,
				rcDot3(2, 0), rcDot3(2, 1), rcDot3(2, 2), 0.0f,
				rcDot3(3, 0), rcDot3(3, 1), rcDot3(3, 2), 0.0f);
}
*/
m4 multiply33(const m4& m, const m4& n)
{
	return m4(
    	m.rows[0].x * n.rows[0].x + m.rows[0].y * n.rows[1].x + m.rows[0].z * n.rows[2].x,
    	m.rows[0].x * n.rows[0].y + m.rows[0].y * n.rows[1].y + m.rows[0].z * n.rows[2].y,
    	m.rows[0].x * n.rows[0].z + m.rows[0].y * n.rows[1].z + m.rows[0].z * n.rows[2].z,
    	0.0f,
    	m.rows[1].x * n.rows[0].x + m.rows[1].y * n.rows[1].x + m.rows[1].z * n.rows[2].x,
    	m.rows[1].x * n.rows[0].y + m.rows[1].y * n.rows[1].y + m.rows[1].z * n.rows[2].y,
    	m.rows[1].x * n.rows[0].z + m.rows[1].y * n.rows[1].z + m.rows[1].z * n.rows[2].z,
    	0.0f,
    	m.rows[2].x * n.rows[0].x + m.rows[2].y * n.rows[1].x + m.rows[2].z * n.rows[2].x,
    	m.rows[2].x * n.rows[0].y + m.rows[2].y * n.rows[1].y + m.rows[2].z * n.rows[2].y,
    	m.rows[2].x * n.rows[0].z + m.rows[2].y * n.rows[1].z + m.rows[2].z * n.rows[2].z,
    	0.0f,
    	m.rows[3].x * n.rows[0].x + m.rows[3].y * n.rows[1].x + m.rows[3].z * n.rows[2].x,
    	m.rows[3].x * n.rows[0].y + m.rows[3].y * n.rows[1].y + m.rows[3].z * n.rows[2].y,
    	m.rows[3].x * n.rows[0].z + m.rows[3].y * n.rows[1].z + m.rows[3].z * n.rows[2].z,
     	1.0f);
}

v3 multiply33(const m4& m, const v3& v)
{	
	return v3( dot(m.rows[0].xyz(), v), dot(m.rows[1].xyz(), v), dot(m.rows[2].xyz(), v) );
}

v3 multiply33Transpose(const m4& m, const v3& v)
{
	return v3(	m.rows[0].x * v.x + m.rows[1].x * v.y + m.rows[2].x * v.z,
				m.rows[0].y * v.x + m.rows[1].y * v.y + m.rows[2].y * v.z,
				m.rows[0].z * v.x + m.rows[1].z * v.y + m.rows[2].z * v.z);
}

m4 look(const v3& a, const v3& b, const v3& c)
{
	v3 z = normalise(b - a);
	v3 x = normalise(cross(z, c));
	v3 y = cross(x, z);
	m4 m(x, y, z, v3(kV3Zero));
	m.setPosition(a);
	return m;
}

r4::r4(const v3& a, const f32 b)
{
	roll(a, b);
}

r4::r4(const v3& a, const v3& b, const f32 c)
{
	f32 d = dot(a, b);
	if(d < 1.0f)
	{
		roll(normalise(cross(b, a)), math::acos(d) * c);
	}
	else
	{
		// identity
		rows[0] = v4(1.0f, 0.0f, 0.0f, 0.0f);
		rows[1] = v4(0.0f, 1.0f, 0.0f, 0.0f);
		rows[2] = v4(0.0f, 0.0f, 1.0f, 0.0f);
		rows[3] = v4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

void r4::roll(const v3& a, const f32 b)
{
	f32 c = math::cos(b);
	f32 s = math::sin(b);
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
	
	rows[0].x = vxx + c;
	rows[0].y = vxy + sz;
	rows[0].z = vxz - sy;
	rows[0].w = 0.0f;
	rows[1].x = vxy - sz;
	rows[1].y = vyy + c;
	rows[1].z = vyz + sx;
	rows[1].w = 0.0f;
	rows[2].x = vxz + sy;
	rows[2].y = vyz - sx;
	rows[2].z = vzz + c;
	rows[2].w = 0.0f;
	rows[3].x = 0.0f;
	rows[3].y = 0.0f;
	rows[3].z = 0.0f;
	rows[3].w = 1.0f;
}