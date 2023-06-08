#pragma once

// GLAD Includes.
#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer( const void* data, const unsigned int size );
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int Size() const { return size; }

private:
	GLuint id;
	unsigned int size;
};
