#include "cmpch.h"

#include "Application.h"

#include "Comet/Events/ApplicationEvent.h"

#include "Comet/Renderer/Renderer.h"

#include "Input.h"

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

		std::string vertexShaderSource = R"(
			#version 410 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;

			out vec3 outColor;
			void main()
			{
				gl_Position = vec4(aPos, 1.0f);
				outColor = aColor;
			}
		)";

		std::string FragmentShaderSource = R"(
			#version 410 core

			out vec4 FragColor;
			in vec3 outColor;

			void main()
			{
				FragColor = vec4(outColor, 1.0f);
			} 
		)";

		m_Shader.reset(new Shader(vertexShaderSource, FragmentShaderSource));
		m_VertexArray.reset(VertexArray::Create());

		float vertices[] = {
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] = {
			0, 1, 2
		};
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
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

		m_Shader->Bind();
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene();
			m_VertexArray->Bind();
			Renderer::Submit(m_VertexArray);
			Renderer::EndScene();
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			//for (Layer* layer: m_LayerStack)
			//{
			//	layer->OnUpdate();
			//}

			//m_ImGuiLayer->Begin();
			//for (Layer* layer : m_LayerStack)
			//	layer->OnImGuiRender();
			//m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}


}