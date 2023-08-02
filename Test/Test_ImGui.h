#pragma once

// Project Includes.
#include "Test.hpp"

namespace Framework::Test
{
	class Test_ImGui : public Test< Test_ImGui >
	{
	public:
		using Test::Test;

		void OnRenderImGui();
	};
}
