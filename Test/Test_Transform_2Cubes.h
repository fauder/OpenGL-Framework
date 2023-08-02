#pragma once

// Project Includes.
#include "Test.hpp"
#include "../Drawable.h"
#include "../Texture.h"

// std Includes.
#include <memory>

namespace Framework::Test
{
	class Test_Transfom_2Cubes : public Test< Test_Transfom_2Cubes >
	{
	public:
		Test_Transfom_2Cubes( GLFWwindow** window = nullptr, const Color4 clear_color = Color4::Clear_Default(),
							  const unsigned int width_pixels = 800, const unsigned int height_pixels = 600, const int pos_x = 1000, const int pos_y = 100 );

		void OnRender();

	private:
		std::unique_ptr< VertexArray > cube_vertex_array;
		std::unique_ptr< Shader > shader;
		std::unique_ptr< Drawable > cube_1;
		std::unique_ptr< Drawable > cube_2;
		std::unique_ptr< Texture > texture_container;
		std::unique_ptr< Texture > texture_awesome_face;

		static constexpr Vector3 rotation_axis = Vector3( Constants< float >::Sqrt_Half(), Constants< float >::Sqrt_Half(), 0.0f );
	};
}
