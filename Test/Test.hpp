#pragma once

// Project Includes.
#include "../Color.hpp"
#include "../Renderer.h"

namespace Framework::Test
{
	class TestInterface
	{
	public:
		virtual void Run() = 0;
		virtual ~TestInterface() {}
	};

	template< class ActualTest > // CRTP.
	class Test : public TestInterface
	{
	public:
		Test( GLFWwindow** window = nullptr, const Color4 clear_color = Color4::Clear_Default(),
			  const unsigned int width_pixels = 800, const unsigned int height_pixels = 600, const int pos_x = 1000, const int pos_y = 100 )
			:
			window( window ? *window : nullptr ),
			renderer( &this->window, width_pixels, height_pixels, pos_x, pos_y, clear_color )
		{
		}

		void Run() override
		{
			Derived()->Run();
		}

	private:
		Test* Derived() { return static_cast< ActualTest* >( this ); }
		Test* Derived() const { return static_cast< ActualTest* >( this ); }

	protected:
		GLFWwindow* window;
		Renderer renderer;
	};
}
