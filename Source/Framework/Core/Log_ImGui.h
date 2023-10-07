#pragma once

// Framework Includes.
#include "Renderer/Material.h"
#include "Renderer/Shader.h"

// Vendor Includes.
#include "Vendor/imgui/imgui.h"

namespace Framework::Log
{
	void Dump( const Material& material,	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoFocusOnAppearing );
	void Dump( const Shader& shader,		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoFocusOnAppearing );
}
