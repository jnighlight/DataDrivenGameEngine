#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Library {
	class FooEntity : public Entity
	{
	public:
		virtual FooEntity::~FooEntity() {};
		void FooEntity::Update(WorldState& worldState) override {
			UNREFERENCED_PARAMETER(worldState);
		}
	};
	CONCRETE_FACTORY(FooEntity, Entity)
}

namespace UnitTestLibraryDesktop
{		
	using namespace Library;
	TEST_CLASS(WorldScopeEntityTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:

		std::string mEntityName;
		std::string mSectorName;
		std::string mWorldName;
		Library::FooEntityFactory* fooEntFact;

		TEST_METHOD_INITIALIZE(setup)
		{
			mEntityName = "entity";
			mSectorName = "sector";
			mWorldName = "world";
			fooEntFact = new Library::FooEntityFactory();
			Library::Factory<Library::Entity>::Add(fooEntFact);
			{
				Library::WorldState test1;
				Library::World test2;
				Library::Sector test3;
				Library::Entity test4;
			}
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}


		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
			Library::Factory<Library::Entity>::Remove(fooEntFact);
			delete fooEntFact;
#endif
		}

		TEST_METHOD(Entity_Constructor)
		{
			Entity entity;
		}

		TEST_METHOD(Entity_Name)
		{
			Entity entity;
			entity.SetName(mEntityName);
			Assert::AreEqual(mEntityName, entity.GetName());
		}

		TEST_METHOD(Entity_Sector)
		{
			Sector sector;
			Entity* entityThing = new Entity();
			Assert::IsNull(entityThing->GetSector());
			entityThing->SetSector(&sector);
			Assert::IsNotNull(entityThing->GetSector());
			Assert::IsTrue(sector == *entityThing->GetSector());
		}

		//TODO: This is a noop for now, so...just covering it
		TEST_METHOD(Entity_Update)
		{
			Entity entity;
			WorldState worldState;
			entity.Update(worldState);
		}

		TEST_METHOD(Entity_Copy)
		{
			std::string entityNameCopy = mEntityName;
			entityNameCopy.append("Copy");

			Entity entity;
			entity.SetName(mEntityName);
			Assert::AreEqual(mEntityName, entity.GetName());
			Entity entityCreateCopy = entity;
			Assert::AreEqual(entityNameCopy, entityCreateCopy.GetName());
			Entity entityOverwriteCopy;
			entityOverwriteCopy = entity;
			Assert::AreEqual(entityNameCopy, entityOverwriteCopy.GetName());
		}

		TEST_METHOD(Entity_Move)
		{
			Entity entity;
			entity.SetName(mEntityName);
			Entity entity2;
			entity2.SetName(mEntityName);

			Assert::AreEqual(mEntityName, entity.GetName());
			Entity entityCreateMove = std::move(entity);
			Assert::AreEqual(mEntityName, entityCreateMove.GetName());
			Entity entityOverwriteMove;
			entityOverwriteMove = std::move(entity);
			Assert::AreEqual(mEntityName, entityOverwriteMove.GetName());
		}

		TEST_METHOD(Entity_Move_InSector)
		{
			Sector sector;
			Entity* entity = new Entity();
			entity->SetName(mEntityName);
			entity->SetSector(&sector);

			Entity* entity2= new Entity();
			entity2->SetName(mEntityName);
			entity2->SetSector(&sector);

			Assert::AreEqual(mEntityName, entity->GetName());
			Entity* entityCreateMove = new Entity(std::move(*entity));
			Assert::AreEqual(mEntityName, entityCreateMove->GetName());

			Entity* entityOverwriteMove = new Entity();
			(*entityOverwriteMove) = std::move(*entity2);
			Assert::AreEqual(mEntityName, entityOverwriteMove->GetName());

			delete entity;
			delete entity2;
		}

		TEST_METHOD(Sector_Copy) {
			std::string sectorNameCopy = mSectorName;
			sectorNameCopy.append("Copy");

			Sector sector;
			sector.SetName(mSectorName);
			Assert::AreEqual(mSectorName, sector.GetName());
			Sector sectorCreateCopy = sector;
			Assert::AreEqual(sectorNameCopy, sectorCreateCopy.GetName());
			Sector sectorOverwriteCopy;
			sectorOverwriteCopy = sector;
			Assert::AreEqual(sectorNameCopy, sectorOverwriteCopy.GetName());
		}

