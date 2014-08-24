#include "cubeRenderer.h"
#include "fwCubeComponent.h"
#include "fwMesh.h"

void CubeRenderer::InitVoxeliser()
{	
	s32 elementLimit = mLimit * 32;
	s32 elementSize = sizeof( u16 );
	
	s32 vertexLimit = mLimit * 24;
	s32 vertexSize = sizeof( f32 ) * 9;
	
	fw::MeshInit( mVoxeliser );
	fw::MeshSetElementData( mVoxeliser, core::alloc( elementSize * elementLimit ), elementSize, 0, elementLimit, false );
	fw::MeshSetVertexData( mVoxeliser, core::alloc( vertexSize * vertexLimit ), vertexSize, 0, vertexLimit, false );
	fw::MeshSetAttrib( mVoxeliser, gs::eAttribVertex, ( void* )( sizeof( f32 ) * 0 ), 3 );
	fw::MeshSetAttrib( mVoxeliser, gs::eAttribColour, ( void* )( sizeof( f32 ) * 3 ), 4 );
	fw::MeshSetAttrib( mVoxeliser, gs::eAttribTcoord, ( void* )( sizeof( f32 ) * 7 ), 2 );
}

void CubeRenderer::InitRenderer()
{
	s32 elementLimit = mLimit * 32;
	s32 elementSize = sizeof( u16 );
	
	s32 vertexLimit = mLimit * 24;
	s32 vertexSize = sizeof( f32 ) * 10;
	
	fw::MeshInit( mRenderer );
	fw::MeshSetElementData( mRenderer, core::alloc( elementSize * elementLimit ), elementSize, 0, elementLimit, false );
	fw::MeshSetVertexData( mRenderer, core::alloc( vertexSize * vertexLimit ), vertexSize, 0, vertexLimit, false );
	fw::MeshSetAttrib( mRenderer, gs::eAttribVertex, ( void* )( sizeof( f32 ) * 0 ), 3 );
	fw::MeshSetAttrib( mRenderer, gs::eAttribNormal, ( void* )( sizeof( f32 ) * 3 ), 3 );
	fw::MeshSetAttrib( mRenderer, gs::eAttribColour, ( void* )( sizeof( f32 ) * 6 ), 4 );
}

void CubeRenderer::AddToVoxeliser( const fw::Component& component )
{
	const fw::CubeComponent& cube = ( const fw::CubeComponent& )component;

	s32 elementCount = fw::MeshGetElementCount( mVoxeliser );
	s32 vertexCount = fw::MeshGetVertexCount( mVoxeliser );
	
	u16* elementData = ( u16* )fw::MeshGetElementData( mVoxeliser, elementCount );
	f32* vertexData = ( f32* )fw::MeshGetVertexData( mVoxeliser, vertexCount );
		
	cube.GenerateElements( elementData, vertexCount );
	cube.GenerateVertices( vertexData, 9 );
	cube.GenerateUniforms( vertexData + 3, 9, cube.Material().Colour() );
	cube.GenerateUniforms( vertexData + 7, 9, v2( cube.Cube().Position().z, cube.Cube().Radius().z ) );
	
	fw::MeshSetElementCount( mVoxeliser, elementCount + 36 );
	fw::MeshSetVertexCount( mVoxeliser, vertexCount + 24 );
}

void CubeRenderer::AddToRenderer( const fw::Component& component )
{
	const fw::CubeComponent& cube = ( const fw::CubeComponent& )component;
	
	s32 elementCount = fw::MeshGetElementCount( mRenderer );
	s32 vertexCount = fw::MeshGetVertexCount( mRenderer );
	
	u16* elementData = ( u16* )fw::MeshGetElementData( mRenderer, elementCount );
	f32* vertexData = ( f32* )fw::MeshGetVertexData( mRenderer, vertexCount );
	
	cube.GenerateElements( elementData, vertexCount );
	cube.GenerateVertices( vertexData, 10 );
	cube.GenerateNormals( vertexData + 3, 10 );
	cube.GenerateUniforms( vertexData + 6, 10, cube.Material().Colour() );
	
	fw::MeshSetElementCount( mRenderer, elementCount + 36 );
	fw::MeshSetVertexCount( mRenderer, vertexCount + 24 );
}

size_t CubeRenderer::ComponentSize()
{
	return sizeof( fw::CubeComponent );
}
