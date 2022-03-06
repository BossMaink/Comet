#include "cmpch.h"

#include "Application.h"

#include "Comet/Events/ApplicationEvent.h"

#include "Comet/Renderer/Renderer.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Comet 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		CM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, this));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose, this));
		//CM_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	bool Application::OnWindowClose(Event& e)
	{
		m_Running = false;
		return true;
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		CM_TRACE(e);

		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFramTime;
			m_LastFramTime = time;

			for (Layer* layer: m_LayerStack)
			{
				layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}


}