// Framework Includes.
#include "Renderer/Graphics.h"
#include "Renderer/VertexArray.h"

namespace Framework
{
	VertexArray::VertexArray( const VertexBuffer& vertex_buffer, const VertexBufferLayout& vertex_buffer_layout )
		:
		vertex_count( vertex_buffer.VertexCount() ),
		id_vertex_buffer( vertex_buffer.ID() )
	{
		GLCALL( glGenVertexArrays( 1, &id ) );

		Bind();
		vertex_buffer.Bind();

		const auto& attribute_list = vertex_buffer_layout.GetAttributes();
		const unsigned int stride = vertex_buffer_layout.GetStride();
		unsigned int offset = 0;
		for( auto i = 0; i < attribute_list.size(); i++ )
		{
			const auto& attribute = attribute_list[ i ];
			GLCALL( glEnableVertexAttribArray( /* Location: */ i ) );
			GLCALL( glVertexAttribPointer( /* Location: */ i, attribute.count, attribute.type, attribute.normalized, stride, BUFFER_OFFSET( offset ) ) );
			offset += attribute.count * attribute.GetSizeOfType();
		}
	}

	VertexArray::~VertexArray()
	{
		GLCALL( glDeleteBuffers( 1, &id_vertex_buffer ) );
		GLCALL( glDeleteVertexArrays( 1, &id ) );
	}

	void VertexArray::Bind() const
	{
		GLCALL( glBindVertexArray( id ) );
	}

	void VertexArray::Unbind() const
	{
		GLCALL( glBindVertexArray( 0 ) );
	}
}
