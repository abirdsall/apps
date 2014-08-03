#include "fwDraw.h"
#include "fwMesh.h"
#include "fwString.h"
#include "gs.h"

namespace fw
{
    Mesh sMesh;
    bool sBatching;
    s32 sBatchSize;

    static const s32 kBatchSizeLimit = 4096;
    static const s32 kVertexAttribSize = 2;
	static const s32 kColourAttribSize = 4;
	static const s32 kTcoordAttribSize = 4;
    static const s32 kAttribSize = kVertexAttribSize + kColourAttribSize + kTcoordAttribSize;

	static gs::ShaderHandle sFillShader;

	void DrawInit()
	{
        DrawLinesInit();
        
        sBatching = false;
        sBatchSize = 0;
        
        s32 elementLimit = kBatchSizeLimit * 6;
		s32 elementSize = sizeof( u16 );
		
		s32 vertexLimit = kBatchSizeLimit * 4;
		s32 vertexSize = sizeof( f32 ) * kAttribSize;
		
		MeshInit( sMesh );
		MeshSetElementData( sMesh, memory::alloc( elementSize * elementLimit ), elementSize, 0, elementLimit, true );
		MeshSetVertexData( sMesh, memory::alloc( vertexSize * vertexLimit ), vertexSize, 0, vertexLimit, true );
		MeshSetAttrib( sMesh, gs::eAttribVertex, ( void* )( sizeof( f32 ) * 0 ), kVertexAttribSize );
        MeshSetAttrib( sMesh, gs::eAttribColour, ( void* )( sizeof( f32 ) * kVertexAttribSize ), kColourAttribSize );
        MeshSetAttrib( sMesh, gs::eAttribTcoord, ( void* )( sizeof( f32 ) * (kVertexAttribSize + kTcoordAttribSize) ), kTcoordAttribSize );
        MeshFinalise( sMesh );
        
#if kBuildOpenGles2
        
        String vShader = "attribute vec2 vertex_position;\n";
        vShader += "attribute vec4 vertex_colour;\n";
		vShader += "attribute vec4 vertex_tcoord;\n";
		vShader += "varying lowp vec4 fragment_colour;\n";
		vShader += "uniform mat4 viewMatrix;\n";
		vShader += "uniform mat4 projMatrix;\n";
		vShader += "void main()\n";
		vShader += "{\n";
		vShader = vShader + "\tgl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n";
		vShader += "\tfragment_colour = vertex_colour;\n";
		vShader += "}\n";
        
		String fShader = "varying lowp vec4 fragment_colour;\n";
		fShader += "void main()\n";
		fShader += "{\n";
		fShader += "\tgl_FragColor = fragment_colour;\n";
		fShader += "}\n";
#else
        
#if kBuildOpenGles3
        String vShader = "#version 300 es\n";
#else //kBuildOpenGl3
        String vShader = "#version 150\n";
#endif
		vShader = vShader + "in vec2 vertex_position;\n";
        vShader += "in vec4 vertex_colour;\n";
		vShader += "in vec4 vertex_tcoord;\n";
		vShader += "out vec4 fragment_colour;\n";
		vShader += "uniform mat4 viewMatrix;\n";
		vShader += "uniform mat4 projMatrix;\n";
		vShader += "void main()\n";
		vShader += "{\n";
		vShader = vShader + "\tgl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, 0, 1);\n";
		vShader += "\tfragment_colour = vertex_colour;\n";
		vShader += "}\n";
        
#if kBuildOpenGles3
        String fShader = "#version 300 es\n";
        fShader += "precision highp float;\n";
#else //kBuildOpenGl3
        String fShader = "#version 150\n";
#endif
		fShader += "in vec4 fragment_colour;\n";
		fShader += "out vec4 output_colour;\n";
		fShader += "void main()\n";
		fShader += "{\n";
		fShader += "\toutput_colour = fragment_colour;\n";
		fShader += "}\n";
#endif
        
		sFillShader = gs::ShaderNew( vShader.toStr(), fShader.toStr() );
	}
	
	void DrawKill()
	{
        DrawLinesKill();
        
		gs::ShaderDelete( sFillShader );
        MeshKill( sMesh );
	}
    
	u16* GenerateElements( u16* p, u16 offset )
	{
		*p ++= 0 + offset;
		*p ++= 1 + offset;
		*p ++= 2 + offset;
		*p ++= 1 + offset;
        *p ++= 3 + offset;
		*p ++= 2 + offset;
		return p;
	}
    
    // 1 3
    // 0 2
    // 012
    // 132
	
	f32* GenerateVertices( f32* p, u32 stride, const fw::Rect& rect )
	{
		stride -= 2;
        *p ++= rect.MinX(); *p ++= rect.MinY(); p += stride;
		*p ++= rect.MinX(); *p ++= rect.MaxY(); p += stride;
		*p ++= rect.MaxX(); *p ++= rect.MinY(); p += stride;
		*p ++= rect.MaxX(); *p ++= rect.MaxY(); p += stride;
		return p;
	}
    
    f32* GenerateWireVertices( f32* p, u32 stride, const fw::Rect& rect )
	{
		stride -= 2;
        *p ++= rect.MinX(); *p ++= rect.MinY(); p += stride;
		*p ++= rect.MinX(); *p ++= rect.MaxY(); p += stride;
		*p ++= rect.MaxX(); *p ++= rect.MaxY(); p += stride;
		*p ++= rect.MaxX(); *p ++= rect.MinY(); p += stride;
		return p;
	}
    
