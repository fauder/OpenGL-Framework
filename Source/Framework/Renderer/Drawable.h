#pragma once

// Vendor GLAD Includes (Has to be included before GLFW!).
#include "Vendor/OpenGL/glad/glad.h"

// Framework Includes.
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "Scene/Transform.h"

namespace Framework
{
	/* Forward Declarations. */
	class VertexArray;

	class Drawable
	{
	public:
		Drawable( Shader* shader, const VertexArray* vertex_array );
		~Drawable();

		void Draw();

	public:
		Transform transform;

	private:
		Shader* shader;
		const VertexArray* vertex_array;
	};
}
