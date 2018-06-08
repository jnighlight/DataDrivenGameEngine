#pragma once
#include <string>
#include "Action.h"
#include "Factory.h"

namespace Library {
	class Datum;

	class ActionList : public Action
	{
	public:
		ActionList();
		virtual ~ActionList();
		virtual void Update(WorldState& worldState) override;

		static const std::string sActionListLabel;
	private:
		void InitializeScope();
		void UpdateExternalAttributes();
		Datum* mActions;
	};
	CONCRETE_FACTORY(ActionList, Action);
}
