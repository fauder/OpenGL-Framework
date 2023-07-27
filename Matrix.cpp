// Project Includes.
#include "Angle.hpp"
#include "Constants.h"
#include "Math.h"
#include "Matrix.h"

namespace Framework::Matrix
{
	/* In row-major form. */
	Matrix4x4 PerspectiveProjection( const float near_plane_offset, const float far_plane_offset, const float aspect_ratio, const Radians vertical_fieldOfView )
	{
		const Radians half_fov( vertical_fieldOfView / 2.0f );

		const auto half_tangent = Math::Tan( half_fov );
		const float half_height = half_tangent * near_plane_offset;
		const float half_width  = aspect_ratio * half_height;
		const float f_plus_n    = far_plane_offset + near_plane_offset;
		const float f_minus_n   = far_plane_offset - near_plane_offset;

		return Matrix4x4
		(
			{
				near_plane_offset / half_width,		0.0f,								0.0f,										0.0f,
				0.0f,								near_plane_offset / half_height,	0.0f,										0.0f,
				0.0f,								0.0f,								-f_plus_n / f_minus_n,						-1.0f,
				0.0f,								0.0f,								-2.0f * far_plane_offset / f_minus_n,		0.0f
			}
		);
	}

	/* In row-major form. Right-handed. Clockwise rotation.
	 * Describes an extrinsic (fixed-axis) rotation, in this order:    bank (around z) -> pitch (around x) -> heading (around y).
	 * an Euler rotation (is intrinsic, body-axis),  in this order: heading (around y) -> pitch (around x) -> bank (around z), which means */
	Matrix4x4 Matrix::FromEuler( Radians heading_around_y, Radians pitch_around_x, Radians bank_around_z )
	{
		const auto sin_pitch   = Math::Sin( pitch_around_x );
		const auto sin_heading = Math::Sin( heading_around_y );
		const auto sin_bank    = Math::Sin( bank_around_z );

		const auto cos_pitch   = Math::Cos( pitch_around_x );
		const auto cos_heading = Math::Cos( heading_around_y );
		const auto cos_bank    = Math::Cos( bank_around_z );

		const auto cos_bank_cos_heading  = cos_bank * cos_heading;
		const auto sin_pitch_sin_heading = sin_pitch * sin_heading;

		return Matrix4x4
		(
			{
				cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading,		sin_bank * cos_pitch,		-cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading,	0.0f,
				-sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading,		cos_bank * cos_pitch,		sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading,		0.0f,
				cos_pitch * sin_heading,										-sin_pitch,					cos_pitch * cos_heading,										0.0f,
				0.0f,															0.0f,						0.0f,															1.0f
			}
		);
	}

	/* In-place modification of the upper-left 3x3 portion. */
	/* In row-major form. Right-handed. Clockwise rotation.
	 * Describes an extrinsic (fixed-axis) rotation, in this order:    bank (around z) -> pitch (around x) -> heading (around y).
	 * an Euler rotation (is intrinsic, body-axis),  in this order: heading (around y) -> pitch (around x) -> bank (around z), which means */
	void Matrix::FromEuler( Matrix4x4& matrix, Radians heading_around_y, Radians pitch_around_x, Radians bank_around_z )
	{
		const auto sin_pitch   = Math::Sin( pitch_around_x );
		const auto sin_heading = Math::Sin( heading_around_y );
		const auto sin_bank    = Math::Sin( bank_around_z );

		const auto cos_pitch   = Math::Cos( pitch_around_x );
		const auto cos_heading = Math::Cos( heading_around_y );
		const auto cos_bank    = Math::Cos( bank_around_z );

		const auto cos_bank_cos_heading = cos_bank * cos_heading;
		const auto sin_pitch_sin_heading = sin_pitch * sin_heading;

		matrix[ 0 ][ 0 ] = cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading;		matrix[ 0 ][ 1 ] = sin_bank * cos_pitch;	matrix[ 0 ][ 2 ] = -cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading;
		matrix[ 1 ][ 0 ] = -sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading;	matrix[ 1 ][ 1 ] = cos_bank * cos_pitch;	matrix[ 1 ][ 2 ] = sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading;	
		matrix[ 2 ][ 0 ] = cos_pitch * sin_heading;										matrix[ 2 ][ 1 ] = -sin_pitch;				matrix[ 2 ][ 2 ] = cos_pitch * cos_heading;									
	}

