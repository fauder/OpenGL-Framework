// Project Includes.
#include "Graphics.h"
#include "VertexBuffer.h"

// std Includes.
#include <iostream>

VertexBuffer::VertexBuffer( const void* data, const unsigned int size )
	:
	size( size )
{
	GLCALL( glGenBuffers( 1, &id ) );
	Bind();
	GLCALL( glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW ) );
}

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
