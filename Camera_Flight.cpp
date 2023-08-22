// Project Includes.
#include "Camera_Flight.h"
#include "Math.h"
#include "Matrix.h"

namespace Framework
{
	using namespace Math::Literals;

	Camera_Flight::Camera_Flight( Transform* const transform, const float aspect_ratio, const float move_speed, const Degrees field_of_view, const float near_plane, const float far_plane )
		:
		Camera( transform, near_plane, far_plane ),
		orientation_spherical( 1.0f, 0_rad, 0_rad ), // Unit-sphere used as these spherical coordinates are used as an orientation & the look-at vector is computed from it.
		move_speed( move_speed ),
		aspect_ratio( aspect_ratio ),
		field_of_view( field_of_view )
	{
	}

	Camera_Flight& Camera_Flight::SetHeading( const Radians new_heading )
	{
		orientation_spherical.Heading() = new_heading;
		RecalculateRotationFromSphericalCoordinates();
		return *this;
	}

	Camera_Flight& Camera_Flight::SetPitch( const Radians new_pitch )
	{
		orientation_spherical.Pitch() = new_pitch;
		RecalculateRotationFromSphericalCoordinates();
		return *this;
	}

	Camera_Flight& Camera_Flight::SetAspectRatio( const float new_aspect_ratio )
	{
		SetProjectionMatrixDirty();
		aspect_ratio = new_aspect_ratio;
		return *this;
	}

	Camera_Flight& Camera_Flight::SetFieldOfView( const Degrees new_fov )
	{
		SetProjectionMatrixDirty();
		field_of_view = new_fov;
		return *this;
	}

	const Matrix4x4& Camera_Flight::GetViewMatrix_Implementation()
	{
		if( transform->IsDirty() )
			view_matrix = transform->GetInverseOfFinalMatrix();

		return view_matrix;
	}

	const Matrix4x4& Camera_Flight::GetProjectionMatrix_Implementation()
	{
		if( projection_matrix_needs_update )
		{
			projection_matrix              = Matrix::PerspectiveProjection( plane_near, plane_far, aspect_ratio, field_of_view );
			projection_matrix_needs_update = false;
		}

		return projection_matrix;
	}

	void Camera_Flight::RecalculateRotationFromSphericalCoordinates()
	{
		transform->SetRotation( Quaternion::LookRotation( Math::ToVector3( orientation_spherical ) ) );
	}
}