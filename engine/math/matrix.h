#ifndef kMatrixH
#define kMatrixH

struct m4;

m4 translate(const v3 &v);
m4 translate(const f32 x, const f32 y, const f32 z);
m4 scale(const f32 x, const f32 y, const f32 z);
m4 rotateX(const f32 angle);
m4 rotateY(const f32 angle);
m4 rotateZ(const f32 angle);
m4 rotateXY(const f32 angleX, const f32 angleY);
m4 rotateZXY(const f32 angleX, const f32 angleY, const f32 angleZ);
m4 identity4(void);
m4 projection(const f32 fovX, const f32 zNear, const f32 zFar);
m4 orthogonal(const f32 left, const f32 right, const f32 top, const f32 bottom, const f32 zNear, const f32 zFar);
m4 operator * (const m4& m, const m4& n);
v4 operator * (const m4& m, const v4& v);
m4 operator * (const m4& m, const f32 x);
m4 operator ! (const m4& m);
m4 transpose(const m4& m);
m4 multiply33(const m4& m, const m4& n);
v3 multiply33(const m4& m, const v3& v);
v3 multiply33Transpose(const m4& m, const v3& v);
m4 look(const v3& a, const v3& b, const v3& c);

struct m4
{
	v4 rows[4];

	m4(void) {}
	m4(const v4 &r0, const v4 &r1, const v4 &r2, const v4 &r3)
	{
		rows[0] = r0;
		rows[1] = r1;
		rows[2] = r2;
		rows[3] = r3;
	}
	
	m4(	const f32 m00, const f32 m01, const f32 m02, const f32 m03,
		const f32 m10, const f32 m11, const f32 m12, const f32 m13,
		const f32 m20, const f32 m21, const f32 m22, const f32 m23,
		const f32 m30, const f32 m31, const f32 m32, const f32 m33 )
	{
		rows[0] = v4(m00, m01, m02, m03);
		rows[1] = v4(m10, m11, m12, m13);
		rows[2] = v4(m20, m21, m22, m23);
		rows[3] = v4(m30, m31, m32, m33);
	}
	
	m4(const f32 *a)
	{
		rows[0] = v4(a[ 0], a[ 1], a[ 2], a[ 3]);
		rows[1] = v4(a[ 4], a[ 5], a[ 6], a[ 7]);
		rows[2] = v4(a[ 8], a[ 9], a[10], a[11]);
		rows[3] = v4(a[12], a[13], a[14], a[15]);
	}
	
	void translate(const v3 &v);
	void stabilise(void);
	
	v4 getColumn(const u8 column) const
	{
		switch(column)
		{
			case 0  : return v4(rows[0].x, rows[1].x, rows[2].x, rows[3].x);
			case 1  : return v4(rows[0].y, rows[1].y, rows[2].y, rows[3].y);
			case 2  : return v4(rows[0].z, rows[1].z, rows[2].z, rows[3].z);
			default : return v4(rows[0].w, rows[1].w, rows[2].w, rows[3].w);
		}
	}

	void setColumn(const u8 column, const v4& v)
	{
		switch(column)
		{
			case 0  : rows[0].x = v.x; rows[1].x = v.y; rows[2].x = v.z; rows[3].x = v.w; return;
			case 1  : rows[0].y = v.x; rows[1].y = v.y; rows[2].y = v.z; rows[3].y = v.w; return;
			case 2  : rows[0].z = v.x; rows[1].z = v.y; rows[2].z = v.z; rows[3].z = v.w; return;
			default : rows[0].w = v.x; rows[1].w = v.y; rows[2].w = v.z; rows[3].w = v.w;
		}
	}

	v3 getPosition(void)
	{
		return - rows[0].w * rows[0].xyz() - rows[1].w * rows[1].xyz() - rows[2].w * rows[2].xyz();
	}

	void setPosition(const v3& position)
	{
		setColumn(3, v4(-multiply33(*this, position), 1.0f));
	}
	
	operator f32 *() const
	{
		return (f32*)&rows[0].x;
	}
};

struct r4 : m4
{
	r4(const v3& a, const f32 b);
	r4(const v3& a, const v3& b, const f32 c = 1.0f);
	void roll(const v3& a, const f32 b);
};

#endif