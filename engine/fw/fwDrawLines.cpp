#include "fw.h"

namespace fw
{
    static Mesh sMesh;
    static s32 sBatchSize;
    
    static const s32 kBatchSizeLimit = 2048;
    static const s32 kVertexAttribSize = 2;
	static const s32 kColourAttribSize = 4;
	static const s32 kTcoordAttribSize = 4;
    static const s32 kAttribSize = kVertexAttribSize + kColourAttribSize + kTcoordAttribSize;
    
	static gs::ShaderHandle sFillShader;
    
	void DrawLinesInit()
	{
        sBatchSize = 0;
        
        s32 elementLimit = kBatchSizeLimit * 2;
		s32 elementSize = sizeof( u16 );
		
		s32 vertexLimit = kBatchSizeLimit * 2;
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
	
	void DrawLinesKill()
	{
		gs::ShaderDelete( sFillShader );
        MeshKill( sMesh );
	}
    
	u16* GenerateElements_( u16* p, u16 offset )
	{
		*p ++= 0 + offset;
		*p ++= 1 + offset;
		return p;
	}
	
	f32* GenerateVertices_( f32* p, u32 stride, const v2& from, const v2& to )
	{
		stride -= 2;
        *p ++= from.x; *p ++= from.y; p += stride;
		*p ++= to.x; *p ++= to.y; p += stride;
		return p;
	}

    f32* GenerateUniforms_( f32* p, s32 stride, v4 u )
	{
		stride -= 4;
        *p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
		*p ++= u.x; *p ++= u.y; *p ++= u.z; *p ++= u.w; p += stride;
        return p;
	}
    
    void DrawLinesAdd( const v2& from, const v2& to, const v4& colour )
	{
        u16* elementData = ( u16* )MeshGetElementData( sMesh, sBatchSize * 2 );
        f32* vertexData = ( f32* )MeshGetVertexData( sMesh, sBatchSize * 2 );
        GenerateElements_( elementData, sBatchSize * 2 );
        GenerateVertices_( vertexData, kAttribSize, from, to );
        GenerateUniforms_( vertexData + kVertexAttribSize, kAttribSize, colour );
        sBatchSize++;
	}

    void DrawLinesBegin()
    {
        sBatchSize = 0;
        gs::ShaderSet( sFillShader );
    }
    
    void DrawLinesEnd()
    {
        MeshSetElementCount( sMesh, 2 * sBatchSize );
        MeshSetVertexCount( sMesh, 2 * sBatchSize );
        MeshDraw( sMesh, gs::ePrimLines );
    }
}