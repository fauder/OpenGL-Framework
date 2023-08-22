#pragma once

// Project Includes.
#include "Camera.h"
#include "Polar.hpp"

namespace Framework
{
	class Camera_Flight : public Camera
	{
	public:
		Camera_Flight( Transform* const transform, const float aspect_ratio, const float move_speed, const Degrees field_of_view = Degrees( 45.0f ), const float near_plane = 0.1f, const float far_plane = 100.0f );

		inline const Radians GetHeading()	const { return orientation_spherical.Heading(); }
		inline const Radians GetPitch()		const { return orientation_spherical.Pitch(); }
		Camera_Flight& SetHeading( const Radians new_heading );
		Camera_Flight& SetPitch( const Radians new_pitch );

		inline const float GetMoveSpeed() const { return move_speed; }
		inline		 float SetMoveSpeed( const float speed ) { return move_speed = speed; }

		inline const float GetAspectRatio() const { return aspect_ratio; }
		Camera_Flight& SetAspectRatio( const float new_aspect_ratio );

		inline const Degrees& GetFieldOfView() const { return field_of_view; }
		Camera_Flight& SetFieldOfView( const Degrees new_fov );

	private:
		const Matrix4x4& GetViewMatrix_Implementation() override;
		const Matrix4x4& GetProjectionMatrix_Implementation() override;

		void RecalculateRotationFromSphericalCoordinates();

	private:
		Math::Polar3_Spherical_Game_RightHanded orientation_spherical; // Spherical coordinates are more natural & convenient to work with for this camera type.
		float move_speed;
		float aspect_ratio;
		Degrees field_of_view;
	};
}
