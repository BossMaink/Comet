#pragma once

#include "Core.h"
#include "Window.h"
#include "Comet/LayerStack.h"
#include "Comet/Events/Event.h"
#include "Comet/Events/ApplicationEvent.h"

#include "Comet/ImGui/ImGuiLayer.h"

#include "Comet/Renderer/Shader.h"
#include "Comet/Renderer/Buffer.h"
#include "Comet/Renderer/VertexArray.h"
#include "Comet/Core/Timestep.h"

#include "Comet/Renderer/OrthographicCamera.h"

namespace Comet {
	class COMET_API Application
	{
	public:
		Application();

		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get(){ return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(Event& e);

	private:
		Scope<Window> m_Window;

		ImGuiLayer* m_ImGuiLayer = nullptr;;

		bool m_Running = true;
		LayerStack m_LayerStack;

		float m_LastFramTime = 0.f;

	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}



