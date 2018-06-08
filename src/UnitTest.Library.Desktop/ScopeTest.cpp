#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct DeepCopyScopeTester: public Library::RTTI
{
	private:                                                                             
		static std::uint64_t sRunTimeTypeId;
	public:                                                                              
		typedef RTTI Parent;                                                             
		static std::string TypeName() { return std::string("DeepCopyScopeTester"); }          
		static std::uint64_t TypeIdClass() { return sRunTimeTypeId; }                        
		virtual std::uint64_t TypeIdInstance() const override { return DeepCopyScopeTester::TypeIdClass(); }
		virtual Library::RTTI* QueryInterface(const std::uint64_t id) const override         
		{                                                                                    
			if (id == sRunTimeTypeId)                                                        
				{ return (RTTI*)this; }                                                      
			else                                                                             
				{ return Parent::QueryInterface(id); }                                       
		}                                                                                    
		virtual bool Is(std::uint64_t id) const override                                     
		{                                                                                    
			if (id == sRunTimeTypeId)                                                        
				{ return true; }                                                             
			else                                                                             
				{ return Parent::Is(id); }                                                   
		}                                                                                    
		virtual bool Is(const std::string& name) const override                              
		{                                                                                    
			if (name == TypeName())                                                          
				{ return true; }                                                             
			else                                                                             
				{ return Parent::Is(name); }                                                 
		}                                                                                    

	uint32_t mInt;
	uint32_t* mIntPtr;
	DeepCopyScopeTester::DeepCopyScopeTester()
		:mInt(0)
		,mIntPtr(new uint32_t(0))
	{}

	DeepCopyScopeTester::DeepCopyScopeTester(uint32_t inInt)
		:mInt(inInt)
		,mIntPtr(new uint32_t(0))
	{}

	DeepCopyScopeTester::DeepCopyScopeTester(uint32_t inInt, uint32_t inIntPtrVal)
		:mInt(inInt)
		,mIntPtr(new uint32_t(inIntPtrVal))
	{}

	DeepCopyScopeTester::DeepCopyScopeTester(const DeepCopyScopeTester& rhs)
		:mInt(rhs.mInt)
		,mIntPtr(new uint32_t(*rhs.mIntPtr))
	{}

	bool DeepCopyScopeTester::Equals(const RTTI* rhs) const override
	{
		if (rhs->Is(DeepCopyScopeTester::TypeIdClass())) {
			DeepCopyScopeTester* casted = rhs->As<DeepCopyScopeTester>();
			return (*this) == (*casted);
		}
		return this == rhs;
	}

	DeepCopyScopeTester& DeepCopyScopeTester::operator=(const DeepCopyScopeTester& rhs)
	{
		mInt = rhs.mInt;
		if (mIntPtr != nullptr) {
			delete mIntPtr;
		}
		mIntPtr = new uint32_t(*rhs.mIntPtr);
	}

	bool DeepCopyScopeTester::operator==(const DeepCopyScopeTester& rhs) const
	{
		return (mInt == rhs.mInt) && (*mIntPtr) == (*rhs.mIntPtr);
	}

	DeepCopyScopeTester::~DeepCopyScopeTester()
	{
		if (mIntPtr != nullptr) {
			delete mIntPtr;
			mIntPtr = nullptr;
		}
	}
};

std::uint64_t DeepCopyScopeTester::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&DeepCopyScopeTester::sRunTimeTypeId);

namespace UnitTestLibraryDesktop
{		
	using namespace Library;
	TEST_CLASS(ScopeTest)
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
		
		TEST_METHOD(Default_Constructor)
		{
			Scope* nullScopePtr = nullptr;
			Scope testScope;
			Assert::IsTrue(nullScopePtr == testScope.GetParent());
		}

