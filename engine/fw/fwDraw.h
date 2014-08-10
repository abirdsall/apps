#ifndef FW_DRAW
#define FW_DRAW

namespace fw
{
	void DrawInit();
	void DrawKill();
	
	void DrawRect( const Rect& vertices, const v4& colour );
	void DrawRect( const Rect& vertices, const Rect& tcoords );
	void DrawRect( const Rect& vertices, const Rect& tcoords, const f32 uniform );
	void DrawCube( const v3& vertex, const v3& radius, const v2& uniform );
	void DrawCube( const v3& vertex, const v3& radius, const v4& colour );
	void DrawCube( const v3& vertex, const v3& radius, const v4& colour, const v2& uniform );
	void DrawWireCube( const v3& vertex, const v3& radius, const v4& uniform );
	
	void FillRect( const Rect& vertices, const v4& colour );
    void Fill4Rect( const Rect& vertices, const v4& colour );
    void FillWireRect( const Rect& vertices, const v4& colour );
    
    void BatchBegin();
    void BatchEnd( gs::ePrim primitive );
}

#endif
