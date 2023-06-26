// Project Includes.
#include "Drawable.h"
#include "Graphics.h"
#include "VertexArray.h"

namespace Framework
{
	Drawable::Drawable( const Shader& shader, const VertexArray& vertex_array )
		:
		shader( shader ),
		vertex_array( vertex_array )
	{
	}

	Drawable::~Drawable()
	{
	}

	void Drawable::Draw() const
	{
		vertex_array.Bind();
		shader.Bind();
		GLCALL( glDrawArrays( GL_TRIANGLES, 0, vertex_array.VertexCount() ) );
	}
}