		TEST_METHOD(Copy_Constructor)
		{
			Scope testScope;
			Scope* nullScopePtr = nullptr;
			Assert::IsTrue(nullScopePtr == testScope.GetParent());
			Datum& intDatum = testScope.Append("Datum");
			intDatum = 10;
			Scope& testScopeThing = testScope.AppendScope("A");
			testScope.AppendScope("A2");
			testScope.AppendScope("A2");
			testScope.AppendScope("A3");
			Scope& testScopeThing2 = testScopeThing.AppendScope("B");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B3");
			testScopeThing2;
			Scope otherTestScope(testScope);
			Assert::IsTrue(otherTestScope == testScope);
			intDatum = 20; //Making sure datum is copied
			Assert::IsFalse(otherTestScope == testScope);

			Assert::IsTrue(nullScopePtr == testScope.GetParent());
			Assert::IsTrue(nullScopePtr == otherTestScope.GetParent());
		}

		TEST_METHOD(Assignment_Constructor)
		{
			Scope* nullScopePtr = nullptr;
			Scope testScope;
			Assert::IsTrue(nullScopePtr == testScope.GetParent());
			Datum& intDatum = testScope.Append("Datum");
			intDatum = 10;
			Scope& testScopeThing = testScope.AppendScope("A");
			testScope.AppendScope("A2");
			testScope.AppendScope("A2");
			testScope.AppendScope("A3");
			Scope& testScopeThing2 = testScopeThing.AppendScope("B");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B3");
			testScopeThing2;
			Scope otherTestScope;
			otherTestScope = testScope;
			Assert::IsTrue(otherTestScope == testScope);
			intDatum = 20; //Making sure datum is copied
			Assert::IsFalse(otherTestScope == testScope);
		}

		TEST_METHOD(Assignment_Constructor_Move)
		{
			Scope* nullScopePtr = nullptr;
			int32_t testInt = 10;
			Scope testScope;
			Assert::IsTrue(nullScopePtr == testScope.GetParent());
			Datum& intDatum = testScope.Append("Datum");
			intDatum.SetStorage(&testInt, 1);
			Scope& testScopeThing = testScope.AppendScope("A");
			testScope.AppendScope("A2");
			testScope.AppendScope("A2");
			testScope.AppendScope("A3");
			Scope& testScopeThing2 = testScopeThing.AppendScope("B");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B3");
			testScopeThing2;
			Scope moveTester = testScope;
			Scope otherTestScope;
			otherTestScope = std::move(testScope);
			Assert::IsTrue(otherTestScope == moveTester);
			Assert::AreEqual(testInt, otherTestScope.Find("Datum")->Get<int32_t>(0));
			testInt = 20; //Making sure moved datum is still referencing correct external
			Assert::AreEqual(testInt, otherTestScope.Find("Datum")->Get<int32_t>(0));
		}

		TEST_METHOD(Constructor_Move)
		{
			Scope* nullScopePtr = nullptr;
			int32_t testInt = 10;
			Scope testScope;
			Assert::IsTrue(nullScopePtr == testScope.GetParent());
			Datum& intDatum = testScope.Append("Datum");
			intDatum.SetStorage(&testInt, 1);
			Scope& testScopeThing = testScope.AppendScope("A");
			testScope.AppendScope("A2");
			testScope.AppendScope("A2");
			testScope.AppendScope("A3");
			Scope& testScopeThing2 = testScopeThing.AppendScope("B");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B2");
			testScopeThing.AppendScope("B3");
			testScopeThing2;
			Scope moveTester = testScope;
			Scope otherTestScope(testScope);
			Assert::IsTrue(otherTestScope == moveTester);
			Assert::AreEqual(testInt, otherTestScope.Find("Datum")->Get<int32_t>(0));
			testInt = 20; //Making sure moved datum is still referencing correct external
			Assert::AreEqual(testInt, otherTestScope.Find("Datum")->Get<int32_t>(0));
		}

