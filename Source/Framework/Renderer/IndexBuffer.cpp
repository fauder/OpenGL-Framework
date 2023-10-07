// GLAD Includes.
#include <glad/glad.h> // TODO: Move this from under C:\Sys or whatever it is in, into the project and get rid of the reference to this external directory.

// Framework Includes.
#include "Renderer/Graphics.h"
#include "Renderer/IndexBuffer.h"

// std Includes.
#include <iostream>

namespace Framework
{
	IndexBuffer::IndexBuffer( const void* data, const unsigned int count )
		:
		count( count )
	{
		ASSERT( sizeof( GLuint ) == sizeof( unsigned int ) );

		GLCALL( glGenBuffers( 1, &index_buffer_object_id ) );
		Bind();
		GLCALL( glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( GLuint ), data, GL_STATIC_DRAW ) );
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCALL( glDeleteBuffers( 1, &index_buffer_object_id ) );
	}

	void IndexBuffer::Bind() const
	{
		GLCALL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_id ) );
	}

	void IndexBuffer::Unbind() const
	{
		GLCALL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
	}
}
