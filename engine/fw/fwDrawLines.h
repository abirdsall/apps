#ifndef FW_LINE_RENDERER
#define FW_LINE_RENDERER

namespace fw
{
	void DrawLinesInit();
	void DrawLinesKill();
	
    void DrawLinesBegin();
    void DrawLinesAdd( const v2& from, const v2& to, const v4& colour );
    void DrawLinesEnd();
}

#endif
