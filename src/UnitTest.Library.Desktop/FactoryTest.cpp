#include "pch.h"
#include "FactoryTestClass.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{		

	TEST_CLASS(FactoryTest)
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
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Constructor)
		{
			Library::DeepCopyFactoryTestFactory deepFactory;
			Library::FooRTTIFactory fooFactory;
		}
		
		TEST_METHOD(Create_Works)
		{
			Library::DeepCopyFactoryTestFactory deepFactory;
			Library::RTTI* deepCopyTester = deepFactory.Create();
			Assert::IsTrue(deepCopyTester->Is(Library::DeepCopyFactoryTest::TypeName()));
			Assert::AreEqual("DeepCopyFactoryTest", deepFactory.ClassName().c_str());
			delete deepCopyTester;

			Library::FooRTTIFactory fooFactory;
			Library::RTTI* fooTester = fooFactory.Create();
			Assert::IsTrue(fooTester->Is(Library::FooRTTI::TypeName()));
			Assert::AreEqual("FooRTTI", fooFactory.ClassName().c_str());
			delete fooTester;
		}

		TEST_METHOD(FactoryManager_Add_Create_Remove)
		{
			Library::DeepCopyFactoryTestFactory* deepFactory = new Library::DeepCopyFactoryTestFactory;
			Library::FooRTTIFactory* fooFactory = new Library::FooRTTIFactory;
			Library::Factory<Library::RTTI>::Add(deepFactory);
			Library::Factory<Library::RTTI>::Add(fooFactory);

			Library::RTTI* createdTest = Library::Factory<Library::RTTI>::Create(deepFactory->ClassName());
			Assert::IsTrue(createdTest->Is(Library::DeepCopyFactoryTest::TypeName()));
			Assert::IsFalse(createdTest->Is(Library::FooRTTI::TypeName()));
			delete createdTest;

			Library::RTTI* createdFoo = Library::Factory<Library::RTTI>::Create(fooFactory->ClassName());
			Assert::IsTrue(createdFoo->Is(Library::FooRTTI::TypeName()));
			Assert::IsFalse(createdFoo->Is(Library::DeepCopyFactoryTest::TypeName()));
			delete createdFoo;

			Assert::IsTrue(Library::Factory<Library::RTTI>::Remove(deepFactory));
			Assert::IsFalse(Library::Factory<Library::RTTI>::Remove(deepFactory)); //Remove should return false if the value is not present to be removed
			Assert::IsTrue(Library::Factory<Library::RTTI>::Remove(fooFactory));
			delete deepFactory;
			delete fooFactory;
		}

		TEST_METHOD(FactoryManager_CreatesOnlyFromExistingFactories)
		{
			auto CantCreateTypeWithoutFactory = [this]() { Library::Factory<Library::RTTI>::Create("Class that doesn't exist"); };
			Assert::ExpectException<std::exception>(CantCreateTypeWithoutFactory);
		}

		TEST_METHOD(FactoryManager_Find)
		{
			Library::DeepCopyFactoryTestFactory* deepFactory = new Library::DeepCopyFactoryTestFactory;
			Library::Factory<Library::RTTI>::Add(deepFactory);

			Library::Factory<Library::RTTI>* foundDeepFactory = Library::Factory<Library::RTTI>::Find(deepFactory->ClassName());
			Assert::IsTrue(foundDeepFactory == deepFactory);

			Assert::IsTrue(Library::Factory<Library::RTTI>::Remove(deepFactory));
			delete deepFactory;

			//Nonexistant returns nullptr
			Library::Factory<Library::RTTI>* shouldBeNull = Library::Factory<Library::RTTI>::Find("Totally doesn't exist");
			Assert::IsTrue(nullptr == shouldBeNull);
		}

		TEST_METHOD(FactoryManager_iter)
		{
			Library::DeepCopyFactoryTestFactory* deepFactory = new Library::DeepCopyFactoryTestFactory;
			Library::Factory<Library::RTTI>::Add(deepFactory);

			auto iter = Library::Factory<Library::RTTI>::begin();
			Assert::IsFalse(iter == Library::Factory<Library::RTTI>::end());
			++iter;
			Assert::IsTrue(iter == Library::Factory<Library::RTTI>::end());

			Assert::IsTrue(Library::Factory<Library::RTTI>::Remove(deepFactory));
			delete deepFactory;
		}
	};
	_CrtMemState FactoryTest::sStartMemState;
}
