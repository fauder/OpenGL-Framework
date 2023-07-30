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
#include "Transform.h"
#include "Vector.hpp"
#include "VertexArray.h"

// std Includes.
#include <istream>
#include <vector>

int main()
{
	using namespace Framework;
	using namespace Math::Literals;

	GLFWwindow* window = nullptr;
	
	const auto width_pixels  = 800;
	const auto height_pixels = 600;
	Renderer renderer( window, width_pixels, height_pixels, 1000, 100 );

	const auto aspect_ratio = float( width_pixels ) / height_pixels;

	/* A scope is created here to prevent the Application to go into an endless loop after the render-loop is exited. */
	{
		// Shader creation & compilation.
		Shader shader( "Asset/Shader/Vertex.shader", "Asset/Shader/Fragment.shader" );

		const std::vector< Math::Vector< float, 3 + 2 + 4 > > vertices = 
		{
			/* Object space coordinates			UVs. */
			{ -0.5f, -0.5f, -0.5f,			0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // FRONT
			{  0.5f, -0.5f, -0.5f,			1.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // FRONT
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // FRONT
			{  0.5f,  0.5f, -0.5f,			1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // FRONT
			{ -0.5f,  0.5f, -0.5f,			0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // FRONT
			{ -0.5f, -0.5f, -0.5f,			0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f }, // FRONT

			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // BACK
			{  0.5f, -0.5f,  0.5f,			1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // BACK
			{  0.5f,  0.5f,  0.5f,			1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // BACK
			{  0.5f,  0.5f,  0.5f,			1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // BACK
			{ -0.5f,  0.5f,  0.5f,			0.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // BACK
			{ -0.5f, -0.5f,  0.5f,			0.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f }, // BACK

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
		VertexArray vertex_array( vertex_buffer, vertex_buffer_layout );

		Drawable cube_1( shader, vertex_array );
		Drawable cube_2( shader, vertex_array );

		renderer.AddDrawable( &cube_1 );
		renderer.AddDrawable( &cube_2 );
		renderer.SetPolygonMode( PolygonMode::FILL );

		Texture texture_container( "Asset/Texture/container.jpg", GL_RGB );
		Texture texture_awesome_face( "Asset/Texture/awesomeface.png", GL_RGBA );

		texture_container.ActivateAndBind( GL_TEXTURE0 );
		texture_awesome_face.ActivateAndBind( GL_TEXTURE1 );

		shader.Bind();
		shader.SetInt( "texture_sampler_1", 0 );
		shader.SetInt( "texture_sampler_2", 1 );

		/* Putting the "camera" on z +3 means moving the world to z -3.*/
		const auto view = Matrix::Translation( 0.0f, 0.0f, -3.0f );

		const auto projection = Matrix::PerspectiveProjection( 0.1f, 100.0f, aspect_ratio, 45.0_deg );

		shader.SetMatrix( "transformation_view",		view );
		shader.SetMatrix( "transformation_projection",	projection );

		const Vector3 cube_rotation_axis( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );

		while( !glfwWindowShouldClose( window ) )
		{
			Input::Process( window );

			shader.Bind();

			const float time     = static_cast< float >( glfwGetTime() );
			const float sin_time = Math::Sin( Radians( time ) );

			cube_1.transform.SetRotation( Quaternion( sin_time * 65.0_deg, cube_rotation_axis ) );
			cube_1.transform.SetTranslation( sin_time * Vector3::Right() );
			cube_1.transform.SetScaling( Vector3( UNIFORM_INITIALIZATION, Math::Clamp( Math::Abs( sin_time ), 0.1f, 1.0f ) ) );

			cube_2.transform.SetRotation( Quaternion( -sin_time * 65.0_deg, cube_rotation_axis ) );
			cube_2.transform.SetTranslation( -sin_time * Vector3::Up() );
			cube_2.transform.SetScaling( Vector3( UNIFORM_INITIALIZATION, Math::Clamp( Math::Abs( sin_time * 0.5f ), 0.1f, 1.0f ) ) );

			renderer.Update( window );
		}
	}

	renderer.CleanUp();

	return 0;
}
