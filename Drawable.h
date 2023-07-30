#pragma once

// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Shader.h"
#include "VertexArray.h"
#include "Transform.h"

namespace Framework
{
	/* Forward Declarations. */
	class VertexArray;

	class Drawable
	{
	public:
		Drawable( Shader& shader, const VertexArray& vertex_array );
		~Drawable();

		void Draw();

	public:
		Transform transform;

	private:
		Shader& shader;
		const VertexArray& vertex_array;
	};
}
