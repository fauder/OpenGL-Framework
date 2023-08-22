// Project Include.
#include "Camera.h"

namespace Framework
{
	Camera::Camera( Transform* const transform, const float near_plane, const float far_plane )
		:
		transform( transform ),
		plane_near( near_plane ),
		plane_far( far_plane ),
		projection_matrix_needs_update( true ),
		view_projection_matrix_needs_update( true )
	{}
	
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
