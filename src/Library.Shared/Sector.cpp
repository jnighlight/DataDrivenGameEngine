#include "pch.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"
#include "Factory.h"
#include "Entity.h"

namespace Library {
	const std::string Sector::sEntityLabel = "Entities";

	Sector::Sector()
	{
		InitializeScope();
		mEntities = Find(Sector::sEntityLabel);
	}

	Sector::~Sector()
	{
	}

	Sector::Sector(const Sector& rhs)
		:Attributed(rhs)
		,mName(rhs.mName)
		,mCurrentWorld(nullptr)
	{
		SectorCopySemantics();
	}

	Sector& Sector::operator=(const Sector& rhs)
	{
		Attributed::operator=(rhs);
		mName = rhs.mName;

		SectorCopySemantics();
		return *this;
	}

	void Sector::SectorCopySemantics()
	{
		UpdateExternalAttributes();
		mName.append("Copy");
		mEntities = Find(Sector::sEntityLabel);
		assert(mEntities != nullptr); //We shouldn't end up without an entity table
	}

	Sector::Sector(Sector&& rhs)
		:Attributed(std::move(rhs))
		,mName(rhs.mName)
		,mCurrentWorld(rhs.mCurrentWorld)
	{
		SectorMoveSemantics(rhs);
	}

	Sector& Sector::operator=(Sector&& rhs)
	{
		Attributed::operator=(std::move(rhs));
		mName = rhs.mName;
		mCurrentWorld = rhs.mCurrentWorld;

		SectorMoveSemantics(rhs);

		return *this;
	}

	void Sector::SectorMoveSemantics(Sector& rhs)
	{
		UpdateExternalAttributes();
		mEntities = Find(Sector::sEntityLabel);
		assert(mEntities != nullptr); //We shouldn't end up without an entity table

		rhs.mCurrentWorld = nullptr;
		rhs.mEntities = nullptr;
	}

	void Sector::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("Name");
		}
		AddExternalPrescribedAttribute("Name", mName, 1);
		AddInternalPrescribedAttributeScope(Sector::sEntityLabel, 0);
	}

	void Sector::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("Name", mName, 1);
	}

	void Sector::SetWorld(World * world)
	{
		mCurrentWorld = world;
		world->Adopt(*this, World::sSectorLabel);
	}

	Entity & Sector::CreateEntity(const std::string & entityType, const std::string & entityName)
	{
		assert(mEntities != nullptr);
		Entity* newEntity = Factory<Entity>::Create(entityType);
		newEntity->SetName(entityName);
		newEntity->SetSector(this);
		return *newEntity;
	}

	void Sector::Update(WorldState & worldState)
	{
		for (uint32_t i = 0; i < mEntities->Size(); ++i) {
			Scope* curEntityRTTI = mEntities->Get<Scope*>(i);
			assert(curEntityRTTI->Is(Entity::TypeIdClass()));
			Entity* curEntity = curEntityRTTI->As<Entity>();
			curEntity->Update(worldState);
		}
	}
}