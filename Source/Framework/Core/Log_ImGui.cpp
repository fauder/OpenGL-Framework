// Framework Includes.
#include "Core/Log_Imgui.h"

#include "Renderer/ShaderTypeInformation.h"

// Vendor Includes.
#include "Vendor/imgui/imgui_internal.h"

namespace Framework::Log
{
	template< Concepts::Arithmetic Coordinate, std::size_t Size >
		requires Concepts::NonZero< Size >
	void Dump( const Math::Vector< Coordinate, Size >& vector )
	{
		if constexpr( std::is_same_v< Coordinate, float > )
		{
			if constexpr( Size == 2 )
				ImGui::Text( "< %.3f, %.3f >", vector.X(), vector.Y() );
			if constexpr( Size == 3 )
				ImGui::Text( "< %.3f, %.3f, %.3f >", vector.X(), vector.Y(), vector.Z() );
			if constexpr( Size == 4 )
				ImGui::Text( "< %.3f, %.3f, %.3f, %.3f >", vector.X(), vector.Y(), vector.Z(), vector.W() );
		}

		if constexpr( std::is_same_v< Coordinate, double > )
		{
			if constexpr( Size == 2 )
				ImGui::Text( "< %.3lf, %.3lf >", vector.X(), vector.Y() );
			if constexpr( Size == 3 )
				ImGui::Text( "< %.3lf, %.3lf, %.3lf >", vector.X(), vector.Y(), vector.Z() );
			if constexpr( Size == 4 )
				ImGui::Text( "< %.3lf, %.3lf, %.3lf, %.3lf >", vector.X(), vector.Y(), vector.Z(), vector.W() );
		}

		if constexpr( std::is_same_v< Coordinate, int > )
		{
			if constexpr( Size == 2 )
				ImGui::Text( "< %d, %d >", vector.X(), vector.Y() );
			if constexpr( Size == 3 )
				ImGui::Text( "< %d, %d, %d >", vector.X(), vector.Y(), vector.Z() );
			if constexpr( Size == 4 )
				ImGui::Text( "< %d, %d, %d, %d >", vector.X(), vector.Y(), vector.Z(), vector.W() );
		}

		if constexpr( std::is_same_v< Coordinate, unsigned int > )
		{
			if constexpr( Size == 2 )
				ImGui::Text( "< %u, %u >", vector.X(), vector.Y() );
			if constexpr( Size == 3 )
				ImGui::Text( "< %u, %u, %u >", vector.X(), vector.Y(), vector.Z() );
			if constexpr( Size == 4 )
				ImGui::Text( "< %u, %u, %u, %u >", vector.X(), vector.Y(), vector.Z(), vector.W() );
		}

		if constexpr( std::is_same_v< Coordinate, bool > )
		{
			if constexpr( Size >= 2 )
			{
				bool x = vector.X(), y = vector.Y();
				ImGui::Checkbox( "", &x ); ImGui::SameLine(); ImGui::Checkbox( "", &y );

			}
			if constexpr( Size >= 3 )
			{
				bool value = vector.Z();
				ImGui::SameLine(); ImGui::Checkbox( "", &value );
			}
			if constexpr( Size >= 4 )
			{
				bool value = vector.W();
				ImGui::SameLine(); ImGui::Checkbox( "", &value );
			}
		}
	}

