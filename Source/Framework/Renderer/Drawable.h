#pragma once

// Vendor GLAD Includes (Has to be included before GLFW!).
#include "Vendor/OpenGL/glad/glad.h"

// Framework Includes.
#include "Renderer/Camera.h"
#include "Renderer/Material.h"
#include "Renderer/VertexArray.h"

#include "Scene/Transform.h"

namespace Framework
{
	/* Forward Declarations. */
	class VertexArray;

	class Drawable
	{
	public:
		Drawable( Material* material, Transform* transform, const VertexArray* vertex_array );
		~Drawable();

		void Submit( Camera* camera );

	public:
		Transform* const transform;

	private:
		Material* material;
		Shader* const shader;
		const VertexArray* vertex_array;
	};
}
