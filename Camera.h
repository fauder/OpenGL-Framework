#pragma once

// Project Includes.
#include "Transform.h"

namespace Framework
{
	class Camera
	{
	public:
		Camera( Transform* const transform, const float near_plane = 0.1f, const float far_plane = 100.0f );

		inline const Matrix4x4& GetViewMatrix()				{ return GetViewMatrix_Implementation(); }
		inline const Matrix4x4& GetProjectionMatrix()		{ return GetProjectionMatrix_Implementation(); }
		const Matrix4x4& GetViewProjectionMatrix();

		Camera& SetNearPlaneOffset( const float offset );
		Camera& SetFarPlaneOffset( const float offset );
		inline float GetNearPlaneOffset() const { return plane_near; }
		inline float GetFarPlaneOffset()  const { return plane_far; }

		Vector3 Right();
		Vector3 Up();
		Vector3 Forward();

	protected:
		void SetProjectionMatrixDirty();
		void SetViewProjectionMatrixDirty();

	private:
		virtual const Matrix4x4& GetViewMatrix_Implementation() = 0;
		virtual const Matrix4x4& GetProjectionMatrix_Implementation() = 0;

	protected:
		Matrix4x4 view_matrix;
		Matrix4x4 projection_matrix;
		Matrix4x4 view_projection_matrix;

		Transform* const transform;

		float plane_near, plane_far;

		/* Do not need the flag below, as Transform's data is the only data needed to update the view matrix and it has IsDirty() query already. */
		//bool view_matrix_needs_update;
		bool projection_matrix_needs_update;
		bool view_projection_matrix_needs_update;
	};
}
