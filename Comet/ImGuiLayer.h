#pragma once

#include "Comet/Layer.h"

namespace Comet {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;
		virtual void OnEvent(Event& e) override;
	private:


	};
}
