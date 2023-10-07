#pragma once

// GLAD Includes.
#include <glad/glad.h> // TODO: Move this from under C:\Sys or whatever it is in, into the project and get rid of the reference to this external directory.

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
