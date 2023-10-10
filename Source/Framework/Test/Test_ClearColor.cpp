// Framework Includes.
#include "Test/Test_ClearColor.h"

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

	void Test_ClearColor::OnRenderImGui()
	{
		Test::OnRenderImGui();

		if( ImGui::Begin( "Clear Color Test", nullptr, CurrentImGuiWindowFlags() | ImGuiWindowFlags_AlwaysAutoResize ) )
			if( ImGui::ColorPicker3( "Clear Color", const_cast< float* >( color_clear.Data() ) ) )
				renderer.SetClearColor( color_clear );

		ImGui::End();
	}
}