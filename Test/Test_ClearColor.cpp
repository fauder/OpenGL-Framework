// Project Includes.
#include "Test_ClearColor.h"

namespace Framework::Test
{
	Test_ClearColor::Test_ClearColor( GLFWwindow** window, Color4 clear_color, const unsigned int width_pixels, const unsigned int height_pixels, const int pos_x, const int pos_y )
		:
		Test( window, clear_color, width_pixels, height_pixels, pos_x, pos_y )
	{}
}