// Project Includes.
#include "Log_Imgui.h"

namespace Framework::Log
{
	void Dump( const Shader& shader, ImGuiWindowFlags window_flags )
	{
		if( ImGui::Begin( "Shader Info", nullptr, window_flags ) )
		{
			const auto uniform_map = shader.GetUniformInformation();

			for( auto& [ name, location ] : uniform_map )
				ImGui::InputInt( name.c_str(), const_cast< int* >( &location ), 0, 0, ImGuiInputTextFlags_ReadOnly );
		}

		ImGui::End();
	}
}
