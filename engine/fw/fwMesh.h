#ifndef FW_MESH
#define FW_MESH

#include "gs.h"

namespace fw
{
	struct Mesh
	{
		u32 mVertexArrayId;
		
		bool mFinalised;
		
		bool mElementDataDirty;
		bool mElementDataDynamic;
		bool mElementDataSet;
		
		bool mVertexDataDirty;
		bool mVertexDataDynamic;
		bool mVertexDataSet;
		
		void* mElementData;
		s32 mElementSize;
		s32 mElementCount;
		s32 mElementLimit;
		u32 mElementBufferId;
		
		void* mVertexData;
		s32 mVertexSize;
		s32 mVertexCount;
		s32 mVertexLimit;
		u32 mVertexBufferId;
		
		void* mAttribOffset[ gs::eAttribCount ];
		s32 mAttribSize[ gs::eAttribCount ];
	};
	
	void MeshInit( Mesh& mesh );
	
	void MeshSetElementData( Mesh& mesh, void* elementData, s32 elementSize, s32 elementCount, s32 elementLimit, bool dynamic );
	void MeshSetVertexData( Mesh& mesh, void* vertexData, s32 vertexSize, s32 vertexCount, s32 vertexLimit, bool dynamic );
	void MeshSetAttrib( Mesh& mesh, gs::eAttrib attrib, void* offset, s32 size );
	
	s32 MeshGetElementCount( Mesh& mesh );
	s32 MeshGetVertexCount( Mesh& mesh );
	
	void MeshSetElementCount( Mesh& mesh, s32 count );
	void MeshSetVertexCount( Mesh& mesh, s32 count );
	
	void* MeshGetElementData( Mesh& mesh, s32 elementIndex );
	void* MeshGetVertexData( Mesh& mesh, s32 vertexIndex );
	
	void MeshFinalise( Mesh& mesh );
	
	void MeshDraw( Mesh& mesh, gs::ePrim primitive );
	
	void MeshKill( Mesh& mesh );
	
	f32* MeshGenerateUniforms( f32* p, s32 stride, const v2& u );
	f32* MeshGenerateUniforms( f32* p, s32 stride, const v4& u );
}

#endif