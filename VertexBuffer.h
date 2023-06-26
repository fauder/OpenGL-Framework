#pragma once

// GLAD Includes.
#include <glad/glad.h>

namespace Framework
{
	class VertexBuffer
	{
	public:
		template< typename Type >
		VertexBuffer( const Type* data, const unsigned int vertex_count )
			:
			vertex_count( vertex_count ),
			size( sizeof( Type )* vertex_count )
		{
			GLCALL( glGenBuffers( 1, &id ) );
			Bind();
			glBufferData( GL_ARRAY_BUFFER, size, static_cast< const void* >( data ), GL_STATIC_DRAW );
		}
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int Size() const { return size; }
		inline unsigned int VertexCount() const { return vertex_count; }

	private:
		GLuint id;
		unsigned int vertex_count;
		unsigned int size;
	};
}
