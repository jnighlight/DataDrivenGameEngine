#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class DebugAction : public Action
	{
	public:
		DebugAction();
		~DebugAction();
		void InitializeScope();
		void UpdateExternalAttributes();
		virtual void Update(WorldState& worldState) override;
	private:
		std::string mMessage = "defaultMessage";
	};
	CONCRETE_FACTORY(DebugAction, Action);
}

