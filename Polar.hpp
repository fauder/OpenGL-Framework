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

	class Polar3_Cylindrical : public Vector3
	{
	public:
		Polar3_Cylindrical()
			:
			Vector3()
		{}

		Polar3_Cylindrical( const float r, const float theta_inDegrees, const float z )
			:
			Vector3( r, theta_inDegrees, z )
		{}

		inline float R() const { return data[ 0 ]; }
		/* In Radians. */
		inline float Angle() const { return data[ 1 ]; }
		inline float Z() const { return data[ 2 ]; }
	};
}
