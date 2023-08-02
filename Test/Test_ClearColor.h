#pragma once

// Project Includes.
#include "Test.hpp"

namespace Framework::Test
{
	class Test_ClearColor : public Test< Test_ClearColor >
	{
	public:
		Test_ClearColor( GLFWwindow** window = nullptr, Color4 clear_color = Color4::Clear_Default(), 
						 const unsigned int width_pixels = 800, const unsigned int height_pixels = 600, const int pos_x = 1000, const int pos_y = 100 );

		void Run() override;
	};
}
