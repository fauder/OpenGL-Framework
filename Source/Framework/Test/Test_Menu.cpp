// Framework Includes.
#include "Test/Test_Menu.h"

namespace Framework::Test
{
	Test_Menu::Test_Menu( Renderer& renderer, std::unique_ptr< TestInterface >& current_test )
		:
		Test( renderer ),
		test_current( current_test ),
		color_clear( 255u, 185u, 0u, 255u )
	{
		renderer.SetClearColor( color_clear );
	}

	void Test_Menu::Unregister( const std::string& name )
	{
		test_creation_info_by_name.erase( name );
	}

	void Test_Menu::ResumeExecution()
	{
		renderer.SetClearColor( color_clear );
		Execute();
	}

	void Test_Menu::OnRenderImGui()
	{
		ImGui::Begin( "Test Menu", nullptr, CurrentImGuiWindowFlags() );

		for( const auto& [ test_name, create_test ] : test_creation_info_by_name )
		{
			if( ImGui::Button( ( R"(Execute Test ")" + test_name + R"(")" ).c_str() ) )
			{
				StopExecution();
				test_current = create_test();
			}
		}

		ImGui::End();
	}
}