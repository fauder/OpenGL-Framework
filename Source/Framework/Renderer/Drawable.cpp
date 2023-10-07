// Framework Includes.
#include "Renderer/Drawable.h"
#include "Renderer/Graphics.h"

namespace Framework
{
	Drawable::Drawable( Shader* shader, const VertexArray* vertex_array )
		:
		shader( shader ),
		vertex_array( vertex_array )
	{
	}

	Drawable::~Drawable()
	{
	}

	void Drawable::Draw()
	{
		vertex_array->Bind();
		shader->SetMatrix( "transformation_world", transform.GetFinalMatrix() );
		shader->Bind();
		GLCALL( glDrawArrays( GL_TRIANGLES, 0, vertex_array->VertexCount() ) );
	}
}
