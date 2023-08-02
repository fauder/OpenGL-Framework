#pragma once

// Project Includes.
#include "Test.hpp"
#include "../Drawable.h"

// std Includes.
#include <memory>

namespace Framework
{
	class Test_Transfom_2Cubes : public Test< Test_Transfom_2Cubes >
	{
	public:
		Test_Transfom_2Cubes( GLFWwindow** window = nullptr, const Color4 clear_color = Color4::Clear_Default(),
							  const unsigned int width_pixels = 800, const unsigned int height_pixels = 600, const int pos_x = 1000, const int pos_y = 100 );
		virtual ~Test_Transfom_2Cubes() override;

		void Run() override;
	};
}
