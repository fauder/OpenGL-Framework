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

	/* Deviates from classical mathematical notation (r, theta, phi), more in-line with video game conventions/needs. In this convention, Y is up, X is right & Z is forward (left-handed).
	 * Uses (r, h, p), h = heading, p = pitch.
	 * Heading = 0 -> facing forward (assuming Pitch is also zero). Its value increases -> clockwise rotation.
	 * Pitch = 0 -> horizontal (on XZ plane). It's values increases -> clockwise rotation around X (i.e, facing "more downward", therefore also called the Angle of Declination).
	*/
	class Polar3_Spherical_Game: public Vector3
	{
	public:
		Polar3_Spherical_Game()
			:
			Vector3()
		{}

		Polar3_Spherical_Game( const float r, const float heading, const float pitch )
			:
			Vector3( r, heading, pitch )
		{}

		inline float R() const { return data[ 0 ]; }
		/* In Radians.
		 * Heading = 0 means forward (assuming Pitch is also zero).
		 * Increasing Heading means clockwise rotation around the Y axis.
		 */
		inline float Heading() const { return data[ 1 ]; }
		/* In Radians. 
		 * Pitch = 0 means horizontal.
		 * Also called the Angle of Declination, as Z decreases with increasing Pitch.
		 */
		inline float Pitch() const { return data[ 2 ]; }
	};
}
