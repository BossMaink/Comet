#pragma once

#ifdef CM_PLATFORM_WINDOWS
	#ifdef CM_BUILD_DLL
		#define COMET_API __declspec(dllexport)
	#else
		#define COMET_API __declspec(dllimport)
	#endif
#else
	#error Comet only supports Windows!
#endif

#define BIT(X) (1 << X)