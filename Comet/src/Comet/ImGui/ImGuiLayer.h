#pragma once

#include "Comet/Layer.h"

namespace Comet {
	class COMET_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void Begin();
		void End();
	private:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		
		virtual void OnImGuiRender() override;

	private:
		float m_Time = 0.f;
	};
}