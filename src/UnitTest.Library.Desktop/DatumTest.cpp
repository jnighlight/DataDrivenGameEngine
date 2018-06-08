#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct DeepCopyDatumTester: public Library::RTTI
{
	private:                                                                             
		static std::uint64_t sRunTimeTypeId;
	public:                                                                              
		typedef RTTI Parent;                                                             
		static std::string TypeName() { return std::string("DeepCopyDatumTester"); }          
		static std::uint64_t TypeIdClass() { return sRunTimeTypeId; }                        
		virtual std::uint64_t TypeIdInstance() const override { return DeepCopyDatumTester::TypeIdClass(); }
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
	DeepCopyDatumTester::DeepCopyDatumTester()
		:mInt(0)
		,mIntPtr(new uint32_t(0))
	{}

	DeepCopyDatumTester::DeepCopyDatumTester(uint32_t inInt)
		:mInt(inInt)
		,mIntPtr(new uint32_t(0))
	{}

	DeepCopyDatumTester::DeepCopyDatumTester(uint32_t inInt, uint32_t inIntPtrVal)
		:mInt(inInt)
		,mIntPtr(new uint32_t(inIntPtrVal))
	{}

	DeepCopyDatumTester::DeepCopyDatumTester(const DeepCopyDatumTester& rhs)
		:mInt(rhs.mInt)
		,mIntPtr(new uint32_t(*rhs.mIntPtr))
	{}

	bool DeepCopyDatumTester::Equals(const RTTI* rhs) const override
	{
		if (rhs->Is(DeepCopyDatumTester::TypeIdClass())) {
			DeepCopyDatumTester* casted = rhs->As<DeepCopyDatumTester>();
			return (*this) == (*casted);
		}
		return this == rhs;
	}

	DeepCopyDatumTester& DeepCopyDatumTester::operator=(const DeepCopyDatumTester& rhs)
	{
		mInt = rhs.mInt;
		if (mIntPtr != nullptr) {
			delete mIntPtr;
		}
		mIntPtr = new uint32_t(*rhs.mIntPtr);
	}

	bool DeepCopyDatumTester::operator==(const DeepCopyDatumTester& rhs) const
	{
		return (mInt == rhs.mInt) && (*mIntPtr) == (*rhs.mIntPtr);
	}

	DeepCopyDatumTester::~DeepCopyDatumTester()
	{
		if (mIntPtr != nullptr) {
			delete mIntPtr;
			mIntPtr = nullptr;
		}
	}

};

