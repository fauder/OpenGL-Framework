#pragma once

// Project Includes.
#include "Graphics.h"

namespace Framework
{
	/* While a CleanUp() function is provided, expected usage is to not invoke it, as a VertexArray will take care of this on destruction. */
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

		void CleanUp() const;

		inline GLuint ID() const { return id; }

		inline unsigned int Size() const { return size; }
		inline unsigned int VertexCount() const { return vertex_count; }

	private:
		GLuint id;
		unsigned int vertex_count;
		unsigned int size;
	};
}
