#pragma once

// std Includes.
#include <numbers>

namespace Framework::Constants
{
	constexpr float PI         = std::numbers::pi_v< float >;
	constexpr float DEG_TO_RAD = PI / 180.0f;
	constexpr float RAD_TO_DEG = 180.0f / PI;
}
