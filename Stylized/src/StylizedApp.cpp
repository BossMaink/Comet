#include <Comet.h>

class Stylized : public Comet::Application
{
public:
	Stylized()
	{

	}

	virtual ~Stylized()
	{

	}
};

Comet::Application* Comet::CreateApplication()
{
	return new Stylized();
}