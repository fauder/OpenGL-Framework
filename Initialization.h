#pragma once

namespace Framework
{
	namespace Initialization
	{
		struct ZeroInitialization {};
		struct NoInitialization {};
	}

	static constexpr Initialization::ZeroInitialization ZERO_INITIALIZATION;
	static constexpr Initialization::NoInitialization   NO_INITIALIZATION;
}