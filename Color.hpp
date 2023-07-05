#pragma once

// Project Includes.
#include "Concepts.h"
#include "Vector.hpp"

namespace Framework
{
	class Color3 : public VectorBase< float, 3 >
	{
		using Base = VectorBase< float, 3 >;

	public:
		Color3()
			:
			Base()
		{}

		Color3( const float r, const float g, const float b )
			:
			Base( r, g, b )
		{}

		constexpr float R() const { return data[ 0 ]; };
		constexpr float G() const { return data[ 1 ]; };
		constexpr float B() const { return data[ 2 ]; };
	};

	class Color4 : public VectorBase< float, 4 >
	{
		using Base = VectorBase< float, 4 >;
	
	public:
		Color4()
			:
			Base()
		{}

		Color4( const float r, const float g, const float b, const float a = 1.0f )
			:
			Base( r, g, b, a )
		{}

		constexpr float R() const { return data[ 0 ]; };
		constexpr float G() const { return data[ 1 ]; };
		constexpr float B() const { return data[ 2 ]; };
		constexpr float A() const { return data[ 3 ]; };
	};
}
