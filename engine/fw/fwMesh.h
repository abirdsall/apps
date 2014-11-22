#ifndef FW_MESH
#define FW_MESH

namespace fw
{
	struct Mesh
	{
		u32 _vertexArrayId;
		
		bool _finalised;
		
		bool _elementDataDirty;
		bool _elementDataDynamic;
		bool _elementDataSet;
		
		bool _vertexDataDirty;
		bool _vertexDataDynamic;
		bool _vertexDataSet;
		
		void* _elementData;
		s32 _elementSize;
		s32 _elementCount;
		s32 _elementLimit;
		u32 _elementBufferId;
		
		void* _vertexData;
		s32 _vertexSize;
		s32 _vertexCount;
		s32 _vertexLimit;
		u32 _vertexBufferId;
		
		void* _attributeOffset[ gs::AttributeCount ];
		s32 _attributeSize[ gs::AttributeCount ];
	};
	
	void MeshInit( Mesh& mesh );
	
	void MeshSetElementData( Mesh& mesh, void* elementData, s32 elementSize, s32 elementCount, s32 elementLimit, bool dynamic );
	void MeshSetVertexData( Mesh& mesh, void* vertexData, s32 vertexSize, s32 vertexCount, s32 vertexLimit, bool dynamic );
	void MeshSetAttrib( Mesh& mesh, gs::Attribute attrib, void* offset, s32 size );
	
	s32 MeshGetElementCount( Mesh& mesh );
	s32 MeshGetVertexCount( Mesh& mesh );
	
	void MeshSetElementCount( Mesh& mesh, s32 count );
	void MeshSetVertexCount( Mesh& mesh, s32 count );
	
	void* MeshGetElementData( Mesh& mesh, s32 elementIndex );
	void* MeshGetVertexData( Mesh& mesh, s32 vertexIndex );
	
	void MeshFinalise( Mesh& mesh );
	
	void MeshDraw( Mesh& mesh, gs::Primitive primitive );
	
	void MeshKill( Mesh& mesh );
	
	f32* MeshGenerateUniforms( f32* p, s32 stride, const v2& u );
	f32* MeshGenerateUniforms( f32* p, s32 stride, const v4& u );
}

#endif