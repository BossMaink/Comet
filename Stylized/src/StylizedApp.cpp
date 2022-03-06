#include <Comet.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{


		std::string vertexShaderSource = R"(
			#version 410 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aColor;

			uniform mat4 u_ViewProjection;

			out vec3 outColor;
			void main()
			{
				gl_Position = u_ViewProjection * vec4(aPos, 1.0f);
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

		m_Shader.reset(new Comet::Shader(vertexShaderSource, FragmentShaderSource));
		m_VertexArray.reset(Comet::VertexArray::Create());

		float vertices[] = {
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Comet::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Comet::VertexBuffer::Create(vertices, sizeof(vertices)));


		Comet::BufferLayout layout = {
			{Comet::ShaderDataType::Float3, "a_Position"},
			{Comet::ShaderDataType::Float3, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] = {
			0, 1, 2
		};
		std::shared_ptr<Comet::IndexBuffer> indexBuffer;
		indexBuffer.reset(Comet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void OnUpdate(Comet::Timestep ts) override
	{
		if (Comet::Input::IsKeyPressed(CM_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Comet::Input::IsKeyPressed(CM_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Comet::Input::IsKeyPressed(CM_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Comet::Input::IsKeyPressed(CM_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Comet::Input::IsKeyPressed(CM_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Comet::Input::IsKeyPressed(CM_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Comet::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		Comet::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Comet::Renderer::BeginScene(m_Camera);
		Comet::Renderer::Submit(m_Shader, m_VertexArray);
		Comet::Renderer::EndScene();
	}

	void OnEvent(Comet::Event& e) override
	{
		//if (e.GetEventType() == Comet::EventType::KeyPressed)
		//{
		//	Comet::KeyPressedEvent& KeyPressedEvent = (Comet::KeyPressedEvent&) e;
		//	if (KeyPressedEvent.GetKeyCode() == CM_KEY_TAB)
		//	{
		//		CM_TRACE("Tab Key is pressed (Event)");
		//	}
		//	CM_TRACE("{0}", (char)KeyPressedEvent.GetKeyCode());
		//}
	}

	virtual void OnImGuiRender() override
	{

	}

private:
	std::shared_ptr<Comet::Shader> m_Shader;
	std::shared_ptr<Comet::VertexArray> m_VertexArray;

	std::shared_ptr<Comet::Shader> m_BlueShader;
	std::shared_ptr<Comet::VertexArray> m_SquareVA;

	Comet::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.f;

	float m_CameraRotation = 0.f;
	float m_CameraRotationSpeed = 180.f;
};

class Stylized : public Comet::Application
{
public:
	Stylized()
	{
		PushLayer(new ExampleLayer());
	}

	virtual ~Stylized()
	{

	}
};

Comet::Application* Comet::CreateApplication()
{
	return new Stylized();
}