		TEST_METHOD(EqualityOperator)
		{
			Scope testScope;
			Assert::IsTrue(testScope == testScope);
			Datum& intDatum = testScope.Append("Datum");
			intDatum = 10;
			Scope otherTestScope(testScope);
			Assert::IsTrue(otherTestScope == testScope);
			Assert::IsFalse(otherTestScope != testScope);
			intDatum = 20;
			Assert::IsFalse(otherTestScope == testScope);
			Assert::IsTrue(otherTestScope != testScope);
			intDatum = 10;
			Assert::IsTrue(otherTestScope == testScope);
			Assert::IsFalse(otherTestScope != testScope);
			Scope& firstScopeAdded = testScope.AppendScope("A2");
			Assert::IsFalse(otherTestScope == testScope);
			Assert::IsTrue(otherTestScope != testScope);
			Scope& secondScopeAdded = otherTestScope.AppendScope("A2");
			Assert::IsTrue(otherTestScope == testScope);
			Assert::IsFalse(otherTestScope != testScope);
			firstScopeAdded.Append("StringDatum") = "Hah";
			Assert::IsFalse(otherTestScope == testScope);
			Assert::IsTrue(otherTestScope != testScope);
			secondScopeAdded.Append("StringDatum") = "Hah";
			Assert::IsTrue(otherTestScope == testScope);
			Assert::IsFalse(otherTestScope != testScope);
		}

		TEST_METHOD(RTTIEqualsMethod)
		{
			Scope testScope;
			Assert::IsTrue(testScope == testScope);
			Datum& intDatum = testScope.Append("Datum");
			intDatum = 10;
			Scope otherTestScope(testScope);
			RTTI* rttiPtr = &otherTestScope;
			Assert::IsTrue(testScope.Equals(rttiPtr));
			intDatum = 20;
			Assert::IsFalse(testScope.Equals(rttiPtr));
			intDatum = 10;
			Assert::IsTrue(testScope.Equals(rttiPtr));
			Scope& firstScopeAdded = testScope.AppendScope("A2");
			Assert::IsFalse(testScope.Equals(rttiPtr));
			Scope& secondScopeAdded = otherTestScope.AppendScope("A2");
			Assert::IsTrue(testScope.Equals(rttiPtr));
			firstScopeAdded.Append("StringDatum") = "Hah";
			Assert::IsFalse(testScope.Equals(rttiPtr));
			secondScopeAdded.Append("StringDatum") = "Hah";
			Assert::IsTrue(testScope.Equals(rttiPtr));

			DeepCopyScopeTester rttiTester;
			Assert::IsFalse(testScope.Equals(&rttiTester));

			Assert::IsFalse(testScope.Equals(nullptr));
		}

		TEST_METHOD(ToString)
		{
			const Scope testScope;
			std::string scopeString("Scope");
			Assert::AreEqual(scopeString, testScope.ToString());

			Scope testScopeNonConst;
			scopeString = "[]";
			Assert::AreEqual(scopeString, testScopeNonConst.ToString());
		}

		TEST_METHOD(BracketOperator_Index)
		{
			int32_t testInt = 10;
			int32_t otherTestInt = 20;
			std::string testString("Boo");
			Scope testScope;
			Datum& key1 = testScope.Append("Key1");
			key1 = testInt;
			Datum& key2 = testScope.Append("Key2");
			key2 = testString;
			Scope& scopeKey3 = testScope.AppendScope("ScopeKey3");
			Datum& key3 = scopeKey3.Append("Key3");
			key3 = otherTestInt;
			Assert::AreEqual(testInt, testScope[0].Get<int32_t>());
			Assert::AreEqual(testString, testScope[1].Get<std::string>());
			Scope* nestedScope = testScope[2].Get<Scope*>();
			Assert::AreEqual(otherTestInt, (*nestedScope)[0].Get<int32_t>());
		}

		TEST_METHOD(BracketOperator_Key)
		{
			int32_t testInt = 10;
			int32_t otherTestInt = 20;
			std::string sKey1("Key1");
			std::string sKey2("Key2");
			std::string sKey3("Key3");
			std::string scopeKey("scopeKey");
			std::string testString("Boo");
			Scope testScope;
			Datum& key1 = testScope.Append(sKey1);
			key1 = testInt;
			Datum& key2 = testScope.Append(sKey2);
			key2 = testString;
			Scope& scopeKey3 = testScope.AppendScope(scopeKey);
			Datum& key3 = scopeKey3.Append(sKey3);
			key3 = otherTestInt;
			Assert::AreEqual(testInt, testScope[sKey1].Get<int32_t>());
			Assert::AreEqual(testString, testScope[sKey2].Get<std::string>());
			Scope* nestedScope = testScope[scopeKey].Get<Scope*>();
			Assert::AreEqual(otherTestInt, (*nestedScope)[sKey3].Get<int32_t>());
		}

