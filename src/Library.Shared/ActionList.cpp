#include "pch.h"
#include "ActionList.h"

namespace Library {
	const std::string ActionList::sActionListLabel = "Actions";

	ActionList::ActionList()
	{
		InitializeScope();
		mActions = Find(ActionList::sActionListLabel);
	}

	ActionList::~ActionList()
	{
	}

	void ActionList::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("Name");
		}
		AddExternalPrescribedAttribute("Name", mName, 1);
		AddInternalPrescribedAttributeScope(ActionList::sActionListLabel, 0);
	}

	void ActionList::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("Name", mName, 1);
	}

	void ActionList::Update(WorldState & worldState)
	{
		for (uint32_t i = 0; i < mActions->Size(); ++i) {
			Scope* curActionRTTI = mActions->Get<Scope*>(i);
			assert(curActionRTTI->Is(Action::TypeIdClass()));
			Action* curEntity = curActionRTTI->As<Action>();
			curEntity->Update(worldState);
		}
	}
}
