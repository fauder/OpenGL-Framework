#pragma once

// std Includes.
#include <concepts>

namespace Framework::Concepts
{
	template< typename T   > concept Arithmetic = std::integral< T > || std::floating_point< T >;

	template< size_t value > concept Nonzero    = ( value != 0 );
}