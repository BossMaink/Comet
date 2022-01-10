#pragma once

#include "Core.h"
#include "Window.h"
#include "Comet/LayerStack.h"
#include "Comet/Events/Event.h"
#include "Comet/Events/ApplicationEvent.h"

namespace Comet {
	class COMET_API Application
	{
	public:
		Application();

		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(Event& e);

		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}



