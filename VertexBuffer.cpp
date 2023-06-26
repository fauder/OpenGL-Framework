// Project Includes.
#include "Graphics.h"
#include "VertexBuffer.h"

// std Includes.
#include <iostream>

namespace Framework
{
	VertexBuffer::~VertexBuffer()
	{
		GLCALL( glDeleteBuffers( 1, &id ) );
	}

	void VertexBuffer::Bind() const
	{
		GLCALL( glBindBuffer( GL_ARRAY_BUFFER, id ) );
	}

	void VertexBuffer::Unbind() const
	{
		GLCALL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	}
}
