#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Comet {

	class COMET_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// Core log macros
#define CM_CORE_TRACE(...)    ::Comet::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CM_CORE_INFO(...)     ::Comet::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CM_CORE_WARN(...)     ::Comet::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CM_CORE_ERROR(...)    ::Comet::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CM_CORE_CRITICAL(...) ::Comet::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define CM_TRACE(...)    ::Comet::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CM_INFO(...)     ::Comet::Log::GetClientLogger()->info(__VA_ARGS__)
#define CM_WARN(...)     ::Comet::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CM_ERROR(...)    ::Comet::Log::GetClientLogger()->error(__VA_ARGS__)
#define CM_CRITICAL(...) ::Comet::Log::GetClientLogger()->critical(__VA_ARGS__)
