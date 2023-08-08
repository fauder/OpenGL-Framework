// Project Includes.
#include "Test_Camera.h"

#include "../Input.h"
#include "../Matrix.h"

namespace Framework::Test
{
	using namespace Framework::Math::Literals;

	Test_Camera::Test_Camera( Renderer& renderer )
		:
		Test( renderer ),
		camera_transform()
	{
		using namespace Framework;

		shader = std::make_unique< Shader >( "Asset/Shader/Vertex.shader", "Asset/Shader/Fragment.shader" );

		const std::vector< Math::Vector< float, 3 + 2 + 4 > > vertices =
		{
			/* Object space coordinates			UVs. */
			{ -0.5f, -0.5f, -0.5f,			0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // BACK
			{  0.5f, -0.5f, -0.5f,			1.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // BACK
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // BACK
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // BACK
			{ -0.5f,  0.5f, -0.5f,			0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // BACK
			{ -0.5f, -0.5f, -0.5f,			0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // BACK

			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // FRONT
			{  0.5f, -0.5f,  0.5f,			1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // FRONT
			{  0.5f,  0.5f,  0.5f,			1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // FRONT
			{  0.5f,  0.5f,  0.5f,			1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // FRONT
			{ -0.5f,  0.5f,  0.5f,			0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // FRONT
			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // FRONT

			{ -0.5f,  0.5f,  0.5f,			1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f }, // LEFT
			{ -0.5f,  0.5f, -0.5f,			1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f }, // LEFT
			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f }, // LEFT
			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f }, // LEFT
			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f }, // LEFT
			{ -0.5f,  0.5f,  0.5f,			1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f }, // LEFT

			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f }, // RIGHT
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f }, // RIGHT
			{  0.5f, -0.5f, -0.5f,			0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f }, // RIGHT
			{  0.5f, -0.5f, -0.5f,			0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f }, // RIGHT
			{  0.5f, -0.5f,  0.5f,			0.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f }, // RIGHT
			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f }, // RIGHT

			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f }, // BOTTOM
			{  0.5f, -0.5f, -0.5f,			1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f }, // BOTTOM
			{  0.5f, -0.5f,  0.5f,			1.0f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f }, // BOTTOM
			{  0.5f, -0.5f,  0.5f,			1.0f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f }, // BOTTOM
			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f,		0.0f, 1.0f, 1.0f, 1.0f }, // BOTTOM
			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f }, // BOTTOM

			{ -0.5f,  0.5f, -0.5f,			0.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f }, // TOP
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f }, // TOP
			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f }, // TOP
			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f }, // TOP
			{ -0.5f,  0.5f,  0.5f,			0.0f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f }, // TOP
			{ -0.5f,  0.5f, -0.5f,			0.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f }  // TOP
		};

		VertexBuffer vertex_buffer( vertices.data(), static_cast< unsigned int >( vertices.size() ) );

		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push< float >( 3 ); // NDC positions.
		vertex_buffer_layout.Push< float >( 2 ); // Tex. coords.
		vertex_buffer_layout.Push< float >( 4 ); // Vertex colors.
		cube_vertex_array = std::make_unique< VertexArray >( vertex_buffer, vertex_buffer_layout );

		cube_1 = std::make_unique< Drawable >( shader.get(), cube_vertex_array.get() );
		ground_plane = std::make_unique< Drawable >( shader.get(), cube_vertex_array.get() );

		renderer.AddDrawable( cube_1.get() );
		renderer.AddDrawable( ground_plane.get() );
		renderer.SetPolygonMode( PolygonMode::FILL );

		texture_container = std::make_unique< Texture >( "Asset/Texture/container.jpg", GL_RGB );

		texture_container->ActivateAndBind( GL_TEXTURE0 );

		shader->Bind();
		shader->SetInt( "texture_sampler_1", 0 );

		shader->SetMatrix( "transformation_projection", Matrix::PerspectiveProjection( 0.1f, 100.0f, renderer.AspectRatio(), 45.0_deg ) );

		// Initial camera position and rotation:
		camera_transform.SetTranslation( Vector3::Backward() * 6.0f ).SetRotation( Math::EulerToQuaternion( 0.0_deg, -45.0_deg, 0.0_deg ) );

		ground_plane->transform.SetScaling( { +36.0f, 0.0f, 36.0f } );
	}

	Test_Camera::~Test_Camera()
	{
		renderer.RemoveDrawable( cube_1.get() );
		renderer.RemoveDrawable( ground_plane.get() );
	}

	void Test_Camera::OnRender()
	{
		const float time     = static_cast< float >( glfwGetTime() );
		const float sin_time = Math::Sin( Radians( time ) );

		camera_transform.SetRotation( Math::EulerToQuaternion( 0.0_deg, sin_time * 45.0_deg, 0.0_deg ) );
		Vector3 camera_position = camera_transform.GetTranslation();
		camera_transform.SetTranslation( camera_position.SetY( sin_time ) );

		shader->SetMatrix( "transformation_view", camera_transform.GetInverseOfFinalMatrix() );
	}
}
