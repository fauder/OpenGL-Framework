#pragma once

// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "IndexBuffer.h"
#include "Shader.h"
#include "Vector.hpp"

// std Includes.
#include <vector>

namespace Framework
{
	/* Forward Declarations. */
	class VertexArray;

	class Drawable_Indexed
	{
	public:
		Drawable_Indexed( const Shader& shader, const VertexArray& vertex_array, const IndexBuffer& index_buffer );
		~Drawable_Indexed();

		void Draw();

	private:
		const IndexBuffer& index_buffer;
		const Shader& shader;
		const VertexArray& vertex_array;
	};
}
