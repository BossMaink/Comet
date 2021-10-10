#pragma once

#include "Application.h"

#ifdef CM_PLATFORM_WINDOWS

extern Comet::Application* Comet::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Comet::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif