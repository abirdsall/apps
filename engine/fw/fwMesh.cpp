#include "fw.h"

namespace fw
{
	void MeshInit( Mesh& mesh )
	{
		mesh._vertexArrayId = gs::NewVertexArray();
		mesh._finalised = false;
		mesh._elementDataSet = false;
		mesh._vertexDataSet = false;
		
		for( s32 i = 0; i < gs::AttributeCount; i++ )
		{
			mesh._attributeSize[ i ] = 0;
		}
	}

	void MeshSetElementData( Mesh& mesh, void* elementData, s32 elementSize, s32 elementCount, s32 elementLimit, bool dynamic )
	{
		ASSERT( !mesh._finalised );
		mesh._elementDataDirty = false;
		mesh._elementDataDynamic = dynamic;
		mesh._elementDataSet = true;
		mesh._elementData = elementData;
		mesh._elementSize = elementSize;
		mesh._elementCount = elementCount;
		mesh._elementLimit = elementLimit;
	}
	
	void MeshSetVertexData( Mesh& mesh, void* vertexData, s32 vertexSize, s32 vertexCount, s32 vertexLimit, bool dynamic )
	{
		ASSERT( !mesh._finalised );
		mesh._vertexDataDirty = false;
		mesh._vertexDataDynamic = dynamic;
		mesh._vertexDataSet = true;
		mesh._vertexData = vertexData;
		mesh._vertexSize = vertexSize;
		mesh._vertexCount = vertexCount;
		mesh._vertexLimit = vertexLimit;
	}
	
	void MeshSetAttrib( Mesh& mesh, gs::Attribute attrib, void* offset, s32 size )
	{
		mesh._attributeOffset[ attrib ] = offset;
		mesh._attributeSize[ attrib ] = size;
	}
	
	s32 MeshGetElementCount( Mesh& mesh )
	{
		return mesh._elementCount;
	}
	
	s32 MeshGetVertexCount( Mesh& mesh )
	{
		return mesh._vertexCount;
	}
	
	void MeshSetElementCount( Mesh& mesh, s32 count )
	{
		mesh._elementCount = count;
        mesh._elementDataDirty = true;
		ASSERT( mesh._elementCount <= mesh._elementLimit );
	}
	
	void MeshSetVertexCount( Mesh& mesh, s32 count )
	{
		mesh._vertexCount = count;
        mesh._vertexDataDirty = true;
		ASSERT( mesh._vertexCount <= mesh._vertexLimit );
	}

	void* MeshGetElementData( Mesh& mesh, s32 elementIndex )
	{
		ASSERT( mesh._elementDataSet );
		return ( void* )( ( ( u8* )mesh._elementData ) + mesh._elementSize * elementIndex );
	}
	
	void* MeshGetVertexData( Mesh& mesh, s32 vertexIndex )
	{
		ASSERT( mesh._vertexDataSet );
		return ( void* )( ( ( u8* )mesh._vertexData ) + mesh._vertexSize * vertexIndex );
	}

	void MeshFinalise( Mesh& mesh )
	{
		ASSERT( !mesh._finalised );
		ASSERT( mesh._elementDataSet );
		ASSERT( mesh._vertexDataSet );
		
		mesh._elementBufferId = gs::NewElementBuffer();
		gs::SetVertexArray( mesh._vertexArrayId );
		gs::SetElementBuffer( mesh._elementBufferId );
		gs::FillElementBuffer( mesh._elementData, mesh._elementSize * mesh._elementLimit, mesh._elementDataDynamic );
        core::set( mesh._elementData, mesh._elementSize * mesh._elementCount, 0 );
        printf("video mem element buffer %d size %d\n", mesh._elementBufferId, mesh._elementSize * mesh._elementLimit / 1024);

        gs::SetElementBuffer( 0 );
        gs::SetVertexArray( 0 );
        
		mesh._vertexBufferId = gs::NewVertexBuffer();
		gs::SetVertexArray( mesh._vertexArrayId );
		gs::SetVertexBuffer( mesh._vertexBufferId );
		gs::FillVertexBuffer( mesh._vertexData, mesh._vertexSize * mesh._vertexLimit, mesh._vertexDataDynamic );
        core::set( mesh._vertexData, mesh._vertexSize * mesh._vertexLimit, 0 );
        printf("video mem vertex buffer %d size %d\n", mesh._vertexBufferId, mesh._vertexSize * mesh._vertexLimit / 1024);
        gs::SetVertexBuffer( 0 );
        gs::SetVertexArray( 0 );
		
		mesh._finalised = true;
	}
	
	void MeshDraw( Mesh& mesh, gs::Primitive primitive )
	{
		ASSERT( mesh._finalised );
		ASSERT( mesh._elementDataSet );
		ASSERT( mesh._vertexDataSet );
		ASSERT( mesh._attributeSize[ gs::AttributeVertex ] > 0 );
		
		gs::SetVertexArray( mesh._vertexArrayId );
		gs::SetVertexBuffer( mesh._vertexBufferId );
        gs::SetElementBuffer( mesh._elementBufferId );
        
        //printf("SZA %d\n", (s32)mesh._vertexArrayId);
		
		if( mesh._vertexDataDirty )
		{
			mesh._vertexDataDirty = false;
            //gs::FillVertexBuffer( Null, mesh._vertexSize * mesh._vertexLimit, mesh._vertexDataDynamic );
            gs::FillVertexBuffer( mesh._vertexData, mesh._vertexSize * mesh._vertexCount, mesh._vertexDataDynamic );
			//gs::UpdateVertexBuffer( mesh._vertexData, mesh._vertexSize * mesh._vertexLimit, 0 );
            //printf("filling vdatabuf %d size %d\n", (s32)mesh._vertexBufferId, mesh._vertexSize * mesh._vertexCount );
            core::set( mesh._vertexData, mesh._vertexSize * mesh._vertexLimit, 0 );
		}
        
		if( mesh._elementDataDirty )
		{
			mesh._elementDataDirty = false;
            //gs::FillElementBuffer( Null, mesh._elementSize * mesh._elementCount, mesh._elementDataDynamic );
			gs::FillElementBuffer( mesh._elementData, mesh._elementSize * mesh._elementCount, mesh._elementDataDynamic );
            //gs::UpdateElementBuffer( mesh._elementData, mesh._elementSize * mesh._elementCount, 0 );
            //printf("filling edatabuf %d size %d\n", (s32)mesh._elementBufferId, mesh._elementSize * mesh._elementCount );
            core::set( mesh._elementData, mesh._elementSize * mesh._elementCount, 0 );
		}
		
        for( s32 i = 0; i < gs::AttributeCount; i++ )
        {
            if( mesh._attributeSize[ i ] > 0 )
            {
                //printf("setting attrib %d vsize %d offset %p\n", i, mesh._vertexSize, (mesh._attributeOffset[ i ]) );
                gs::SetArray( ( gs::Attribute )i, mesh._attributeSize[ i ], mesh._vertexSize, mesh._attributeOffset[ i ] );
            }
        }

		gs::DrawElements( primitive, mesh._elementCount );
				
		for( s32 i = 0; i < gs::AttributeCount; i++ )
		{
			if( mesh._attributeSize[ i ] > 0 )
			{
                //printf("unsetting attrib %d\n", i );
				gs::UnsetArray( ( gs::Attribute )i );
			}
		}
		
        gs::SetElementBuffer( 0 );

		gs::SetVertexBuffer( 0 );
        
        gs::SetVertexArray( 0 );
	}
	
	void MeshKill( Mesh& mesh )
	{
		gs::DeleteVertexArray( mesh._vertexArrayId );
		
		if( mesh._elementDataSet )
		{
			gs::DeleteElementBuffer( mesh._elementBufferId );
			mesh._elementDataSet = false;
		}
		
		if( mesh._vertexDataSet )
		{
			gs::DeleteVertexBuffer( mesh._vertexBufferId );
			mesh._vertexDataSet = false;
		}
		
		if( mesh._elementData )
		{
			core::xfree( mesh._elementData );
			mesh._elementData = Null;
		}
		
		if( mesh._vertexData )
		{
			core::xfree( mesh._vertexData );
			mesh._vertexData = Null;
		}		
	}
}
