#pragma once

#include "Core.h"

namespace Comet {
	class COMET_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}



