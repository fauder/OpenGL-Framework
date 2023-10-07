#pragma once

// Vendor GLAD Includes (Has to be included before GLFW!).
#include <Vendor/OpenGL/glad/glad.h>

// Framework Includes.
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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
