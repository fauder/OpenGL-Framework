#pragma once

// Framework Includes.
#include "Test/Test.hpp"

namespace Framework::Test
{
	class Test_ClearColor : public Test< Test_ClearColor >
	{
		friend Test< Test_ClearColor >;

	public:
		Test_ClearColor( Color4 clear_color = Color4::Clear_Default() );
		~Test_ClearColor() override;

	protected:
		void OnRenderImGui();

	private:
		Color4 color_clear, color_clear_original;
	};
}
