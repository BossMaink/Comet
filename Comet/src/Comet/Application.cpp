#include "cmpch.h"

#include "Application.h"

#include "Comet/Events/ApplicationEvent.h"

#include "Input.h"

#include <glad/glad.h>

namespace Comet 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		CM_CORE_ASSERTS(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, this));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
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

		std::string vertexShaderSource = R"(
			#version 410 core
			layout (location = 0) in vec3 aPos;
			void main()
			{
				gl_Position = vec4(aPos, 1);
			}
		)";

		std::string FragmentShaderSource = R"(
			#version 410 core
			out vec4 FragColor;

			void main()
			{
				FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
			} 
		)";

		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* source = (const GLchar*)vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &source, NULL);
		glCompileShader(vertexShader);

		int32_t success = 1;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			CM_CORE_ERROR("{0}", infoLog.data());
			CM_CORE_ASSERTS(false, "Vertex shader compilation failure!");
		}

		uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		source = (const GLchar*)FragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &source, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			CM_CORE_ERROR("{0}", infoLog.data());
			CM_CORE_ASSERTS(false, "Fragment shader compilation failure!");
			return;
		}

		uint32_t shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint maxLength = 0;
			glGetProgramiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(shaderProgram);

			CM_CORE_ERROR("{0}", infoLog.data());
			CM_CORE_ASSERTS(false, "Program link failure!");
			return;
		}
		uint32_t VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		std::vector<float> vertices = {
			0.5f, 0.5f, 0.0f,   // ÓÒÉÏ½Ç
			0.5f, -0.5f, 0.0f,  // ÓÒÏÂ½Ç
			-0.5f, -0.5f, 0.0f, // ×óÏÂ½Ç
			-0.5f, 0.5f, 0.0f   // ×óÉÏ½Ç
		};

		uint32_t indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		uint32_t VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (const void*)&vertices[0], GL_STATIC_DRAW);

		unsigned int EBO;
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (const void*)indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		while (m_Running)
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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