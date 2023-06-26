#pragma once

// std Includes.
#include "Concepts.h"

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
}
