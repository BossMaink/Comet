#pragma once

#include "Comet/Layer.h"

namespace Comet {
	class COMET_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;

	private:
		float m_Time = 0.f;
	};
}