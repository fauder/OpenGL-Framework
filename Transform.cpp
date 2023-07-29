// Project Includes.
#include "Transform.h"
#include "Math.h"
#include "Matrix.h"

namespace Framework
{
	Transform::Transform()
		:
		scale( Vector3( 1.0f, 1.0f, 1.0f ) ),
		rotation(),
		translation(),
		scaling_needsUpdate( true ),
		rotation_needsUpdate( true ),
		translation_needsUpdate( true ),
		final_matrix_needsUpdate( true )
	{
	}

	Transform::Transform( const Vector3& scale )
		:
		scale( scale ),
		rotation(),
		translation(),
		scaling_needsUpdate( true ),
		rotation_needsUpdate( true ),
		translation_needsUpdate( true ),
		final_matrix_needsUpdate( true )
	{
	}

	Transform::Transform( const Vector3& scale, const Vector3& translation )
		:
		scale( scale ),
		rotation(),
		translation( translation ),
		scaling_needsUpdate( true ),
		rotation_needsUpdate( true ),
		translation_needsUpdate( true ),
		final_matrix_needsUpdate( true )
	{
	}

	Transform::Transform( const Vector3& scale, const Quaternion& rotation, const Vector3& translation )
		:
		scale( scale ),
		rotation( rotation ),
		translation( translation ),
		scaling_needsUpdate( true ),
		rotation_needsUpdate( true ),
		translation_needsUpdate( true ),
		final_matrix_needsUpdate( true )
	{
	}

	Transform::~Transform()
	{
	}

	Transform& Transform::SetScaling( const Vector3& scale )
	{
		this->scale = scale;
		scaling_needsUpdate = final_matrix_needsUpdate = true;

		return *this;
	}

	Transform& Transform::SetRotation( const Quaternion& rotation )
	{
	#ifdef _DEBUG
		ASSERT( rotation.IsNormalized() && R"(Transform::SetRotation(): The quaternion "rotation" is not normalized!)" );
	#endif // _DEBUG

		this->rotation = rotation;
		rotation_needsUpdate = final_matrix_needsUpdate = true;

		return *this;
	}

	Transform& Transform::SetTranslation( const Vector3& translation )
	{
		this->translation = translation;
		translation_needsUpdate = final_matrix_needsUpdate = true;

		return *this;
	}

	const Vector3& Transform::GetScaling() const
	{
		return scale;
	}

	/* Usage: First query whether using this representation or not. */
	const Quaternion& Transform::GetRotation() const
	{
		return rotation;
	}

	const Vector3& Transform::GetTranslation() const
	{
		return translation;
	}

	const Matrix4x4& Transform::GetScalingMatrix()
	{
		if( scaling_needsUpdate )
		{
			scaling_matrix.SetDiagonals( scale );
			scaling_needsUpdate = false;
		}

		return scaling_matrix;
	}

	const Matrix4x4& Transform::GetRotationAndTranslationMatrix()
	{
		if( rotation_needsUpdate )
		{
			rotation_and_translation_matrix = Math::QuaternionToMatrix( rotation );
			rotation_needsUpdate = false;
		}

		if( translation_needsUpdate )
		{
			rotation_and_translation_matrix.SetTranslation( translation );
			translation_needsUpdate = false;
		}

		return rotation_and_translation_matrix;
	}

	const Matrix4x4& Transform::GetFinalMatrix()
	{
		if( final_matrix_needsUpdate )
		{
			final_matrix = GetScalingMatrix() * GetRotationAndTranslationMatrix();
			final_matrix_needsUpdate = false;
		}

		return final_matrix;
	}
}
