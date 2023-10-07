#pragma once

// Project Includes.
#include "Material.h"
#include "Shader.h"

// ImGui Includes.
#include "Vendor/imgui.h"

namespace Framework::Log
{
	void Dump( const Material& material,	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoFocusOnAppearing );
	void Dump( const Shader& shader,		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoFocusOnAppearing );
}
