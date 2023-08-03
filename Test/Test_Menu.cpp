// Project Includes.
#include "Test_Menu.h"

namespace Framework::Test
{
	Test_Menu::Test_Menu( Renderer& renderer, std::unique_ptr< TestInterface >& current_test )
		:
		Test( renderer ),
		test_current( current_test )
	{
	}

	void Test_Menu::Unregister( const std::string& name )
	{
		test_creation_info_by_name.erase( name );
	}

	void Test_Menu::OnRenderImGui()
	{
		ImGui::Begin( "Test Menu" );

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