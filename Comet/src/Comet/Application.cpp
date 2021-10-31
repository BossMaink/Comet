#include "cmpch.h"

#include "Application.h"

#include "Comet/Events/ApplicationEvent.h"
#include "Comet/Log.h"

namespace Comet 
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		CM_TRACE(e);

		while(true);
	}
}