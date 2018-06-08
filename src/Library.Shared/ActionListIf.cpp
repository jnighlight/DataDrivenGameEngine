#include "pch.h"
#include "ActionListIf.h"

namespace Library
{
	ActionListIf::ActionListIf()
	{
		InitializeScope();
	}
	
	
	ActionListIf::~ActionListIf()
	{
	}

	void ActionListIf::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("Condition");
		}
		AddExternalPrescribedAttribute("Condition", mCondition, 1);
	}

	void ActionListIf::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("Condition", mCondition, 1);
	}

	void ActionListIf::Update(WorldState & worldState)
	{
		Scope* parent = nullptr;
		Action* correctAction = nullptr;
		Datum* conditionDatum = Search(mCondition, parent);
		if (conditionDatum != nullptr && conditionDatum->Type() == Datum::DatumType::E_INTEGER) {
			std::string thenOrElse;
			if (conditionDatum->Get<int32_t>() != 0) {
				thenOrElse = "Then";
			} else {
				thenOrElse = "Else";
			}
			Datum* actionDatum = Find("Actions");
			for (uint32_t i = 0; i < actionDatum->Size(); ++i) {
				assert(actionDatum->Get<Scope*>(i)->Is(Action::TypeIdClass()));
				Action* actionIter = actionDatum->Get<Scope*>(i)->As<Action>();
				std::string name = actionIter->GetName();
				if (name == thenOrElse) {
					correctAction = actionIter;
					break;
				}
			}
			if (correctAction != nullptr) {
				correctAction->Update(worldState);
			}
		} else {
			throw std::exception("Conditional datum for if statement does not follow expectations");
		}
	}
}
