#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Library::SList<int>::Iterator>(const Library::SList<int>::Iterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(SListTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:

		struct DeepCopyTester
		{
			int mInt;
			int* mIntPtr;
			DeepCopyTester::DeepCopyTester()
				:mInt(0)
				,mIntPtr(new int(0))
			{}

			DeepCopyTester::DeepCopyTester(int inInt)
				:mInt(inInt)
				,mIntPtr(new int(0))
			{}

			DeepCopyTester::DeepCopyTester(int inInt, int inIntPtrVal)
				:mInt(inInt)
				,mIntPtr(new int(inIntPtrVal))
			{}

			DeepCopyTester::DeepCopyTester(const DeepCopyTester& rhs)
				:mInt(rhs.mInt)
				,mIntPtr(new int(*rhs.mIntPtr))
			{}

			DeepCopyTester& DeepCopyTester::operator=(const DeepCopyTester& rhs)
			{
				mInt = rhs.mInt;
				if (mIntPtr != nullptr) {
					delete mIntPtr;
				}
				mIntPtr = new int(*rhs.mIntPtr);
			}

			DeepCopyTester::~DeepCopyTester()
			{
				if (mIntPtr != nullptr) {
					delete mIntPtr;
					mIntPtr = nullptr;
				}
			}
		};

		void FrontToExceptConst(const Library::SList<int>& list) {
			list.Front();
		}
		void FrontToExcept(Library::SList<int>& list) {
			list.Front();
		}
		void BackToExceptConst(const Library::SList<int>& list) {
			list.Back();
		}
		void BackToExcept(Library::SList<int>& list) {
			list.Back();
		}
		void PopFrontToExcept(Library::SList<int>& list) {
			list.PopFront();
		}
		void ItRefFail() {
			auto it = mIntSList.end();
			int& test = *it;
			test;
		}
		void PopulateIntList(int populateCount) {
			for (int i = 0; i < populateCount; ++i) {
				mIntSList.PushBack(i);
			}
		}

		DeepCopyTester mDeepCopyTester;
		int mDeepCopyInt;
		int mTestInt;
		int mSizeTest;
		Library::SList<int> mIntSList;
		Library::SList<DeepCopyTester> mDCPSList;
		Library::SList<int*> mIntPtrSList;

		TEST_METHOD_INITIALIZE(setup)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			mTestInt = 5;
			mSizeTest = 10;
			mDeepCopyInt = 10;

			mDeepCopyTester.mInt = mDeepCopyInt;
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			//Removing all pointers so there should be no extra memory hanging around
			mIntSList.Clear();
			mDCPSList.Clear();
			mIntPtrSList.Clear();
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
			Assert::AreEqual(0, mIntSList.Size());
			Assert::IsTrue(mIntSList.IsEmpty());
		}

		TEST_METHOD(PushBack_Simple)
		{
			mIntSList.PushBack(mTestInt);
			Assert::AreEqual(1, mIntSList.Size());
			Assert::IsFalse(mIntSList.IsEmpty());
			Assert::AreEqual(mTestInt, mIntSList.Back());
		}

		TEST_METHOD(PushBack_Multiple)
		{
			mIntSList.PushFront(0);
			mIntSList.PushBack(0);
			mIntSList.PushBack(mTestInt);
			Assert::AreEqual(mTestInt, mIntSList.Back());
			Assert::AreEqual(0, mIntSList.Front());
		}

		TEST_METHOD(PushBack_CopiesInput)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			Assert::AreEqual(mDeepCopyTester.mInt, mDCPSList.Back().mInt);
			mDeepCopyTester.mInt++;
			Assert::AreNotEqual(mDeepCopyTester.mInt, mDCPSList.Back().mInt);
		}

		TEST_METHOD(PushBack_Pointer)
		{
			int intToAdd = 10;
			mIntPtrSList.PushBack(&intToAdd);
			Assert::AreEqual(&intToAdd, mIntPtrSList.Back());

			Assert::AreEqual(intToAdd, *(mIntPtrSList.Back()));
			intToAdd++;
			Assert::AreEqual(intToAdd, *(mIntPtrSList.Back()));

			//This will break if the implementation is incorrect, or at least a kitten will die
			mIntPtrSList.Clear();
		}

		TEST_METHOD(PushFront_Simple)
		{
			mIntSList.PushFront(mTestInt);
			Assert::AreEqual(1, mIntSList.Size());
			Assert::IsFalse(mIntSList.IsEmpty());
			Assert::AreEqual(mTestInt, mIntSList.Front());
		}

		TEST_METHOD(PushFront_Multiple)
		{
			mIntSList.PushFront(0);
			mIntSList.PushBack(0);
			mIntSList.PushFront(mTestInt);
			Assert::AreEqual(mTestInt, mIntSList.Front());
			Assert::AreEqual(0, mIntSList.Back());
		}

		TEST_METHOD(PushFront_CopiesInput)
		{
			mDCPSList.PushFront(mDeepCopyTester);
			Assert::AreEqual(mDeepCopyTester.mInt, mDCPSList.Front().mInt);
			mDeepCopyTester.mInt++;
			Assert::AreNotEqual(mDeepCopyTester.mInt, mDCPSList.Front().mInt);
		}

		TEST_METHOD(PushFront_Pointer)
		{
			int intToAdd = 10;
			mIntPtrSList.PushFront(&intToAdd);
			Assert::AreEqual(&intToAdd, mIntPtrSList.Front());

			Assert::AreEqual(intToAdd, *(mIntPtrSList.Front()));
			intToAdd++;
			Assert::AreEqual(intToAdd, *(mIntPtrSList.Front()));

			//This will break if the implementation is incorrect, or at least a kitten will die
			mIntPtrSList.Clear();
		}

		TEST_METHOD(FrontAndBackAreSameWithOneValue)
		{
			mIntSList.PushFront(mTestInt);
			Assert::AreEqual(mIntSList.Front(), mIntSList.Back());
		}

		TEST_METHOD(PopFront_Simple)
		{
			mIntSList.PushFront(0);
			mIntSList.PopFront();
			Assert::IsTrue(mIntSList.IsEmpty());
			Assert::AreEqual(0, mIntSList.Size());
		}

		TEST_METHOD(PopFront_Multiple)
		{
			mIntSList.PushFront(0);
			mIntSList.PushBack(0);
			mIntSList.PopFront();
			Assert::IsFalse(mIntSList.IsEmpty());
			Assert::AreEqual(1, mIntSList.Size());
			mIntSList.PopFront();
			Assert::IsTrue(mIntSList.IsEmpty());
			Assert::AreEqual(0, mIntSList.Size());
		}

		TEST_METHOD(PopFront_ThrowsExceptionIfEmpty)
		{
			auto popFrontFunc = [this] {PopFrontToExcept(mIntSList); };
			Assert::ExpectException<std::exception>(popFrontFunc);
		}

		TEST_METHOD(IsEmpty)
		{
			Assert::IsTrue(mIntSList.IsEmpty());
			mIntSList.PushFront(0);
			Assert::IsFalse(mIntSList.IsEmpty());
			mIntSList.PopFront();
			Assert::IsTrue(mIntSList.IsEmpty());
			mIntSList.PushBack(0);
			Assert::IsFalse(mIntSList.IsEmpty());
			mIntSList.Clear();
			Assert::IsTrue(mIntSList.IsEmpty());
		}

		TEST_METHOD(Front_Simple)
		{
			auto frontFunc = [this] {FrontToExcept(mIntSList); };
			Assert::ExpectException<std::exception>(frontFunc);
			mIntSList.PushFront(mTestInt);
			Assert::AreEqual(mTestInt, mIntSList.Front());
		}

		TEST_METHOD(Front_Const)
		{
			auto frontFunc = [this] {FrontToExceptConst(mIntSList); };
			Assert::ExpectException<std::exception>(frontFunc);
			mIntSList.PushFront(mTestInt);
			const Library::SList<int>& mConstIntSList = mIntSList;
			const int& constInt = mConstIntSList.Front();
			Assert::AreEqual(mTestInt, constInt);
		}

		TEST_METHOD(Front_ReturnsReference)
		{
			mDCPSList.PushFront(mDeepCopyTester);
			DeepCopyTester& deepCopyRef = mDCPSList.Front();
			deepCopyRef.mInt++;
			Assert::AreEqual(deepCopyRef.mInt, mDCPSList.Front().mInt);
			Assert::AreNotEqual(mDeepCopyTester.mInt, mDCPSList.Front().mInt);
		}

		TEST_METHOD(Back_Simple)
		{
			auto backFunc = [this] {BackToExcept(mIntSList); };
			Assert::ExpectException<std::exception>(backFunc);
			mIntSList.PushBack(mTestInt);
			Assert::AreEqual(mTestInt, mIntSList.Back());
		}

		TEST_METHOD(Back_Const)
		{
			auto backFunc = [this] {BackToExceptConst(mIntSList); };
			Assert::ExpectException<std::exception>(backFunc);
			mIntSList.PushBack(mTestInt);
			const Library::SList<int>& mConstIntSList = mIntSList;
			const int& constInt = mConstIntSList.Back();
			Assert::AreEqual(mTestInt, constInt);
		}

		TEST_METHOD(Back_ReturnsReference)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			DeepCopyTester& deepCopyRef = mDCPSList.Back();
			deepCopyRef.mInt++;
			Assert::AreEqual(deepCopyRef.mInt, mDCPSList.Back().mInt);
			Assert::AreNotEqual(mDeepCopyTester.mInt, mDCPSList.Back().mInt);
		}

		TEST_METHOD(Size)
		{
			Assert::AreEqual(0, mIntSList.Size());
			for (int i = 0; i < mSizeTest; i++) {
				Assert::AreEqual(i, mIntSList.Size());
				mIntSList.PushBack(mTestInt);
			}

			for (int i = mSizeTest; i > 0; i--) {
				Assert::AreEqual(i, mIntSList.Size());
				mIntSList.PopFront();
			}
			Assert::AreEqual(0, mIntSList.Size());
			for (int i = 0; i < mSizeTest; i++) {
				mIntSList.PushBack(mTestInt);
			}
			mIntSList.Clear();
			Assert::AreEqual(0, mIntSList.Size());
		}

		TEST_METHOD(Clear)
		{
			Assert::AreEqual(0, mIntSList.Size());
			Assert::IsTrue(mIntSList.IsEmpty());
			for (int i = 0; i < mSizeTest; i++) {
				mIntSList.PushBack(mTestInt);
			}
			Assert::AreNotEqual(0, mIntSList.Size());
			Assert::IsFalse(mIntSList.IsEmpty());
			mIntSList.Clear();
			Assert::AreEqual(0, mIntSList.Size());
			Assert::IsTrue(mIntSList.IsEmpty());
		}

		TEST_METHOD(CopyConstructor)
		{
			for (int i = 0; i < mSizeTest; i++) {
				mIntSList.PushBack(mTestInt);
			}
			Library::SList<int> copyList(mIntSList);
			Assert::AreEqual(mIntSList.Size(), copyList.Size());
			Assert::IsFalse(copyList.IsEmpty());
			Assert::AreEqual(mIntSList.Front(), copyList.Front());
			{
				int& frontRef = copyList.Front();
				frontRef++;
			}
			Assert::AreNotEqual(mIntSList.Front(), copyList.Front());
			mIntSList.PopFront();
			copyList.PopFront();

			while (!mIntSList.IsEmpty() && !copyList.IsEmpty()) {
				Assert::AreEqual(mIntSList.Front(), copyList.Front());
				mIntSList.PopFront();
				copyList.PopFront();
			}
			Assert::AreEqual(mIntSList.Size(), copyList.Size());
			Assert::IsTrue(copyList.IsEmpty());
		}

		TEST_METHOD(CopyConstructor_DeepCopies)
		{
			mDCPSList.PushBack(mDeepCopyTester);
			Library::SList<DeepCopyTester> copyList(mDCPSList);

			Assert::AreEqual(mDCPSList.Size(), copyList.Size());
			Assert::IsFalse(copyList.IsEmpty());
			Assert::AreEqual(mDCPSList.Front().mInt, copyList.Front().mInt);
			{
				DeepCopyTester& frontRef = copyList.Front();
				frontRef.mInt++;
			}
			Assert::AreNotEqual(mDCPSList.Front().mInt, copyList.Front().mInt);
		}

		TEST_METHOD(OperatorEquals)
		{
			Library::SList<int> copyList;
			copyList.PushBack(3);
			copyList.PushFront(2);
			copyList.PushBack(1);

			for (int i = 0; i < mSizeTest; i++) {
				mIntSList.PushBack(mTestInt);
			}

			copyList = mIntSList;

			Assert::AreEqual(mIntSList.Size(), copyList.Size());
			Assert::IsFalse(copyList.IsEmpty());
			Assert::AreEqual(mIntSList.Front(), copyList.Front());
			{
				int& frontRef = copyList.Front();
				frontRef++;
			}
			Assert::AreNotEqual(mIntSList.Front(), copyList.Front());
			mIntSList.PopFront();
			copyList.PopFront();

			while (!mIntSList.IsEmpty() && !copyList.IsEmpty()) {
				Assert::AreEqual(mIntSList.Front(), copyList.Front());
				mIntSList.PopFront();
				copyList.PopFront();
			}
			Assert::AreEqual(mIntSList.Size(), copyList.Size());
			Assert::IsTrue(copyList.IsEmpty());
		}

		TEST_METHOD(OperatorEquals_DeepCopies)
		{
			Library::SList<DeepCopyTester> copyList;
			mDCPSList.PushBack(mDeepCopyTester);

			copyList = mDCPSList;

			Assert::AreEqual(mDCPSList.Size(), copyList.Size());
			Assert::IsFalse(copyList.IsEmpty());
			Assert::AreEqual(mDCPSList.Front().mInt, copyList.Front().mInt);
			{
				DeepCopyTester& frontRef = copyList.Front();
				frontRef.mInt++;
			}
			Assert::AreNotEqual(mDCPSList.Front().mInt, copyList.Front().mInt);
		}

		TEST_METHOD(OperatorEquals_ShortCircuitTestPath)
		{
			Library::SList<int> copyList;
			copyList.PushBack(3);
			copyList.PushFront(2);
			copyList.PushBack(1);

			copyList = copyList;

			Assert::AreEqual(3, copyList.Size());
			Assert::IsFalse(copyList.IsEmpty());
		}

		TEST_METHOD(Iterator_DefaultConstructor)
		{
			Library::SList<int>::Iterator it;
			Library::SList<int>::Iterator otherIt;
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
			Library::SList<int>::Iterator it = mIntSList.begin();
			for (int i = 0; i < mSizeTest; i++) {
				Assert::AreEqual(i, (*it));
				Assert::IsFalse(mIntSList.end() == it);
				++it;
			}
			Assert::AreEqual(mIntSList.end(), it);
		}

		TEST_METHOD(Iterator_EqualsAndNotEqualsOperator)
		{
			PopulateIntList(mSizeTest);
			Library::SList<int> otherIntList;
			Library::SList<int>::Iterator it = mIntSList.begin();
			for (int i = 0; i < mSizeTest; i++) {
				Assert::IsTrue(it != mIntSList.end());

				//Note that we're checking that it doesn't equals the end of a different int list either
				Assert::IsTrue(it != otherIntList.end()); 
				++it;
			}
			Assert::IsTrue(it == mIntSList.end());
			Assert::IsTrue(it != otherIntList.end()); 
		}

		TEST_METHOD(InsertAfter)
		{
			mIntSList.PushBack(1);
			mIntSList.PushBack(2);
			mIntSList.PushBack(3);
			Library::SList<int>::Iterator it = mIntSList.begin();
			mIntSList.InsertAfter(it, 10);
			Assert::AreEqual(4, mIntSList.Size());
			Assert::AreEqual(1, (*it));
			++it;
			Assert::AreEqual(10, (*it));
			++it;
			Assert::AreEqual(2, (*it));
			mIntSList.InsertAfter(it, 20);
			Assert::AreEqual(5, mIntSList.Size());
			++it;
			Assert::AreEqual(20, (*it));
			++it;
			Assert::AreEqual(3, (*it));

			//Inserting at list.end() should be equivalent to PushBack();
			it = mIntSList.end();
			mIntSList.InsertAfter(it, 30);
			Assert::AreEqual(6, mIntSList.Size());
			Assert::AreEqual(30, mIntSList.Back());
		}

		TEST_METHOD(InsertAfter_WrongListThrowsException)
		{
			Library::SList<int> otherList;
			auto it = otherList.begin();
			auto wrongListException = [this, it]() { mIntSList.InsertAfter(it, 30); };
			Assert::ExpectException<std::exception>(wrongListException);
			otherList.PushBack(4);
			it = otherList.begin();
			auto otherWrongListException = [this, it]() { mIntSList.InsertAfter(it, 30); };
			Assert::ExpectException<std::exception>(otherWrongListException);
		}

		TEST_METHOD(Find)
		{
			mIntSList.PushBack(1);
			mIntSList.PushBack(2);
			mIntSList.PushBack(3);
			Library::SList<int>::Iterator it = mIntSList.Find(2);
			Assert::AreEqual(2, *it);
			it = mIntSList.Find(3);
			Assert::AreEqual(3, *it);
			it = mIntSList.Find(1);
			Assert::AreEqual(1, *it);
			Assert::AreEqual(mIntSList.begin(), it);
			it = mIntSList.Find(4);
			Assert::AreEqual(mIntSList.end(), it);
		}

		TEST_METHOD(RemoveFirst)
		{
			mIntSList.PushBack(1);
			mIntSList.PushBack(2);
			mIntSList.PushBack(1);
			mIntSList.PushBack(3);
			mIntSList.PushBack(1);
			mIntSList.RemoveFirst(1);
			Assert::AreEqual(4, mIntSList.Size());

			auto it = mIntSList.begin();
			Assert::AreEqual(2, *it);
			++it;
			Assert::AreEqual(1, *it);
			++it;
			Assert::AreEqual(3, *it);
			++it;
			Assert::AreEqual(1, *it);

			mIntSList.RemoveFirst(1);

			it = mIntSList.begin();
			Assert::AreEqual(3, mIntSList.Size());
			Assert::AreEqual(2, *it);
			++it;
			Assert::AreEqual(3, *it);
			++it;
			Assert::AreEqual(1, *it);

			mIntSList.RemoveFirst(1);

			it = mIntSList.begin();
			Assert::AreEqual(2, mIntSList.Size());
			Assert::AreEqual(2, *it);
			++it;
			Assert::AreEqual(3, *it);
			++it;
			Assert::AreEqual(mIntSList.end(), it);

			//This will break if implemented incorrectly. Should be a noop
			mIntSList.RemoveFirst(1);

			it = mIntSList.begin();
			Assert::AreEqual(2, mIntSList.Size());
			Assert::AreEqual(2, *it);
			++it;
			Assert::AreEqual(3, *it);
			++it;
			Assert::AreEqual(mIntSList.end(), it);
		}

		TEST_METHOD(RemoveAll)
		{
			mIntSList.PushBack(1);
			mIntSList.PushBack(2);
			mIntSList.PushBack(1);
			mIntSList.PushBack(3);
			mIntSList.PushBack(1);
			mIntSList.RemoveAll(1);
			Assert::AreEqual(2, mIntSList.Size());

			auto it = mIntSList.begin();
			Assert::AreEqual(2, *it);
			++it;
			Assert::AreEqual(3, *it);
			++it;
			Assert::AreEqual(mIntSList.end(), it);

			//This should be a noop
			mIntSList.RemoveFirst(1);

			it = mIntSList.begin();
			Assert::AreEqual(2, *it);
			++it;
			Assert::AreEqual(3, *it);
			++it;
			Assert::AreEqual(mIntSList.end(), it);
		}

		TEST_METHOD(IteratorDereferenceException)
		{
			auto it = mIntSList.end();
			auto exception = [this]() { ItRefFail(); };
			Assert::ExpectException<std::exception>(exception);
			mIntSList.PushBack(1);
			it = mIntSList.end();
			Assert::ExpectException<std::exception>(exception);

			const auto constIt = mIntSList.end();
			auto constException = [constIt]() { *constIt; };
			Assert::ExpectException<std::exception>(constException);
		}

		TEST_METHOD(IteratorDereference)
		{
			mIntSList.PushBack(1);
			auto it = mIntSList.begin();
			Assert::AreEqual(1, *it);
			const auto constIt = mIntSList.begin();
			const int& itRef = *constIt;
			Assert::AreEqual(1, itRef);
		}

		TEST_METHOD(IteratorIncrementor)
		{
			mIntSList.PushBack(1);
			mIntSList.PushBack(2);
			mIntSList.PushBack(3);
			auto it = mIntSList.begin();
			Assert::AreEqual(2, *(++it));
			Assert::AreEqual(2, *(it++));
			Assert::AreEqual(3, *it);
		}
	};
	_CrtMemState SListTest::sStartMemState;
}