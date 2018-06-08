#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct AttributedTester: public Library::Attributed
{
	int32_t mTestInt;
	float_t mTestFloat;
	glm::vec4 mTestVector;
	glm::mat4 mTestMatrix;
	std::string mTestString;
	Scope* mTestScope;
	RTTI* mTestRTTI = nullptr;

	uint32_t* mIntPtr;

	AttributedTester::AttributedTester()
		:mTestInt(0)
		,mIntPtr(new uint32_t(0))
	{
		InitializeScope();
	}

	AttributedTester::AttributedTester(uint32_t inInt)
		:mTestInt(inInt)
		,mIntPtr(new uint32_t(0))
	{
		InitializeScope();
	}

	AttributedTester::AttributedTester(uint32_t inInt, uint32_t inIntPtrVal)
		:mTestInt(inInt)
		,mIntPtr(new uint32_t(inIntPtrVal))
	{
		InitializeScope();
	}

	AttributedTester::AttributedTester(const AttributedTester& rhs)
		:Attributed(rhs)
		,mTestInt(rhs.mTestInt)
		,mIntPtr(new uint32_t(*rhs.mIntPtr))
		,mTestString(rhs.mTestString)
	{
		UpdateExternalAttributes();
	}

	AttributedTester& AttributedTester::operator=(const AttributedTester& rhs)
	{
		Attributed::operator=(rhs);
		mTestInt = rhs.mTestInt;
		mTestString = rhs.mTestString;
		if (mIntPtr != nullptr) {
			delete mIntPtr;
		}
		mIntPtr = new uint32_t(*rhs.mIntPtr);
		UpdateExternalAttributes();
		return *this;
	}

	AttributedTester::AttributedTester(AttributedTester&& rhs)
		:Attributed(std::move(rhs))
		,mTestInt(rhs.mTestInt)
		,mIntPtr(rhs.mIntPtr)
		,mTestString(rhs.mTestString)
	{
		rhs.mIntPtr = nullptr;
		rhs.mTestInt = 0;
		rhs.mTestString = "";
		UpdateExternalAttributes();
	}

	AttributedTester& AttributedTester::operator=(AttributedTester&& rhs)
	{
		Attributed::operator=(std::move(rhs));
		if (mIntPtr != nullptr) {
			delete mIntPtr;
		}
		mIntPtr = rhs.mIntPtr;
		mTestInt = rhs.mTestInt;
		mTestString = rhs.mTestString;

		rhs.mIntPtr = nullptr;
		rhs.mTestInt = 0;
		rhs.mTestString = "";

		UpdateExternalAttributes();
		return *this;
	}

	void AttributedTester::UpdateNonexistantAttribute()
	{
		UpdateExternalPrescribedAttribute("THINGTHATDOESNTEXIST", mTestInt, 1);
	}

	void AttributedTester::InitializeScope()
	{
		if (!IsInitialized()) {
			AddPrescribedAttributeToStaticHash("TestInt");
			AddPrescribedAttributeToStaticHash("TestString");
			AddPrescribedAttributeToStaticHash("TestInternalInt");
			AddPrescribedAttributeToStaticHash("TestFloat");
			AddPrescribedAttributeToStaticHash("TestVector");
			AddPrescribedAttributeToStaticHash("TestMatrix");
			AddPrescribedAttributeToStaticHash("TestRTTI");

			AddPrescribedAttributeToStaticHash("TestInternalInt");
			AddPrescribedAttributeToStaticHash("TestInternalString");
			AddPrescribedAttributeToStaticHash("TestInternalInternalInt");
			AddPrescribedAttributeToStaticHash("TestInternalFloat");
			AddPrescribedAttributeToStaticHash("TestInternalVector");
			AddPrescribedAttributeToStaticHash("TestInternalMatrix");
			AddPrescribedAttributeToStaticHash("TestInternalRTTI");
			AddPrescribedAttributeToStaticHash("TestInternalScope");
		}
		AddExternalPrescribedAttribute("TestInt", mTestInt, 1);
		AddExternalPrescribedAttribute("TestString", mTestString, 1);
		AddExternalPrescribedAttribute("TestFloat", mTestFloat, 1);
		AddExternalPrescribedAttribute("TestVector", mTestVector, 1);
		AddExternalPrescribedAttribute("TestMatrix", mTestMatrix, 1);
		AddExternalPrescribedAttribute("TestRTTI", mTestRTTI, 1);

		AddInternalPrescribedAttribute("TestInternalInt", 12, 2);
		AddInternalPrescribedAttribute("TestInternalFloat", 12.0f, 2);
		AddInternalPrescribedAttribute("TestInternalString", "Test", 2);
		AddInternalPrescribedAttribute("TestInternalVector", glm::vec4(), 2);
		AddInternalPrescribedAttribute("TestInternalMatrix", glm::mat4(), 2);
		AddInternalPrescribedAttribute("TestInternalRTTI", mTestRTTI, 2);
		AddInternalPrescribedAttributeScope("TestInternalScope", 2);

		Library::Datum& auxDatum = AppendAuxiliaryAttribute("TestAux");
		auxDatum = 25;
	}

	void AttributedTester::UpdateExternalAttributes()
	{
		UpdateExternalPrescribedAttribute("TestInt", mTestInt, 1);
		UpdateExternalPrescribedAttribute("TestString", mTestString, 1);
		UpdateExternalPrescribedAttribute("TestFloat", mTestFloat, 1);
		UpdateExternalPrescribedAttribute("TestVector", mTestVector, 1);
		UpdateExternalPrescribedAttribute("TestMatrix", mTestMatrix, 1);
		UpdateExternalPrescribedAttribute("TestRTTI", mTestRTTI, 1);
	}

	bool AttributedTester::operator==(const AttributedTester& rhs) const
	{
		return (mTestInt == rhs.mTestInt) && (*mIntPtr) == (*rhs.mIntPtr);
	}

	AttributedTester::~AttributedTester()
	{
		if (mIntPtr != nullptr) {
			delete mIntPtr;
			mIntPtr = nullptr;
		}
	}
	RTTI_DECLARATIONS(AttributedTester, Attributed)
};

