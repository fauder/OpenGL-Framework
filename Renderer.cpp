// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Renderer.h"
#include "Window.h"

// std Includes.
#include <iostream>

Renderer::Renderer( GLFWwindow*& window, const int width, const int height, const int pos_x, const int pos_y )
{
	try
	{
		Window::InitializeGLFWAndCreateWindow( window, width, height, pos_x, pos_y );

		// GLAD needs the created window's context made current BEFORE it is initialized.
		InitializeGLAD();

		Window::Framebuffer_Size_Callback( window, width, height );
		glfwSetFramebufferSizeCallback( window, Window::Framebuffer_Size_Callback );

	}
	catch( const std::logic_error& e )
	{
		std::cerr << "ERROR::GRAPHICS::CONSTRUCTION_ERROR:\n\t" << e.what() << std::endl;
	}
}

void Renderer::Update( GLFWwindow* window ) const
{
	GLCALL( glClearColor( 0.2f, 0.3f, 0.3f, 1.0f ) );
	GLCALL( glClear( GL_COLOR_BUFFER_BIT ) );

	for( const auto& drawable : drawable_list )
	{
		drawable.Draw();
	}

	glfwSwapBuffers( window );
	glfwPollEvents();
}

void Renderer::CleanUp() const
{
	glfwTerminate();
}

void Renderer::AddDrawable( const Drawable& drawable )
{
	drawable_list.push_back( drawable );
}

void Renderer::SetPolygonMode( const PolygonMode mode ) const
{
	GLCALL( glPolygonMode( GL_FRONT_AND_BACK, GL_POINT + ( unsigned int )mode ) );
}

/* GLAD needs the created window's context made current BEFORE it is initialized. */
void Renderer::InitializeGLAD() const
{
	if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
		throw std::logic_error( "ERROR::GRAPHICS::GLAD::FAILED_TO_INITIALIZE!" );
}