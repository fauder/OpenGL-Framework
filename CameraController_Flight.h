#pragma once

// Project Includes.
#include "Camera.h"
#include "Polar.hpp"

namespace Framework
{
	class CameraController_Flight
	{
	public:
		CameraController_Flight( Camera* camera, const float move_speed );

		inline const Radians GetHeading()	const { return orientation_spherical.Heading(); }
		inline const Radians GetPitch()		const { return orientation_spherical.Pitch(); }
		CameraController_Flight& SetHeading( const Radians new_heading );
		CameraController_Flight& SetPitch( const Radians new_pitch );

		inline const float GetMoveSpeed() const { return move_speed; }
		inline		 float SetMoveSpeed( const float speed ) { return move_speed = speed; }

	private:
		void RecalculateRotationFromSphericalCoordinates();

	private:
		Math::Polar3_Spherical_Game_RightHanded orientation_spherical; // Spherical coordinates are more natural & convenient to work with for this camera type.
		float move_speed;
		Camera* camera;
	};
}
