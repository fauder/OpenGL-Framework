#pragma once

// Project Includes.
#include "MatrixBase.hpp"
#include "Vector.hpp"

namespace Framework::Matrix
{
	/* In row-major form. */ Matrix4x4 OrthographicProjection( const float left_plane_offset,   const float right_plane_offset,
															   const float bottom_plane_offset, const float top_plane_offset,
															   const float near_plane_offset,   const float far_plane_offset );
	/* In row-major form. */ Matrix4x4 PerspectiveProjection( const float near_plane_offset, const float far_plane_offset,
															  const float aspect_ratio,      const float field_of_view_inDegrees );

	/* In row-major form. */ Matrix4x4 Scaling( const float newScale );
	/* In row-major form. */ Matrix4x4 Scaling( const float newScale_x, const float newScale_y, const float newScale_z );
	/* In row-major form. */ Matrix4x4 ScalingOnX( const float newScale );
	/* In row-major form. */ Matrix4x4 ScalingOnY( const float newScale );
	/* In row-major form. */ Matrix4x4 ScalingOnZ( const float newScale );

	/* In row-major form. */ Matrix4x4 GeneralRotation( const Vector3& angles_xyz_inDegrees );
	/* In row-major form. */ Matrix4x4 RotationAroundX( const float angle_inDegrees );
	/* In row-major form. */ Matrix4x4 RotationAroundY( const float angle_inDegrees );
	/* In row-major form. */ Matrix4x4 RotationAroundZ( const float angle_inDegrees );
	/* In row-major form. */ Matrix4x4 RotationAroundAxis( const float angle_inDegrees, Vector3 vector );

	/* In row-major form. */ Matrix4x4 TranslationOnX( const float delta );
	/* In row-major form. */ Matrix4x4 TranslationOnY( const float delta );
	/* In row-major form. */ Matrix4x4 TranslationOnZ( const float delta );
	/* In row-major form. */ Matrix4x4 Translation( const float delta_x, const float delta_y, const float delta_z );
};
