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

	Matrix4x4 Matrix::GeneralRotation( Degrees around_x, Degrees around_y, Degrees around_z )
	{
		const Radians alpha( around_x );
		const Radians  beta( around_y );
		const Radians gamma( around_z );

		const auto sinAlpha = std::sin( float( alpha ) );
		const auto sinBeta  = std::sin( float( beta  ) );
		const auto sinGamma = std::sin( float( gamma ) );

		const auto cosAlpha = std::cos( float( alpha ) );
		const auto cosBeta  = std::cos( float( beta  ) );
		const auto cosGamma = std::cos( float( gamma ) );

		const auto sinBetaCosGamma = sinBeta * cosGamma;
		const auto sinBetaSinGamma = sinBeta * sinGamma;

		return Matrix4x4
		(
			{
				cosBeta * cosGamma,										cosBeta * sinGamma,										-sinBeta,				0.0f,
				sinAlpha * sinBetaCosGamma - cosAlpha * sinGamma,		sinAlpha * sinBetaSinGamma + cosAlpha * cosGamma,		sinAlpha * cosBeta,		0.0f,
				cosAlpha * sinBetaCosGamma + sinAlpha * sinGamma,		cosAlpha * sinBetaSinGamma - sinAlpha * cosGamma,		cosAlpha * cosBeta,		0.0f,
				0.0f,													0.0f,													0.0f,					1.0f
			}
		);
	}

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

	Matrix4x4 RotationAroundAxis( Degrees angle, Vector3 vector )
	{
		vector.Normalize();

		const Radians theta( angle );

		const auto nx = vector.X();
		const auto ny = vector.Y();
		const auto nz = vector.Z();
		const auto cosTheta = std::cos( float( theta ) );
		const auto sinTheta = std::sin( float( theta ) );
		const auto one_minus_cosTheta = 1.0f - cosTheta;
		const auto nx_times_one_minus_cosTheta = nx * one_minus_cosTheta;
		const auto ny_times_one_minus_cosTheta = ny * one_minus_cosTheta;
		const auto nz_times_one_minus_cosTheta = nz * one_minus_cosTheta;
		const auto nx_sinTheta = nx * sinTheta;
		const auto ny_sinTheta = ny * sinTheta;
		const auto nz_sinTheta = nz * sinTheta;

		return Matrix4x4
		(
			{
				nx * nx_times_one_minus_cosTheta + cosTheta,		ny * nx_times_one_minus_cosTheta + nz_sinTheta,			nz * nx_times_one_minus_cosTheta - ny_sinTheta,		0.0f,
				nx * ny_times_one_minus_cosTheta - nz_sinTheta,		ny * ny_times_one_minus_cosTheta + cosTheta,			nz * ny_times_one_minus_cosTheta + nx_sinTheta,		0.0f,
				nx * nz_times_one_minus_cosTheta + ny_sinTheta,		ny * nz_times_one_minus_cosTheta - nx_sinTheta,			nz * nz_times_one_minus_cosTheta + cosTheta,		0.0f,
				0.0f,												0.0f,													0.0f,												1.0f
			}
		);
	}
}