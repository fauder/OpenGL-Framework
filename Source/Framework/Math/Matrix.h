#pragma once

// Framework Includes.
#include "Math/Angle.hpp"
#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"

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
															  const float aspect_ratio,      const Radians field_of_view );

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

	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundX( Radians pitch );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundY( Radians heading );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundZ( Radians bank );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ Matrix4x4 RotationAroundAxis( Radians angle, Vector3 vector );
	/* In row-major form. Right-handed. Counter-clockwise rotation. */ void RotationAroundAxis( Matrix4x4& matrix, Radians angle, Vector3 vector );

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

	/* In row-major form. */ constexpr Matrix4x4 Translation( const Vector3 delta )
	{
		return Matrix4x4
		(
			{
				1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				delta.X(),	delta.Y(),	delta.Z(),	1.0f
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

	/* In row-major form. Right-handed. */
	constexpr Matrix4x4 LookAt( const Vector3& camera_position, Vector3 to_target_normalized, const Vector3& world_up_normalized = Vector3::Up() )
	{
	#ifdef _DEBUG
		ASSERT( to_target_normalized.IsNormalized() && R"(Matrix::LookAt(): "to_target_normalized" is not normalized!)" );
		ASSERT(  world_up_normalized.IsNormalized() && R"(Matrix::LookAt():  "world_up_normalized" is not normalized!)" );
	#endif
		const auto to_camera_right_normalized = Math::Cross( world_up_normalized,  to_target_normalized		  );
		const auto to_camera_up_normalized    = Math::Cross( to_target_normalized, to_camera_right_normalized );

		/* We have constructed a right-handed basis, but it is rotated by 180.0 degrees compared to the default basis. Keeping the Y axis the same, negating the X & Z axes will revert this rotation. */

		return Matrix::Translation( -camera_position ) * Matrix4x4
		{
			{ // This is the TRANSPOSED rotation matrix, utilizing the basis constructed above.
				-to_camera_right_normalized.X(),	to_camera_up_normalized.X(),	-to_target_normalized.X(),	0.0f,
				-to_camera_right_normalized.Y(),	to_camera_up_normalized.Y(),	-to_target_normalized.Y(),	0.0f,
				-to_camera_right_normalized.Z(),	to_camera_up_normalized.Z(),	-to_target_normalized.Z(),	0.0f,
				 0.0f,								 0.0f,							 0.0f,						1.0f
			}
		};
	}
};
