// Project Includes.
#include "Drawable_Indexed.h"
#include "Graphics.h"
#include "VertexArray.h"

namespace Framework
{
	Drawable_Indexed::Drawable_Indexed( const Shader& shader, const VertexArray& vertex_array, const IndexBuffer& index_buffer )
		:
		index_buffer( index_buffer ),
		shader( shader ),
		vertex_array( vertex_array )
	{
	}

	Drawable_Indexed::~Drawable_Indexed()
	{
	}

	void Drawable_Indexed::Draw()
	{
		vertex_array.Bind();
		shader.Bind();
		GLCALL( glDrawElements( GL_TRIANGLES, index_buffer.Count(), GL_UNSIGNED_INT, 0 ) );
	}
}
