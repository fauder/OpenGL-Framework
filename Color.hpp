#pragma once

// Project Includes.
#include "Vector.hpp"

class Color : public VectorBase< float, 4 >
{
public:
	Color()
		:
		VectorBase< float, 4 >()
	{}

	Color( const float r, const float g, const float b, const float a = 1.0f )
		:
		VectorBase< float, 4 >( r, g, b, a )
	{}

	inline float R() const { return data[ 0 ]; };
	inline float G() const { return data[ 1 ]; };
	inline float B() const { return data[ 2 ]; };
	inline float A() const { return data[ 3 ]; };
};