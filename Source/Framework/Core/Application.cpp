// Vendor GLAD Includes (Has to be included before GLFW!).
#include "Vendor/OpenGL/glad/glad.h"

// Framework Includes.
#include "Core/ImGuiSetup.h"
#include "Core/Platform.h"

#include "Renderer/Renderer.h"

#include "Test/Test_Menu.h"
#include "Test/Test_Lighting.h"
#include "Test/Test_Camera_WalkAround.h"
#include "Test/Test_Camera_LookAt.h"
#include "Test/Test_ClearColor.h"
#include "Test/Test_Transform_2Cubes.h"
#include "Test/Test_ImGui.h"

using namespace Framework;
using namespace Framework::Test;

int main()
{
	Platform::InitializeAndCreateWindow( 1600, 900, 800, 200 );

	Framework::ImGuiSetup::Initialize();

	/* Platform::CleanUp() will destroy the OpenGL context, which will cause GlGetError() calls (in OpenGL types' destructors) to return an error and cause an endless loop.
	 * To prevent it, all Test code is vacuumed inside a local scope, to ensure all destructors run before Platform::CleanUp(). */
	{
		std::unique_ptr< TestInterface > test_current;
		std::unique_ptr< Test_Menu > test_menu = std::make_unique< Test_Menu >( test_current );

		test_menu->Register< Test_Lighting >();
		test_menu->Register< Test_Camera_WalkAround >();
		test_menu->Register< Test_Camera_LookAt >();
		test_menu->Register< Test_ClearColor >( Color4::Cyan() );
		test_menu->Register< Test_Transfom_2Cubes >();
		test_menu->Register< Test_ImGui >();

		test_menu->AutoExecute< Test_Lighting >();

		bool continue_executing_tests = false;

		do
		{
			if( !test_current )
			{
				Platform::ChangeTitle( "OpenGL Framework: Test Menu" );
				test_menu->ResumeExecution(); // Enters its own loop.
			}

			/* Execution reaches here -> means menu stopped executing -> means either a test was selected by the user or back button was clicked. */

			continue_executing_tests = ( bool )test_current;

			if( test_current )
			{
				Platform::ChangeTitle( ( "OpenGL Framework: Test/" + test_current->GetName() ).c_str());
				test_current->Execute(); // Enters its own loop.
				test_current.reset();
			}

		}
		while( continue_executing_tests );
	}

	Framework::ImGuiSetup::Shutdown();

	Platform::CleanUp();

	return 0;
}
