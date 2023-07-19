#pragma once

// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Shader.h"
#include "VertexArray.h"

namespace Framework
{
	/* Forward Declarations. */
	class VertexArray;

	class Drawable
	{
	public:
		Drawable( const Shader& shader, const VertexArray& vertex_array );
		~Drawable();

		void Draw() const;

	private:
		const Shader& shader;
		const VertexArray& vertex_array;
	};
}
