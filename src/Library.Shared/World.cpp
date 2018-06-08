#include "pch.h"
#include "World.h"
#include "Sector.h"

namespace Library {
	const std::string World::sSectorLabel = "Sectors";

	World::World()
	{
		InitializeScope();
		mSectors = Find(World::sSectorLabel);
	}


	World::~World()
	{
	}

	World::World(const World& rhs)
		:Attributed(rhs)
		,mName(rhs.mName)
	{
		WorldCopySemantics();
	}

	World& World::operator=(const World& rhs)
	{
		Attributed::operator=(rhs);
		mName = rhs.mName;
		WorldCopySemantics();
		return *this;
	}

	void World::WorldCopySemantics()
	{
		UpdateExternalAttributes();
		mName.append("Copy");
		mSectors = Find(World::sSectorLabel);
		assert(mSectors != nullptr); //We shouldn't end up without a sector table
	}

	World::World(World&& rhs)
		:Attributed(std::move(rhs))
		,mName(rhs.mName)
	{
		WorldMoveSemantics();
	}

	World& World::operator=(World&& rhs)
	{
		Attributed::operator=(std::move(rhs));
		mName = rhs.mName;

		WorldMoveSemantics();

		return *this;
	}

	void World::WorldMoveSemantics()
	{
		UpdateExternalAttributes();
		mSectors = Find(World::sSectorLabel);
		assert(mSectors != nullptr); //We shouldn't end up without a sector table
	}

	void World::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("Name");
		}
		AddExternalPrescribedAttribute("Name", mName, 1);
		AddInternalPrescribedAttributeScope(World::sSectorLabel, 0);
	}

	void World::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("Name", mName, 1);
	}

	Sector& World::CreateSector(const std::string & sectorName)
	{
		Sector* newSector = new Sector;
		newSector->SetName(sectorName);
		newSector->SetWorld(this);
		return *newSector;
	}

	void World::Update(WorldState & worldState)
	{
		for (uint32_t i = 0; i < mSectors->Size(); ++i) {
			Scope* curSectorRTTI = mSectors->Get<Scope*>(i);
			assert(curSectorRTTI->Is(Sector::TypeIdClass()));
			Sector* curSector = curSectorRTTI->As<Sector>();
			curSector->Update(worldState);
		}
	}
}