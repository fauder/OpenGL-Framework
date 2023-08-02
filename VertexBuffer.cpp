// Project Includes.
#include "VertexBuffer.h"

namespace Framework
{
	VertexBuffer::~VertexBuffer()
	{
	}

	void VertexBuffer::Bind() const
	{
		GLCALL( glBindBuffer( GL_ARRAY_BUFFER, id ) );
	}

	void VertexBuffer::Unbind() const
	{
		GLCALL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	}

	void VertexBuffer::CleanUp() const
	{
		GLCALL( glDeleteBuffers( 1, &id ) );
	}
}
