#include "pch.h"
#include <cassert>
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{		
	using namespace Library;
	TEST_CLASS(JsonParseMasterTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:

		TEST_METHOD_INITIALIZE(setup)
		{
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
				//Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
		
		TEST_METHOD(Default_Functionality)
		{
			Scope scope;
			JsonParseMaster test;
			DatumParseHelper datumHelper;
			DatumSharedData datumSharedData;
			datumSharedData.SetScope(&scope);

			test.SetSharedData(&datumSharedData);

			test.AddHelper(&datumHelper);
			test.ParseFromFile("../../../JsonParseMasterTestJson.json");

			Library::JsonParseMaster::SharedData* inSharedData = test.GetSharedData();
			Assert::IsTrue(inSharedData->Is(DatumSharedData::TypeIdClass()));
			Logger::WriteMessage(scope.ToString().c_str());
		}

		/*
		TEST_METHOD(ParsesInts)
		{
			Scope scope;
			JsonParseMaster test;
			DatumParseHelper datumHelper;
			TableSharedData datumSharedData;
			datumSharedData.entity = &scope;
			datumSharedData.isParsingEntity = true;

			test.SetSharedData(&datumSharedData);

			test.AddHelper(&datumHelper);
			test.ParseFromFile("../../../JsonParseMasterTestJson.json");

			Library::JsonParseMaster::SharedData* inSharedData = test.GetSharedData();
			Assert::IsTrue(inSharedData->Is(DatumSharedData::TypeIdClass()));
			Datum* health = scope.Find("Health");
			Assert::IsFalse(nullptr == health);
			Assert::IsTrue(health->Type() == Datum::DatumType::E_INTEGER);
			Assert::AreEqual(10, health->Get<int32_t>());
		}


		TEST_METHOD(ParsesVectors)
		{
			Scope scope;
			JsonParseMaster test;
			DatumParseHelper datumHelper;
			DatumSharedData datumSharedData;
			datumSharedData.SetScope(&scope);

			test.SetSharedData(&datumSharedData);

			test.AddHelper(&datumHelper);
			test.ParseFromFile("../../../JsonParseMasterTestJson.json");

			Library::JsonParseMaster::SharedData* inSharedData = test.GetSharedData();
			Assert::IsTrue(inSharedData->Is(DatumSharedData::TypeIdClass()));
			Datum* location = scope.Find("StartLocation");
			Assert::IsFalse(nullptr == location);
			Assert::IsTrue(location->Type() == Datum::DatumType::E_VECTOR);
			glm::vec4 locationVec = location->Get<glm::vec4>();
			Assert::AreEqual(1.1f, locationVec.x);
			Assert::AreEqual(2.2f, locationVec.y);
			Assert::AreEqual(3.3f, locationVec.z);
			Assert::AreEqual(4.4f, locationVec.w);
		}*/

		TEST_METHOD(RemoveHelper)
		{
			Scope scope;
			JsonParseMaster test;
			DatumParseHelper datumHelper;
			DatumSharedData datumSharedData;
			datumSharedData.SetScope(&scope);
			test.SetSharedData(&datumSharedData);
			test.AddHelper(&datumHelper);
			test.RemoveHelper(&datumHelper);
		}

		/*
		TEST_METHOD(CloneWorks)
		{
			Scope scope;
			JsonParseMaster test;
			DatumParseHelper datumHelper;
			DatumSharedData datumSharedData;
			datumSharedData.SetScope(&scope);
			test.SetSharedData(&datumSharedData);
			test.AddHelper(&datumHelper);

			JsonParseMaster* cloneTest = test.Clone();
			DatumSharedData* internalSharedData = static_cast<DatumSharedData*>(cloneTest->GetSharedData());
			internalSharedData->SetScope(&scope);
			cloneTest->ParseFromFile("../../../JsonParseMasterTestJson.json");
			delete cloneTest;
		}*/
	};

	_CrtMemState JsonParseMasterTest::sStartMemState;
}
