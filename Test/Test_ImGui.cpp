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
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool show_demo_window = true;

        if( show_demo_window == false && ImGui::Button( "Show/Hide Demo Window" ) )
            show_demo_window = true;

        if( show_demo_window )
            ImGui::ShowDemoWindow( &show_demo_window );

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize( window, &display_w, &display_h );
        /*glViewport( 0, 0, display_w, display_h );
        glClearColor( clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w );
        glClear( GL_COLOR_BUFFER_BIT );*/
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
	}
}
