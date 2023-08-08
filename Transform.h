#pragma once

// Project Includes.
#include "Angle.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "Vector.hpp"

namespace Framework
{
	class Transform
	{
	public:
		Transform();
		Transform( const Vector3& scale );
		Transform( const Vector3& scale, const Vector3& translation );
		Transform( const Vector3& scale, const Quaternion& rotation, const Vector3& translation );
		~Transform();

		Transform& SetScaling( const Vector3& scale );
		Transform& SetRotation( const Quaternion& quaternion );
		Transform& SetTranslation( const Vector3& translation );

		const Vector3& GetScaling() const;
		const Quaternion& GetRotation() const;
		const Vector3& GetTranslation() const;

		const Matrix4x4& GetFinalMatrix();
		const Matrix4x4 GetInverseOfFinalMatrix();

	private:
		const Matrix4x4& GetScalingMatrix();
		const Matrix4x4& GetRotationAndTranslationMatrix();

	private:
		Vector3 scale;
		Vector3 translation;
		Quaternion rotation;

		Matrix4x4 scaling_matrix;
		Matrix4x4 rotation_and_translation_matrix;

		Matrix4x4 final_matrix;

		bool scaling_needsUpdate;
		bool rotation_needsUpdate;
		bool translation_needsUpdate;

		bool final_matrix_needsUpdate;
	};
}