		TEST_METHOD(Sector_Move)
		{
			Sector sector;
			sector.SetName(mSectorName);
			Sector sector2;
			sector2.SetName(mSectorName);

			Assert::AreEqual(mSectorName, sector.GetName());
			Sector sectorCreateMove = std::move(sector);
			Assert::AreEqual(mSectorName, sectorCreateMove.GetName());
			Sector sectorOverwriteMove;
			sectorOverwriteMove = std::move(sector);
			Assert::AreEqual(mSectorName, sectorOverwriteMove.GetName());
		}

		TEST_METHOD(Sector_Move_InSector)
		{
			World world;
			Sector* sector = new Sector();
			sector->SetName(mSectorName);
			sector->SetWorld(&world);

			Sector* sector2= new Sector();
			sector2->SetName(mSectorName);
			sector2->SetWorld(&world);

			Assert::AreEqual(mSectorName, sector->GetName());
			Sector* sectorCreateMove = new Sector(std::move(*sector));
			Assert::AreEqual(mSectorName, sectorCreateMove->GetName());

			Sector* sectorOverwriteMove = new Sector();
			(*sectorOverwriteMove) = std::move(*sector2);
			Assert::AreEqual(mSectorName, sectorOverwriteMove->GetName());

			delete sector;
			delete sector2;
		}

		TEST_METHOD(Sector_Update)
		{
			Sector sector;
			sector.SetName("sector");
			sector.CreateEntity("FooEntity", "Entity1");
			WorldState worldState;
			sector.Update(worldState); //This doesn't do anything yet, so basically just checking it runs right
		}
		
		TEST_METHOD(World_Constructor)
		{
			World world;
			Datum& defaultSectors = world.GetSectors();
			Assert::AreEqual(0u, defaultSectors.Size());
		}

		TEST_METHOD(World_Name)
		{
			World world;
			std::string worldName = "worldName";
			world.SetName(worldName);
			Assert::AreEqual(worldName, world.GetName());
		}

		TEST_METHOD(World_CreateSectors)
		{
			World world;
			std::string sectorName = "newSector";
			Sector& defaultSector = world.CreateSector(sectorName);
			Assert::AreEqual(sectorName, defaultSector.GetName());

			Datum& defaultSectors = world.GetSectors();
			Assert::AreEqual(1u, defaultSectors.Size());
		}

		TEST_METHOD(World_CopyConstructors)
		{
			World world;
			world.SetName("bob");
			world.CreateSector("Sector1");
			World worldCopy = world;

			Datum& defaultSectorsCopy = worldCopy.GetSectors();
			Assert::AreEqual(1u, defaultSectorsCopy.Size());

			World worldOverwrite;
			worldOverwrite = world;
			Datum& defaultSectorsOverwrite = worldOverwrite.GetSectors();
			Assert::AreEqual(1u, defaultSectorsOverwrite.Size());
		}

		TEST_METHOD(World_MoveConstructors)
		{
			World world;
			world.SetName("bob");
			world.CreateSector("Sector1");
			World worldCopy = std::move(world);

			World world2;
			world2.SetName("bob");
			world2.CreateSector("Sector1");

			Datum& defaultSectorsCopy = worldCopy.GetSectors();
			Assert::AreEqual(1u, defaultSectorsCopy.Size());

			World worldOverwrite;
			worldOverwrite = std::move(world2);
			Datum& defaultSectorsOverwrite = worldOverwrite.GetSectors();
			Assert::AreEqual(1u, defaultSectorsOverwrite.Size());
		}

		TEST_METHOD(World_Update)
		{
			World world;
			world.SetName("bob");
			world.CreateSector("World1");
			WorldState worldState;
			world.Update(worldState); //This doesn't do anything yet, so basically just checking it runs right
		}
	};

	_CrtMemState WorldScopeEntityTest::sStartMemState;
}
