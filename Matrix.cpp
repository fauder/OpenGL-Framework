// Project Includes.
#include "Angle.hpp"
#include "Constants.h"
#include "Math.h"
#include "Matrix.h"

// std Includes.
#include <cmath>

namespace Framework::Matrix
{
	/* In row-major form. */
	Matrix4x4 PerspectiveProjection( const float near_plane_offset, const float far_plane_offset, const float aspect_ratio, const Degrees vertical_fieldOfView )
	{
		const Radians half_fov( vertical_fieldOfView / 2.0f );

		const auto half_tangent = std::tan( float( half_fov ) );
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

	/* In row-major form. Right-handed. Clockwise rotation. */
	Matrix4x4 Matrix::GeneralRotation( Degrees around_x, Degrees around_y, Degrees around_z )
	{
		const Radians alpha( around_x );
		const Radians  beta( around_y );
		const Radians gamma( around_z );

		const auto sin_alpha = std::sin( float( alpha ) );
		const auto sin_beta  = std::sin( float( beta  ) );
		const auto sin_gamma = std::sin( float( gamma ) );

		const auto cos_alpha = std::cos( float( alpha ) );
		const auto cos_beta  = std::cos( float( beta  ) );
		const auto cos_gamma = std::cos( float( gamma ) );

		const auto sin_beta_cos_gamma = sin_beta * cos_gamma;
		const auto sin_beta_sin_gamma = sin_beta * sin_gamma;

		return Matrix4x4
		(
			{
				cos_beta * cos_gamma,									cos_beta * sin_gamma,									-sin_beta,				0.0f,
				sin_alpha * sin_beta_cos_gamma - cos_alpha * sin_gamma,	sin_alpha * sin_beta_sin_gamma + cos_alpha * cos_gamma,	sin_alpha * cos_beta,	0.0f,
				cos_alpha * sin_beta_cos_gamma + sin_alpha * sin_gamma,	cos_alpha * sin_beta_sin_gamma - sin_alpha * cos_gamma,	cos_alpha * cos_beta,	0.0f,
				0.0f,													0.0f,													0.0f,					1.0f
			}
		);
	}

	/* In row-major form. Right-handed. Clockwise rotation. */
	/* In-place modification of the upper-left 3x3 portion. */
	void Matrix::GeneralRotation( Matrix4x4& matrix, Degrees around_x, Degrees around_y, Degrees around_z )
	{
		const Radians alpha( around_x );
		const Radians  beta( around_y );
		const Radians gamma( around_z );

		const auto sin_alpha = std::sin( float( alpha ) );
		const auto sin_beta  = std::sin( float( beta  ) );
		const auto sin_gamma = std::sin( float( gamma ) );

		const auto cos_alpha = std::cos( float( alpha ) );
		const auto cos_beta  = std::cos( float( beta  ) );
		const auto cos_gamma = std::cos( float( gamma ) );

		const auto sin_beta_cos_gamma = sin_beta * cos_gamma;
		const auto sin_beta_sin_gamma = sin_beta * sin_gamma;

		matrix[ 0 ][ 0 ] = cos_beta * cos_gamma;									matrix[ 0 ][ 1 ] = cos_beta * sin_gamma;									matrix[ 0 ][ 2 ] = -sin_beta;
		matrix[ 1 ][ 0 ] = sin_alpha * sin_beta_cos_gamma - cos_alpha * sin_gamma;	matrix[ 1 ][ 1 ] = sin_alpha * sin_beta_sin_gamma + cos_alpha * cos_gamma;	matrix[ 1 ][ 2 ] = sin_alpha * cos_beta;
		matrix[ 2 ][ 0 ] = cos_alpha * sin_beta_cos_gamma + sin_alpha * sin_gamma,	matrix[ 2 ][ 1 ] = cos_alpha * sin_beta_sin_gamma - sin_alpha * cos_gamma;	matrix[ 2 ][ 2 ] = cos_alpha * cos_beta;
	}

	/* In row-major form. Right-handed. Clockwise rotation. */
	Matrix4x4 RotationAroundX( Degrees angle )
	{
		const Radians alpha( angle );
		const auto cosine_term = std::cos( float( alpha ) );
		const auto   sine_term = std::sin( float( alpha ) );

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
	Matrix4x4 RotationAroundY( Degrees angle )
	{
		const Radians beta( angle );
		const auto cosine_term = std::cos( float( beta ) );
		const auto   sine_term = std::sin( float( beta ) );

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
	Matrix4x4 RotationAroundZ( Degrees angle )
	{
		const Radians gamma( angle );
		const auto cosine_term = std::cos( float( gamma ) );
		const auto   sine_term = std::sin( float( gamma ) );

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
	Matrix4x4 RotationAroundAxis( Degrees angle, Vector3 vector )
	{
		vector.Normalize();

		const Radians theta( angle );

		const auto nx                           = vector.X();
		const auto ny                           = vector.Y();
		const auto nz                           = vector.Z();
		const auto cos_theta                    = std::cos( float( theta ) );
		const auto sin_theta                    = std::sin( float( theta ) );
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

	/* In row-major form. Right-handed. Clockwise rotation. */
	/* In-place modification of the upper-left 3x3 portion. */
	void Matrix::RotationAroundAxis( Matrix4x4& matrix, Degrees angle, Vector3 vector )
	{
		vector.Normalize();

		const Radians theta( angle );

		const auto nx                           = vector.X();
		const auto ny                           = vector.Y();
		const auto nz                           = vector.Z();
		const auto cos_theta                    = std::cos( float( theta ) );
		const auto sin_theta                    = std::sin( float( theta ) );
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