    f32* GenerateUniforms( f32* p, s32 stride, v4 u )
	{
		stride -= 4;
        *p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
        return p;
	}
    
    f32* GenerateUniforms( f32* p, s32 stride, const fw::Rect& rect )
	{
		stride -= 2;
        *p++ = rect.MinX(); *p++ = rect.MinY(); p += stride;
        *p++ = rect.MinX(); *p++ = rect.MaxY(); p += stride;
        *p++ = rect.MaxX(); *p++ = rect.MinY(); p += stride;
        *p++ = rect.MaxX(); *p++ = rect.MaxY(); p += stride;
        return p;
	}
    
    f32* GenerateUniforms( f32* p, s32 stride, const fw::Rect& rect, const f32 u )
	{
		stride -= 3;
        *p++ = rect.MinX(); *p++ = rect.MinY(); *p++ = u; p += stride;
        *p++ = rect.MinX(); *p++ = rect.MaxY(); *p++ = u; p += stride;
        *p++ = rect.MaxX(); *p++ = rect.MinY(); *p++ = u; p += stride;
        *p++ = rect.MaxX(); *p++ = rect.MaxY(); *p++ = u; p += stride;
        return p;
	}

	void DrawWireRect( const Rect& vertices, const v4& colour )
	{
		u16* elementData = ( u16* )MeshGetElementData( sMesh, 0 );
		f32* vertexData = ( f32* )MeshGetVertexData( sMesh, 0 );
        MeshSetElementCount( sMesh, 6 );
        MeshSetVertexCount( sMesh, 4 );
		GenerateElements( elementData, 0 );
		GenerateWireVertices( vertexData, kAttribSize, vertices );
        GenerateUniforms( vertexData + kVertexAttribSize, kAttribSize, colour );
        MeshDraw( sMesh, gs::ePrimLineLoop );
	}
    
    void DrawRect( const Rect& vertices, const v4& colour )
	{
        if( sBatching )
        {
            u16* elementData = ( u16* )MeshGetElementData( sMesh, sBatchSize * 6 );
            f32* vertexData = ( f32* )MeshGetVertexData( sMesh, sBatchSize * 4 );
            GenerateElements( elementData, sBatchSize * 4 );
            GenerateVertices( vertexData, kAttribSize, vertices );
            GenerateUniforms( vertexData + kVertexAttribSize, kAttribSize, colour );
            sBatchSize++;
        }
        else
        {
            u16* elementData = ( u16* )MeshGetElementData( sMesh, 0 );
            f32* vertexData = ( f32* )MeshGetVertexData( sMesh, 0 );
            MeshSetElementCount( sMesh, 6 );
            MeshSetVertexCount( sMesh, 4 );
            GenerateElements( elementData, 0 );
            GenerateVertices( vertexData, kAttribSize, vertices );
            GenerateUniforms( vertexData + kVertexAttribSize, kAttribSize, colour );
            MeshDraw( sMesh, gs::ePrimTriangles );
        }
	}
	
	void DrawRect( const Rect& vertices, const Rect& tcoords )
	{
		u16* elementData = ( u16* )MeshGetElementData( sMesh, 0 );
		f32* vertexData = ( f32* )MeshGetVertexData( sMesh, 0 );
        MeshSetElementCount( sMesh, 6 );
        MeshSetVertexCount( sMesh, 4 );
		GenerateElements( elementData, 0 );
		GenerateVertices( vertexData, kAttribSize, vertices );
        GenerateUniforms( vertexData + (kVertexAttribSize + kColourAttribSize), kAttribSize, tcoords );
        MeshDraw( sMesh, gs::ePrimTriangles );
	}
	
	void DrawRect( const Rect& vertices, const Rect& tcoords, const f32 uniform )
	{
		u16* elementData = ( u16* )MeshGetElementData( sMesh, 0 );
		f32* vertexData = ( f32* )MeshGetVertexData( sMesh, 0 );
        MeshSetElementCount( sMesh, 6 );
        MeshSetVertexCount( sMesh, 4 );
		GenerateElements( elementData, 0 );
		GenerateVertices( vertexData, kAttribSize, vertices );
        GenerateUniforms( vertexData + (kVertexAttribSize + kColourAttribSize), kAttribSize, tcoords, uniform );
        MeshDraw( sMesh, gs::ePrimTriangles );
	}

	void FillRect( const Rect& vertices, const v4& colour )
	{
        if( !sBatching )
        {
            gs::ShaderSet( sFillShader );
        }
		DrawRect( vertices, colour );
	}
	
	void FillWireRect( const Rect& vertices, const v4& colour )
	{
		gs::ShaderSet( sFillShader );
		DrawWireRect( vertices, colour );
	}
    
    void BatchBegin()
    {
        // should be triangles not triangle strip
        gs::ShaderSet( sFillShader );

        sBatching = true;
        sBatchSize = 0;
        
        MeshSetElementCount( sMesh, 0 );
        MeshSetVertexCount( sMesh, 0 );
    }
    
    void BatchEnd( gs::ePrim primitive )
    {
        MeshSetElementCount( sMesh, 6 * sBatchSize );
        MeshSetVertexCount( sMesh, 4 * sBatchSize );

        MeshDraw( sMesh, primitive );
        
        sBatching = false;
    }
}