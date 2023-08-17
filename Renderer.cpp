// Project Includes.
#include "Graphics.h"
#include "Platform.h"
#include "Renderer.h"

// std Includes.
#include <iostream>

namespace Framework
{
	Renderer::Renderer( const int width_pixels, const int height_pixels, const int pos_x, const int pos_y, const Color4 clear_color )
		:
		pixel_width( width_pixels ),
		pixel_height( height_pixels ),
		aspect_ratio( float( pixel_width ) / pixel_height ),
		color_clear( clear_color )
	{
		try
		{
			Platform::InitializeAndCreateWindow( pixel_width, pixel_height, pos_x, pos_y );
		}
		catch( const std::logic_error& e )
		{
			std::cerr << "ERROR::RENDERER::CONSTRUCTION_ERROR:\n\t" << e.what() << std::endl;
		}

		EnableDepthTest();
	}

	void Renderer::BeginFrame() const
	{
		Clear();
	}

	void Renderer::DrawFrame()
	{
		for( auto drawable : drawable_list )
			drawable->Draw();
	}

	void Renderer::EndFrame() const
	{
		Platform::SwapBuffers();
	}

	void Renderer::CleanUp() const
	{
		Platform::CleanUp();
	}

	void Renderer::Clear() const
	{
		GLCALL( glClearColor( color_clear.R(), color_clear.G(), color_clear.B(), color_clear.A() ) );
		GLCALL( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
	}

	void Renderer::Clear( GLbitfield mask ) const
	{
		GLCALL( glClearColor( color_clear.R(), color_clear.G(), color_clear.B(), color_clear.A() ) );
		GLCALL( glClear( mask ) );
	}

	void Renderer::Clear( const Color4& clear_color, GLbitfield mask ) const
	{
		GLCALL( glClearColor( clear_color.R(), clear_color.G(), clear_color.B(), clear_color.A() ) );
		GLCALL( glClear( mask ) );
	}

	void Renderer::SetClearColor( const Color4& clear_color )
	{
		color_clear = clear_color;
	}

	const Color4& Renderer::GetClearColor() const
	{
		return color_clear;
	}

	void Renderer::AddDrawable( Drawable* drawable )
	{
		drawable_list.push_back( drawable );
	}

	void Renderer::RemoveDrawable( Drawable* drawable )
	{
		drawable_list.erase( std::find( drawable_list.cbegin(), drawable_list.cend(), drawable ) );
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
}
