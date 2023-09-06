// Project Include.
#include "Camera.h"
#include "Matrix.h"

namespace Framework
{
	Camera::Camera( Transform* const transform, float aspect_ratio, const float near_plane, const float far_plane, Degrees field_of_view )
		:
		transform( transform ),
		plane_near( near_plane ),
		plane_far( far_plane ),
		aspect_ratio( aspect_ratio ),
		field_of_view( field_of_view ),
		projection_matrix_needs_update( true ),
		view_projection_matrix_needs_update( true )
	{}

	const Matrix4x4& Camera::GetViewMatrix()
	{
		if( transform->IsDirty() )
			view_matrix = transform->GetInverseOfFinalMatrix_NoScale();

		return view_matrix;
	}

	const Matrix4x4& Camera::GetProjectionMatrix()
	{
		if( projection_matrix_needs_update )
		{
			projection_matrix = Matrix::PerspectiveProjection( plane_near, plane_far, aspect_ratio, field_of_view );
			projection_matrix_needs_update = false;
		}

		return projection_matrix;
	}
	
	const Matrix4x4& Camera::GetViewProjectionMatrix()
	{
		if( view_projection_matrix_needs_update || transform->IsDirty() )
		{
			view_projection_matrix = GetViewMatrix() * GetProjectionMatrix();
			view_projection_matrix_needs_update = false;
		}

		return view_projection_matrix;
	}
	
	Camera& Camera::SetNearPlaneOffset( const float offset )
	{
		plane_near = offset;
		SetProjectionMatrixDirty();
		return *this;
	}

	Camera& Camera::SetFarPlaneOffset( const float offset )
	{
		plane_far = offset;
		SetProjectionMatrixDirty();
		return *this;
	}

	Vector3 Camera::Right()
	{
		GetViewMatrix();
		return view_matrix.GetColumn< 3 >( 0 );
	}

	Vector3 Camera::Up()
	{
		GetViewMatrix();
		return view_matrix.GetColumn< 3 >( 1 );
	}

	Vector3 Camera::Forward()
	{
		GetViewMatrix();
		return -view_matrix.GetColumn< 3 >( 2 ); 
	}

	Camera& Camera::SetAspectRatio( const float new_aspect_ratio )
	{
		SetProjectionMatrixDirty();
		aspect_ratio = new_aspect_ratio;
		return *this;
	}

	Camera& Camera::SetFieldOfView( const Degrees new_fov )
	{
		SetProjectionMatrixDirty();
		field_of_view = new_fov;
		return *this;
	}

	Camera& Camera::SetLookRotation( const Vector3& look_at )
	{
		transform->SetRotation( Quaternion::LookRotation( look_at ) );
		return *this;
	}

	void Camera::SetProjectionMatrixDirty()
	{
		projection_matrix_needs_update = true;
		SetViewProjectionMatrixDirty();
	}

	void Camera::SetViewProjectionMatrixDirty()
	{
		view_projection_matrix_needs_update = true;
	}
}
