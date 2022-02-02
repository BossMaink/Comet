#include <Comet.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		if (Comet::Input::IsKeyPressed(CM_KEY_TAB))
		{
			CM_TRACE("Tab Key is pressed (poll)");
		}
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
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}
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