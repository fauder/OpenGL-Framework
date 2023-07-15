#pragma once

// std Includes.
#include <numbers>

namespace Framework
{
	template< typename T > struct Constants
	{};

	template<> struct Constants< float >
	{
		Constants() = delete;

		static constexpr float Pi()				{ return std::numbers::pi_v< float >;	}
		static constexpr float Half_Pi()		{ return Pi() / 2.0f;					}
		static constexpr float Quarter_Pi()		{ return Pi() / 4.0f;					}
		static constexpr float Two_Pi()			{ return Pi() * 2.0f;					}
		static constexpr float Deg_To_Rad()		{ return Pi() / 180.0f;					}
		static constexpr float Rad_To_Deg()		{ return 180.0f / Pi();					}

		static constexpr float Sqrt_2()			{ return 1.414213562f;					}
		static constexpr float Sqrt_3()			{ return 1.732050808f;					}
		static constexpr float Sqrt_Half()		{ return 0.707106781f;					}
	};

	template<> struct Constants< double >
	{
		Constants() = delete;
	
		static constexpr double Pi()			{ return std::numbers::pi_v< double >;	}
		static constexpr double Half_Pi()		{ return Pi() / 2.0;					}
		static constexpr double Quarter_Pi()	{ return Pi() / 4.0;					}
		static constexpr double Two_Pi()		{ return Pi() * 2.0;					}
		static constexpr double Deg_To_Rad()	{ return Pi() / 180.0;					}
		static constexpr double Rad_To_Deg()	{ return 180.0 / Pi();					}

		static constexpr double Sqrt_2()		{ return 1.4142135623730950;			}
		static constexpr double Sqrt_3()		{ return 1.7320508075688773;			}
		static constexpr double Sqrt_Half()		{ return 0.7071067811865475;			}
	};
}
