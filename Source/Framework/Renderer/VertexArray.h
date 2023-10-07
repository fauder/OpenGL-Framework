#pragma once

// Framework Includes
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"

namespace Framework
{
	class VertexArray
	{
	public:
		// We could make it so the buffer & layout are "Add()ed" after construction. This way, a VA could change its vb & layout after creation.
		// We won't make it so in the beginning to see if the need arises & learn through experience.
		VertexArray( const VertexBuffer& vertex_buffer, const VertexBufferLayout& vertex_buffer_layout );
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		inline unsigned int VertexCount() const { return vertex_count; }

	private:
		unsigned int id;
		unsigned int vertex_count;

		unsigned int id_vertex_buffer;
	};
}
