#pragma once

// std Includes.
#include <type_traits>

namespace Framework::Utility
{
	template< class Type, typename = typename std::enable_if_t< std::is_unsigned_v< Type > > >
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
}
