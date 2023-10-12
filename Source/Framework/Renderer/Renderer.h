#pragma once

// Framework Includes.
#include "Math/Color.hpp"

#include "Renderer/Camera.h"
#include "Renderer/Drawable.h"

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
		Renderer( Camera* camera, const Color4 clear_color = Color4::Clear_Default() );
		~Renderer();

		void OnFrameBufferResize( const int width_new_pixels, const int height_new_pixels );

		void SetCamera( Camera* camera_to_render_into );

		void BeginFrame() const;
		void DrawFrame();
		void EndFrame() const;

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
		Camera* camera_current;
		unsigned int pixel_width, pixel_height;
		float aspect_ratio;
		std::vector< Drawable* > drawable_list;
		Color4 color_clear;
	};
}
