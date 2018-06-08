#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct DeepCopyTester
{
	uint32_t mInt;
	uint32_t* mIntPtr;
	DeepCopyTester::DeepCopyTester()
		:mInt(0)
		,mIntPtr(new uint32_t(0))
	{}

	DeepCopyTester::DeepCopyTester(uint32_t inInt)
		:mInt(inInt)
		,mIntPtr(new uint32_t(0))
	{}

	DeepCopyTester::DeepCopyTester(uint32_t inInt, uint32_t inIntPtrVal)
		:mInt(inInt)
		,mIntPtr(new uint32_t(inIntPtrVal))
	{}

	DeepCopyTester::DeepCopyTester(const DeepCopyTester& rhs)
		:mInt(rhs.mInt)
		,mIntPtr(new uint32_t(*rhs.mIntPtr))
	{}

	DeepCopyTester& DeepCopyTester::operator=(const DeepCopyTester& rhs)
	{
		mInt = rhs.mInt;
		if (mIntPtr != nullptr) {
			delete mIntPtr;
		}
		mIntPtr = new uint32_t(*rhs.mIntPtr);
	}

	bool DeepCopyTester::operator==(const DeepCopyTester& rhs) const
	{
		return (mInt == rhs.mInt) && (*mIntPtr) == (*rhs.mIntPtr);
	}

	DeepCopyTester::~DeepCopyTester()
	{
		if (mIntPtr != nullptr) {
			delete mIntPtr;
			mIntPtr = nullptr;
		}
	}
};

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Library::Vector<uint32_t>::Iterator>(const Library::Vector<uint32_t>::Iterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	std::wstring ToString<Library::Vector<DeepCopyTester>::Iterator>(const Library::Vector<DeepCopyTester>::Iterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	std::wstring ToString<DeepCopyTester>(const DeepCopyTester& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(VectorTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:


		void ReserveToExcept(Library::Vector<DeepCopyTester>& vector, uint32_t reserve) {
			vector.Reserve(reserve);
		}
		
		void FrontToExcept(Library::Vector<DeepCopyTester>& vector) {
			vector.Front();
		}
		void FrontToExceptConst(const Library::Vector<DeepCopyTester>& vector) {
			vector.Front();
		}
		void BackToExcept(Library::Vector<DeepCopyTester>& vector) {
			vector.Back();
		}
		void BackToExceptConst(const Library::Vector<DeepCopyTester>& vector) {
			vector.Back();
		}
		void PopBackToExcept(Library::Vector<DeepCopyTester>& vector) {
			vector.PopBack();
		}
		void AtToExcept(Library::Vector<DeepCopyTester>& vector, uint32_t index) {
			vector.At(index);
		}
		void ConstAtToExcept(const Library::Vector<DeepCopyTester>& vector, uint32_t index) {
			vector.At(index);
		}
		void AtOperatorToExcept(Library::Vector<DeepCopyTester>& vector, uint32_t index) {
			vector[index];
		}
		void ConstOperatorAtToExcept(const Library::Vector<DeepCopyTester>& vector, uint32_t index) {
			vector[index];
		}
		void RemoveToExcept(Library::Vector<DeepCopyTester>& vector, uint32_t index) {
			vector.Remove(index);
		}
		void PopulateIntList(uint32_t populateCount) {
			for (uint32_t i = 0; i < populateCount; ++i) {
				mIntSList.PushBack(i);
			}
		}
		void ItRefFail() {
			auto it = mIntSList.end();
			uint32_t& test = *it;
			test;
		}

		DeepCopyTester mDeepCopyTester;
		int mDeepCopyInt;
		int mTestInt;
		uint32_t mSizeTest;
		uint32_t mVectorDefaultCapacity;

		Library::Vector<uint32_t> mIntSList;
		Library::Vector<int*> mIntPtrSList;
		Library::Vector<DeepCopyTester> mDCPSList;

		TEST_METHOD_INITIALIZE(setup)
		{
			mIntSList.Clear();
			mIntSList.ShrinkToFit();
			mIntPtrSList.Clear();
			mIntPtrSList.ShrinkToFit();
			mDCPSList.Clear();
			mDCPSList.ShrinkToFit();
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			mTestInt = 5;
			mSizeTest = 10;
			mDeepCopyInt = 10;
			mVectorDefaultCapacity = 10;

			mDeepCopyTester.mInt = mDeepCopyInt;
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			//Removing all pointers so there should be no extra memory hanging around
			mIntSList.Clear();
			mIntSList.ShrinkToFit();
			mIntPtrSList.Clear();
			mIntPtrSList.ShrinkToFit();
			mDCPSList.Clear();
			mDCPSList.ShrinkToFit();
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
			Library::Vector<DeepCopyTester> capacityConstructorTester;
			Assert::AreEqual(0u, capacityConstructorTester.Size());
			Assert::IsTrue(capacityConstructorTester.IsEmpty());
			Assert::AreEqual(mVectorDefaultCapacity, capacityConstructorTester.Capacity());
		}

		TEST_METHOD(Capacity_Constructor)
		{
			uint32_t testCapacity = 5;
			Library::Vector<DeepCopyTester> capacityConstructorTester(testCapacity);
			Assert::AreEqual(0u, capacityConstructorTester.Size());
			Assert::IsTrue(capacityConstructorTester.IsEmpty());
			Assert::AreEqual(testCapacity, capacityConstructorTester.Capacity());
		}

		TEST_METHOD(CopyConstructor)
		{
			uint32_t testInt = 2;
			mDeepCopyTester.mInt = testInt;
			mDCPSList.PushBack(mDeepCopyTester);
			Library::Vector<DeepCopyTester> capacityConstructorTester(mDCPSList);
			Assert::AreEqual(1u, capacityConstructorTester.Size());
			Assert::IsFalse(capacityConstructorTester.IsEmpty());
			Assert::AreEqual(testInt, capacityConstructorTester.Front().mInt);
			Assert::AreEqual(mDCPSList.Capacity(), capacityConstructorTester.Capacity());
		}

		TEST_METHOD(CopyConstructor_DeepCopies)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			uint32_t* insertingPtr = mDCPSList.Front().mIntPtr;
			Library::Vector<DeepCopyTester> capacityConstructorTester(mDCPSList);
			Assert::AreNotEqual(insertingPtr, capacityConstructorTester.Front().mIntPtr);
		}

		TEST_METHOD(AssignmentOperator)
		{
			uint32_t testInt = 2;
			uint32_t wrongValues = 12;
			mDeepCopyTester.mInt = testInt;
			mDCPSList.PushBack(mDeepCopyTester);

			mDeepCopyTester.mInt = wrongValues;
			Library::Vector<DeepCopyTester> capacityConstructorTester(wrongValues);
			for (uint32_t i = 0; i < wrongValues; ++i) {
				capacityConstructorTester.PushBack(mDeepCopyTester);
			}
			capacityConstructorTester = mDCPSList;

			Assert::AreEqual(1u, capacityConstructorTester.Size());
			Assert::IsFalse(capacityConstructorTester.IsEmpty());
			Assert::AreEqual(testInt, capacityConstructorTester.Front().mInt);
			Assert::AreEqual(mDCPSList.Capacity(), capacityConstructorTester.Capacity());
		}

		TEST_METHOD(AssignmentOperator_DeepCopies)
		{
			uint32_t wrongValues = 12;
			mDCPSList.PushBack(mDeepCopyTester);
			uint32_t* insertingPtr = mDCPSList.Front().mIntPtr;

			Library::Vector<DeepCopyTester> capacityConstructorTester(wrongValues);
			for (uint32_t i = 0; i < wrongValues; ++i) {
				capacityConstructorTester.PushBack(mDeepCopyTester);
			}
			capacityConstructorTester = mDCPSList;

			Assert::AreNotEqual(insertingPtr, capacityConstructorTester.Front().mIntPtr);
		}
	
		TEST_METHOD(AssignmentOperator_ShortCircuits)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			uint32_t beforeCapacity = mDCPSList.Capacity();
			mDCPSList = mDCPSList;
			Assert::AreEqual(1u, mDCPSList.Size());
			Assert::IsFalse(mDCPSList.IsEmpty());
			Assert::AreEqual(mDeepCopyTester.mInt, mDCPSList.Front().mInt);
			Assert::AreEqual(beforeCapacity, mDCPSList.Capacity());
		}

		TEST_METHOD(PushBack_Simple)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			Assert::AreEqual(1U, mDCPSList.Size());
			Assert::IsFalse(mDCPSList.IsEmpty());
		}
		
		TEST_METHOD(PushBack_IncreaseCapacity)
		{
			uint32_t emptyCapacity = 0;
			Library::Vector<DeepCopyTester> capacityConstructorTester(emptyCapacity);
			capacityConstructorTester.PushBack(mDeepCopyTester);
			Assert::AreEqual(1U, capacityConstructorTester.Size());
			Assert::IsFalse(capacityConstructorTester.IsEmpty());
			Assert::AreEqual((emptyCapacity + 1) * 2, capacityConstructorTester.Capacity());
		}

		TEST_METHOD(PushBack_Multiple)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			mDCPSList.PushBack(mDeepCopyTester);
			Assert::AreEqual(2U, mDCPSList.Size());
			Assert::IsFalse(mDCPSList.IsEmpty());
		}

		TEST_METHOD(Reserve_basic)
		{
			uint32_t reserveSize = 3;
			mDCPSList.Reserve(reserveSize);
			Assert::AreEqual(0U, mDCPSList.Size());
			Assert::IsTrue(mDCPSList.IsEmpty());
			Assert::AreEqual(reserveSize, mDCPSList.Capacity());
		}
		
		TEST_METHOD(Reserve_shortCircuit)
		{
			uint32_t reserveSize = 3;
			mDCPSList.Reserve(reserveSize);
			mDCPSList.Reserve(reserveSize);
			Assert::AreEqual(0U, mDCPSList.Size());
			Assert::IsTrue(mDCPSList.IsEmpty());
			Assert::AreEqual(reserveSize, mDCPSList.Capacity());
		}

		TEST_METHOD(Reserve_throwsError)
		{
			uint32_t reserveSize = 3;
			mDCPSList.Reserve(reserveSize);
			mDCPSList.PushBack(mDeepCopyTester);
			mDCPSList.PushBack(mDeepCopyTester);

			auto frontFunc = [this] {ReserveToExcept(mDCPSList, 1); };
			Assert::ExpectException<std::exception>(frontFunc);

			Assert::AreEqual(2U, mDCPSList.Size());
			Assert::IsFalse(mDCPSList.IsEmpty());
			Assert::AreEqual(reserveSize, mDCPSList.Capacity());
		}

		TEST_METHOD(Reserve_shallowCopiesDataCorrectly)
		{
			uint32_t reserveSize = 3;
			mDCPSList.Reserve(reserveSize);
			
			mDCPSList.PushBack(mDeepCopyTester);
			uint32_t insertedInt = mDCPSList.Front().mInt;
			uint32_t* insertedPtr = mDCPSList.Front().mIntPtr;
			++reserveSize;
			mDCPSList.Reserve(reserveSize);

			Assert::AreEqual(insertedPtr, mDCPSList.Front().mIntPtr);
			Assert::AreEqual(insertedInt, mDCPSList.Front().mInt);
			Assert::AreEqual(reserveSize, mDCPSList.Capacity());
		}

		TEST_METHOD(Front)
		{
			uint32_t correctVal = 2;
			uint32_t wrongVal = 4;
			mDeepCopyTester.mInt = correctVal;
			mDCPSList.PushBack(mDeepCopyTester);
			mDeepCopyTester.mInt = wrongVal;
			mDCPSList.PushBack(mDeepCopyTester);

			Assert::AreEqual(correctVal, mDCPSList.Front().mInt);
		}

		TEST_METHOD(Front_exception)
		{
			auto frontFunc = [this] {FrontToExcept(mDCPSList); };
			Assert::ExpectException<std::exception>(frontFunc);
		}
		
		TEST_METHOD(ConstFront)
		{
			uint32_t correctVal = 2;
			uint32_t wrongVal = 4;
			mDeepCopyTester.mInt = correctVal;
			mDCPSList.PushBack(mDeepCopyTester);
			mDeepCopyTester.mInt = wrongVal;
			mDCPSList.PushBack(mDeepCopyTester);
			const Library::Vector<DeepCopyTester> mDCPSListConst = mDCPSList;

			Assert::AreEqual(correctVal, mDCPSListConst.Front().mInt);
		}

		TEST_METHOD(ConstFront_exception)
		{
			const Library::Vector<DeepCopyTester> mDCPSListConst = mDCPSList;
			auto frontFunc = [this, mDCPSListConst] () {FrontToExceptConst(mDCPSListConst); };
			Assert::ExpectException<std::exception>(frontFunc);
		}
		
		TEST_METHOD(Back)
		{
			uint32_t correctVal = 2;
			uint32_t wrongVal = 4;
			mDeepCopyTester.mInt = wrongVal;
			mDCPSList.PushBack(mDeepCopyTester);
			mDeepCopyTester.mInt = correctVal;
			mDCPSList.PushBack(mDeepCopyTester);

			Assert::AreEqual(correctVal, mDCPSList.Back().mInt);
		}

		TEST_METHOD(Back_exception)
		{
			auto backFunc = [this] {BackToExcept(mDCPSList); };
			Assert::ExpectException<std::exception>(backFunc);
		}
		
		TEST_METHOD(ConstBack)
		{
			uint32_t correctVal = 2;
			uint32_t wrongVal = 4;
			mDeepCopyTester.mInt = wrongVal;
			mDCPSList.PushBack(mDeepCopyTester);
			mDeepCopyTester.mInt = correctVal;
			mDCPSList.PushBack(mDeepCopyTester);
			const Library::Vector<DeepCopyTester> mDCPSListConst = mDCPSList;

			Assert::AreEqual(correctVal, mDCPSListConst.Back().mInt);
		}

		TEST_METHOD(ConstBack_exception)
		{
			const Library::Vector<DeepCopyTester> mDCPSListConst = mDCPSList;
			auto backFunc = [this, mDCPSListConst] () {BackToExceptConst(mDCPSListConst); };
			Assert::ExpectException<std::exception>(backFunc);
		}

		TEST_METHOD(PopBack)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			mDCPSList.PushBack(mDeepCopyTester);
			Assert::AreEqual(2U, mDCPSList.Size());
			mDCPSList.PopBack();
			Assert::AreEqual(1U, mDCPSList.Size());
			Assert::IsFalse(mDCPSList.IsEmpty());
			mDCPSList.PopBack();
			Assert::AreEqual(0U, mDCPSList.Size());
			Assert::IsTrue(mDCPSList.IsEmpty());
		}
		
		TEST_METHOD(PopBack_EmptyException)
		{
			auto frontFunc = [this] () {PopBackToExcept(mDCPSList); };
			Assert::ExpectException<std::exception>(frontFunc);
		}
		
		TEST_METHOD(ShrinkToFit)
		{
			uint32_t shrinkSize = 10;
			mDCPSList.Reserve(shrinkSize * 2);
			for (uint32_t i = 0; i < shrinkSize; ++i) {
				mDCPSList.PushBack(mDeepCopyTester);
			}
			Assert::AreEqual(shrinkSize, mDCPSList.Size());
			Assert::AreEqual(shrinkSize * 2, mDCPSList.Capacity());
			mDCPSList.ShrinkToFit();
			Assert::AreEqual(shrinkSize, mDCPSList.Size());
			Assert::AreEqual(shrinkSize, mDCPSList.Capacity());
			Assert::IsFalse(mDCPSList.IsEmpty());
		}

		TEST_METHOD(ShrinkToFit_ShortCircuits)
		{
			uint32_t shrinkSize = 10;
			mDCPSList.Reserve(shrinkSize);
			for (uint32_t i = 0; i < shrinkSize; ++i) {
				mDCPSList.PushBack(mDeepCopyTester);
			}
			Assert::AreEqual(shrinkSize, mDCPSList.Size());
			Assert::AreEqual(shrinkSize, mDCPSList.Capacity());
			mDCPSList.ShrinkToFit();
			Assert::AreEqual(shrinkSize, mDCPSList.Size());
			Assert::AreEqual(shrinkSize, mDCPSList.Capacity());
			Assert::IsFalse(mDCPSList.IsEmpty());
		}

		TEST_METHOD(ShrinkToFit_SetsToZero)
		{
			uint32_t capacity = 10;
			mDCPSList.Reserve(capacity);
			Assert::AreEqual(0u, mDCPSList.Size());
			Assert::AreEqual(capacity, mDCPSList.Capacity());
			Assert::IsTrue(mDCPSList.IsEmpty());
			mDCPSList.ShrinkToFit();
			Assert::AreEqual(0u, mDCPSList.Size());
			Assert::AreEqual(0u, mDCPSList.Capacity());
			Assert::IsTrue(mDCPSList.IsEmpty());
		}

		TEST_METHOD(At)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			Assert::AreEqual(mDCPSList.Front().mInt, mDCPSList.At(0).mInt);
			Assert::AreEqual(mDCPSList.Front().mIntPtr, mDCPSList.At(0).mIntPtr);
		}

		TEST_METHOD(ConstAt)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			const Library::Vector<DeepCopyTester> mDCPSListConst = mDCPSList;
			Assert::AreEqual(mDCPSListConst.Front().mInt, mDCPSListConst.At(0).mInt);
			Assert::AreEqual(mDCPSListConst.Front().mIntPtr, mDCPSListConst.At(0).mIntPtr);
		}

		TEST_METHOD(At_Exception)
		{
			auto atFunc = [this] () {AtToExcept(mDCPSList, 0); };
			Assert::ExpectException<std::exception>(atFunc);
		}

		TEST_METHOD(ConstAt_Exception)
		{
			auto atFunc = [this] () {ConstAtToExcept(mDCPSList, 0); };
			Assert::ExpectException<std::exception>(atFunc);
		}

		TEST_METHOD(AtOperator)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			Assert::AreEqual(mDCPSList.Front().mInt, mDCPSList[0].mInt);
			Assert::AreEqual(mDCPSList.Front().mIntPtr, mDCPSList[0].mIntPtr);
		}

		TEST_METHOD(ConstAtOperator)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			const Library::Vector<DeepCopyTester> mDCPSListConst = mDCPSList;
			Assert::AreEqual(mDCPSListConst.Front().mInt, mDCPSListConst[0].mInt);
			Assert::AreEqual(mDCPSListConst.Front().mIntPtr, mDCPSListConst[0].mIntPtr);
		}

		TEST_METHOD(AtOperator_Exception)
		{
			auto atFunc = [this] () {AtOperatorToExcept(mDCPSList, 0); };
			Assert::ExpectException<std::exception>(atFunc);
		}

		TEST_METHOD(ConstAtOperator_Exception)
		{
			auto atFunc = [this] () {ConstOperatorAtToExcept(mDCPSList, 0); };
			Assert::ExpectException<std::exception>(atFunc);
		}

		TEST_METHOD(Remove)
		{
			uint32_t capacity = 10;
			uint32_t first = 1;
			uint32_t second = 2;
			uint32_t third = 3;
			mDCPSList.Reserve(capacity);
			mDeepCopyTester.mInt = first;
			mDCPSList.PushBack(mDeepCopyTester);
			mDeepCopyTester.mInt = second;
			mDCPSList.PushBack(mDeepCopyTester);
			mDeepCopyTester.mInt = third;
			mDCPSList.PushBack(mDeepCopyTester);
			mDCPSList.Remove(1);
			Assert::AreEqual(2U, mDCPSList.Size());
			Assert::AreEqual(capacity, mDCPSList.Capacity());
			Assert::AreEqual(first, mDCPSList.Front().mInt);
			Assert::AreEqual(third, mDCPSList.Back().mInt);
			mDCPSList.Remove(1);
			Assert::AreEqual(1U, mDCPSList.Size());
			Assert::AreEqual(capacity, mDCPSList.Capacity());
			Assert::AreEqual(first, mDCPSList.Front().mInt);
			Assert::AreEqual(first, mDCPSList.Back().mInt);
			mDCPSList.Remove(0);
			Assert::AreEqual(0U, mDCPSList.Size());
			Assert::AreEqual(capacity, mDCPSList.Capacity());
		}

		TEST_METHOD(Remove_EmptyVectorException)
		{
			auto atFunc = [this] () {RemoveToExcept(mDCPSList, 0); };
			Assert::ExpectException<std::exception>(atFunc);
		}

		TEST_METHOD(Iterator_DefaultConstructor)
		{
			Library::Vector<uint32_t>::Iterator it;
			Library::Vector<uint32_t>::Iterator otherIt;
			Assert::AreEqual(it, otherIt);

			//Because it hasn't been initialized, this should short circuit and not do anything
			++it;
			Assert::AreEqual(it, otherIt);
			it = mIntSList.begin();
			Assert::AreNotEqual(it, otherIt);
			otherIt = mIntSList.begin();
			Assert::AreEqual(it, otherIt);
		}

		TEST_METHOD(Iterator_Simple)
		{
			PopulateIntList(mSizeTest);
			Library::Vector<uint32_t>::Iterator it = mIntSList.begin();
			for (uint32_t i = 0; i < mSizeTest; i++) {
				Assert::AreEqual(i, (*it));
				Assert::IsFalse(mIntSList.end() == it);
				++it;
			}
			Assert::AreEqual(mIntSList.end(), it);
		}

		TEST_METHOD(Iterator_EqualsAndNotEqualsOperator)
		{
			PopulateIntList(mSizeTest);
			Library::Vector<uint32_t> otherIntList;
			Library::Vector<uint32_t>::Iterator it = mIntSList.begin();
			for (uint32_t i = 0; i < mSizeTest; i++) {
				Assert::IsTrue(it != mIntSList.end());

				//Note that we're checking that it doesn't equals the end of a different int list either
				Assert::IsTrue(it != otherIntList.end()); 
				++it;
			}
			Assert::IsTrue(it == mIntSList.end());
			Assert::IsTrue(it != otherIntList.end()); 
		}

		TEST_METHOD(IteratorDereferenceException)
		{
			auto it = mIntSList.end();
			auto exception = [this]() { ItRefFail(); };
			Assert::ExpectException<std::exception>(exception);
			mIntSList.PushBack(1);
			it = mIntSList.end();
			Assert::ExpectException<std::exception>(exception);
		}

		TEST_METHOD(IteratorDereference)
		{
			uint32_t testPush = 1;
			mIntSList.PushBack(testPush);
			auto it = mIntSList.begin();
			Assert::AreEqual(testPush, *it);
		}

		TEST_METHOD(IteratorIncrementor)
		{
			uint32_t first = 1;
			uint32_t second = 2;
			uint32_t third = 3;
			mIntSList.PushBack(first);
			mIntSList.PushBack(second);
			mIntSList.PushBack(third);
			auto it = mIntSList.begin();
			Assert::AreEqual(second, *(++it));
			Assert::AreEqual(second, *(it++));
			Assert::AreEqual(third, *it);
			++it;
			Assert::AreEqual(mIntSList.end(), it);
			++it;
			Assert::AreEqual(mIntSList.end(), it);
			it++;
			Assert::AreEqual(mIntSList.end(), it);
		}

		TEST_METHOD(IncrementOwnerlessIterator)
		{
			Library::Vector<uint32_t>::Iterator it;
			Library::Vector<uint32_t>::Iterator otherIt;
			it++;
			Assert::AreEqual(it, otherIt);
			++it;
			Assert::AreEqual(it, otherIt);
		}

		TEST_METHOD(Find)
		{
			uint32_t first = 1;
			uint32_t second = 2;
			uint32_t third = 3;
			uint32_t notInList = 999;
			mDeepCopyTester.mInt = first;
			mDCPSList.PushBack(mDeepCopyTester);
			mDeepCopyTester.mInt = second;
			mDCPSList.PushBack(mDeepCopyTester);

			mDeepCopyTester.mInt = third;
			mDCPSList.PushBack(mDeepCopyTester);
			Library::Vector<DeepCopyTester>::Iterator it = mDCPSList.Find(mDeepCopyTester);
			Assert::AreEqual(mDCPSList.At(2), *it);

			mDeepCopyTester.mInt = second;
			it = mDCPSList.Find(mDeepCopyTester);
			Assert::AreEqual(mDCPSList.At(1), *it);

			mDeepCopyTester.mInt =notInList;
			it = mDCPSList.Find(mDeepCopyTester);
			Assert::AreEqual(mDCPSList.end(), it);

			mDeepCopyTester.mInt = 1;
		}
	};
	_CrtMemState VectorTest::sStartMemState;
}
