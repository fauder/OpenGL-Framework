#pragma once

#ifdef _DEBUG
#define CONSTEXPR_ON_RELEASE
#else
#define CONSTEXPR_ON_RELEASE constexpr
#endif // _DEBUG