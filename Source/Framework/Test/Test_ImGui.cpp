// Framework Includes.
#include "Test/Test_ImGui.h"

// Vendor Includes.
#include "Vendor/imgui.h"
#include "Vendor/imgui_impl_glfw.h"
#include "Vendor/imgui_impl_opengl3.h"

namespace Framework::Test
{
	void Test_ImGui::OnRenderImGui()
	{
        static bool show_demo_window = true;

        if( show_demo_window == false && ImGui::Button( "Show/Hide Demo Window" ) )
            show_demo_window = true;

        if( show_demo_window )
            ImGui::ShowDemoWindow( &show_demo_window );
	}
}
