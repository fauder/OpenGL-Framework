#pragma once

// Project Includes.
#include "Vector.hpp"

namespace Framework
{
	// TODO: When these are necessary (for example, camera manipulation may utilize polar coordinates);
	// TODO: (cont'd) revise (if necessary) all classes according to XYZ coordinates & handedness utilized in-framework.

	/* In classical mathematical notation (r, theta). X is right & Y is forward & Z is up (right-handed space.) */
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

	/* In classical mathematical notation (r, theta, z).  X is right & Y is forward & Z is up (right-handed space.) */
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

	/* In classical mathematical notation (r, theta, phi).  X is right & Y is forward & Z is up (right-handed space.).
	 * Phi starts as completely vertical (up) and rotates "downward" as its value increases (i.e phi = 0 equals vertical (up) direction & phi = 90 equals horizontal direction.
	 */
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
		/* In Radians.
		 * Also called Azimuth or Longitude.
		 */
		inline float Theta() const { return data[ 1 ]; }
		/* In Radians.
		 * Also called Zenith. Latitude is (90 - Zenith/Phi).
		 */
		inline float Phi() const { return data[ 2 ]; }
	};
}
