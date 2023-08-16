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
		translation( ZERO_INITIALIZATION ),
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
		translation( ZERO_INITIALIZATION ),
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

	Transform& Transform::OffsetScaling( const Vector3& delta )
	{
		return SetScaling( scale + delta );
	}

	Transform& Transform::MultiplyScaling( const Vector3& multiplier )
	{
		return SetScaling( scale * multiplier );
	}

	Transform& Transform::MultiplyScaling( const float multiplier )
	{
		return SetScaling( scale * multiplier );
	}

	Transform& Transform::OffsetTranslation( const Vector3& delta )
	{
		return SetTranslation( translation + delta );
	}

	Transform& Transform::MultiplyTranslation( const Vector3& multiplier )
	{
		return SetTranslation( translation * multiplier );
	}

	Transform& Transform::MultiplyTranslation( const float multiplier )
	{
		return SetTranslation( translation * multiplier );
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
			rotation_and_translation_matrix = Matrix4x4( Math::QuaternionToMatrix3x3( rotation ), rotation_and_translation_matrix.GetRow< 3 >( 3 ) );
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

	const Matrix4x4 Transform::GetInverseOfFinalMatrix()
	{
		/* Instead of actually calculating the inverse of the matrix, we'll leverage the information we have on our two current matrices:
		 * Scaling Matrix: We can simply use the inverses (with respect to multiplication) of diagonal elements' values to reverse the scaling operation.
		 * Rotation And Translation Matrix:
		 *		We can simply transpose the rotation part (upper 3x3 portion of the 4x4 matrix) since rotation matrices are orthogonal.
		 *		We can simply use the inverses (with respect to addition) of translation elements (first 3 elements of the last row of the 4x4 matrix) to reverse the translation operation.
		*/

		// Force-update the matrices to get up-to-date values.
		GetScalingMatrix();
		GetRotationAndTranslationMatrix();

		const Vector4 inverse_scale( 1.0f / scaling_matrix[ 0 ][ 0 ], 1.0f / scaling_matrix[ 1 ][ 1 ], 1.0f / scaling_matrix[ 2 ][ 2 ], 1.0f );

		const Matrix4x4 inverse_scaling_matrix( inverse_scale );
		const Matrix4x4 inverse_rotation_matrix( rotation_and_translation_matrix.SubMatrix< 3 >().Transposed() );
		const Matrix4x4 inverse_translation_matrix( Matrix4x4{}.SetTranslation( -rotation_and_translation_matrix.GetRow< 3 >( 3 /* Last Row. */ ) ) );

		return inverse_translation_matrix * inverse_rotation_matrix * inverse_scaling_matrix;
	}
}
