#pragma once

#ifdef CM_PLATFORM_WINDOWS
#if CM_DYNAMIC_LINK
	#ifdef CM_BUILD_DLL
		#define COMET_API __declspec(dllexport)
	#else
		#define COMET_API __declspec(dllimport)
	#endif
#else
	#define COMET_API
#endif
#else
	#error Comet only supports Windows!
#endif

#ifdef CM_ENABLE_ASSERTS
	#define CM_ASSERT(x, ...) {if(!(x)) { CM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define CM_CORE_ASSERTS(x, ...) {if(!(x)) {CM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define CM_ASSERT(x, ...)
	#define CM_CORE_ASSERTS(x, ...)
#endif // CM_ENABLE_ASSERTS


#define BIT(X) (1 << X)

#define BIND_EVENT_FN(x, y) std::bind(&x, y, std::placeholders::_1)