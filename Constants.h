#pragma once

// std Includes.
#include <numbers>

namespace Framework::Constants
{
	constexpr float PI         = std::numbers::pi_v< float >;
	constexpr float TWO_PI     = PI * 2.0f;
	constexpr float DEG_TO_RAD = PI / 180.0f;
	constexpr float RAD_TO_DEG = 180.0f / PI;

	template< std::floating_point T >
	constexpr T Epsilon()
	{
		if constexpr( std::is_same_v< T, float > )
			return 10e-5f;
		else
			return 10e-15;
	}

	template< std::floating_point T >
	constexpr T TwoEpsilon() { return 2 * Epsilon< T >(); }
}
