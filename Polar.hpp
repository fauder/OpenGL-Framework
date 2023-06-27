#pragma once

// Project Includes.
#include "Vector.hpp"

namespace Framework
{
	class Polar2 : public Vector2
	{
	public:
		Polar2()
			:
			Vector2()
		{}

		Polar2( const float r, const float theta_inDegrees )
			:
			Vector2( r, theta_inDegrees )
		{}

		inline float R() const { return data[ 0 ]; }
		/* In Radians. */
		inline float Angle() const { return data[ 1 ]; }
	};
}