std::uint64_t DeepCopyDatumTester::sRunTimeTypeId = reinterpret_cast<std::uint64_t>(&DeepCopyDatumTester::sRunTimeTypeId);

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		std::wstring retString;
		std::string comma(",");
		std::wstring wComma(comma.begin(), comma.end());
		retString.append(std::to_wstring(t.x));
		retString.append(wComma);
		retString.append(std::to_wstring(t.y));
		retString.append(wComma);
		retString.append(std::to_wstring(t.z));
		retString.append(wComma);
		retString.append(std::to_wstring(t.w));
		return retString;
	}

	template<>
	std::wstring ToString<Library::Datum::DatumType>(const Library::Datum::DatumType& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	std::wstring ToString<Library::Datum>(const Library::Datum& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	std::wstring ToString<>(const glm::mat4& t)
	{
		std::wstring retString;
		std::string comma(",");
		std::wstring wComma(comma.begin(), comma.end());
		std::string pipe("|");
		std::wstring wPipe(pipe.begin(), pipe.end());
		for (int i = 0; i < 4; ++i) {
			retString.append(std::to_wstring(t[i].x));
			retString.append(wComma);
			retString.append(std::to_wstring(t[i].y));
			retString.append(wComma);
			retString.append(std::to_wstring(t[i].z));
			retString.append(wComma);
			retString.append(std::to_wstring(t[i].w));
			retString.append(wPipe);
		}
		return retString;
	}
}

namespace UnitTestLibraryDesktop
{		
	using namespace Library;
	TEST_CLASS(DatumTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:

		void IntOutOfBoundsToExcept(Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<int32_t>(outOfBounds);
		}

		void FloatOutOfBoundsToExcept(Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<float_t>(outOfBounds);
		}

		void VectorOutOfBoundsToExcept(Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<glm::vec4>(outOfBounds);
		}

		void MatrixOutOfBoundsToExcept(Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<glm::mat4>(outOfBounds);
		}

		void ScopeOutOfBoundsToExcept(Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<Scope*>(outOfBounds);
		}

		void StringOutOfBoundsToExcept(Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<std::string>(outOfBounds);
		}

		void RTTIOutOfBoundsToExcept(Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<Library::RTTI*>(outOfBounds);
		}

		void IntOutOfBoundsToExceptConst(const Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<int32_t>(outOfBounds);
		}

		void FloatOutOfBoundsToExceptConst(const Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<float_t>(outOfBounds);
		}

		void VectorOutOfBoundsToExceptConst(const Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<glm::vec4>(outOfBounds);
		}

		void MatrixOutOfBoundsToExceptConst(const Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<glm::mat4>(outOfBounds);
		}

		void ScopeOutOfBoundsToExceptConst(const Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<Scope*>(outOfBounds);
		}

		void StringOutOfBoundsToExceptConst(const Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<std::string>(outOfBounds);
		}

		void RTTIOutOfBoundsToExceptConst(const Library::Datum& datum, uint32_t outOfBounds) {
			datum.Get<Library::RTTI*>(outOfBounds);
		}

		void IntInvalidTypeToExcept(Library::Datum& datum) {
			datum.Get<int32_t>();
		}

		void FloatInvalidTypeToExcept(Library::Datum& datum) {
			datum.Get<float_t>();
		}

		void VectorInvalidTypeToExcept(Library::Datum& datum) {
			datum.Get<glm::vec4>();
		}

		void MatrixInvalidTypeToExcept(Library::Datum& datum) {
			datum.Get<glm::mat4>();
		}

		void ScopeInvalidTypeToExcept(Library::Datum& datum) {
			datum.Get<glm::mat4>();
		}

		void StringInvalidTypeToExcept(Library::Datum& datum) {
			datum.Get<std::string>();
		}

		void RTTIInvalidTypeToExcept(Library::Datum& datum) {
			datum.Get<Library::RTTI*>();
		}

		void IntInvalidTypeToExceptConst( const Library::Datum& datum) {
			datum.Get<int32_t>();
		}

		void FloatInvalidTypeToExceptConst( const Library::Datum& datum) {
			datum.Get<float_t>();
		}

		void VectorInvalidTypeToExceptConst( const Library::Datum& datum) {
			datum.Get<glm::vec4>();
		}

		void MatrixInvalidTypeToExceptConst( const Library::Datum& datum) {
			datum.Get<glm::mat4>();
		}

		void ScopeInvalidTypeToExceptConst( const Library::Datum& datum) {
			datum.Get<Scope*>();
		}

		void StringInvalidTypeToExceptConst( const Library::Datum& datum) {
			datum.Get<std::string>();
		}

		void RTTIInvalidTypeToExceptConst( const Library::Datum& datum) {
			datum.Get<Library::RTTI*>();
		}

		void SetTypeToExcept(Library::Datum& datum, Library::Datum::DatumType type) {
			datum.SetType(type);
		}

		void SetStorageForWrongStringTypeToExcept(Library::Datum& datum) {
			datum.SetStorage(mTestStringArray, 2);
		}
		void SetStorageForWrongIntTypeToExcept(Library::Datum& datum) {
			datum.SetStorage(mTestIntArray, 2);
		}
		void SetStorageForWrongFloatTypeToExcept(Library::Datum& datum) {
			datum.SetStorage(mTestFloatArray, 2);
		}
		void SetStorageForWrongVecTypeToExcept(Library::Datum& datum) {
			datum.SetStorage(mTestVecArray, 2);
		}
		void SetStorageForWrongMatrixTypeToExcept(Library::Datum& datum) {
			datum.SetStorage(mTestMatrixArray, 2);
		}

		void SetStorageForWrongScopeTypeToExcept(Library::Datum& datum) {
			datum.SetStorage(mTestScopeArray, 2);
		}
		void SetStorageForWrongRTTITypeToExcept(Library::Datum& datum) {
			datum.SetStorage(mTestArray, 2);
		}
		void RTTICantSetStringToExcept(Library::Datum& datum, std::string toString) {
			datum.SetFromString(toString); 
		}

		int32_t mTestInt;
		int32_t mOtherTestInt;
		int32_t mTestIntArray[2];
		float_t mTestFloat;
		float_t mOtherTestFloat;
		float_t mTestFloatArray[2];
		glm::vec4 mTestVec;
		glm::vec4 mOtherTestVec;
		glm::vec4 mTestVecArray[2];
		glm::mat4 mTestMatrix;
		glm::mat4 mOtherTestMatrix;
		glm::mat4 mTestMatrixArray[2];
		Scope mFirstScope;
		Scope mSecondScope;
		Scope* mTestScope;
		Scope* mOtherTestScope;
		Scope* mTestScopeArray[2];
		std::string mTestString;
		std::string mOtherTestString;
		std::string mTestStringArray[2];
		DeepCopyDatumTester mTester;
		DeepCopyDatumTester mOtherTester;
		Library::RTTI* mTestRTTI = nullptr;
		Library::RTTI* mOtherTestRTTI = nullptr;
		Library::RTTI* mTestArray[2];

		TEST_METHOD_INITIALIZE(setup)
		{
			mTestScope = &mFirstScope;
			mTestScope->Append("append1");
			mOtherTestScope = &mSecondScope;
			mOtherTestScope->Append("append2");
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			mTestInt = 1;
			mOtherTestInt = 2;
			mTestFloat = 1.25f;
			mOtherTestFloat = 3.336f;
			mTestVec = { 1.0f ,2.0f,3.0f,4.0f };
			mOtherTestVec = { 3.11f,1.22f,8.33f,99.44f };
			mTestMatrix = { 1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f,12.0f,13.0f,14.0f,15.0f,16.0f };
			mOtherTestMatrix = { 3.32f,1.34f,83.3f,13.3f,32.8f,12.0f,1.32f,32.21f,3.0f,30.3f,2.32f,1.3f,321.32f,34.32f,32.3f,36.1f };
			mTestString = "HEYA";
			mOtherTestString = "wat";
			mTester.mInt = 50;
			(*mTester.mIntPtr) = 100;
			mOtherTester.mInt = 25;
			(*mOtherTester.mIntPtr) = 26;
			mTestRTTI = &mTester;
			mOtherTestRTTI = &mOtherTester;
			mTestArray[0] = mTestRTTI;
			mTestArray[1] = mOtherTestRTTI;
			mTestIntArray[0] = mTestInt;
			mTestIntArray[1] = mOtherTestInt;
			mTestFloatArray[0] = mTestFloat;
			mTestFloatArray[1] = mOtherTestFloat;
			mTestVecArray[0] = mTestVec;
			mTestVecArray[1] = mOtherTestVec;
			mTestMatrixArray[0] = mTestMatrix;
			mTestMatrixArray[1] = mOtherTestMatrix;
			mTestScopeArray[0] = mTestScope;
			mTestScopeArray[1] = mOtherTestScope;
			mTestStringArray[0] = mTestString;
			mTestStringArray[1] = mOtherTestString;
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
			Datum testDatum;
			Assert::AreEqual(0u, testDatum.Size());
			Assert::AreEqual(0u, testDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_UNKNOWN, testDatum.Type());
		}

		TEST_METHOD(Basic_Behavior_Int)
		{
			Datum IntTestDatum(mTestInt);
			Assert::AreEqual(1u, IntTestDatum.Size());
			Assert::AreEqual(1u, IntTestDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_INTEGER, IntTestDatum.Type());
			Assert::AreEqual(mTestInt, IntTestDatum.Get<int32_t>());
			Datum IntOtherTestDatum(mTestInt);
			Assert::AreEqual(IntTestDatum, IntOtherTestDatum);
			IntTestDatum = mOtherTestInt;
			Assert::AreNotEqual(IntTestDatum, IntOtherTestDatum);
			Assert::IsTrue(IntTestDatum != IntOtherTestDatum);

			IntTestDatum = mTestInt;
			IntTestDatum.PushBack(mOtherTestInt);
			Assert::AreEqual(2u, IntTestDatum.Size());
			Assert::IsTrue(IntTestDatum.Capacity() >= IntTestDatum.Size());
			Assert::AreEqual(mTestInt, IntTestDatum.Get<int32_t>(0));
			Assert::AreEqual(mOtherTestInt, IntTestDatum.Get<int32_t>(1));

			auto outOfBoundsExcept = [this, &IntTestDatum]() {IntOutOfBoundsToExcept(IntTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);
			auto outOfBoundsExceptConst = [this, &IntTestDatum]() {IntOutOfBoundsToExceptConst(IntTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExceptConst);

			Datum notSetDatum = "hai";
			auto invalidTypeExcept = [this, &notSetDatum]() {IntInvalidTypeToExcept(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExcept);
			auto invalidTypeExceptConst = [this, &notSetDatum]() {IntInvalidTypeToExceptConst(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExceptConst);
		}

		TEST_METHOD(Basic_Behavior_Float)
		{
			Datum FloatTestDatum(mTestFloat);
			Assert::AreEqual(1u, FloatTestDatum.Size());
			Assert::AreEqual(1u, FloatTestDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_FLOAT, FloatTestDatum.Type());
			Assert::AreEqual(mTestFloat, FloatTestDatum.Get<float_t>());
			Datum FloatOtherTestDatum(mTestFloat);
			Assert::AreEqual(FloatTestDatum, FloatOtherTestDatum);
			FloatTestDatum = mOtherTestFloat;
			Assert::AreNotEqual(FloatTestDatum, FloatOtherTestDatum);
			Assert::IsTrue(FloatTestDatum != FloatOtherTestDatum);

			FloatTestDatum = mTestFloat;
			FloatTestDatum.PushBack(mOtherTestFloat);
			Assert::AreEqual(2u, FloatTestDatum.Size());
			Assert::IsTrue(FloatTestDatum.Capacity() >= FloatTestDatum.Size());
			Assert::AreEqual(mTestFloat, FloatTestDatum.Get<float_t>(0));
			Assert::AreEqual(mOtherTestFloat, FloatTestDatum.Get<float_t>(1));

			auto outOfBoundsExcept = [this, &FloatTestDatum]() {FloatOutOfBoundsToExcept(FloatTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);
			auto outOfBoundsExceptConst = [this, &FloatTestDatum]() {FloatOutOfBoundsToExceptConst(FloatTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExceptConst);

			Datum notSetDatum = "hai";
			auto invalidTypeExcept = [this, &notSetDatum]() {FloatInvalidTypeToExcept(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExcept);
			auto invalidTypeExceptConst = [this, &notSetDatum]() {FloatInvalidTypeToExceptConst(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExceptConst);
		}

		TEST_METHOD(Basic_Behavior_Vec)
		{
			Datum VecTestDatum(mTestVec);
			Assert::AreEqual(1u, VecTestDatum.Size());
			Assert::AreEqual(1u, VecTestDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_VECTOR, VecTestDatum.Type());
			Assert::AreEqual(mTestVec, VecTestDatum.Get<glm::vec4>());
			Datum VecOtherTestDatum(mTestVec);
			Assert::AreEqual(VecTestDatum, VecOtherTestDatum);
			VecTestDatum = mOtherTestVec;
			Assert::AreNotEqual(VecTestDatum, VecOtherTestDatum);
			Assert::IsTrue(VecTestDatum != VecOtherTestDatum);

			VecTestDatum = mTestVec;
			VecTestDatum.PushBack(mOtherTestVec);
			Assert::AreEqual(2u, VecTestDatum.Size());
			Assert::IsTrue(VecTestDatum.Capacity() >= VecTestDatum.Size());
			Assert::AreEqual(mTestVec, VecTestDatum.Get<glm::vec4>(0));
			Assert::AreEqual(mOtherTestVec, VecTestDatum.Get<glm::vec4>(1));

			auto outOfBoundsExcept = [this, &VecTestDatum]() {VectorOutOfBoundsToExcept(VecTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);
			auto outOfBoundsExceptConst = [this, &VecTestDatum]() {VectorOutOfBoundsToExceptConst(VecTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExceptConst);

			Datum notSetDatum = "hai";
			auto invalidTypeExcept = [this, &notSetDatum]() {VectorInvalidTypeToExcept(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExcept);
			auto invalidTypeExceptConst = [this, &notSetDatum]() {VectorInvalidTypeToExceptConst(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExceptConst);
		}

		TEST_METHOD(Basic_Behavior_Matrix)
		{
			Datum MatrixTestDatum(mTestMatrix);
			Assert::AreEqual(1u, MatrixTestDatum.Size());
			Assert::AreEqual(1u, MatrixTestDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_MATRIX, MatrixTestDatum.Type());
			Assert::AreEqual(mTestMatrix, MatrixTestDatum.Get<glm::mat4>());
			Datum MatrixOtherTestDatum(mTestMatrix);
			Assert::AreEqual(MatrixTestDatum, MatrixOtherTestDatum);
			MatrixTestDatum = mOtherTestMatrix;
			Assert::AreNotEqual(MatrixTestDatum, MatrixOtherTestDatum);
			Assert::IsTrue(MatrixTestDatum != MatrixOtherTestDatum);

			MatrixTestDatum = mTestMatrix;
			MatrixTestDatum.PushBack(mOtherTestMatrix);
			Assert::AreEqual(2u, MatrixTestDatum.Size());
			Assert::IsTrue(MatrixTestDatum.Capacity() >= MatrixTestDatum.Size());
			Assert::AreEqual(mTestMatrix, MatrixTestDatum.Get<glm::mat4>(0));
			Assert::AreEqual(mOtherTestMatrix, MatrixTestDatum.Get<glm::mat4>(1));

			auto outOfBoundsExcept = [this, &MatrixTestDatum]() {MatrixOutOfBoundsToExcept(MatrixTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);
			auto outOfBoundsExceptConst = [this, &MatrixTestDatum]() {MatrixOutOfBoundsToExceptConst(MatrixTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExceptConst);

			Datum notSetDatum = "hai";
			auto invalidTypeExcept = [this, &notSetDatum]() {MatrixInvalidTypeToExcept(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExcept);
			auto invalidTypeExceptConst = [this, &notSetDatum]() {MatrixInvalidTypeToExceptConst(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExceptConst);
		}

		TEST_METHOD(Basic_Behavior_Scope)
		{
			Datum ScopeTestDatum(mTestScope);
			Assert::AreEqual(1u, ScopeTestDatum.Size());
			Assert::AreEqual(1u, ScopeTestDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_TABLE, ScopeTestDatum.Type());
			Assert::IsTrue(mTestScope == ScopeTestDatum.Get<Scope*>());
			Datum ScopeOtherTestDatum(mTestScope);
			Assert::AreEqual(ScopeTestDatum, ScopeOtherTestDatum);
			ScopeTestDatum = mOtherTestScope;
			Assert::AreNotEqual(ScopeTestDatum, ScopeOtherTestDatum);
			Assert::IsTrue(ScopeTestDatum != ScopeOtherTestDatum);

			ScopeTestDatum = mTestScope;
			ScopeTestDatum.PushBack(mOtherTestScope);
			Assert::AreEqual(2u, ScopeTestDatum.Size());
			Assert::IsTrue(ScopeTestDatum.Capacity() >= ScopeTestDatum.Size());
			Assert::IsTrue(mTestScope == ScopeTestDatum.Get<Scope*>(0));
			Assert::IsTrue(mOtherTestScope == ScopeTestDatum.Get<Scope*>(1));

			auto outOfBoundsExcept = [this, &ScopeTestDatum]() {ScopeOutOfBoundsToExcept(ScopeTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);
			auto outOfBoundsExceptConst = [this, &ScopeTestDatum]() {ScopeOutOfBoundsToExceptConst(ScopeTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExceptConst);

			Datum notSetDatum = "hai";
			auto invalidTypeExcept = [this, &notSetDatum]() {ScopeInvalidTypeToExcept(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExcept);
			auto invalidTypeExceptConst = [this, &notSetDatum]() {ScopeInvalidTypeToExceptConst(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExceptConst);
		}

		TEST_METHOD(Basic_Behavior_String)
		{
			Datum StringTestDatum(mTestString);
			Assert::AreEqual(1u, StringTestDatum.Size());
			Assert::AreEqual(1u, StringTestDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_STRING, StringTestDatum.Type());
			Assert::AreEqual(mTestString, StringTestDatum.Get<std::string>());
			Datum StringOtherTestDatum(mTestString);
			Assert::AreEqual(StringTestDatum, StringOtherTestDatum);
			StringTestDatum = mOtherTestString;
			Assert::AreNotEqual(StringTestDatum, StringOtherTestDatum);
			Assert::IsTrue(StringTestDatum != StringOtherTestDatum);

			StringTestDatum = mTestString;
			StringTestDatum.PushBack(mOtherTestString);
			Assert::AreEqual(2u, StringTestDatum.Size());
			Assert::IsTrue(StringTestDatum.Capacity() >= StringTestDatum.Size());
			Assert::AreEqual(mTestString, StringTestDatum.Get<std::string>(0));
			Assert::AreEqual(mOtherTestString, StringTestDatum.Get<std::string>(1));

			auto outOfBoundsExcept = [this, &StringTestDatum]() {StringOutOfBoundsToExcept(StringTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);
			auto outOfBoundsExceptConst = [this, &StringTestDatum]() {StringOutOfBoundsToExceptConst(StringTestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExceptConst);

			Datum notSetDatum = 1;
			auto invalidTypeExcept = [this, &notSetDatum]() {StringInvalidTypeToExcept(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExcept);
			auto invalidTypeExceptConst = [this, &notSetDatum]() {StringInvalidTypeToExceptConst(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExceptConst);
		}

		TEST_METHOD(Basic_Behavior_RTTI)
		{
			DeepCopyDatumTester testTHing(10, 20);
			Datum RTTITestDatum(mTestRTTI);
			Assert::AreEqual(1u, RTTITestDatum.Size());
			Assert::AreEqual(1u, RTTITestDatum.Capacity());
			Assert::AreEqual(Datum::DatumType::E_USER_DEFINED, RTTITestDatum.Type());
			Assert::IsTrue(mTestRTTI == RTTITestDatum.Get<Library::RTTI*>());
			Datum RTTIOtherTestDatum(mTestRTTI);
			Assert::IsTrue(RTTITestDatum == RTTIOtherTestDatum);
			RTTITestDatum = mOtherTestRTTI;
			Assert::IsFalse(RTTITestDatum == RTTIOtherTestDatum);
			Assert::IsTrue(RTTITestDatum != RTTIOtherTestDatum);

			RTTITestDatum = mTestRTTI;
			RTTITestDatum.PushBack(mOtherTestRTTI);
			Assert::AreEqual(2u, RTTITestDatum.Size());
			Assert::IsTrue(RTTITestDatum.Capacity() >= RTTITestDatum.Size());
			Assert::IsTrue(mTestRTTI == RTTITestDatum.Get<Library::RTTI*>(0));
			Assert::IsTrue(mOtherTestRTTI == RTTITestDatum.Get<Library::RTTI*>(1));

			auto outOfBoundsExcept = [this, &RTTITestDatum]() {RTTIOutOfBoundsToExcept(RTTITestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);
			auto outOfBoundsExceptConst = [this, &RTTITestDatum]() {RTTIOutOfBoundsToExceptConst(RTTITestDatum, 3); };
			Assert::ExpectException<std::exception>(outOfBoundsExceptConst);

			Datum notSetDatum = 1;
			auto invalidTypeExcept = [this, &notSetDatum]() {RTTIInvalidTypeToExcept(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExcept);
			auto invalidTypeExceptConst = [this, &notSetDatum]() {RTTIInvalidTypeToExceptConst(notSetDatum); };
			Assert::ExpectException<std::exception>(invalidTypeExceptConst);
		}

		TEST_METHOD(External_Storage)
		{
			Datum externalStorageTest;
			externalStorageTest.SetStorage(mTestArray, 2);
			Assert::AreEqual(2u, externalStorageTest.Size());
			Assert::AreEqual(2u, externalStorageTest.Capacity());
			Assert::AreEqual(Datum::DatumType::E_USER_DEFINED, externalStorageTest.Type());
			Assert::IsTrue(mTestArray[0] == externalStorageTest.Get<Library::RTTI*>());
			Assert::IsTrue(mTestArray[0] == externalStorageTest.Get<Library::RTTI*>(0));
			Assert::IsTrue(mTestArray[1] == externalStorageTest.Get<Library::RTTI*>(1));
		}

		TEST_METHOD(Reserve)
		{
			uint32_t newCapacity = 5;
			uint32_t smallerCapacity = 2;
			Datum reserveTestDatum = mTestInt;
			Assert::AreEqual(1u, reserveTestDatum.Capacity());
			Assert::AreEqual(1u, reserveTestDatum.Size());
			Assert::AreEqual(mTestInt, reserveTestDatum.Get<int32_t>());
			reserveTestDatum.Reserve(newCapacity);
			Assert::AreEqual(newCapacity, reserveTestDatum.Capacity());
			Assert::AreEqual(mTestInt, reserveTestDatum.Get<int32_t>());
			Assert::AreEqual(1u, reserveTestDatum.Size());
			reserveTestDatum.Reserve(smallerCapacity);
			Assert::AreEqual(newCapacity, reserveTestDatum.Capacity());
			Assert::AreEqual(mTestInt, reserveTestDatum.Get<int32_t>());
			Assert::AreEqual(1u, reserveTestDatum.Size());
		}

		TEST_METHOD(Reserve_Exceptions)
		{
			Datum blankDatum;
			auto unknownReserverExcept = [this, &blankDatum]() {blankDatum.Reserve(1); };
			Assert::ExpectException<std::exception>(unknownReserverExcept);
			blankDatum.SetStorage(mTestIntArray, 2);
			auto externalSetStorageExcept = [this, &blankDatum]() {blankDatum.Reserve(1); };
			Assert::ExpectException<std::exception>(externalSetStorageExcept);
		}

		TEST_METHOD(Resize)
		{
			uint32_t newSize = 5;
			uint32_t smallerSize = 2;
			uint32_t zeroSize = 0;

			Datum reserveTestDatum = mTestString;
			Assert::AreEqual(1u, reserveTestDatum.Capacity());
			Assert::AreEqual(1u, reserveTestDatum.Size());
			Assert::AreEqual(mTestString, reserveTestDatum.Get<std::string>());

			reserveTestDatum.Resize(newSize);
			Assert::AreEqual(newSize, reserveTestDatum.Capacity());
			Assert::AreEqual(newSize, reserveTestDatum.Size());
			Assert::AreEqual(mTestString, reserveTestDatum.Get<std::string>());

			reserveTestDatum.Resize(smallerSize);
			Assert::AreEqual(smallerSize, reserveTestDatum.Capacity());
			Assert::AreEqual(smallerSize, reserveTestDatum.Size());
			Assert::AreEqual(mTestString, reserveTestDatum.Get<std::string>());

			reserveTestDatum.Resize(zeroSize);
			Assert::AreEqual(zeroSize, reserveTestDatum.Capacity());
			Assert::AreEqual(zeroSize, reserveTestDatum.Size());
		}

		TEST_METHOD(Resize_CantResizeExternal)
		{
			Datum blankDatum;
			blankDatum.SetStorage(mTestIntArray, 2);
			auto externalResizeStorageExcept = [this, &blankDatum]() {blankDatum.Resize(1); };
			Assert::ExpectException<std::exception>(externalResizeStorageExcept);
		}

		TEST_METHOD(Remove)
		{
			Datum reserveTestDatum = mTestString;
			reserveTestDatum.PushBack(mOtherTestString);
			Assert::AreEqual(2u, reserveTestDatum.Size());
			uint32_t largerCap = reserveTestDatum.Capacity();

			reserveTestDatum.Remove(0);
			Assert::AreEqual(1u, reserveTestDatum.Size());
			Assert::AreEqual(largerCap, reserveTestDatum.Capacity());
			Assert::AreEqual(mOtherTestString, reserveTestDatum.Get<std::string>());

			reserveTestDatum.PushBack(mTestString);
			reserveTestDatum.Remove(1);
			Assert::AreEqual(1u, reserveTestDatum.Size());
			Assert::AreEqual(largerCap, reserveTestDatum.Capacity());
			Assert::AreEqual(mOtherTestString, reserveTestDatum.Get<std::string>());

			reserveTestDatum.Remove(0);
			Assert::AreEqual(0u, reserveTestDatum.Size());
			Assert::AreEqual(largerCap, reserveTestDatum.Capacity());

			Datum removeIntTest = mTestInt;
			removeIntTest.PushBack(mOtherTestInt);
			Assert::AreEqual(2u, removeIntTest.Size());
			largerCap = removeIntTest.Capacity();

			removeIntTest.Remove(0);
			Assert::AreEqual(1u, removeIntTest.Size());
			Assert::AreEqual(largerCap, removeIntTest.Capacity());
			Assert::AreEqual(mOtherTestInt, removeIntTest.Get<int32_t>());

			removeIntTest.PushBack(mTestInt);
			removeIntTest.Remove(1);
			Assert::AreEqual(1u, removeIntTest.Size());
			Assert::AreEqual(largerCap, removeIntTest.Capacity());
			Assert::AreEqual(mOtherTestInt, removeIntTest.Get<int32_t>());

			removeIntTest.Remove(0);
			Assert::AreEqual(0u, removeIntTest.Size());
			Assert::AreEqual(largerCap, removeIntTest.Capacity());
		}

		TEST_METHOD(Remove_Exceptions)
		{
			Datum removeDatum;
			auto removeOutOfBoundsExcept = [this, &removeDatum]() {removeDatum.Remove(0); };
			Assert::ExpectException<std::exception>(removeOutOfBoundsExcept);

			removeDatum.SetStorage(mTestIntArray, 2);
			auto removeExternalExcept = [this, &removeDatum]() {removeDatum.Remove(0); };
			Assert::ExpectException<std::exception>(removeExternalExcept);
		}

		TEST_METHOD(SetType)
		{
			Datum notSetDatum;
			Library::Datum::DatumType typeToSet = Library::Datum::DatumType::E_INTEGER;

			auto setToUnknownExcept = [this, &notSetDatum]() {SetTypeToExcept(notSetDatum, Library::Datum::DatumType::E_UNKNOWN); };
			Assert::ExpectException<std::exception>(setToUnknownExcept);
			auto setToMaxExcept = [this, &notSetDatum]() {SetTypeToExcept(notSetDatum, Library::Datum::DatumType::E_MAX); };
			Assert::ExpectException<std::exception>(setToMaxExcept);
			notSetDatum.SetType(typeToSet);
			Assert::AreEqual(typeToSet, notSetDatum.Type());
			auto resetTypeExcept = [this, &notSetDatum]() {SetTypeToExcept(notSetDatum, Library::Datum::DatumType::E_STRING); };
			Assert::ExpectException<std::exception>(resetTypeExcept);
		}

		TEST_METHOD(ToAndFromString_Int)
		{
			Datum toStringDatum = mTestInt;
			std::string toString = toStringDatum.ToString();
			int32_t wrongVal = mOtherTestInt;

			Datum fromStringDatum = wrongVal; //Can only set values that are already in the datum from string
			fromStringDatum.SetType(Library::Datum::DatumType::E_INTEGER);
			fromStringDatum.SetFromString(toString);

			Assert::AreEqual(mTestInt, toStringDatum.Get<int32_t>());
			Assert::AreEqual(toStringDatum.Get<int32_t>(), fromStringDatum.Get<int32_t>());
		}

		TEST_METHOD(ToAndFromString_Float)
		{
			Datum toStringDatum = mTestFloat;
			std::string toString = toStringDatum.ToString();
			float_t wrongVal = mOtherTestFloat;

			Datum fromStringDatum = wrongVal; //Can only set values that are already in the datum from string
			fromStringDatum.SetType(Library::Datum::DatumType::E_FLOAT);
			fromStringDatum.SetFromString(toString);

			Assert::AreEqual(mTestFloat, toStringDatum.Get<float_t>());
			Assert::AreEqual(toStringDatum.Get<float_t>(), fromStringDatum.Get<float_t>());
		}

		TEST_METHOD(ToAndFromString_Vec)
		{
			Datum toStringDatum = mTestVec;
			std::string toString = toStringDatum.ToString();
			glm::vec4 wrongVal = mOtherTestVec;

			Datum fromStringDatum = wrongVal; //Can only set values that are already in the datum from string
			fromStringDatum.SetType(Library::Datum::DatumType::E_VECTOR);
			fromStringDatum.SetFromString(toString);

			Assert::AreEqual(mTestVec, toStringDatum.Get<glm::vec4>());
			Assert::AreEqual(toStringDatum.Get<glm::vec4>(), fromStringDatum.Get<glm::vec4>());
		}

		TEST_METHOD(ToAndFromString_Matrix)
		{
			Datum toStringDatum = mTestMatrix;
			std::string toString = toStringDatum.ToString();
			glm::mat4 wrongVal = mOtherTestMatrix;

			Datum fromStringDatum = wrongVal; //Can only set values that are already in the datum from string
			fromStringDatum.SetType(Library::Datum::DatumType::E_MATRIX);
			fromStringDatum.SetFromString(toString);

			Assert::AreEqual(mTestMatrix, toStringDatum.Get<glm::mat4>());
			Assert::AreEqual(toStringDatum.Get<glm::mat4>(), fromStringDatum.Get<glm::mat4>());
		}

		TEST_METHOD(ToAndFromString_String)
		{
			Datum toStringDatum = mTestString;
			std::string toString = toStringDatum.ToString();
			std::string wrongVal = mOtherTestString;

			Datum fromStringDatum = wrongVal; //Can only set values that are already in the datum from string
			fromStringDatum.SetType(Library::Datum::DatumType::E_STRING);
			fromStringDatum.SetFromString(toString);

			Assert::AreEqual(mTestString, toStringDatum.Get<std::string>());
			Assert::AreEqual(toStringDatum.Get<std::string>(), fromStringDatum.Get<std::string>());
		}

		TEST_METHOD(ToAndFromString_Exceptions)
		{
			Datum blankDatum;
			auto blankDatumToStringExcept = [this, &blankDatum]() {blankDatum.SetFromString("", 0); };
			Assert::ExpectException<std::exception>(blankDatumToStringExcept);

			blankDatum.SetType(Library::Datum::DatumType::E_INTEGER);
			auto outOfBoundsToStringExcept = [this, &blankDatum]() {blankDatum.SetFromString("", 0); };
			Assert::ExpectException<std::exception>(outOfBoundsToStringExcept);

		}

		TEST_METHOD(ToAndFromString_RTTI)
		{
			Datum toStringDatum = mTestRTTI;
			std::string toString = toStringDatum.ToString();
			Library::RTTI* wrongVal = mOtherTestRTTI;

			Datum fromStringDatum = wrongVal;
			fromStringDatum.SetType(Library::Datum::DatumType::E_USER_DEFINED);

			auto RTTIFromStringExcept = [this, &fromStringDatum, &toString]() {RTTICantSetStringToExcept(fromStringDatum, toString); };
			Assert::ExpectException<std::exception>(RTTIFromStringExcept);
		}

		TEST_METHOD(ToStringExceptions)
		{
			Datum toStringDatum;
			auto unknownTypeExcept = [this, &toStringDatum]() { toStringDatum.ToString(0); };
			Assert::ExpectException<std::exception>(unknownTypeExcept);
			
			toStringDatum.SetType(Library::Datum::DatumType::E_INTEGER);
			auto outOfBoundsExcept = [this, &toStringDatum]() { toStringDatum.ToString(1); };
			Assert::ExpectException<std::exception>(outOfBoundsExcept);

			toStringDatum.SetType(Library::Datum::DatumType::E_INTEGER);
			Assert::AreEqual("[Empty]", toStringDatum.ToString(0).c_str());
		}

		TEST_METHOD(ExternalSetStorageExceptions)
		{

			Datum externalStorageStringDatum;
			externalStorageStringDatum.SetStorage(mTestStringArray, 2);
			auto wrongStorageTypeInt = [this, &externalStorageStringDatum]() {SetStorageForWrongIntTypeToExcept(externalStorageStringDatum); };
			Assert::ExpectException<std::exception>(wrongStorageTypeInt);

			Datum externalStorageIntDatum;
			externalStorageIntDatum.SetStorage(mTestIntArray, 2);
			auto wrongStorageTypeFloat = [this, &externalStorageIntDatum]() {SetStorageForWrongFloatTypeToExcept(externalStorageIntDatum); };
			Assert::ExpectException<std::exception>(wrongStorageTypeFloat);
			auto wrongStorageTypeVec = [this, &externalStorageIntDatum]() {SetStorageForWrongVecTypeToExcept(externalStorageIntDatum); };
			Assert::ExpectException<std::exception>(wrongStorageTypeVec);
			auto wrongStorageTypeMatrix = [this, &externalStorageIntDatum]() {SetStorageForWrongMatrixTypeToExcept(externalStorageIntDatum); };
			Assert::ExpectException<std::exception>(wrongStorageTypeMatrix);
			auto wrongStorageTypeString = [this, &externalStorageIntDatum]() {SetStorageForWrongStringTypeToExcept(externalStorageIntDatum); };
			Assert::ExpectException<std::exception>(wrongStorageTypeString);
			auto wrongStorageTypeRTTI = [this, &externalStorageIntDatum]() {SetStorageForWrongRTTITypeToExcept(externalStorageIntDatum); };
			Assert::ExpectException<std::exception>(wrongStorageTypeRTTI);
		}

		TEST_METHOD(ExternalStorageTest_Int)
		{
			Datum externalStorageIntDatum;
			externalStorageIntDatum.SetStorage(mTestIntArray, 2);
			Assert::AreEqual(mTestInt, externalStorageIntDatum.Get<int32_t>());
			Assert::AreEqual(mOtherTestInt, externalStorageIntDatum.Get<int32_t>(1));
			externalStorageIntDatum.Set(mOtherTestInt);
			Assert::AreEqual(mOtherTestInt, externalStorageIntDatum.Get<int32_t>(0));
			Assert::AreEqual(mOtherTestInt, externalStorageIntDatum.Get<int32_t>(1));
			Assert::AreEqual(mOtherTestInt, mTestIntArray[0]);
			Assert::AreEqual(mOtherTestInt, mTestIntArray[1]);

		}

		TEST_METHOD(ExternalStorageTest_Float)
		{
			Datum externalStorageFloatDatum;
			externalStorageFloatDatum.SetStorage(mTestFloatArray, 2);
			Assert::AreEqual(mTestFloat, externalStorageFloatDatum.Get<float_t>());
			Assert::AreEqual(mOtherTestFloat, externalStorageFloatDatum.Get<float_t>(1));
			externalStorageFloatDatum.Set(mOtherTestFloat);
			Assert::AreEqual(mOtherTestFloat, externalStorageFloatDatum.Get<float_t>(0));
			Assert::AreEqual(mOtherTestFloat, externalStorageFloatDatum.Get<float_t>(1));
			Assert::AreEqual(mOtherTestFloat, mTestFloatArray[0]);
			Assert::AreEqual(mOtherTestFloat, mTestFloatArray[1]);

		}

		TEST_METHOD(ExternalStorageTest_Vec)
		{
			Datum externalStorageVecDatum;
			externalStorageVecDatum.SetStorage(mTestVecArray, 2);
			Assert::AreEqual(mTestVec, externalStorageVecDatum.Get<glm::vec4>());
			Assert::AreEqual(mOtherTestVec, externalStorageVecDatum.Get<glm::vec4>(1));
			externalStorageVecDatum.Set(mOtherTestVec);
			Assert::AreEqual(mOtherTestVec, externalStorageVecDatum.Get<glm::vec4>(0));
			Assert::AreEqual(mOtherTestVec, externalStorageVecDatum.Get<glm::vec4>(1));
			Assert::AreEqual(mOtherTestVec, mTestVecArray[0]);
			Assert::AreEqual(mOtherTestVec, mTestVecArray[1]);
		}

		TEST_METHOD(ExternalStorageTest_Matrix)
		{
			Datum externalStorageMatrixDatum;
			externalStorageMatrixDatum.SetStorage(mTestMatrixArray, 2);
			Assert::AreEqual(mTestMatrix, externalStorageMatrixDatum.Get<glm::mat4>());
			Assert::AreEqual(mOtherTestMatrix, externalStorageMatrixDatum.Get<glm::mat4>(1));
			externalStorageMatrixDatum.Set(mOtherTestMatrix);
			Assert::AreEqual(mOtherTestMatrix, externalStorageMatrixDatum.Get<glm::mat4>(0));
			Assert::AreEqual(mOtherTestMatrix, externalStorageMatrixDatum.Get<glm::mat4>(1));
			Assert::AreEqual(mOtherTestMatrix, mTestMatrixArray[0]);
			Assert::AreEqual(mOtherTestMatrix, mTestMatrixArray[1]);
		}

		TEST_METHOD(ExternalStorageTest_Scope)
		{
			Datum externalStorageScopeDatum;
			externalStorageScopeDatum.SetStorage(mTestScopeArray, 2);
			Assert::IsTrue(mTestScope == externalStorageScopeDatum.Get<Scope*>());
			Assert::IsTrue(mOtherTestScope == externalStorageScopeDatum.Get<Scope*>(1));
			externalStorageScopeDatum.Set(mOtherTestScope);
			Assert::IsTrue(mOtherTestScope == externalStorageScopeDatum.Get<Scope*>(0));
			Assert::IsTrue(mOtherTestScope == externalStorageScopeDatum.Get<Scope*>(1));
			Assert::IsTrue(mOtherTestScope == mTestScopeArray[0]);
			Assert::IsTrue(mOtherTestScope == mTestScopeArray[1]);
		}

		TEST_METHOD(ExternalStorageTest_String)
		{
			Datum externalStorageStringDatum;
			externalStorageStringDatum.SetStorage(mTestStringArray, 2);
			Assert::AreEqual(mTestString, externalStorageStringDatum.Get<std::string>());
			Assert::AreEqual(mOtherTestString, externalStorageStringDatum.Get<std::string>(1));
			externalStorageStringDatum.Set(mOtherTestString);
			Assert::AreEqual(mOtherTestString, externalStorageStringDatum.Get<std::string>(0));
			Assert::AreEqual(mOtherTestString, externalStorageStringDatum.Get<std::string>(1));
			Assert::AreEqual(mOtherTestString, mTestStringArray[0]);
			Assert::AreEqual(mOtherTestString, mTestStringArray[1]);
		}

		TEST_METHOD(ExternalStorageTest_RTTI)
		{
			Datum externalStorageRTTIDatum;
			externalStorageRTTIDatum.SetStorage(mTestArray, 2);
			Assert::IsTrue(mTestRTTI == externalStorageRTTIDatum.Get<Library::RTTI*>());
			Assert::IsTrue(mOtherTestRTTI == externalStorageRTTIDatum.Get<Library::RTTI*>(1));
			externalStorageRTTIDatum.Set(mOtherTestRTTI);
			Assert::IsTrue(mOtherTestRTTI == externalStorageRTTIDatum.Get<Library::RTTI*>(0));
			Assert::IsTrue(mOtherTestRTTI == externalStorageRTTIDatum.Get<Library::RTTI*>(1));
			Assert::IsTrue(mOtherTestRTTI == mTestArray[0]);
			Assert::IsTrue(mOtherTestRTTI == mTestArray[1]);
		}

		TEST_METHOD(InternalCopyOperators)
		{
			Datum internalStorageStringDatum = mTestString;
			Datum internalCopyString = internalStorageStringDatum;
			Assert::AreEqual(internalStorageStringDatum.Get<std::string>(), internalCopyString.Get<std::string>());
			internalStorageStringDatum.Set(mOtherTestString);
			Assert::AreNotEqual(internalStorageStringDatum.Get<std::string>(), internalCopyString.Get<std::string>());

			Datum internalStorageIntDatum = mTestInt;
			Datum internalCopyInt = internalStorageIntDatum;
			Assert::AreEqual(internalStorageIntDatum.Get<int32_t>(), internalCopyInt.Get<int32_t>());
			internalStorageIntDatum.Set(mOtherTestInt);
			Assert::AreNotEqual(internalStorageIntDatum.Get<int32_t>(), internalCopyInt.Get<int32_t>());
		}

		TEST_METHOD(externalCopyOperators)
		{
			Datum externalStorageStringDatum;
			externalStorageStringDatum.SetStorage(mTestStringArray, 2);
			Datum externalCopyString = externalStorageStringDatum;
			Assert::AreEqual(externalStorageStringDatum.Get<std::string>(), externalCopyString.Get<std::string>());
			externalStorageStringDatum.Set(mOtherTestString);
			Assert::AreEqual(externalStorageStringDatum.Get<std::string>(), externalCopyString.Get<std::string>()); //They're pointing to the same data. One changes the other

			Datum externalStorageIntDatum;
			externalStorageIntDatum.SetStorage(mTestIntArray, 2);
			Datum externalCopyInt = externalStorageIntDatum;
			Assert::AreEqual(externalStorageIntDatum.Get<int32_t>(), externalCopyInt.Get<int32_t>());
			externalStorageIntDatum.Set(mOtherTestInt);
			Assert::AreEqual(externalStorageIntDatum.Get<int32_t>(), externalCopyInt.Get<int32_t>());
		}

		TEST_METHOD(InternalMoveOperators)
		{
			Datum internalStorageStringDatum = mTestString;
			Datum internalMoveString = std::move(internalStorageStringDatum);
			Assert::AreEqual(0u, internalStorageStringDatum.Size());
			Assert::AreEqual(0u, internalStorageStringDatum.Capacity());
			Assert::AreEqual(1u, internalMoveString.Size());
			Assert::AreEqual(1u, internalMoveString.Capacity());
			Assert::AreEqual(mTestString, internalMoveString.Get<std::string>());

			Datum internalStorageIntDatum = mTestInt;
			Datum internalMoveInt(std::move(internalStorageIntDatum));
			Assert::AreEqual(0u, internalStorageIntDatum.Size());
			Assert::AreEqual(0u, internalStorageIntDatum.Capacity());
			Assert::AreEqual(1u, internalMoveInt.Size());
			Assert::AreEqual(1u, internalMoveInt.Capacity());
			Assert::AreEqual(mTestInt, internalMoveInt.Get<int32_t>());
		}

		TEST_METHOD(externalMoveOperators)
		{
			Datum externalStorageStringDatum;
			externalStorageStringDatum.SetStorage(mTestStringArray, 2);
			Datum externalMoveString = std::move(externalStorageStringDatum);
			Assert::AreEqual(0u, externalStorageStringDatum.Size());
			Assert::AreEqual(0u, externalStorageStringDatum.Capacity());
			Assert::AreEqual(2u, externalMoveString.Size());
			Assert::AreEqual(2u, externalMoveString.Capacity());
			externalMoveString.Set(mOtherTestString);
			Assert::AreEqual(externalMoveString.Get<std::string>(), mTestStringArray[0]);

			Datum externalStorageIntDatum;
			externalStorageIntDatum.SetStorage(mTestIntArray, 2);
			Datum externalMoveInt = std::move(externalStorageIntDatum);
			Assert::AreEqual(0u, externalStorageIntDatum.Size());
			Assert::AreEqual(0u, externalStorageIntDatum.Capacity());
			Assert::AreEqual(2u, externalMoveInt.Size());
			Assert::AreEqual(2u, externalMoveInt.Capacity());
			externalMoveInt.Set(mOtherTestInt);
			Assert::AreEqual(externalMoveInt.Get<int32_t>(), mTestIntArray[0]);
		}

		TEST_METHOD(InternalCopyAssignmentOperators)
		{
			Datum internalStorageStringDatum = mTestString;
			Datum internalCopyString;
			internalCopyString = internalStorageStringDatum;
			Assert::AreEqual(internalStorageStringDatum.Get<std::string>(), internalCopyString.Get<std::string>());
			internalStorageStringDatum.Set(mOtherTestString);
			Assert::AreNotEqual(internalStorageStringDatum.Get<std::string>(), internalCopyString.Get<std::string>());

			Datum internalStorageIntDatum = mTestInt;
			Datum internalCopyInt;
			internalCopyInt = internalStorageIntDatum;
			Assert::AreEqual(internalStorageIntDatum.Get<int32_t>(), internalCopyInt.Get<int32_t>());
			internalStorageIntDatum.Set(mOtherTestInt);
			Assert::AreNotEqual(internalStorageIntDatum.Get<int32_t>(), internalCopyInt.Get<int32_t>());
		}

		TEST_METHOD(externalCopyAssignmentOperators)
		{
			Datum externalStorageStringDatum;
			externalStorageStringDatum.SetStorage(mTestStringArray, 2);
			Datum externalCopyString;
			externalCopyString = externalStorageStringDatum;
			Assert::AreEqual(externalStorageStringDatum.Get<std::string>(), externalCopyString.Get<std::string>());
			externalStorageStringDatum.Set(mOtherTestString);
			Assert::AreEqual(externalStorageStringDatum.Get<std::string>(), externalCopyString.Get<std::string>()); //They're pointing to the same data. One changes the other

			Datum externalStorageIntDatum;
			externalStorageIntDatum.SetStorage(mTestIntArray, 2);
			Datum externalCopyInt;
			externalCopyInt = externalStorageIntDatum;
			Assert::AreEqual(externalStorageIntDatum.Get<int32_t>(), externalCopyInt.Get<int32_t>());
			externalStorageIntDatum.Set(mOtherTestInt);
			Assert::AreEqual(externalStorageIntDatum.Get<int32_t>(), externalCopyInt.Get<int32_t>());
		}
		
		TEST_METHOD(InternalCopyAssignmentOperatorsOnExternal)
		{
			Datum internalStorageStringDatum = mTestString;
			Datum internalCopyString;
			internalCopyString.SetStorage(mTestStringArray, 2);
			internalCopyString = internalStorageStringDatum;
			Assert::AreEqual(internalStorageStringDatum.Get<std::string>(), internalCopyString.Get<std::string>());
			internalStorageStringDatum.Set(mOtherTestString);
			Assert::AreNotEqual(internalStorageStringDatum.Get<std::string>(), internalCopyString.Get<std::string>());

			Datum internalStorageIntDatum = mTestInt;
			Datum internalCopyInt;
			internalCopyInt.SetStorage(mTestIntArray, 2);
			internalCopyInt = internalStorageIntDatum;
			Assert::AreEqual(internalStorageIntDatum.Get<int32_t>(), internalCopyInt.Get<int32_t>());
			internalStorageIntDatum.Set(mOtherTestInt);
			Assert::AreNotEqual(internalStorageIntDatum.Get<int32_t>(), internalCopyInt.Get<int32_t>());
		}

		TEST_METHOD(externalCopyAssignmentOperatorsOnExternal)
		{
			Datum externalStorageStringDatum;
			externalStorageStringDatum.SetStorage(mTestStringArray, 2);
			Datum externalCopyString;
			externalCopyString.SetStorage(mTestStringArray, 2);
			externalCopyString = externalStorageStringDatum;
			Assert::AreEqual(externalStorageStringDatum.Get<std::string>(), externalCopyString.Get<std::string>());
			externalStorageStringDatum.Set(mOtherTestString);
			Assert::AreEqual(externalStorageStringDatum.Get<std::string>(), externalCopyString.Get<std::string>()); //They're pointing to the same data. One changes the other
			Assert::AreEqual(mOtherTestString, externalCopyString.Get<std::string>());

			Datum externalStorageIntDatum;
			externalStorageIntDatum.SetStorage(mTestIntArray, 2);
			Datum externalCopyInt;
			externalCopyInt.SetStorage(mTestIntArray, 2);
			externalCopyInt = externalStorageIntDatum;
			Assert::AreEqual(externalStorageIntDatum.Get<int32_t>(), externalCopyInt.Get<int32_t>());
			externalStorageIntDatum.Set(mOtherTestInt);
			Assert::AreEqual(externalStorageIntDatum.Get<int32_t>(), externalCopyInt.Get<int32_t>());
			Assert::AreEqual(mOtherTestInt, externalCopyInt.Get<int32_t>());
		}

		TEST_METHOD(InternalMoveAssignmentOperators)
		{
			Datum internalStorageStringDatum = mTestString;
			Datum internalMoveString;
			internalMoveString = std::move(internalStorageStringDatum);
			Assert::AreEqual(0u, internalStorageStringDatum.Size());
			Assert::AreEqual(0u, internalStorageStringDatum.Capacity());
			Assert::AreEqual(1u, internalMoveString.Size());
			Assert::AreEqual(1u, internalMoveString.Capacity());
			Assert::AreEqual(mTestString, internalMoveString.Get<std::string>());

			Datum internalStorageIntDatum = mTestInt;
			Datum internalMoveInt;
			internalMoveInt = std::move(internalStorageIntDatum);
			Assert::AreEqual(0u, internalStorageIntDatum.Size());
			Assert::AreEqual(0u, internalStorageIntDatum.Capacity());
			Assert::AreEqual(1u, internalMoveInt.Size());
			Assert::AreEqual(1u, internalMoveInt.Capacity());
			Assert::AreEqual(mTestInt, internalMoveInt.Get<int32_t>());
		}

		TEST_METHOD(externalMoveAssignmentOperators)
		{
			Datum externalStorageStringDatum;
			externalStorageStringDatum.SetStorage(mTestStringArray, 2);
			Datum externalMoveString;
			externalMoveString = std::move(externalStorageStringDatum);
			Assert::AreEqual(0u, externalStorageStringDatum.Size());
			Assert::AreEqual(0u, externalStorageStringDatum.Capacity());
			Assert::AreEqual(2u, externalMoveString.Size());
			Assert::AreEqual(2u, externalMoveString.Capacity());
			externalMoveString.Set(mOtherTestString);
			Assert::AreEqual(externalMoveString.Get<std::string>(), mTestStringArray[0]);

			Datum externalStorageIntDatum;
			externalStorageIntDatum.SetStorage(mTestIntArray, 2);
			Datum externalMoveInt;
			externalMoveInt = std::move(externalStorageIntDatum);
			Assert::AreEqual(0u, externalStorageIntDatum.Size());
			Assert::AreEqual(0u, externalStorageIntDatum.Capacity());
			Assert::AreEqual(2u, externalMoveInt.Size());
			Assert::AreEqual(2u, externalMoveInt.Capacity());
			externalMoveInt.Set(mOtherTestInt);
			Assert::AreEqual(externalMoveInt.Get<int32_t>(), mTestIntArray[0]);
		}

		TEST_METHOD(AssignmentCopyOperatorExceptions)
		{
			Datum intDatum = mTestInt;
			Datum stringDatum = mTestString;
			auto wrongAssignment = [this, &intDatum, &stringDatum]() {intDatum = stringDatum; };
			Assert::ExpectException<std::exception>(wrongAssignment);
			auto wrongAssignmentMove = [this, &intDatum, &stringDatum]() {intDatum = std::move(stringDatum); };
			Assert::ExpectException<std::exception>(wrongAssignmentMove);
		}

		TEST_METHOD(AssignmentImplicitOperatorSizes)
		{
			Datum implicitIntDatum;
			implicitIntDatum = mTestInt;
			Assert::AreEqual(mTestInt, implicitIntDatum.Get<int32_t>());
			implicitIntDatum = mOtherTestInt;
			Assert::AreEqual(mOtherTestInt, implicitIntDatum.Get<int32_t>());

			Datum implicitFloatDatum;
			implicitFloatDatum = mTestFloat;
			Assert::AreEqual(mTestFloat, implicitFloatDatum.Get<float_t>());
			implicitFloatDatum = mOtherTestFloat;
			Assert::AreEqual(mOtherTestFloat, implicitFloatDatum.Get<float_t>());

			Datum implicitVecDatum;
			implicitVecDatum = mTestVec;
			Assert::AreEqual(mTestVec, implicitVecDatum.Get<glm::vec4>());
			implicitVecDatum = mOtherTestVec;
			Assert::AreEqual(mOtherTestVec, implicitVecDatum.Get<glm::vec4>());

			Datum implicitMatrixDatum;
			implicitMatrixDatum = mTestMatrix;
			Assert::AreEqual(mTestMatrix, implicitMatrixDatum.Get<glm::mat4>());
			implicitMatrixDatum = mOtherTestMatrix;
			Assert::AreEqual(mOtherTestMatrix, implicitMatrixDatum.Get<glm::mat4>());

			Datum implicitStringDatum;
			implicitStringDatum = mTestString;
			Assert::AreEqual(mTestString, implicitStringDatum.Get<std::string>());
			implicitStringDatum = mOtherTestString;
			Assert::AreEqual(mOtherTestString, implicitStringDatum.Get<std::string>());

			Datum implicitRTTIDatum;
			implicitRTTIDatum = mTestRTTI;
			Assert::IsTrue(mTestRTTI == implicitRTTIDatum.Get<Library::RTTI*>());
			implicitRTTIDatum = mOtherTestRTTI;
			Assert::IsTrue(mOtherTestRTTI == implicitRTTIDatum.Get<Library::RTTI*>());
		}

		TEST_METHOD(AssignmentImplicitOperatorExceptions)
		{
			Datum stringDatum = mTestString;
			auto wrongAssignmentInt = [this, &stringDatum]() {stringDatum = mTestInt; };
			Assert::ExpectException<std::exception>(wrongAssignmentInt);

			Datum intDatum = mTestInt;
			auto wrongAssignmentFloat = [this, &intDatum]() {intDatum = mTestFloat; };
			Assert::ExpectException<std::exception>(wrongAssignmentFloat);
			auto wrongAssignmentVec = [this, &intDatum]() {intDatum = mTestVec; };
			Assert::ExpectException<std::exception>(wrongAssignmentVec);
			auto wrongAssignmentMatrix = [this, &intDatum]() {intDatum = mTestMatrix; };
			Assert::ExpectException<std::exception>(wrongAssignmentMatrix);
			auto wrongAssignmentString = [this, &intDatum]() {intDatum = mTestString; };
			Assert::ExpectException<std::exception>(wrongAssignmentString);
			auto wrongAssignmentRTTI = [this, &intDatum]() {intDatum = mTestRTTI; };
			Assert::ExpectException<std::exception>(wrongAssignmentRTTI);
		}

		TEST_METHOD(PushBackExceptions_WrongType)
		{
			Datum pushBackInt = mTestInt;

			auto wrongTypePushBackFloat = [this, &pushBackInt]() {pushBackInt.PushBack(mTestFloat); };
			Assert::ExpectException<std::exception>(wrongTypePushBackFloat);
			auto wrongTypePushBackVec = [this, &pushBackInt]() {pushBackInt.PushBack(mTestVec); };
			Assert::ExpectException<std::exception>(wrongTypePushBackVec);
			auto wrongTypePushBackMatrix = [this, &pushBackInt]() {pushBackInt.PushBack(mTestMatrix); };
			Assert::ExpectException<std::exception>(wrongTypePushBackMatrix);
			auto wrongTypePushBackString = [this, &pushBackInt]() {pushBackInt.PushBack(mTestString); };
			Assert::ExpectException<std::exception>(wrongTypePushBackString);
			auto wrongTypePushBackRTTI = [this, &pushBackInt]() {pushBackInt.PushBack(mTestRTTI); };
			Assert::ExpectException<std::exception>(wrongTypePushBackRTTI);

			Datum pushBackString = mTestString;

			auto wrongTypePushBackInt = [this, &pushBackString]() {pushBackString.PushBack(mTestInt); };
			Assert::ExpectException<std::exception>(wrongTypePushBackInt);
		}

		TEST_METHOD(PushBackExceptions_External)
		{
			Datum pushBackInt;
			pushBackInt.SetStorage(mTestIntArray, 2);

			auto externalIntExcept = [this, &pushBackInt]() {pushBackInt.PushBack(mTestInt); };
			Assert::ExpectException<std::exception>(externalIntExcept);
			auto externalFloatExcept = [this, &pushBackInt]() {pushBackInt.PushBack(mTestFloat); };
			Assert::ExpectException<std::exception>(externalFloatExcept);
			auto externalVecExcept = [this, &pushBackInt]() {pushBackInt.PushBack(mTestVec); };
			Assert::ExpectException<std::exception>(externalVecExcept);
			auto externalMatrixExcept = [this, &pushBackInt]() {pushBackInt.PushBack(mTestMatrix); };
			Assert::ExpectException<std::exception>(externalMatrixExcept);
			auto externalStringExcept = [this, &pushBackInt]() {pushBackInt.PushBack(mTestString); };
			Assert::ExpectException<std::exception>(externalStringExcept);
			auto externalRTTIExcept = [this, &pushBackInt]() {pushBackInt.PushBack(mTestRTTI); };
			Assert::ExpectException<std::exception>(externalRTTIExcept);
		}

		TEST_METHOD(Set_SetsType)
		{
			Datum setException = mTestInt;
			Datum setExceptionString = mTestString;

			auto setWrongTypeIntExcept = [this, &setExceptionString]() {setExceptionString.Set(mTestInt); };
			Assert::ExpectException<std::exception>(setWrongTypeIntExcept);
			auto setWrongTypeFloatExcept = [this, &setException]() {setException.Set(mTestFloat); };
			Assert::ExpectException<std::exception>(setWrongTypeFloatExcept);
			auto setWrongTypeVecExcept = [this, &setException]() {setException.Set(mTestVec); };
			Assert::ExpectException<std::exception>(setWrongTypeVecExcept);
			auto setWrongTypeMatrixExcept = [this, &setException]() {setException.Set(mTestMatrix); };
			Assert::ExpectException<std::exception>(setWrongTypeMatrixExcept);
			auto setWrongTypeStringExcept = [this, &setException]() {setException.Set(mTestString); };
			Assert::ExpectException<std::exception>(setWrongTypeStringExcept);
			auto setWrongTypeRTTIExcept = [this, &setException]() {setException.Set(mTestRTTI); };
			Assert::ExpectException<std::exception>(setWrongTypeRTTIExcept);
		}

		TEST_METHOD(OperatorEqualsShortCircuit)
		{
			Datum datumInt = mTestInt;
			Datum datumString = mTestString;
			Assert::IsTrue(datumInt != datumString);

			Datum biggerDatumInt = mTestInt;
			biggerDatumInt.PushBack(mOtherTestInt);
			Assert::IsTrue(datumInt != datumString);
		}

		TEST_METHOD(Clear_CantClearExternal)
		{
			Datum externalInt;
			externalInt.SetStorage(mTestIntArray, 2);
			auto clearExternalExcept = [this, &externalInt]() {externalInt.Clear(); };
			Assert::ExpectException<std::exception>(clearExternalExcept);
		}

		TEST_METHOD(Find_IntWorks)
		{
			Datum findIntDatum = mTestInt;
			findIntDatum.PushBack(mOtherTestInt);
			uint32_t index = 99;
			Assert::IsTrue(findIntDatum.Find(mTestInt, index));
			Assert::AreEqual(0u, index);
			Assert::IsTrue(findIntDatum.Find(mOtherTestInt, index));
			Assert::AreEqual(1u, index);

			index = 99;
			Assert::IsFalse(findIntDatum.Find(99999, index));
			Assert::AreEqual(99u, index);
		}

		TEST_METHOD(Find_FloatWorks)
		{
			Datum findFloatDatum = mTestFloat;
			findFloatDatum.PushBack(mOtherTestFloat);
			uint32_t index = 99;
			Assert::IsTrue(findFloatDatum.Find(mTestFloat, index));
			Assert::AreEqual(0u, index);
			Assert::IsTrue(findFloatDatum.Find(mOtherTestFloat, index));
			Assert::AreEqual(1u, index);

			index = 99;
			Assert::IsFalse(findFloatDatum.Find(99999, index));
			Assert::AreEqual(99u, index);
		}

		TEST_METHOD(Find_VecWorks)
		{
			Datum findVecDatum = mTestVec;
			findVecDatum.PushBack(mOtherTestVec);
			uint32_t index = 99;
			Assert::IsTrue(findVecDatum.Find(mTestVec, index));
			Assert::AreEqual(0u, index);
			Assert::IsTrue(findVecDatum.Find(mOtherTestVec, index));
			Assert::AreEqual(1u, index);

			index = 99;
			Assert::IsFalse(findVecDatum.Find(99999, index));
			Assert::AreEqual(99u, index);
		}

		TEST_METHOD(Find_MatrixWorks)
		{
			Datum findMatrixDatum = mTestMatrix;
			findMatrixDatum.PushBack(mOtherTestMatrix);
			uint32_t index = 99;
			Assert::IsTrue(findMatrixDatum.Find(mTestMatrix, index));
			Assert::AreEqual(0u, index);
			Assert::IsTrue(findMatrixDatum.Find(mOtherTestMatrix, index));
			Assert::AreEqual(1u, index);

			index = 99;
			Assert::IsFalse(findMatrixDatum.Find(99999, index));
			Assert::AreEqual(99u, index);
		}

		TEST_METHOD(Find_StringWorks)
		{
			Datum findStringDatum = mTestString;
			findStringDatum.PushBack(mOtherTestString);
			uint32_t index = 99;
			Assert::IsTrue(findStringDatum.Find(mTestString, index));
			Assert::AreEqual(0u, index);
			Assert::IsTrue(findStringDatum.Find(mOtherTestString, index));
			Assert::AreEqual(1u, index);

			index = 99;
			Assert::IsFalse(findStringDatum.Find(99999, index));
			Assert::AreEqual(99u, index);
		}

		TEST_METHOD(Find_RTTIWorks)
		{
			Datum findRTTIDatum = mTestRTTI;
			findRTTIDatum.PushBack(mOtherTestRTTI);
			uint32_t index = 99;
			Assert::IsTrue(findRTTIDatum.Find(mTestRTTI, index));
			Assert::AreEqual(0u, index);
			Assert::IsTrue(findRTTIDatum.Find(mOtherTestRTTI, index));
			Assert::AreEqual(1u, index);

			index = 99;
			Assert::IsFalse(findRTTIDatum.Find(99999, index));
			Assert::AreEqual(99u, index);
		}

		TEST_METHOD(Find_ScopeWorks)
		{
			Datum findScopeDatum = mTestScope;
			findScopeDatum.PushBack(mOtherTestScope);
			uint32_t index = 99;
			Assert::IsTrue(findScopeDatum.Find(*mTestScope, index));
			Assert::AreEqual(0u, index);
			Assert::IsTrue(findScopeDatum.Find(*mOtherTestScope, index));
			Assert::AreEqual(1u, index);

			index = 99;
			Assert::IsFalse(findScopeDatum.Find(99999, index));
			Assert::AreEqual(99u, index);
		}
	};
	_CrtMemState DatumTest::sStartMemState;
}
