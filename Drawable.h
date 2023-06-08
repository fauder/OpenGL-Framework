#pragma once

// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "IndexBuffer.h"
#include "Shader.h"
#include "Vector.hpp"

// std Includes.
#include <vector>

/* Forward Declarations. */
class VertexArray;

class Drawable
{
public:
	Drawable( const Shader& shader, const VertexArray& vertex_array, const IndexBuffer& index_buffer );
	~Drawable();

	void Draw();

private:
	const IndexBuffer& index_buffer;
	const Shader& shader;
	const VertexArray& vertex_array;
};