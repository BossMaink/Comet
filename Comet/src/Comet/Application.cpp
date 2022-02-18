#include "cmpch.h"

#include "Application.h"

#include "Comet/Events/ApplicationEvent.h"

#include "Input.h"

#include <glad/glad.h>

namespace Comet 
{
	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Comet::ShaderDataType::Float:    return GL_FLOAT;
		case Comet::ShaderDataType::Float2:   return GL_FLOAT;
		case Comet::ShaderDataType::Float3:   return GL_FLOAT;
		case Comet::ShaderDataType::Float4:   return GL_FLOAT;
		case Comet::ShaderDataType::Mat3:     return GL_FLOAT;
		case Comet::ShaderDataType::Mat4:     return GL_FLOAT;
		case Comet::ShaderDataType::Int:      return GL_INT;
		case Comet::ShaderDataType::Int2:     return GL_INT;
		case Comet::ShaderDataType::Int3:     return GL_INT;
		case Comet::ShaderDataType::Int4:     return GL_INT;
		case Comet::ShaderDataType::Bool:     return GL_BOOL;
		}

		CM_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[] = {
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float3, "a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			glEnableVertexAttribArray(index);
			++index;
		}

		uint32_t indices[] = {
			0, 1, 2
		};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/ sizeof(uint32_t)));

		m_Shader->Bind();
		while (m_Running)
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
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