	template< Concepts::Arithmetic Type, std::size_t RowSize, std::size_t ColumnSize >
		requires Concepts::NonZero< RowSize > && Concepts::NonZero< ColumnSize >
	void Dump( const Math::Matrix< Type, RowSize, ColumnSize >& matrix )
	{
		if( ImGui::TreeNode( &matrix, "(Show/Hide)" ) == false )
			return;

		if( ImGui::BeginTable( "matrix-dump-table", ColumnSize, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths ) )
		{
			Utility::constexpr_for< 0, RowSize, +1 >( [ & ]( const auto row_index )
			{
				Utility::constexpr_for< 0, ColumnSize, +1 >( [ & ]( const auto column_index )
				{
					ImGui::TableNextColumn();
					
					if constexpr( std::is_same_v< Type, float > )
						ImGui::Text( "%.3f", matrix[ row_index ][ column_index ] );
					if constexpr( std::is_same_v< Type, double > )
						ImGui::Text( "%.3lf", matrix[ row_index ][ column_index ] );
					if constexpr( std::is_same_v< Type, int > )
						ImGui::Text( "%d", matrix[ row_index ][ column_index ] );
					if constexpr( std::is_same_v< Type, unsigned int > )
						ImGui::Text( "%u", matrix[ row_index ][ column_index ] );
					if constexpr( std::is_same_v< Type, bool > )
					{
						bool value = matrix[ row_index ][ column_index ];
						ImGui::Checkbox( "", &value );
					}
				} );
			} );

			// TEMP: This is a hack to get nested tables working with auto-resize. It has a problem; When the matrix tables are hidden/shown, the window resizes erratically for 4-5 frames.
			// Source: https://github.com/ocornut/imgui/issues/6586#issuecomment-1631455446
			float table_width = ImGui::GetCurrentContext()->CurrentTable->ColumnsAutoFitWidth;
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
			ImGui::EndTable();
			ImGui::Dummy( ImVec2( table_width, 0 ) );
			ImGui::PopStyleVar();
		}

		ImGui::TreePop();
	}

	void DumpUniform( const Material& material, const ShaderUniformInformation& uniform_info )
	{
		switch( uniform_info.type )
		{
			/* Scalars & vectors: */
			case GL_FLOAT								: ImGui::Text( "%.3f", material.GetUniformValue< float >( uniform_info ) ); break;
			case GL_FLOAT_VEC2							: Dump( material.GetUniformValue< Vector2 >( uniform_info ) ); break;
			case GL_FLOAT_VEC3							: Dump( material.GetUniformValue< Vector3 >( uniform_info ) ); break;
			case GL_FLOAT_VEC4							: Dump( material.GetUniformValue< Vector4 >( uniform_info ) ); break;
			
			case GL_DOUBLE								: ImGui::Text( "%.3lf", material.GetUniformValue< double >( uniform_info ) ); break;
			// OpenGL 3.3 does not have vectors of double type.
			
			case GL_INT									: ImGui::Text( "%d", material.GetUniformValue< int >( uniform_info ) ); break;
			case GL_INT_VEC2							: Dump( material.GetUniformValue< Vector2I >( uniform_info ) ); break;
			case GL_INT_VEC3							: Dump( material.GetUniformValue< Vector3I >( uniform_info ) ); break;
			case GL_INT_VEC4							: Dump( material.GetUniformValue< Vector4I >( uniform_info ) ); break;
			
			case GL_UNSIGNED_INT						: ImGui::Text( "%u", material.GetUniformValue< unsigned int >( uniform_info ) ); break;
			case GL_UNSIGNED_INT_VEC2					: Dump( material.GetUniformValue< Vector2U >( uniform_info ) ); break;
			case GL_UNSIGNED_INT_VEC3					: Dump( material.GetUniformValue< Vector3U >( uniform_info ) ); break;
			case GL_UNSIGNED_INT_VEC4					: Dump( material.GetUniformValue< Vector4U >( uniform_info ) ); break;
			
			case GL_BOOL:
			{
				bool value = material.GetUniformValue< bool >( uniform_info );
				ImGui::Checkbox( "", &value );
				break;
			}
			case GL_BOOL_VEC2							: Dump( material.GetUniformValue< Vector2B >( uniform_info ) ); break;
			case GL_BOOL_VEC3							: Dump( material.GetUniformValue< Vector3B >( uniform_info ) ); break;
			case GL_BOOL_VEC4							: Dump( material.GetUniformValue< Vector4B >( uniform_info ) ); break;
			
			/* Float matrices: */
			case GL_FLOAT_MAT2 							: Dump( material.GetUniformValue< Matrix2x2 >( uniform_info ) ); break;
			case GL_FLOAT_MAT3 							: Dump( material.GetUniformValue< Matrix3x3 >( uniform_info ) ); break;
			case GL_FLOAT_MAT4 							: Dump( material.GetUniformValue< Matrix4x4 >( uniform_info ) ); break;

			case GL_FLOAT_MAT2x3 						: Dump( material.GetUniformValue< Matrix2x3 >( uniform_info ) ); break;
			case GL_FLOAT_MAT2x4 						: Dump( material.GetUniformValue< Matrix2x4 >( uniform_info ) ); break;
			case GL_FLOAT_MAT3x2 						: Dump( material.GetUniformValue< Matrix3x2 >( uniform_info ) ); break;
			case GL_FLOAT_MAT3x4 						: Dump( material.GetUniformValue< Matrix3x4 >( uniform_info ) ); break;
			case GL_FLOAT_MAT4x2 						: Dump( material.GetUniformValue< Matrix4x2 >( uniform_info ) ); break;
			case GL_FLOAT_MAT4x3 						: Dump( material.GetUniformValue< Matrix4x3 >( uniform_info ) ); break;

			// OpenGL 3.3 does not have matrices of double type.

			/* Texture samplers: */
			case GL_SAMPLER_1D 							: ImGui::Text( "%d", material.GetUniformValue< int >( uniform_info ) ); break;
			case GL_SAMPLER_2D 							: ImGui::Text( "%d", material.GetUniformValue< int >( uniform_info ) ); break;
			case GL_SAMPLER_3D 							: ImGui::Text( "%d", material.GetUniformValue< int >( uniform_info ) ); break;

			default:
				ImGui::TextUnformatted( "[Not Implemented Yet]" );
				break;
		}
	}

