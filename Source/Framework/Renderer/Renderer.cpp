// Framework Includes.
#include "Core/Platform.h"

#include "Renderer/Graphics.h"
#include "Renderer/Renderer.h"

namespace Framework
{
	Renderer::Renderer( Camera* camera, const Color4 clear_color )
		:
		camera_current( camera ),
		pixel_width( Platform::GetFrameBufferWidthInPixels() ),
		pixel_height( Platform::GetFrameBufferHeightInPixels() ),
		aspect_ratio( float( pixel_width ) / pixel_height ),
		color_clear( clear_color )
	{
		Platform::SetFrameBufferResizeCallback( [ = ]( const int width_new_pixels, const int height_new_pixels )
		{
			this->OnFrameBufferResize( width_new_pixels, height_new_pixels );
		} );

		EnableDepthTest();
	}

	Renderer::~Renderer()
	{
		Platform::SetFrameBufferResizeCallback();
	}

	void Renderer::OnFrameBufferResize( const int width_new_pixels, const int height_new_pixels )
	{
		pixel_width  = width_new_pixels;
		pixel_height = height_new_pixels;

		aspect_ratio = float( pixel_width ) / pixel_height;
	}

	void Renderer::SetCamera( Camera* new_camera )
	{
		camera_current = new_camera;
	}

	void Renderer::BeginFrame() const
	{
		Clear();
	}

	void Renderer::DrawFrame()
	{
		for( auto drawable : drawable_list )
			drawable->Submit( camera_current );
	}

	void Renderer::EndFrame() const
	{
		Platform::SwapBuffers();
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
