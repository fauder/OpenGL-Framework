// Framework Includes.
#include "Renderer/Drawable.h"
#include "Renderer/Graphics.h"

namespace Framework
{
	Drawable::Drawable( Material* material, Transform* transform, const VertexArray* vertex_array )
		:
		transform( transform ),
		material( material ),
		shader( material->shader ),
		vertex_array( vertex_array )
	{
	}

	Drawable::~Drawable()
	{
	}

	void Drawable::Submit( Camera* camera )
	{
		vertex_array->Bind();
		shader->Bind();

		material->SetMatrix( "transform_world",		 transform->GetFinalMatrix() );

		material->SetMatrix( "transform_view",		 camera->GetViewMatrix() );
		material->SetMatrix( "transform_projection", camera->GetProjectionMatrix() );

		GLCALL( glDrawArrays( GL_TRIANGLES, 0, vertex_array->VertexCount() ) );
	}
}
