#pragma once

// Project Includes.
#include "Color.hpp"
#include "Drawable.h"

// std Includes.
#include <vector>

namespace Framework
{
	enum class PolygonMode
	{
		POINT = 0, LINE = 1, FILL = 2
	};

	class Renderer
	{
	public:
		Renderer( const int width_pixels = 800, const int height_pixels = 600, const int pos_x = 0, const int pos_y = 0, const Color4 clear_color = Color4::Clear_Default() );

		void BeginFrame() const;
		void DrawFrame();
		void EndFrame() const;

		void CleanUp() const;

		void Clear() const;
		void Clear( GLbitfield mask ) const;
		void Clear( const Color4& clear_color, GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) const;

		const Color4& GetClearColor() const;
		void SetClearColor( const Color4& clear_color );

		void AddDrawable( Drawable* drawable );
		void RemoveDrawable( Drawable* drawable );

		void SetPolygonMode( const PolygonMode mode ) const;

		void EnableDepthTest() const;
		void DisableDepthTest() const;

		constexpr unsigned int	PixelWidth() const	{ return pixel_width; }
		constexpr unsigned int	PixelHeight() const { return pixel_height; }
		constexpr float			AspectRatio() const { return aspect_ratio; }

	private:
		unsigned int pixel_width, pixel_height;
		float aspect_ratio;
		std::vector< Drawable* > drawable_list;
		Color4 color_clear;
	};
}
