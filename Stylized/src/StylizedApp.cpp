#include <Comet.h>

class ExampleLayer : public Comet::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		CM_INFO("ExampleLayer Update");
	}

	void OnEvent(Comet::Event& e) override
	{
		CM_TRACE("{0}", e);
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