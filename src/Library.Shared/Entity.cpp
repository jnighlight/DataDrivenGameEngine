#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "ActionList.h"
#include "Factory.h"

namespace Library {
	const std::string Entity::sActionsLabel = "Actions";
	Entity::Entity()
	{
		InitializeScope();
		mActions = Find(Entity::sActionsLabel);
	}

	Entity::~Entity()
	{
	}

	Entity::Entity(const Entity& rhs)
		:Attributed(rhs)
		,mName(rhs.mName)
		,mCurrentSector(nullptr)
	{
		UpdateExternalAttributes();
		mName.append("Copy");
		mActions = Find(Entity::sActionsLabel);
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		Attributed::operator=(rhs);
		mName = rhs.mName;
		mCurrentSector = nullptr;
		UpdateExternalAttributes();
		mName.append("Copy");
		mActions = Find(Entity::sActionsLabel);
		return *this;
	}

	Entity::Entity(Entity&& rhs)
		:Attributed(std::move(rhs))
		,mName(rhs.mName)
		,mCurrentSector(rhs.mCurrentSector)
	{
		EntityMoveSemantics(rhs);
	}

	Entity& Entity::operator=(Entity&& rhs)
	{
		Attributed::operator=(std::move(rhs));
		mName = rhs.mName;
		EntityMoveSemantics(rhs);
		return *this;
	}

	void Entity::EntityMoveSemantics(Entity& rhs)
	{
		UpdateExternalAttributes();
		mCurrentSector = rhs.mCurrentSector;
		mActions = Find(Entity::sActionsLabel);
	}

	void Entity::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("Name");
			AddPrescribedAttributeToStaticHash(Entity::sActionsLabel);
		}
		AddExternalPrescribedAttribute("Name", mName, 1);
		AddInternalPrescribedAttributeScope(Entity::sActionsLabel, 0);
	}

	void Entity::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("Name", mName, 1);
	}

	void Entity::SetSector(Sector * sector)
	{ 
		mCurrentSector = sector;
		sector->Adopt(*this, Sector::sEntityLabel);
	}

	void Entity::Update(WorldState & worldState)
	{
		for (uint32_t i = 0; i < mActions->Size(); ++i) {
			Scope* curActionRTTI = mActions->Get<Scope*>(i);
			assert(curActionRTTI->Is(Action::TypeIdClass()));
			Action* curAction = curActionRTTI->As<Action>();
			curAction->Update(worldState);
		}
	}
	Action & Entity::CreateAction(const std::string & actionType, const std::string & actionName)
	{
		//TODO: This may be wrong...we may have to shove this into a scope? I'm not sure
		Action* createdAction = Factory<Action>::Create(actionType);
		createdAction->SetName(actionName);
		mActions->PushBack(createdAction);
		return *createdAction;
	}
}
