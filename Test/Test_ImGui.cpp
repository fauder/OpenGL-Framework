// Project Includes.
#include "Test_ImGui.h"

// ImGui Includes.
#include "../Vendor/imgui.h"
#include "../Vendor/imgui_impl_glfw.h"
#include "../Vendor/imgui_impl_opengl3.h"

namespace Framework::Test
{
	void Test_ImGui::OnRenderImGui()
	{
        const auto& io = ImGui::GetIO();

        static bool show_demo_window = true;

        if( show_demo_window == false && ImGui::Button( "Show/Hide Demo Window" ) )
            show_demo_window = true;

        if( show_demo_window )
            ImGui::ShowDemoWindow( &show_demo_window );

        ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate );
	}
}
