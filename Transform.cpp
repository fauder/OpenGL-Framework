// Project Includes.
#include "Transform.h"
#include "Math.h"
#include "Matrix.h"

namespace Framework
{
	Transform::Transform()
		:
		scale( Vector3( 1.0f, 1.0f, 1.0f ) ),
		rotation_euler_inDegrees( Vector3() ),
		translation( Vector3() ),
		rotation_axis( Vector3() ),
		rotation_angle( 0.0f ),
		rotation_usingRepresentation_eulerAngles( true ),
		scaling_needsUpdate( true ),
		rotation_needsUpdate( true ),
		translation_needsUpdate( true ),
		final_matrix_needsUpdate( true )
	{
	}

	Transform::Transform( const Vector3& scale )
		:
		scale( scale ),
		rotation_euler_inDegrees( Vector3() ),
		translation( Vector3() ),
		rotation_axis( Vector3() ),
		rotation_angle( 0.0f ),
		rotation_usingRepresentation_eulerAngles( true ),
		scaling_needsUpdate( true ),
		rotation_needsUpdate( true ),
		translation_needsUpdate( true ),
		final_matrix_needsUpdate( true )
	{
	}

	Transform::Transform( const Vector3& scale, const Vector3& translation )
		:
		scale( scale ),
		rotation_euler_inDegrees( Vector3() ),
		translation( translation ),
		rotation_axis( Vector3() ),
		rotation_angle( 0.0f ),
		rotation_usingRepresentation_eulerAngles( true ),
		scaling_needsUpdate( true ),
		rotation_needsUpdate( true ),
		translation_needsUpdate( true ),
		final_matrix_needsUpdate( true )
	{
	}

	Transform::Transform( const Vector3& scale, const Vector3& rotation_euler_inDegrees, const Vector3& translation )
		:
		scale( scale ),
		rotation_euler_inDegrees( rotation_euler_inDegrees ),
		translation( translation ),
		rotation_axis( Vector3() ),
		rotation_angle( 0.0f ),
		rotation_usingRepresentation_eulerAngles( true ),
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

	Transform& Transform::SetRotation( const Vector3& rotation_euler_inDegrees )
	{
		// TODO: Switch to quaternions instead of euler angle triplets so we can represent both euler-angles & angle-axis representations uniformly and also query for both from the quaternion.
		rotation_usingRepresentation_eulerAngles = true;
		this->rotation_euler_inDegrees = rotation_euler_inDegrees;
		rotation_needsUpdate = final_matrix_needsUpdate = true;

		return *this;
	}

	Transform& Transform::SetRotation( Degrees angle, const Vector3& axis )
	{
		// TODO: Switch to quaternions instead of euler angle triplets so we can represent both euler-angles & angle-axis representations uniformly and also query for both from the quaternion.
		rotation_usingRepresentation_eulerAngles = false;
		this->rotation_angle = angle;
		this->rotation_axis = axis;
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
	const Vector3& Transform::GetRotationEuler() const
	{
		return rotation_euler_inDegrees;
	}

	/* Usage: First query whether using this representation or not.
		Angle is in degrees. */
	Degrees Transform::GetRotationAngleAroundAxis() const
	{
		return rotation_angle;
	}

	/* Usage: First query whether using this representation or not. */
	const Vector3& Transform::GetRotationAxis() const
	{
		return rotation_axis;
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
			if( rotation_usingRepresentation_eulerAngles )
				Math::FromEuler( rotation_and_translation_matrix,
								 Degrees( rotation_euler_inDegrees.X() ),
								 Degrees( rotation_euler_inDegrees.Y() ),
								 Degrees( rotation_euler_inDegrees.Z() ) );
			else
				Matrix::RotationAroundAxis( rotation_and_translation_matrix,
											rotation_angle, rotation_axis );

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