	/* Expects matrix in row-major form. Right-handed. Clockwise rotation.
	 * The matrix should describe an extrinsic (fixed-axis) rotation, in this order:	bank (around z) -> pitch (around x) -> heading (around y), which results in
	 * the Euler rotation (that is intrinsic & around body-axes) in this order:		 heading (around y) -> pitch (around x) -> bank (around z). */
	void Matrix::ToEuler( const Matrix4x4& matrix, Radians& heading_around_y, Radians& pitch_around_x, Radians& bank_around_z )
	{
		/* The matrix' values for reference:
		{
			matrix[ 0 ][ 0 ] = cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading		matrix[ 0 ][ 1 ] = sin_bank * cos_pitch		matrix[ 0 ][ 2 ] = -cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading
			matrix[ 1 ][ 0 ] = -sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading	matrix[ 1 ][ 1 ] = cos_bank * cos_pitch		matrix[ 1 ][ 2 ] = sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading
			matrix[ 2 ][ 0 ] = cos_pitch * sin_heading										matrix[ 2 ][ 1 ] = -sin_pitch				matrix[ 2 ][ 2 ] = cos_pitch * cos_heading
		}*/

		using namespace Math::Literals;

		const auto sin_pitch = -matrix[ 2 ][ 1 ];

		if( sin_pitch <= -1.0f )
			pitch_around_x = Radians( -Constants< float >::Half_Pi() );
		else if( sin_pitch >= +1.0f )
			pitch_around_x = Radians( Constants< float >::Half_Pi() );
		else
			pitch_around_x = Math::Asin( sin_pitch );

		const auto cos_pitch = Math::Cos( pitch_around_x );

		if( Math::IsZero( cos_pitch ) ) // Which means pitch is +90 or -90 -> gimbal lock!
		{
			// Set bank to zero & assign all rotation around the vertical axis to heading.
			bank_around_z = 0.0_rad;

			// Since bank = 0, it means that cos_bank = 1 & sin_bank = 0. Reminder: cos_pitch is also 0.

			/* Updated matrix' values for reference:
			{
				matrix[ 0 ][ 0 ] = cos_heading					matrix[ 0 ][ 1 ] = 0				matrix[ 0 ][ 2 ] = -sin_heading
				matrix[ 1 ][ 0 ] = sin_pitch_sin_heading		matrix[ 1 ][ 1 ] = 0				matrix[ 1 ][ 2 ] = sin_pitch * cos_heading
				matrix[ 2 ][ 0 ] = 0							matrix[ 2 ][ 1 ] = -sin_pitch		matrix[ 2 ][ 2 ] = 0
			}*/

			heading_around_y = Math::Atan2( -matrix[ 0 ][ 2 ], matrix[ 0 ][ 0 ] );
		}
		else
		{

			/*
			const auto sin_heading = matrix[ 2 ][ 0 ] / cos_pitch;
			const auto cos_heading = matrix[ 2 ][ 2 ] / cos_pitch;
			*/

			/* Since cos_pitch is a common denominator between these two terms, we can directly pass the numerators of these fractions to atan2() instead. */

			heading_around_y = Math::Atan2( matrix[ 2 ][ 0 ], matrix[ 2 ][ 2 ] );

			/* Similar method for bank: */

			bank_around_z = Math::Atan2( matrix[ 0 ][ 1 ], matrix[ 1 ][ 1 ] );
		}
	}

	/* In row-major form. Right-handed. Clockwise rotation. */
	Matrix4x4 RotationAroundX( Radians pitch )
	{
		const auto cosine_term = Math::Cos( pitch );
		const auto   sine_term = Math::Sin( pitch );

		return Matrix4x4
		(
			{
				1.0f,			0.0f,			0.0f,			0.0f,
				0.0f,			cosine_term,	sine_term,		0.0f,
				0.0f,			-sine_term,		cosine_term,	0.0f,
				0.0f,			0.0f,			0.0f,			1.0f
			}
		);
	}

	/* In row-major form. Right-handed. Clockwise rotation. */
	Matrix4x4 RotationAroundY( Radians heading )
	{
		const auto cosine_term = Math::Cos( heading );
		const auto   sine_term = Math::Sin( heading );

		return Matrix4x4
		(
			{
				cosine_term,	0.0f,			-sine_term,		0.0f,
				0.0f,			1.0f,			0.0f,			0.0f,
				sine_term,		0.0f,			cosine_term,	0.0f,
				0.0f,			0.0f,			0.0f,			1.0f
			}
		);
	}

