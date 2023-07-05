// Project Includes.
#include "Transform.h"
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
		needsUpdate_scaling( true ),
		needsUpdate_rotation( true ),
		needsUpdate_translation( true ),
		needsUpdate_finalMatrix( true )
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
		needsUpdate_scaling( true ),
		needsUpdate_rotation( true ),
		needsUpdate_translation( true ),
		needsUpdate_finalMatrix( true )
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
		needsUpdate_scaling( true ),
		needsUpdate_rotation( true ),
		needsUpdate_translation( true ),
		needsUpdate_finalMatrix( true )
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
		needsUpdate_scaling( true ),
		needsUpdate_rotation( true ),
		needsUpdate_translation( true ),
		needsUpdate_finalMatrix( true )
	{
	}

	Transform::~Transform()
	{
	}

	Transform& Transform::SetScaling( const Vector3& scale )
	{
		this->scale = scale;
		needsUpdate_scaling = needsUpdate_finalMatrix = true;

		return *this;
	}

	Transform& Transform::SetRotation( const Vector3& rotation_euler_inDegrees )
	{
		// TODO: Switch to quaternions instead of euler angle triplets so we can represent both euler-angles & angle-axis representations uniformly and also query for both from the quaternion.
		rotation_usingRepresentation_eulerAngles = true;
		this->rotation_euler_inDegrees = rotation_euler_inDegrees;
		needsUpdate_rotation = needsUpdate_finalMatrix = true;

		return *this;
	}

	Transform& Transform::SetRotation( Degrees angle, const Vector3& axis )
	{
		// TODO: Switch to quaternions instead of euler angle triplets so we can represent both euler-angles & angle-axis representations uniformly and also query for both from the quaternion.
		rotation_usingRepresentation_eulerAngles = false;
		this->rotation_angle = angle;
		this->rotation_axis = axis;
		needsUpdate_rotation = needsUpdate_finalMatrix = true;

		return *this;
	}

	Transform& Transform::SetTranslation( const Vector3& translation )
	{
		this->translation = translation;
		needsUpdate_translation = needsUpdate_finalMatrix = true;

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
		if( needsUpdate_scaling )
		{
			matrix_scaling.SetDiagonals( scale );
			needsUpdate_scaling = false;
		}

		return matrix_scaling;
	}

	const Matrix4x4& Transform::GetRotationMatrix()
	{
		if( needsUpdate_rotation )
		{
			if( rotation_usingRepresentation_eulerAngles )
				matrix_rotation = Matrix::GeneralRotation( Degrees( rotation_euler_inDegrees.X() ), 
														   Degrees( rotation_euler_inDegrees.Y() ),
														   Degrees( rotation_euler_inDegrees.Z() ) );
			else
				matrix_rotation = Matrix::RotationAroundAxis( rotation_angle, rotation_axis );

			needsUpdate_rotation = false;
		}

		return matrix_rotation;
	}

	const Matrix4x4& Transform::GetTranslationMatrix()
	{
		if( needsUpdate_translation )
		{
			matrix_translation.SetTranslation( translation );
			needsUpdate_translation = false;
		}

		return matrix_translation;
	}

	const Matrix4x4& Transform::GetFinalMatrix()
	{
		if( needsUpdate_finalMatrix )
		{
			matrix_final = GetScalingMatrix() * GetRotationMatrix() * GetTranslationMatrix();
			needsUpdate_finalMatrix = false;
		}

		return matrix_final;
	}
}
