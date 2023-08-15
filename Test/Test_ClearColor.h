#pragma once

// Project Includes.
#include "Test.hpp"

namespace Framework::Test
{
	class Test_ClearColor : public Test< Test_ClearColor >
	{
		friend Test< Test_ClearColor >;

	public:
		Test_ClearColor( Renderer& renderer, Color4 clear_color = Color4::Clear_Default() );
		~Test_ClearColor() override;

	protected:
		void OnRenderImGui();

	private:
		Color4 color_clear, color_clear_original;
	};
}
