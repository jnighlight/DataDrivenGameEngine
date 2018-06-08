#include "pch.h"
#include <cassert>
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"
#include "ActionParseHelper.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "DebugAction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Library {
	class FooEntity : public Entity
	{
	public:
		virtual FooEntity::~FooEntity() {};
		/*
		void FooEntity::Update(WorldState& worldState) override {
			UNREFERENCED_PARAMETER(worldState);
		}*/
	};
	CONCRETE_FACTORY(FooEntity, Entity)
}

namespace UnitTestLibraryDesktop
{		
	using namespace Library;
	TEST_CLASS(ActionParseHelperTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:

		Library::FooEntityFactory* fooEntFact;
		Library::DebugActionFactory* debugActionFact;
		Library::ActionListFactory* actionListFact;
		Library::ActionListIfFactory* actionListIfFact;
		Library::ActionCreateActionFactory* actionCreateActionFact;
		Library::ActionDestroyActionFactory* actionDestroyActionFact;

		TEST_METHOD_INITIALIZE(setup)
		{
			fooEntFact = new Library::FooEntityFactory();
			debugActionFact = new Library::DebugActionFactory();
			actionListFact = new Library::ActionListFactory();
			actionListIfFact = new Library::ActionListIfFactory();
			actionCreateActionFact = new Library::ActionCreateActionFactory();
			actionDestroyActionFact = new Library::ActionDestroyActionFactory();
			Library::Factory<Library::Entity>::Add(fooEntFact);
			Library::Factory<Library::Action>::Add(debugActionFact);
			Library::Factory<Library::Action>::Add(actionListFact);
			Library::Factory<Library::Action>::Add(actionListIfFact);
			Library::Factory<Library::Action>::Add(actionCreateActionFact);
			Library::Factory<Library::Action>::Add(actionDestroyActionFact);
			{
				Library::WorldState test1;
				Library::World test2;
				Library::Sector test3;
				Library::Entity test4;
				Library::ActionList test5;
				Library::DebugAction test6;
				Library::ActionListIf test7;
				Library::ActionCreateAction test8;
				Library::ActionDestroyAction test9;
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
			Library::Factory<Library::Action>::Remove(debugActionFact);
			Library::Factory<Library::Action>::Remove(actionListFact);
			Library::Factory<Library::Action>::Remove(actionListIfFact);
			Library::Factory<Library::Action>::Remove(actionCreateActionFact);
			Library::Factory<Library::Action>::Remove(actionDestroyActionFact);
			delete fooEntFact;
			delete debugActionFact;
			delete actionListFact;
			delete actionListIfFact;
			delete actionCreateActionFact;
			delete actionDestroyActionFact;
#endif
		}
		
		TEST_METHOD(Default_Functionality)
		{
			World world;
			JsonParseMaster test;
			TableParseHelper tableHelper;
			DatumParseHelper datumHelper;
			ActionParseHelper actionHelper;
			TableSharedData datumSharedData;
			datumSharedData.world = &world;

			test.SetSharedData(&datumSharedData);

			test.AddHelper(&tableHelper);
			test.AddHelper(&datumHelper);
			test.AddHelper(&actionHelper);
			test.ParseFromFile("../../../TableParseHelper.json");


			/*
			Datum& sectorRef = world.GetSectors();
			Scope* sectorScope = sectorRef.Get<Scope*>();
			Assert::IsTrue(sectorScope->Is(Sector::TypeName()));
			Sector* sector = sectorScope->As<Sector>();

			Datum& entityRef = sector->GetEntities();
			Scope* entityScope = entityRef.Get<Scope*>();
			Assert::IsTrue(entityScope->Is(Entity::TypeName()));
			Entity* entity = entityScope->As<Entity>();
			Assert::IsTrue(entity->Is(FooEntity::TypeName()));
			FooEntity* fooEntity = entityScope->As<FooEntity>();
			Assert::AreEqual("Entity1", fooEntity->GetName().c_str());
			*/
			Logger::WriteMessage(world.ToString().c_str());
			WorldState worldState;
			world.Update(worldState);
			Logger::WriteMessage(world.ToString().c_str());
		}

		/*
		TEST_METHOD(RemoveHelper)
		{
			World world;
			JsonParseMaster test;
			TableParseHelper datumHelper;
			TableSharedData datumSharedData;
			datumSharedData.world = &world;
			test.SetSharedData(&datumSharedData);
			test.AddHelper(&datumHelper);
			test.RemoveHelper(&datumHelper);
		}
		*/
	};

	_CrtMemState ActionParseHelperTest::sStartMemState;
}
