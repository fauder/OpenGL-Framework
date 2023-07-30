// Project Includes.
#include "Graphics.h"
#include "Renderer.h"

// std Includes.
#include <iostream>

namespace Framework
{
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

		EnableDepthTest();
	}

	void Renderer::Update( GLFWwindow* window )
	{
		GLCALL( glClearColor( 0.2f, 0.3f, 0.3f, 1.0f ) );
		GLCALL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		for( auto drawable : drawable_list )
		{
			drawable->Draw();
		}

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	void Renderer::CleanUp() const
	{
		glfwTerminate();
	}

	void Renderer::AddDrawable( Drawable* drawable )
	{
		drawable_list.push_back( drawable );
	}

	void Renderer::SetPolygonMode( const PolygonMode mode ) const
	{
		GLCALL( glPolygonMode( GL_FRONT_AND_BACK, GL_POINT + ( unsigned int )mode ) );
	}

	void Renderer::EnableDepthTest() const
	{
		glEnable( GL_DEPTH_TEST );
	}

	void Renderer::DisableDepthTest() const
	{
		glDisable( GL_DEPTH_TEST );
	}

	/* GLAD needs the created window's context made current BEFORE it is initialized. */
	void Renderer::InitializeGLAD() const
	{
		if( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
			throw std::logic_error( "ERROR::GRAPHICS::GLAD::FAILED_TO_INITIALIZE!" );
	}
}
