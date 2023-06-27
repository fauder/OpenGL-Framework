#pragma once

// Project Includes.
#include "Vector.hpp"

namespace Framework
{
	/* In classical mathematical notation (r, theta). Assumes left-handed space. */
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
		inline float Theta() const { return data[ 1 ]; }
	};

	/* In classical mathematical notation (r, theta, z). Assumes left-handed space. */
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
		inline float Theta() const { return data[ 1 ]; }
		inline float Z() const { return data[ 2 ]; }
	};

	/* In classical mathematical notation (r, theta, phi). Assumes left-handed space.
		Phi starts as completely vertical (up) and rotates "downward" as its value increases (i.e phi = 0 equals vertical (up) direction & phi = 90 equals horizontal direction. */
	class Polar3_Spherical : public Vector3
	{
	public:
		Polar3_Spherical()
			:
			Vector3()
		{}

		Polar3_Spherical( const float r, const float theta_inDegrees, const float phi_inDegrees )
			:
			Vector3( r, theta_inDegrees, phi_inDegrees )
		{}

		inline float R() const { return data[ 0 ]; }
		/* In Radians. */
		inline float Theta() const { return data[ 1 ]; } // Also called Azimuth or Longitude.
		/* In Radians. */
		inline float Phi() const { return data[ 2 ]; } // Also called Zenith. Latitude is (90 - Zenith/Phi).
	};
		/* In Radians. */
		inline float Phi() const { return data[ 2 ]; }
	};
}
