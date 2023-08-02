// Project Includes.
#include "Graphics.h"
#include "Renderer.h"

// std Includes.
#include <iostream>

namespace Framework
{
	Renderer::Renderer( GLFWwindow** created_window, const int width_pixels, const int height_pixels, const int pos_x, const int pos_y, const Color4 clear_color )
		:
		pixel_width( width_pixels ),
		pixel_height( height_pixels ),
		aspect_ratio( float( pixel_width ) / pixel_height ),
		color_clear( clear_color )
	{
		try
		{
			window = Window::InitializeGLFWAndCreateWindow( pixel_width, pixel_height, pos_x, pos_y );

			// GLAD needs the created window's context made current BEFORE it is initialized.
			InitializeGLAD();

			Window::Framebuffer_Size_Callback( window, pixel_width, pixel_height );
			glfwSetFramebufferSizeCallback( window, Window::Framebuffer_Size_Callback );

			if( created_window != nullptr )
				*created_window = window;
		}
		catch( const std::logic_error& e )
		{
			std::cerr << "ERROR::GRAPHICS::CONSTRUCTION_ERROR:\n\t" << e.what() << std::endl;
		}

		EnableDepthTest();
	}

	void Renderer::Update()
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
