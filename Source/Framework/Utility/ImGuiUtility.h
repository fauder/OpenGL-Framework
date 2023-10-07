#pragma once

// Framework Includes.
#include "Math/Vector.hpp"

// Vendor includes.
#include "Vendor/imgui.h"

// std Includes.
#include <array>

namespace Framework::ImGuiUtility
{
	/* Taken directly from imgui_demo.cpp.
		Helper to display a little (?) mark which shows a tooltip when hovered.
		In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md) */
	void HelpMarker( const char* description, const char* marker_text = "(?)", const int wrap = 35.0f );

	void DisabledButton( const char* text, const ImVec2& size = ImVec2( 120, 0 ) );

	template< size_t size >
	void SetNextItemRightAligned( const int itemNo_StartingFromRight, const std::array< float, size >& itemWidths )
	{
		float totalItemWidth = 0.0f;
		for( auto i = 0; i < itemNo_StartingFromRight; i++ )
			totalItemWidth += itemWidths[ i ] + ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX( ImGui::GetWindowWidth() - totalItemWidth );
	}

	/* Only works with same width items. */
	void SetNextItemRightAligned( const int itemNo_StartingFromRight, const float itemWidth );

	void GroupBoxBegin( const char* title, const float itemOffsetFromLeftBorder );
	void GroupBoxEnd( const Vector2 bottomRightSpacing );
}