RTTI_DEFINITIONS(AttributedTester);

namespace UnitTestLibraryDesktop
{		
	using namespace Library;
	TEST_CLASS(AttributedTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:

		TEST_METHOD_INITIALIZE(setup)
		{
			Attributed::ClearHash();
			{
				AttributedTester test; //Creating instance here to populate static hash map 
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
#endif
		}
		
		TEST_METHOD(Default_Constructor)
		{
			AttributedTester tester;
			Datum* testInt = tester.Find("TestInt");
			Assert::AreEqual(tester.mTestInt, testInt->Get<int32_t>());
			tester.mTestInt = 123; //Assert it being a reference
			Assert::AreEqual(tester.mTestInt, testInt->Get<int32_t>());

			Datum* testString = tester.Find("TestString");
			Assert::AreEqual(tester.mTestString, testString->Get<std::string>());
			tester.mTestString = "kms"; //Assert it being a reference
			Assert::AreEqual(tester.mTestString, testString->Get<std::string>());

			Datum* testInternalInt = tester.Find("TestInternalInt");
			Assert::AreEqual(2u, testInternalInt->Size());
			Assert::AreEqual(12, testInternalInt->Get<int32_t>());
			Assert::AreEqual(12, testInternalInt->Get<int32_t>(1));
		}

		TEST_METHOD(Copy_Constructor)
		{
			AttributedTester tester;
			AttributedTester copyTester(tester);

			Datum* testInt = tester.Find("TestInt");
			Datum* copyTestInt = copyTester.Find("TestInt");
			Assert::AreEqual(testInt->Get<int32_t>(), copyTestInt->Get<int32_t>());
			tester.mTestInt = 123;
			Assert::AreNotEqual(testInt->Get<int32_t>(), copyTestInt->Get<int32_t>());

			Datum* testString = tester.Find("TestString");
			Datum* copyTestString = copyTester.Find("TestString");
			Assert::AreEqual(testString->Get<std::string>(), copyTestString->Get<std::string>());
			tester.mTestString = "hah";
			Assert::AreNotEqual(testString->Get<std::string>(), copyTestString->Get<std::string>());

			Datum* testThis = tester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&tester) == testThis->Get<RTTI*>());

			Datum* testCopyThis = copyTester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&copyTester) == testCopyThis->Get<RTTI*>());
		}

		TEST_METHOD(Copy_Assignment_Operator)
		{
			AttributedTester tester;
			AttributedTester copyTester;
			copyTester = tester;

			Datum* testInt = tester.Find("TestInt");
			Datum* copyTestInt = copyTester.Find("TestInt");
			Assert::AreEqual(testInt->Get<int32_t>(), copyTestInt->Get<int32_t>());
			tester.mTestInt = 123;
			Assert::AreNotEqual(testInt->Get<int32_t>(), copyTestInt->Get<int32_t>());

			Datum* testString = tester.Find("TestString");
			Datum* copyTestString = copyTester.Find("TestString");
			Assert::AreEqual(testString->Get<std::string>(), copyTestString->Get<std::string>());
			tester.mTestString = "hah";
			Assert::AreNotEqual(testString->Get<std::string>(), copyTestString->Get<std::string>());

			Datum* testThis = tester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&tester) == testThis->Get<RTTI*>());

			Datum* testCopyThis = copyTester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&copyTester) == testCopyThis->Get<RTTI*>());
		}

		TEST_METHOD(Move_Constructor)
		{
			int32_t a = 123;
			std::string b = "hah";
			int32_t a2 = 321;
			std::string b2 = "ho";
			AttributedTester tester;
			tester.mTestInt = a;
			tester.mTestString = b;
			AttributedTester moveTester(std::move(tester));

			Datum* moveTestInt = moveTester.Find("TestInt");
			Assert::AreEqual(a, moveTestInt->Get<int32_t>());
			moveTester.mTestInt = a2;
			Assert::AreEqual(a2, moveTestInt->Get<int32_t>());

			Datum* moveTestString = moveTester.Find("TestString");
			Assert::AreEqual(b, moveTestString->Get<std::string>());
			moveTester.mTestString = b2;
			Assert::AreEqual(b2, moveTestString->Get<std::string>());

			Datum* testThis = tester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&tester) == testThis->Get<RTTI*>());

			Datum* testMoveThis = moveTester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&moveTester) == testMoveThis->Get<RTTI*>());
		}

		TEST_METHOD(Move_Assignment_Operator)
		{
			int32_t a = 123;
			std::string b = "hah";
			int32_t a2 = 321;
			std::string b2 = "ho";
			AttributedTester tester;
			tester.mTestInt = a;
			tester.mTestString = b;
			AttributedTester moveTester;
			moveTester = std::move(tester);

			Datum* moveTestInt = moveTester.Find("TestInt");
			Assert::AreEqual(a, moveTestInt->Get<int32_t>());
			moveTester.mTestInt = a2;
			Assert::AreEqual(a2, moveTestInt->Get<int32_t>());

			Datum* moveTestString = moveTester.Find("TestString");
			Assert::AreEqual(b, moveTestString->Get<std::string>());
			moveTester.mTestString = b2;
			Assert::AreEqual(b2, moveTestString->Get<std::string>());

			Datum* testThis = tester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&tester) == testThis->Get<RTTI*>());

			Datum* testMoveThis = moveTester.Find("this");
			Assert::IsTrue(static_cast<RTTI*>(&moveTester) == testMoveThis->Get<RTTI*>());
		}

		TEST_METHOD(Attributes)
		{
			AttributedTester test;
			Vector<std::string> attributes;
			test.Attributes(attributes);
			Assert::AreEqual(15u, attributes.Size());
			Assert::IsTrue(attributes.Find("this") != attributes.end());
			Assert::IsTrue(attributes.Find("TestInt") != attributes.end());
			Assert::IsTrue(attributes.Find("TestString") != attributes.end());
			Assert::IsTrue(attributes.Find("TestInternalInt") != attributes.end());
			Assert::IsTrue(attributes.Find("TestAux") != attributes.end());
			Assert::IsTrue(attributes.Find("notin") == attributes.end());
		}

		TEST_METHOD(Auxiliary_Attributes)
		{
			AttributedTester test;
			Vector<std::string> attributes;
			test.AuxiliaryAttributes(attributes);
			Assert::AreEqual(1u, attributes.Size());
			Assert::IsTrue(attributes.Find("TestAux") != attributes.end());
			Assert::IsTrue(attributes.Find("notin") == attributes.end());
		}

		TEST_METHOD(Prescribed_Attributes)
		{
			AttributedTester test;
			Vector<std::string> attributes;
			test.PrescribedAttributes(attributes);
			Assert::AreEqual(14u, attributes.Size());
			Assert::IsTrue(attributes.Find("this") != attributes.end());
			Assert::IsTrue(attributes.Find("TestInt") != attributes.end());
			Assert::IsTrue(attributes.Find("TestString") != attributes.end());
			Assert::IsTrue(attributes.Find("TestInternalInt") != attributes.end());
			Assert::IsTrue(attributes.Find("notin") == attributes.end());
		}

		TEST_METHOD(IsAttribute_Tests)
		{
			AttributedTester test;
			Assert::IsTrue(test.IsAttribute("this"));
			Assert::IsTrue(test.IsAttribute("TestInt"));
			Assert::IsTrue(test.IsPrescribedAttribute("this"));
			Assert::IsTrue(test.IsPrescribedAttribute("TestInt"));
			Assert::IsFalse(test.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(test.IsAuxiliaryAttribute("TestInt"));
			Assert::IsTrue(test.IsAuxiliaryAttribute("TestAux"));
		}

		TEST_METHOD(UpdatingNonexistantAttributeExcepts)
		{
			AttributedTester test;
			auto CantUpdateNonexistant = [this, &test]() { test.UpdateNonexistantAttribute(); };
			Assert::ExpectException<std::exception>(CantUpdateNonexistant);
		}
	};

	_CrtMemState AttributedTest::sStartMemState;
}
