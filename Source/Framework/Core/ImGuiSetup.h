#pragma once

// Vendor Includes.
#include "Vendor/imgui.h"
#include "Vendor/imgui_impl_glfw.h"
#include "Vendor/imgui_impl_opengl3.h"

namespace Framework::ImGuiSetup
{
    void Initialize();
    void Shutdown();

    void BeginFrame();
    void EndFrame();
}
