#pragma once

#include "Core.h"
#include "Core/Timestep.h"
#include "Events/Event.h"

namespace Comet
{
	class COMET_API Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_DebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event){}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}

