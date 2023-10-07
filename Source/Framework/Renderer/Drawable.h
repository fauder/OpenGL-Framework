#pragma once

// GLAD Includes.
#include <glad/glad.h> // TODO: Move this from under C:\Sys or whatever it is in, into the project and get rid of the reference to this external directory.

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
