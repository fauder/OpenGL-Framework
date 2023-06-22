// Project Includes.
#include "Constants.h"
#include "Matrix.h"

// std Includes.
#include <cmath>

namespace Matrix
{
	/* In row-major form. */
	Matrix4x4 OrthographicProjection( const float left_plane_offset, const float right_plane_offset, const float bottom_plane_offset, const float top_plane_offset, const float near_plane_offset, const float far_plane_offset )
	{
		const float f_plus_n  = far_plane_offset + near_plane_offset;
		const float f_minus_n = far_plane_offset - near_plane_offset;

		return Matrix4x4
		{
			1.0f / right_plane_offset,			0.0f,								0.0f,						0.0f,
			0.0f,								1.0f / top_plane_offset,			0.0f,						0.0f,
			0.0f,								0.0f,								-2.0f / f_minus_n,			0.0f,
			0.0f,								0.0f,								-f_plus_n / f_minus_n,		1.0f
		};
	}

	/* In row-major form. */
	Matrix4x4 PerspectiveProjection( const float near_plane_offset, const float far_plane_offset, const float aspect_ratio, const float vertical_fieldOfView_inDegrees )
	{
		const auto half_fov_inRadians = ( vertical_fieldOfView_inDegrees / 2.0f ) * Constants::DEG_TO_RAD;

		// tan( half_fov_inRadians ) = half_height / near_plane_offset.

		const auto half_tangent = std::tan( half_fov_inRadians );
		const float half_height = half_tangent * near_plane_offset;
		const float half_width  = aspect_ratio * half_height;
		const float f_plus_n    = far_plane_offset + near_plane_offset;
		const float f_minus_n   = far_plane_offset - near_plane_offset;

		return Matrix4x4
		{
			near_plane_offset / half_width,		0.0f,								0.0f,										0.0f,
			0.0f,								near_plane_offset / half_height,	0.0f,										0.0f,
			0.0f,								0.0f,								-f_plus_n / f_minus_n,						-1.0f,
			0.0f,								0.0f,								-2.0f * far_plane_offset / f_minus_n,		0.0f
		};
	}

	Matrix4x4 Scaling( const float newScale )
	{
		return Matrix4x4
		{
			newScale,	0.0f,		0.0f,		0.0f,
			0.0f,		newScale,	0.0f,		0.0f,
			0.0f,		0.0f,		newScale,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f
		};
	}

	Matrix4x4 Scaling( const float newScale_x, const float newScale_y, const float newScale_z )
	{
		return Matrix4x4
		{
			newScale_x,	0.0f,		0.0f,		0.0f,
			0.0f,		newScale_y,	0.0f,		0.0f,
			0.0f,		0.0f,		newScale_z,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f
		};
	}

	Matrix4x4 ScalingOnX( const float newScale )
	{
		return Matrix4x4
		{
			newScale,	0.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,		0.0f,
			0.0f,		0.0f,		1.0f,		0.0f,
			0.0f,		0.0f,		0.0f,		1.0f
		};
	}

	Matrix4x4 ScalingOnY( const float newScale )
	{
		return Matrix4x4
		{
			1.0f,		0.0f,		0.0f,		0.0f,
			0.0f,		newScale,	0.0f,		0.0f,
			0.0f,		0.0f,		1.0f,		0.0f,
			0.0f,		0.0f,		0.0f,		1.0f
		};
	}

	Matrix4x4 ScalingOnZ( const float newScale )
	{
		return Matrix4x4
		{
			1.0f,		0.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,		0.0f,
			0.0f,		0.0f,		newScale,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f
		};
	}

	Matrix4x4 RotationAroundX( const float angle_inDegrees )
	{
		const auto angle_inRadians = angle_inDegrees * Constants::DEG_TO_RAD;
		const auto cosine_term = std::cos( angle_inRadians );
		const auto   sine_term = std::sin( angle_inRadians );

		return Matrix4x4
		{
			1.0f,			0.0f,			0.0f,			0.0f,
			0.0f,			cosine_term,	sine_term,		0.0f,
			0.0f,			-sine_term,		cosine_term,	0.0f,
			0.0f,			0.0f,			0.0f,			1.0f
		};
	}

	Matrix4x4 RotationAroundY( const float angle_inDegrees )
	{
		const auto angle_inRadians = angle_inDegrees * Constants::DEG_TO_RAD;
		const auto cosine_term = std::cos( angle_inRadians );
		const auto   sine_term = std::sin( angle_inRadians );

		return Matrix4x4
		{
			cosine_term,	0.0f,			-sine_term,		0.0f,
			0.0f,			1.0f,			0.0f,			0.0f,
			sine_term,		0.0f,			cosine_term,	0.0f,
			0.0f,			0.0f,			0.0f,			1.0f
		};
	}

	Matrix4x4 RotationAroundZ( const float angle_inDegrees )
	{
		const auto angle_inRadians = angle_inDegrees * Constants::DEG_TO_RAD;
		const auto cosine_term = std::cos( angle_inRadians );
		const auto   sine_term = std::sin( angle_inRadians );

		return Matrix4x4
		{
			cosine_term,	sine_term,		0.0f,			0.0f,
			-sine_term,		cosine_term,	0.0f,			0.0f,
			0.0f,			0.0f,			1.0f,			0.0f,
			0.0f,			0.0f,			0.0f,			1.0f
		};
	}

	Matrix4x4 Translation( const float delta_x, const float delta_y, const float delta_z )
	{
		return Matrix4x4
		{
			1.0f,		0.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,		0.0f,
			0.0f,		0.0f,		1.0f,		0.0f,
			delta_x,	delta_y,	delta_z,	1.0f
		};
	}

	Matrix4x4 TranslationOnX( const float delta )
	{
		return Matrix4x4
		{
			1.0f,		0.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,		0.0f,
			0.0f,		0.0f,		1.0f,		0.0f,
			delta,		0.0f,		0.0f,		1.0f
		};
	}

	Matrix4x4 TranslationOnY( const float delta )
	{
		return Matrix4x4
		{
			1.0f,		0.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,		0.0f,
			0.0f,		0.0f,		1.0f,		0.0f,
			0.0f,		delta,		0.0f,		1.0f
		};
	}

	Matrix4x4 TranslationOnZ( const float delta )
	{
		return Matrix4x4
		{
			1.0f,		0.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,		0.0f,
			0.0f,		0.0f,		1.0f,		0.0f,
			0.0f,		0.0f,		delta,		1.0f
		};
	}
}