// GLAD Includes.
#include <glad/glad.h>

// Project Includes.
#include "Renderer.h"
#include "Window.h"

// std Includes.
#include <iostream>

Renderer::Renderer( GLFWwindow*& window )
{
	try
	{
		Window::InitializeGLFWAndCreateWindow( window );

		// GLAD needs the created window's context made current BEFORE it is initialized.
		InitializeGLAD();

		Window::Framebuffer_Size_Callback( window, 800, 600 );
		glfwSetFramebufferSizeCallback( window, Window::Framebuffer_Size_Callback );

	}
	catch( const std::logic_error& e )
	{
		std::cerr << "ERROR::GRAPHICS::CONSTRUCTION_ERROR:\n\t" << e.what() << std::endl;
	}
}

void Renderer::Update( GLFWwindow* window )
{
	GLCALL( glClearColor( 0.2f, 0.3f, 0.3f, 1.0f ) );
	GLCALL( glClear( GL_COLOR_BUFFER_BIT ) );

	for( auto& drawable : drawable_list )
	{
		drawable.Draw();
	}

	glfwSwapBuffers( window );
	glfwPollEvents();
}

void Renderer::CleanUp()
{
	glfwTerminate();
}

void Renderer::AddDrawable( const Drawable& drawable )
{
	drawable_list.push_back( drawable );
}

void Renderer::SetPolygonMode( const PolygonMode mode )
{
	GLCALL( glPolygonMode( GL_FRONT_AND_BACK, GL_POINT + ( unsigned int )mode ) );
}

/* GLAD needs the created window's context made current BEFORE it is initialized. */
void Renderer::InitializeGLAD()
{
	if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
		throw std::logic_error( "ERROR::GRAPHICS::GLAD::FAILED_TO_INITIALIZE!" );
}