// Framework Includes.
#include "Utility/ImGuiUtility.h"

namespace Framework::ImGuiUtility
{
/* Helper to display a little (?) mark which shows a tooltip when hovered.
		   In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md) */
	void HelpMarker( const char* description, const char* marker_text, const int wrap )
	{
		ImGui::TextDisabled( marker_text );
		if( ImGui::IsItemHovered() )
		{
			ImGui::BeginTooltip();
			if( wrap )
				ImGui::PushTextWrapPos( ImGui::GetFontSize() * wrap );
			ImGui::TextUnformatted( description );
			if( wrap )
				ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void DisabledButton( const char* text, const ImVec2& size )
	{
		const ImVec4 buttonColor( ImGui::GetStyleColorVec4( ImGuiCol_Button ) );
		ImGui::PushStyleColor( ImGuiCol_Text, ImGui::GetStyleColorVec4( ImGuiCol_TextDisabled ) );
		ImGui::PushStyleColor( ImGuiCol_Button, buttonColor );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, buttonColor );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, buttonColor );
		ImGui::Button( text, size );
		ImGui::PopStyleColor( 4 );
	}

	/* Only works with same width items. */
	void SetNextItemRightAligned( const int itemNo_StartingFromRight, const float itemWidth )
	{
		ImGui::SetCursorPosX( ImGui::GetWindowWidth() - itemNo_StartingFromRight * ( ImGui::GetStyle().ItemSpacing.x + itemWidth ) );
	}

	void SetNextWindowPos( const HorizontalWindowPositioning horizontal_positioning, const VerticalWindowPositioning vertical_positioning, const ImGuiCond condition )
	{
		const auto& io = ImGui::GetIO();
		const auto horizontal_position = horizontal_positioning == HorizontalWindowPositioning::RIGHT
											? io.DisplaySize.x
											: horizontal_positioning == HorizontalWindowPositioning::CENTER
												? io.DisplaySize.x / 2.0f
												: 0.0f;
		const auto vertical_position = vertical_positioning == VerticalWindowPositioning::BOTTOM
											? io.DisplaySize.y
											: vertical_positioning == VerticalWindowPositioning::CENTER
												? io.DisplaySize.y / 2.0f
												: 0.0f;
		const auto horizontal_pivot = ( float )horizontal_positioning / 2.0f; // Map to [+1, 0] range.
		const auto vertical_pivot   = ( float )vertical_positioning   / 2.0f; // Map to [+1, 0] range.

		ImGui::SetNextWindowPos( { horizontal_position, vertical_position }, condition, { horizontal_pivot, vertical_pivot } );
	}

	void GroupBoxBegin( const char* title, const float itemOffsetFromLeftBorder )
	{
		ImGui::TextUnformatted( title );
		ImGui::BeginGroup();
		ImGui::Spacing();
		ImGui::Indent( itemOffsetFromLeftBorder );
	}

	// TODO: Handle conversion between Framework::Math::Vector# and ImVec#.
	void GroupBoxEnd( const Vector2 bottomRightSpacing )
	{
		ImGui::EndGroup();
		const ImVec2 item_rect_max( ImGui::GetItemRectMax() );
		const Vector2 rect_summed( *reinterpret_cast< const Vector2* >( &item_rect_max ) + bottomRightSpacing );
		ImGui::GetWindowDrawList()->AddRect( ImGui::GetItemRectMin(),
											 *reinterpret_cast< const ImVec2* >( &rect_summed ),
											 ImGui::GetColorU32( ImGuiCol_Border ) );
	}
}
