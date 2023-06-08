// Make sure GLAD is included before GLFW3!
#include <glad/glad.h>

// Project Includes.
#include "Drawable.h"
#include "IndexBuffer.h"
#include "Input.h"
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

	GLFWwindow* window;
	Renderer renderer( window );
	
	/* A scope is created here to prevent the Application to go into an endless loop after the render-loop is exited. */
	{
		// Shader creation & compilation.
		Shader shader( "Asset/Shader/Vertex.shader", "Asset/Shader/Fragment.shader" );

		const VectorBase< float, 7 > vertices[] =
		{
			{ +0.5f, +0.5f, +0.0f, 1.0f, 0.0f, 0.0f, 1.0f },  // Top right.
			{ +0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 0.0f, 1.0f },  // Bottom right.
			{ -0.5f, -0.5f, +0.0f, 1.0f, 0.0f, 1.0f, 1.0f },  // Bottom left.
			{ -0.5f,  0.5f, +0.0f, 0.0f, 1.0f, 1.0f, 1.0f }   // Top left.
		};
		const Vector3I indices[] =
		{ // Note that we start from 0!
			{ 0, 1, 3 },   // First triangle.
			{ 1, 2, 3 }    // Second triangle.
		};

		VertexBuffer vertex_buffer( vertices, sizeof( vertices ) );

		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push< float >( 3 );
		vertex_buffer_layout.Push< float >( 4 );
		VertexArray vertex_array( vertex_buffer, vertex_buffer_layout );

		IndexBuffer index_buffer( indices, 6 );

		Drawable quad( shader, vertex_array, index_buffer );

		renderer.AddDrawable( quad );
		renderer.SetPolygonMode( PolygonMode::FILL );

		Color color;

		Texture test( "Asset/Texture/container.jpg", GL_RGB );

		while( !glfwWindowShouldClose( window ) )
		{
			Input::Process( window );

			shader.Bind();
			/*const float time_seconds_normalized = static_cast< float >( std::sin( glfwGetTime() * 10 ) / 2.0 + 0.5 );
			color.Set( time_seconds_normalized, 1.0f, 1.0f, 1.0f );
			shader.SetColor( "u_color", color );*/
			const float offset_horizontal = static_cast< float >( std::sin( glfwGetTime() * 2 ) / 2.0f );
			shader.SetFloat( "offset_horizontal", offset_horizontal );
			renderer.Update( window );
		}
	}

	renderer.CleanUp();

	return 0;
}