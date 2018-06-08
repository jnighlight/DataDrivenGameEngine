#include "pch.h"
#include "ActionDestroyAction.h"
#include "WorldState.h"

namespace Library
{
	ActionDestroyAction::ActionDestroyAction()
	{
		InitializeScope();
	}
	
	ActionDestroyAction::~ActionDestroyAction()
	{
	}

	void ActionDestroyAction::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("ActionName");
		}
		AddExternalPrescribedAttribute("ActionName", mActionName, 1);
	}

	void ActionDestroyAction::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("ActionName", mActionName, 1);
	}

	void ActionDestroyAction::Update(WorldState & worldState)
	{
		worldState.mAction = this;
		Action* foundActionToDestroy = nullptr;
		Scope* curParent = mParentScope;
		while (curParent != nullptr) {
			Datum* curScopeAction = mParentScope->Find("Actions");
			for (uint32_t i = 0; i < curScopeAction->Size(); ++i) {
				Scope* actionScope = curScopeAction->Get<Scope*>(i);
				assert(actionScope->Is(Action::TypeIdClass()));
				Action* curAction = actionScope->As<Action>();
				if (curAction->GetName() == mActionName) {
					foundActionToDestroy = curAction;
					break;
				}
			}
			curParent = curParent->GetParent();
		}
		if (foundActionToDestroy != nullptr) {
			foundActionToDestroy->MarkForDelete();
		}
		worldState.mAction = nullptr;
	}
}
