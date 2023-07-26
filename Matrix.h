#pragma once

// Project Includes.
#include "Angle.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

/* Constexpr functions can not be in the implementation file. So the ones that are constexpr are on this header file, the ones not constexpr are on the impl. (cpp) file. */
namespace Framework::Matrix
{
	/* In row-major form. */
	constexpr Matrix4x4 OrthographicProjection( const float left_plane_offset, const float right_plane_offset, const float bottom_plane_offset, const float top_plane_offset, 
												const float near_plane_offset, const float far_plane_offset )
	{
		const float f_plus_n  = far_plane_offset + near_plane_offset;
		const float f_minus_n = far_plane_offset - near_plane_offset;

		return Matrix4x4
		(
			{
				1.0f / right_plane_offset,			0.0f,								0.0f,						0.0f,
				0.0f,								1.0f / top_plane_offset,			0.0f,						0.0f,
				0.0f,								0.0f,								-2.0f / f_minus_n,			0.0f,
				0.0f,								0.0f,								-f_plus_n / f_minus_n,		1.0f
			}
		);
	}

	/* In row-major form. */ Matrix4x4 PerspectiveProjection( const float near_plane_offset, const float far_plane_offset,
															  const float aspect_ratio,      const Degrees field_of_view );

	/* In row-major form. */ constexpr Matrix4x4 Scaling( const float newScale )
	{
		return Matrix4x4
		(
			{
				newScale,	0.0f,		0.0f,		0.0f,
				0.0f,		newScale,	0.0f,		0.0f,
				0.0f,		0.0f,		newScale,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			}
		);
	}

	/* In row-major form. */ constexpr Matrix4x4 Scaling( const float newScale_x, const float newScale_y, const float newScale_z )
	{
		return Matrix4x4
		(
			{
				newScale_x,	0.0f,		0.0f,		0.0f,
				0.0f,		newScale_y,	0.0f,		0.0f,
				0.0f,		0.0f,		newScale_z,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			}
		);
	}

	/* In row-major form. */ constexpr Matrix4x4 ScalingOnX( const float newScale )
	{
		return Matrix4x4
		(
			{
				newScale,	0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			}
		);
	}

	/* In row-major form. */ constexpr Matrix4x4 ScalingOnY( const float newScale )
	{
		return Matrix4x4
		(
			{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		newScale,	0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			}
		);
	}

	/* In row-major form. */ constexpr Matrix4x4 ScalingOnZ( const float newScale )
	{
		return Matrix4x4
		(
			{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		newScale,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			}
		);
	}

	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 GeneralRotation( Degrees around_x, Degrees around_y, Degrees around_z );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ void GeneralRotation( Matrix4x4& matrix, Degrees around_x, Degrees around_y, Degrees around_z );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundX( Degrees angle );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundY( Degrees angle );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundZ( Degrees angle );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundAxis( Degrees angle, Vector3 vector );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ void RotationAroundAxis( Matrix4x4& matrix, Degrees angle, Vector3 vector );

	/* In row-major form. */ constexpr Matrix4x4 TranslationOnX( const float delta )
	{
		return Matrix4x4
		(
			{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				delta,		0.0f,		0.0f,		1.0f
			}
		);
	}

	/* In row-major form. */ constexpr Matrix4x4 Translation( const float delta_x, const float delta_y, const float delta_z )
	{
		return Matrix4x4
		(
			{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				delta_x,	delta_y,	delta_z,	1.0f
			}
		);
	}

	/* In row-major form. */ constexpr Matrix4x4 TranslationOnY( const float delta )
	{
		return Matrix4x4
		(
			{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				0.0f,		delta,		0.0f,		1.0f
			}
		);
	}

	/* In row-major form. */ constexpr Matrix4x4 TranslationOnZ( const float delta )
	{
		return Matrix4x4
		(
			{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				0.0f,		0.0f,		delta,		1.0f
			}
		);
	}
};
