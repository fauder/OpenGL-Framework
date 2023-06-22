// Make sure GLAD is included before GLFW3!
#include <glad/glad.h>

// Project Includes.
#include "Drawable.h"
#include "IndexBuffer.h"
#include "Input.h"
#include "Matrix.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Vector.hpp"
#include "VertexArray.h"

// std Includes.
#include <istream>
#include <vector>

int main()
{
	GLFWwindow* window = nullptr;
	
	const auto width_pixels  = 800;
	const auto height_pixels = 600;
	Renderer renderer( window, width_pixels, height_pixels, 1000, 100 );

	const auto aspect_ratio = float( width_pixels ) / height_pixels;

	/* A scope is created here to prevent the Application to go into an endless loop after the render-loop is exited. */
	{
		// Shader creation & compilation.
		Shader shader( "Asset/Shader/Vertex.shader", "Asset/Shader/Fragment.shader" );

		const std::vector< VectorBase< float, 3 + 2 > > vertices =
		{
			/* Object space coordinates			UVs. */
			{ -0.5f, -0.5f, -0.5f,			0.0f, 0.0f },
			{  0.5f, -0.5f, -0.5f,			1.0f, 0.0f },
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f },
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f },
			{ -0.5f,  0.5f, -0.5f,			0.0f, 1.0f },
			{ -0.5f, -0.5f, -0.5f,			0.0f, 0.0f },

			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f },
			{  0.5f, -0.5f,  0.5f,			1.0f, 0.0f },
			{  0.5f,  0.5f,  0.5f,			1.0f, 1.0f },
			{  0.5f,  0.5f,  0.5f,			1.0f, 1.0f },
			{ -0.5f,  0.5f,  0.5f,			0.0f, 1.0f },
			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f },

			{ -0.5f,  0.5f,  0.5f,			1.0f, 0.0f },
			{ -0.5f,  0.5f, -0.5f,			1.0f, 1.0f },
			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f },
			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f },
			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f },
			{ -0.5f,  0.5f,  0.5f,			1.0f, 0.0f },

			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f },
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f },
			{  0.5f, -0.5f, -0.5f,			0.0f, 1.0f },
			{  0.5f, -0.5f, -0.5f,			0.0f, 1.0f },
			{  0.5f, -0.5f,  0.5f,			0.0f, 0.0f },
			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f },

			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f },
			{  0.5f, -0.5f, -0.5f,			1.0f, 1.0f },
			{  0.5f, -0.5f,  0.5f,			1.0f, 0.0f },
			{  0.5f, -0.5f,  0.5f,			1.0f, 0.0f },
			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f },
			{ -0.5f, -0.5f, -0.5f,			0.0f, 1.0f },

			{ -0.5f,  0.5f, -0.5f,			0.0f, 1.0f },
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f },
			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f },
			{  0.5f,  0.5f,  0.5f,			1.0f, 0.0f },
			{ -0.5f,  0.5f,  0.5f,			0.0f, 0.0f },
			{ -0.5f,  0.5f, -0.5f,			0.0f, 1.0f }
		};

		VertexBuffer vertex_buffer( vertices.data(), static_cast< unsigned int >( vertices.size() ) );

		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push< float >( 3 ); // NDC positions.
		vertex_buffer_layout.Push< float >( 2 ); // Tex. coords.
		VertexArray vertex_array( vertex_buffer, vertex_buffer_layout );

		Drawable cube( shader, vertex_array );

		renderer.AddDrawable( cube );
		renderer.SetPolygonMode( PolygonMode::FILL );

		Color color;

		Texture texture_container( "Asset/Texture/container.jpg", GL_RGB );
		Texture texture_awesome_face( "Asset/Texture/awesomeface.png", GL_RGBA );

		texture_container.ActivateAndBind( GL_TEXTURE0 );
		texture_awesome_face.ActivateAndBind( GL_TEXTURE1 );

		shader.Bind();
		shader.SetInt( "texture_sampler_1", 0 );
		shader.SetInt( "texture_sampler_2", 1 );

		/* Putting the "camera" on z +3 means moving the world to z -3.*/
		const auto view = Matrix::Translation( 0.0f, 0.0f, -3.0f );

		const auto projection = Matrix::PerspectiveProjection( 0.1f, 100.0f, aspect_ratio, 45.0f );

		shader.SetMatrix( "transformation_view",		view );
		shader.SetMatrix( "transformation_projection",	projection );

		Matrix4x4 rotation;

		while( !glfwWindowShouldClose( window ) )
		{
			Input::Process( window );

			shader.Bind();

			const float time = static_cast< float >( glfwGetTime() );
			rotation = Matrix::RotationAroundAxis( time * 50.0f, Vector3{ 0.5f, 1.0f, 0.0f } );
			shader.SetMatrix( "transformation_world", rotation );

			renderer.Update( window );
		}
	}

	renderer.CleanUp();

	return 0;
}