		TEST_METHOD(Append_DoubleAppends)
		{
			Scope testScope;
			std::string sKey1("key1");
			Datum& appendedDatum = testScope.Append(sKey1);
			Datum& otherAppendedDatum = testScope.Append(sKey1);
			Assert::IsTrue(&appendedDatum == &otherAppendedDatum);
			appendedDatum = 10;
			Assert::AreEqual(appendedDatum.Get<int32_t>(), appendedDatum.Get<int32_t>());
		}

		TEST_METHOD(Append_CantTakeEmptyString)
		{
			Scope testScope;
			auto CantAppendEmptyString = [this, &testScope]() { testScope.Append(""); };
			Assert::ExpectException<std::exception>(CantAppendEmptyString);
		}

		TEST_METHOD(FindName)
		{
			Scope testScope;
			std::string sKey1("key1");
			std::string sEmptyString("");
			Scope& appendedDatum = testScope.AppendScope(sKey1);
			Assert::AreEqual(sKey1, testScope.FindName(appendedDatum));
			Assert::AreEqual(sEmptyString, testScope.FindName(testScope));//Not owned scope returns empty string
		}

		TEST_METHOD(Find)
		{
			Scope testScope;
			std::string sKey1("key1");
			std::string sEmptyString("");
			std::string sRandomString("NotInScope");
			Datum& appendedDatum = testScope.Append(sKey1);
			Datum* nullDatumPtr = nullptr;
			Assert::IsTrue(&appendedDatum == testScope.Find(sKey1));
			Assert::IsTrue(nullDatumPtr == testScope.Find(sRandomString));
		}

		TEST_METHOD(Search)
		{
			Scope testScope;
			std::string sScopeKey1("scopeKey1");
			std::string sDatumKey1("datumKey1");
			std::string sRandomKey("notInScope");
			Scope& childScope = testScope.AppendScope(sScopeKey1);
			Datum& childDatum = testScope.Append(sDatumKey1);

			Scope* parentScope = nullptr;
			Datum* foundDatum = childScope.Search(sDatumKey1, parentScope);
			Assert::IsTrue(&childDatum == foundDatum);
			Assert::IsTrue(&testScope == parentScope);
			parentScope = nullptr;
			foundDatum = childScope.Search(sRandomKey, parentScope);
			Assert::IsTrue(nullptr == foundDatum);
			Assert::IsTrue(nullptr == parentScope);
		}

		TEST_METHOD(Adopt)
		{
			Scope testScope;
			Scope* scopeToAdopt = new Scope();
			Scope* otherScopeToAdopt = new Scope();
			std::string sScopeKey("scopeKey");

			testScope.Adopt(*scopeToAdopt, sScopeKey);

			Datum* childDatum = testScope.Find(sScopeKey);
			Assert::AreEqual(1u, childDatum->Size());

			testScope.Adopt(*otherScopeToAdopt, sScopeKey);
			Assert::AreEqual(2u, childDatum->Size());
		}

		TEST_METHOD(Orphan)
		{
			Scope testScope;
			std::string sScopeKey("scopeKey");
			testScope.AppendScope(sScopeKey);
			Datum* childDatum = testScope.Find(sScopeKey);
			Scope* childScope = childDatum->Get<Scope*>();
			Assert::AreEqual(1u, childDatum->Size());
			testScope.Orphan(*childScope);
			Assert::AreEqual(0u, childDatum->Size());
			delete(childScope);

			auto cantOrphanSelf = [this, &testScope]() { testScope.Orphan(testScope); };
			Assert::ExpectException<std::exception>(cantOrphanSelf);
		}
	};

	_CrtMemState ScopeTest::sStartMemState;
}
