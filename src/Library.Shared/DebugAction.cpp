#include "pch.h"
#include "DebugAction.h"

namespace Library
{
	DebugAction::DebugAction()
	{
		InitializeScope();
	}
	
	
	DebugAction::~DebugAction()
	{
	}

	void DebugAction::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("Message");
		}
		AddExternalPrescribedAttribute("Message", mMessage, 1);
	}

	void DebugAction::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("Message", mMessage, 1);
	}

	void DebugAction::Update(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
	}
}
