#pragma once

// std Includes.
#include <concepts>

namespace Framework::Utility
{
	template< std::unsigned_integral Type >
	int DigitCount( Type number )
	{
		int digits = 0;
		if( number < 0 ) // Remove this line if block if '-' counts as a digit.
			digits = 1; 

		while( number )
		{
			number /= 10;
			digits++;
		}
		return digits;
	}

	template< auto Start, auto End, auto Increment, class Func >
	constexpr void constexpr_for( Func&& f )
	{
		if constexpr( Start < End )
		{
			f( std::integral_constant< decltype( Start ), Start >() );
			constexpr_for< Start + Increment, End, Increment >( f );
		}
	}
}
