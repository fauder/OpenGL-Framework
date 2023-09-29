// Project Includes.
#include "Log_Imgui.h"
#include "ShaderTypeInformation.h"

namespace Framework::Log
{
	void Dump( const Shader& shader, ImGuiWindowFlags window_flags )
	{
		if( ImGui::Begin( "Shader Info.", nullptr, window_flags | ImGuiWindowFlags_AlwaysAutoResize ) )
		{
			const auto& uniform_map = shader.GetUniformInformations();

			ImGui::SeparatorText( "General Information" );
			if( ImGui::BeginTable( "General", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths ) )
			{
				ImGui::TableNextColumn(); ImGui::TextUnformatted( "Shader Name" );
				ImGui::TableNextColumn(); ImGui::Text( R"("%s")", shader.Name().c_str() );

				ImGui::EndTable();
			}

			ImGui::SeparatorText( "Uniforms" );
            if( ImGui::BeginTable( "Uniforms", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths ) )
            {
				ImGui::TableSetupColumn( "Name" );
				ImGui::TableSetupColumn( "Location" );
				ImGui::TableSetupColumn( "Size" );
				ImGui::TableSetupColumn( "Offset" );
				ImGui::TableSetupColumn( "Type" );

				ImGui::TableHeadersRow();

				for( auto& [ name, uniform_info ] : uniform_map )
				{
					ImGui::TableNextColumn(); ImGui::TextUnformatted( name.c_str() );
					ImGui::TableNextColumn(); ImGui::Text( "%d", uniform_info.location );
					ImGui::TableNextColumn(); ImGui::Text( "%d", uniform_info.size );
					ImGui::TableNextColumn(); ImGui::Text( "%d", uniform_info.offset );
					ImGui::TableNextColumn(); ImGui::TextUnformatted( GetNameOfType( uniform_info.type ) );
				}

                ImGui::EndTable();
            }
		}

		ImGui::End();
	}
}
