#pragma once

#include "Application.h"

#ifdef CM_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Comet::Log::Init();
	CM_CORE_WARN("Initialized log");
	CM_INFO("Hello World!");

	auto* app = Comet::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif