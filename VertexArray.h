#pragma once

// Project Includes
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	// We could make it so the buffer & layout are "Add()ed" after construction. This way, a VA could change its vb & layout after creation.
	// We won't make it so in the beginning to see if the need arises & learn through experience.
	VertexArray( const VertexBuffer& vertex_buffer, const VertexBufferLayout& vertex_buffer_layout );
	~VertexArray();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int id;
};