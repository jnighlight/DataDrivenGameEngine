#include "pch.h"
#include "ActionCreateAction.h"
#include "WorldState.h"

namespace Library
{
	ActionCreateAction::ActionCreateAction()
	{
		InitializeScope();
	}
	
	ActionCreateAction::~ActionCreateAction()
	{
	}

	void ActionCreateAction::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("Prototype");
			AddPrescribedAttributeToStaticHash("PrototypeName");
		}
		AddExternalPrescribedAttribute("Prototype", mPrototype, 1);
		AddExternalPrescribedAttribute("PrototypeName", mPrototypeName, 1);
	}

	void ActionCreateAction::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("Prototype", mPrototype, 1);
		UpdateExternalPrescribedAttribute("PrototypeName", mPrototypeName, 1);
	}

	void ActionCreateAction::Update(WorldState & worldState)
	{
		worldState.mAction = this;
		Action* createdAction = Factory<Action>::Create(mPrototype);
		createdAction->SetName(mPrototypeName);
		mParentScope->Adopt(*createdAction, "Actions");
		worldState.mAction = nullptr;
	}
}