	/* In row-major form. Right-handed. Clockwise rotation. */
	Matrix4x4 RotationAroundZ( Radians bank )
	{
		const auto cosine_term = Math::Cos( bank );
		const auto   sine_term = Math::Sin( bank );

		return Matrix4x4
		(
			{
				cosine_term,	sine_term,		0.0f,			0.0f,
				-sine_term,		cosine_term,	0.0f,			0.0f,
				0.0f,			0.0f,			1.0f,			0.0f,
				0.0f,			0.0f,			0.0f,			1.0f
			}
		);
	}

	/* In row-major form. Right-handed. Clockwise rotation. */
	Matrix4x4 RotationAroundAxis( Radians angle, Vector3 vector )
	{
		vector.Normalize();

		const Radians theta( angle );

		const auto nx                           = vector.X();
		const auto ny                           = vector.Y();
		const auto nz                           = vector.Z();
		const auto cos_theta                    = Math::Cos( theta );
		const auto sin_theta                    = Math::Sin( theta );
		const auto one_minus_cos_theta          = 1.0f - cos_theta;
		const auto nx_times_one_minus_cos_theta = nx * one_minus_cos_theta;
		const auto ny_times_one_minus_cos_theta = ny * one_minus_cos_theta;
		const auto nz_times_one_minus_cos_theta = nz * one_minus_cos_theta;
		const auto nx_sin_theta                 = nx * sin_theta;
		const auto ny_sin_theta                 = ny * sin_theta;
		const auto nz_sin_theta                 = nz * sin_theta;

		return Matrix4x4
		(
			{
				nx * nx_times_one_minus_cos_theta + cos_theta,		ny * nx_times_one_minus_cos_theta + nz_sin_theta,		nz * nx_times_one_minus_cos_theta - ny_sin_theta,	0.0f,
				nx * ny_times_one_minus_cos_theta - nz_sin_theta,	ny * ny_times_one_minus_cos_theta + cos_theta,			nz * ny_times_one_minus_cos_theta + nx_sin_theta,	0.0f,
				nx * nz_times_one_minus_cos_theta + ny_sin_theta,	ny * nz_times_one_minus_cos_theta - nx_sin_theta,		nz * nz_times_one_minus_cos_theta + cos_theta,		0.0f,
				0.0f,												0.0f,													0.0f,												1.0f
			}
		);
	}

	/* In-place modification of the upper-left 3x3 portion. */
	/* In row-major form. Right-handed. Clockwise rotation. */
	void Matrix::RotationAroundAxis( Matrix4x4& matrix, Radians angle, Vector3 vector )
	{
		vector.Normalize();

		const Radians theta( angle );

		const auto nx                           = vector.X();
		const auto ny                           = vector.Y();
		const auto nz                           = vector.Z();
		const auto cos_theta                    = Math::Cos( theta );
		const auto sin_theta                    = Math::Sin( theta );
		const auto one_minus_cos_theta          = 1.0f - cos_theta;
		const auto nx_times_one_minus_cos_theta = nx * one_minus_cos_theta;
		const auto ny_times_one_minus_cos_theta = ny * one_minus_cos_theta;
		const auto nz_times_one_minus_cos_theta = nz * one_minus_cos_theta;
		const auto nx_sin_theta                 = nx * sin_theta;
		const auto ny_sin_theta                 = ny * sin_theta;
		const auto nz_sin_theta                 = nz * sin_theta;

		matrix[ 0 ][ 0 ] = nx * nx_times_one_minus_cos_theta + cos_theta;		matrix[ 0 ][ 1 ] = ny * nx_times_one_minus_cos_theta + nz_sin_theta;	matrix[ 0 ][ 2 ] = nz * nx_times_one_minus_cos_theta - ny_sin_theta;
		matrix[ 1 ][ 0 ] = nx * ny_times_one_minus_cos_theta - nz_sin_theta;	matrix[ 1 ][ 1 ] = ny * ny_times_one_minus_cos_theta + cos_theta;		matrix[ 1 ][ 2 ] = nz * ny_times_one_minus_cos_theta + nx_sin_theta;
		matrix[ 2 ][ 0 ] = nx * nz_times_one_minus_cos_theta + ny_sin_theta;	matrix[ 2 ][ 1 ] = ny * nz_times_one_minus_cos_theta - nx_sin_theta;	matrix[ 2 ][ 2 ] = nz * nz_times_one_minus_cos_theta + cos_theta;	
	}
}