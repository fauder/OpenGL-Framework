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

		const auto half_tangent = Tan( half_fov );
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
	 * Describes an extrinsic (fixed-axis) rotation, in this order: first heading (around y), then pitch (around x) and finally bank (around z). */
	Matrix4x4 Matrix::FromEuler( Radians heading_around_y, Radians pitch_around_x, Radians bank_around_z )
	{
		const auto sin_pitch   = Sin( pitch_around_x );
		const auto sin_heading = Sin( heading_around_y );
		const auto sin_bank    = Sin( bank_around_z );

		const auto cos_pitch   = Cos( pitch_around_x );
		const auto cos_heading = Cos( heading_around_y );
		const auto cos_bank    = Cos( bank_around_z );

		const auto cos_bank_cos_heading  = cos_bank * cos_heading;
		const auto sin_pitch_sin_heading = sin_pitch * sin_heading;

		return Matrix4x4
		(
			{
				cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading,		sin_bank * cos_pitch,											-cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading,	0.0f,
				-sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading,		cos_bank * cos_pitch,											sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading,		0.0f,
				cos_pitch * sin_heading,										-sin_pitch,														cos_pitch * cos_heading,										0.0f,
				0.0f,															0.0f,															0.0f,															1.0f
			}
		);
	}

	/* In-place modification of the upper-left 3x3 portion. */
	/* In row-major form. Right-handed. Clockwise rotation.
	 * Describes an extrinsic (fixed-axis) rotation, in this order: first heading (around y), then pitch (around x) and finally bank (around z). */
	void Matrix::FromEuler( Matrix4x4& matrix, Radians heading_around_y, Radians pitch_around_x, Radians bank_around_z )
	{
		const auto sin_pitch   = Sin( pitch_around_x );
		const auto sin_heading = Sin( heading_around_y );
		const auto sin_bank    = Sin( bank_around_z );

		const auto cos_pitch   = Cos( pitch_around_x );
		const auto cos_heading = Cos( heading_around_y );
		const auto cos_bank    = Cos( bank_around_z );

		const auto cos_bank_cos_heading = cos_bank * cos_heading;
		const auto sin_pitch_sin_heading = sin_pitch * sin_heading;

		matrix[ 0 ][ 0 ] = cos_bank_cos_heading + sin_bank * sin_pitch_sin_heading;		matrix[ 0 ][ 1 ] = sin_bank * cos_pitch;	matrix[ 0 ][ 2 ] = -cos_bank * sin_heading + sin_bank * sin_pitch * cos_heading;
		matrix[ 1 ][ 0 ] = -sin_bank * cos_heading + cos_bank * sin_pitch_sin_heading;	matrix[ 1 ][ 1 ] = cos_bank * cos_pitch;	matrix[ 1 ][ 2 ] = sin_bank * sin_heading + sin_pitch * cos_bank_cos_heading;	
		matrix[ 2 ][ 0 ] = cos_pitch * sin_heading;										matrix[ 2 ][ 1 ] = -sin_pitch;				matrix[ 2 ][ 2 ] = cos_pitch * cos_heading;									
	}

	/* In row-major form. Right-handed. Clockwise rotation. */
	Matrix4x4 RotationAroundX( Radians pitch )
	{
		const auto cosine_term = Cos( pitch );
		const auto   sine_term = Sin( pitch );

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
		const auto cosine_term = Cos( heading );
		const auto   sine_term = Sin( heading );

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
		const auto cosine_term = Cos( bank );
		const auto   sine_term = Sin( bank );

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
		const auto cos_theta                    = Cos( theta );
		const auto sin_theta                    = Sin( theta );
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
		const auto cos_theta                    = Cos( theta );
		const auto sin_theta                    = Sin( theta );
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