	void Dump( const Material& material, ImGuiWindowFlags window_flags )
	{
		if( ImGui::Begin( "Material Info.", nullptr, window_flags | ImGuiWindowFlags_AlwaysAutoResize ) )
		{
			ImGui::SeparatorText( "General Information" );
			if( ImGui::BeginTable( "General", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths ) )
			{
				ImGui::TableNextColumn(); ImGui::TextUnformatted( "Material Name" );
				ImGui::TableNextColumn(); ImGui::Text( R"("%s")", material.GetName().c_str() );

				ImGui::EndTable();
			}

			const auto& uniform_map = material.shader->GetUniformInformations();

			ImGui::SeparatorText( "Parameters" );
			if( ImGui::BeginTable( "Parameters", /*6*/ 3, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths ) )
			{
				ImGui::TableSetupColumn( "Name" );
				ImGui::TableSetupColumn( "Value" );
				/*ImGui::TableSetupColumn( "Location" );
				ImGui::TableSetupColumn( "Size" );
				ImGui::TableSetupColumn( "Offset" );*/
				ImGui::TableSetupColumn( "Type" );

				ImGui::TableHeadersRow();

				for( auto& [ name, uniform_info ] : uniform_map )
				{
					ImGui::TableNextColumn(); ImGui::TextUnformatted( name.c_str() );
					ImGui::TableNextColumn(); DumpUniform( material, uniform_info );
					/*ImGui::TableNextColumn(); ImGui::Text( "%d", uniform_info.location );
					ImGui::TableNextColumn(); ImGui::Text( "%d", uniform_info.size );
					ImGui::TableNextColumn(); ImGui::Text( "%d", uniform_info.offset );*/
					ImGui::TableNextColumn(); ImGui::TextUnformatted( GetNameOfType( uniform_info.type ) );
				}

				ImGui::EndTable();
			}
		}

		ImGui::End();
	}

	void Dump( const Shader& shader, ImGuiWindowFlags window_flags )
	{
		if( ImGui::Begin( "Shader Info.", nullptr, window_flags | ImGuiWindowFlags_AlwaysAutoResize ) )
		{
			const auto& uniform_map = shader.GetUniformInformations();

			ImGui::SeparatorText( "General Information" );
			if( ImGui::BeginTable( "General", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PreciseWidths ) )
			{
				ImGui::TableNextColumn(); ImGui::TextUnformatted( "Shader Name" );
				ImGui::TableNextColumn(); ImGui::Text( R"("%s")", shader.GetName().c_str() );

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
