#include <Comet.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
			uniform mat4 u_Transform;

			out vec3 outColor;
			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0f);
				outColor = aColor;
			}
		)";

		std::string FragmentShaderSource = R"(
			#version 410 core

			out vec4 FragColor;
			in vec3 outColor;

			uniform vec3 u_Color;

			void main()
			{
				FragColor = vec4(u_Color, 1.0f);
			} 
		)";

		m_Shader = Comet::Shader::Create("VertexPosColor", vertexShaderSource, FragmentShaderSource);
		m_VertexArray.reset(Comet::VertexArray::Create());

		auto TextureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");
		m_TextureVA.reset(Comet::VertexArray::Create());

		m_Texture = Comet::Texture2D::Create("assets/textures/tile.jpg");
		m_LogoTexture = Comet::Texture2D::Create("assets/textures/logo.png");

		std::dynamic_pointer_cast<Comet::OpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<Comet::OpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Comet::Ref<Comet::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Comet::VertexBuffer::Create(vertices, sizeof(vertices)));


		Comet::BufferLayout layout = {
			{Comet::ShaderDataType::Float3, "a_Position"},
			{Comet::ShaderDataType::Float2, "a_TexCoord"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_TextureVA->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};
		Comet::Ref<Comet::IndexBuffer> indexBuffer;
		indexBuffer.reset(Comet::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		m_TextureVA->SetIndexBuffer(indexBuffer);

		std::dynamic_pointer_cast<Comet::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Comet::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_SquareColor);
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
		//Comet::Renderer::Submit(m_Shader, m_VertexArray);

		glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));


		std::dynamic_pointer_cast<Comet::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Comet::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_SquareColor);
	

		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.f), pos) * scale;
				Comet::Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("TextureShader");

		m_Texture->Bind();
		Comet::Renderer::Submit(textureShader, m_TextureVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_LogoTexture->Bind();
		Comet::Renderer::Submit(textureShader, m_TextureVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

private:
	Comet::ShaderLibrary m_ShaderLibrary;
	Comet::Ref<Comet::Shader> m_Shader;
	Comet::Ref<Comet::VertexArray> m_VertexArray;

	Comet::Ref<Comet::VertexArray> m_TextureVA;

	Comet::Ref<Comet::Texture2D> m_Texture;
	Comet::Ref<Comet::Texture2D> m_LogoTexture;

	Comet::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.f;

	float m_CameraRotation = 0.f;
	float m_CameraRotationSpeed = 180.f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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