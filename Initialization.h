#pragma once

namespace Framework
{
	namespace Initialization
	{
		struct InitializeZero {};
		struct NoInitialization {};
	}

	static constexpr Initialization::InitializeZero INITIALIZE_ZERO;
	static constexpr Initialization::NoInitialization NO_INITIALIZATION;
}