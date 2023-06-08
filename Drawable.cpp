// Project Includes.
#include "Drawable.h"
#include "Graphics.h"
#include "VertexArray.h"

Drawable::Drawable( const Shader& shader, const VertexArray& vertex_array, const IndexBuffer& index_buffer )
	:
	index_buffer( index_buffer ),
	shader( shader ),
	vertex_array( vertex_array )
{
}

Drawable::~Drawable()
{
}

void Drawable::Draw()
{
	vertex_array.Bind();
	shader.Bind();
	GLCALL( glDrawElements( GL_TRIANGLES, index_buffer.Count(), GL_UNSIGNED_INT, 0 ) );
}
