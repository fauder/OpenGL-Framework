#pragma once

// Project Includes.
#include "Matrix.hpp"
#include "Vector.hpp"

namespace Framework
{
	class Transform
	{
	public:
		Transform();
		Transform( const Vector3& scale );
		Transform( const Vector3& scale, const Vector3& translation );
		Transform( const Vector3& scale, const Vector3& rotation_euler, const Vector3& translation );
		~Transform();

		Transform& SetScaling( const Vector3& scale );
		Transform& SetRotation( const Vector3& rotation_euler );
		Transform& SetRotation( const float angle_inDegrees, const Vector3& axis );
		Transform& SetTranslation( const Vector3& translation );

		const Vector3& GetScaling() const;
		inline bool IsUsingEulerAngleRepresentation() const { return rotation_usingRepresentation_eulerAngles; }
		/* Usage: First query whether using this representation or not. */
		const Vector3& GetRotationEuler() const;
		/* Usage: First query whether using this representation or not.
			Angle is in degrees. */
		float GetRotationAngleAroundAxis() const;
		/* Usage: First query whether using this representation or not. */
		const Vector3& GetRotationAxis() const;
		const Vector3& GetTranslation() const;

		const Matrix4x4& GetScalingMatrix();
		const Matrix4x4& GetRotationMatrix();
		const Matrix4x4& GetTranslationMatrix();
		const Matrix4x4& GetFinalMatrix();

	private:
		Vector3 scale;
		// TODO: Switch to quaternions instead of euler angle triplets so we can represent both euler-angles & angle-axis representations uniformly and also query for both from the quaternion.
		Vector3 rotation_euler;
		Vector3 translation;

		Vector3 rotation_axis;
		float rotation_angle_inDegrees;

		bool rotation_usingRepresentation_eulerAngles;

		Matrix4x4 matrix_scaling;
		Matrix4x4 matrix_rotation;
		Matrix4x4 matrix_translation;

		Matrix4x4 matrix_final;

		bool needsUpdate_scaling;
		bool needsUpdate_rotation;
		bool needsUpdate_translation;

		bool needsUpdate_finalMatrix;
	};
}
