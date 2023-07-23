#pragma once

// Project Includes.
#include "Angle.hpp"
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
		Transform( const Vector3& scale, const Vector3& rotation_euler_inDegrees, const Vector3& translation );
		~Transform();

		Transform& SetScaling( const Vector3& scale );
		Transform& SetRotation( const Vector3& rotation_euler_inDegrees );
		Transform& SetRotation( Degrees angle, const Vector3& axis );
		Transform& SetTranslation( const Vector3& translation );

		const Vector3& GetScaling() const;
		inline bool IsUsingEulerAngleRepresentation() const { return rotation_usingRepresentation_eulerAngles; }
		/* Usage: First query whether using this representation or not. */
		const Vector3& GetRotationEuler() const;
		/* Usage: First query whether using this representation or not. */
		Degrees GetRotationAngleAroundAxis() const;
		/* Usage: First query whether using this representation or not. */
		const Vector3& GetRotationAxis() const;
		const Vector3& GetTranslation() const;

		const Matrix4x4& GetFinalMatrix();

	private:
		const Matrix4x4& GetScalingMatrix();
		const Matrix4x4& GetRotationAndTranslationMatrix();

	private:
		Vector3 scale;
		// TODO: Switch to quaternions instead of euler angle triplets so we can represent both euler-angles & angle-axis representations uniformly and also query for both from the quaternion.
		Vector3 rotation_euler_inDegrees;
		Vector3 translation;

		Vector3 rotation_axis;
		Degrees rotation_angle;

		bool rotation_usingRepresentation_eulerAngles;

		Matrix4x4 scaling_matrix;
		Matrix4x4 rotation_and_translation_matrix;

		Matrix4x4 final_matrix;

		bool scaling_needsUpdate;
		bool rotation_needsUpdate;
		bool translation_needsUpdate;

		bool final_matrix_needsUpdate;
	};
}
