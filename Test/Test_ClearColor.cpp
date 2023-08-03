// Project Includes.
#include "Test_ClearColor.h"

namespace Framework::Test
{
	Test_ClearColor::Test_ClearColor( Renderer& renderer, Color4 clear_color )
		:
		Test( renderer ),
		color_clear( clear_color ),
		color_clear_original( renderer.GetClearColor() )
	{
		renderer.SetClearColor( clear_color );
	}

	Test_ClearColor::~Test_ClearColor()
	{
		renderer.SetClearColor( color_clear_original );
	}
}