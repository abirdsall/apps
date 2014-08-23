#include "fw.h"

namespace fw
{
	void MeshInit( Mesh& mesh )
	{
		mesh.mVertexArrayId = gs::NewVertexArray();
		mesh.mFinalised = false;
		mesh.mElementDataSet = false;
		mesh.mVertexDataSet = false;
		
		for( s32 i = 0; i < gs::eAttribCount; i++ )
		{
			mesh.mAttribSize[ i ] = 0;
		}
	}

	void MeshSetElementData( Mesh& mesh, void* elementData, s32 elementSize, s32 elementCount, s32 elementLimit, bool dynamic )
	{
		ASSERT( !mesh.mFinalised );
		mesh.mElementDataDirty = false;
		mesh.mElementDataDynamic = dynamic;
		mesh.mElementDataSet = true;
		mesh.mElementData = elementData;
		mesh.mElementSize = elementSize;
		mesh.mElementCount = elementCount;
		mesh.mElementLimit = elementLimit;
	}
	
	void MeshSetVertexData( Mesh& mesh, void* vertexData, s32 vertexSize, s32 vertexCount, s32 vertexLimit, bool dynamic )
	{
		ASSERT( !mesh.mFinalised );
		mesh.mVertexDataDirty = false;
		mesh.mVertexDataDynamic = dynamic;
		mesh.mVertexDataSet = true;
		mesh.mVertexData = vertexData;
		mesh.mVertexSize = vertexSize;
		mesh.mVertexCount = vertexCount;
		mesh.mVertexLimit = vertexLimit;
	}
	
	void MeshSetAttrib( Mesh& mesh, gs::eAttrib attrib, void* offset, s32 size )
	{
		mesh.mAttribOffset[ attrib ] = offset;
		mesh.mAttribSize[ attrib ] = size;
	}
	
	s32 MeshGetElementCount( Mesh& mesh )
	{
		return mesh.mElementCount;
	}
	
	s32 MeshGetVertexCount( Mesh& mesh )
	{
		return mesh.mVertexCount;
	}
	
	void MeshSetElementCount( Mesh& mesh, s32 count )
	{
		mesh.mElementCount = count;
        mesh.mElementDataDirty = true;
		ASSERT( mesh.mElementCount <= mesh.mElementLimit );
	}
	
	void MeshSetVertexCount( Mesh& mesh, s32 count )
	{
		mesh.mVertexCount = count;
        mesh.mVertexDataDirty = true;
		ASSERT( mesh.mVertexCount <= mesh.mVertexLimit );
	}

	void* MeshGetElementData( Mesh& mesh, s32 elementIndex )
	{
		ASSERT( mesh.mElementDataSet );
		return ( void* )( ( ( u8* )mesh.mElementData ) + mesh.mElementSize * elementIndex );
	}
	
	void* MeshGetVertexData( Mesh& mesh, s32 vertexIndex )
	{
		ASSERT( mesh.mVertexDataSet );
		return ( void* )( ( ( u8* )mesh.mVertexData ) + mesh.mVertexSize * vertexIndex );
	}

	void MeshFinalise( Mesh& mesh )
	{
		ASSERT( !mesh.mFinalised );
		ASSERT( mesh.mElementDataSet );
		ASSERT( mesh.mVertexDataSet );
		
		mesh.mElementBufferId = gs::NewElementBuffer();
		gs::SetVertexArray( mesh.mVertexArrayId );
		gs::SetElementBuffer( mesh.mElementBufferId );
		gs::FillElementBuffer( mesh.mElementData, mesh.mElementSize * mesh.mElementLimit, mesh.mElementDataDynamic );
        gs::SetElementBuffer( 0 );
        gs::SetVertexArray( 0 );
        
		mesh.mVertexBufferId = gs::NewVertexBuffer();
		gs::SetVertexArray( mesh.mVertexArrayId );
		gs::SetVertexBuffer( mesh.mVertexBufferId );
		gs::FillVertexBuffer( mesh.mVertexData, mesh.mVertexSize * mesh.mVertexLimit, mesh.mVertexDataDynamic );
        gs::SetVertexBuffer( 0 );
        gs::SetVertexArray( 0 );
		
		mesh.mFinalised = true;
	}
	
	void MeshDraw( Mesh& mesh, gs::ePrim primitive )
	{
		ASSERT( mesh.mFinalised );
		ASSERT( mesh.mElementDataSet );
		ASSERT( mesh.mVertexDataSet );
		ASSERT( mesh.mAttribSize[ gs::eAttribVertex ] > 0 );
		
		gs::SetVertexArray( mesh.mVertexArrayId );
		gs::SetVertexBuffer( mesh.mVertexBufferId );
		
		if( mesh.mVertexDataDirty )
		{
			mesh.mVertexDataDirty = false;
            gs::FillVertexBuffer( mesh.mVertexData, mesh.mVertexSize * mesh.mVertexLimit, false );
			//gs::UpdateVertexBuffer( mesh.mVertexData, mesh.mVertexSize * mesh.mVertexLimit, 0 );
		}
		
		for( s32 i = 0; i < gs::eAttribCount; i++ )
		{
			if( mesh.mAttribSize[ i ] > 0 )
			{
				gs::SetArray( ( gs::eAttrib )i, mesh.mAttribSize[ i ], mesh.mVertexSize, mesh.mAttribOffset[ i ] );
			}
		}
		
		gs::SetElementBuffer( mesh.mElementBufferId );
		
		if( mesh.mElementDataDirty )
		{
			mesh.mElementDataDirty = false;
			gs::FillElementBuffer( mesh.mElementData, mesh.mElementSize * mesh.mElementCount, false );
            //gs::UpdateElementBuffer( mesh.mElementData, mesh.mElementSize * mesh.mElementCount, 0 );
		}
		
		gs::DrawElements( primitive, mesh.mElementCount );
		
		gs::SetElementBuffer( 0 );
		
		for( s32 i = 0; i < gs::eAttribCount; i++ )
		{
			if( mesh.mAttribSize[ i ] > 0 )
			{
				gs::UnsetArray( ( gs::eAttrib )i );
			}
		}
		
		gs::SetVertexBuffer( 0 );
        
        gs::SetVertexArray( 0 );
	}
	
	void MeshKill( Mesh& mesh )
	{
		gs::DeleteVertexArray( mesh.mVertexArrayId );
		
		if( mesh.mElementDataSet )
		{
			gs::DeleteElementBuffer( mesh.mElementBufferId );
			mesh.mElementDataSet = false;
		}
		
		if( mesh.mVertexDataSet )
		{
			gs::DeleteVertexBuffer( mesh.mVertexBufferId );
			mesh.mVertexDataSet = false;
		}
		
		if( mesh.mElementData )
		{
			core::free( mesh.mElementData );
			mesh.mElementData = kNull;
		}
		
		if( mesh.mVertexData )
		{
			core::free( mesh.mVertexData );
			mesh.mVertexData = kNull;
		}		
	}
}
