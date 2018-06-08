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
		return (*this);
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

class DeepCopyHashFunctor {
public:
	uint32_t operator()(const DeepCopyTester& objectToHash) const {
		return objectToHash.mInt;
	}
};


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>::Iterator >(const Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>::Iterator& t)
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
	TEST_CLASS(HashMapTest)
	{
	private:
		static _CrtMemState sStartMemState;
	public:


		void InvalidIteratorExcept(Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>::Iterator& iter) {
			*iter;
		}

		void AtNotInHashExcept(Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>& hash, DeepCopyTester& notInHash) {
			hash.At(notInHash);
		}

		void EmptyHashExcept() {
			Library::HashMap<uint32_t, uint32_t> hash(0);
		}

		DeepCopyTester mDeepCopyTester;
		int mDeepCopyInt;
		int mTestInt;
		uint32_t mSizeTest;
		uint32_t mHashMapDefaultCapacity;

		TEST_METHOD_INITIALIZE(setup)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			mTestInt = 5;
			mSizeTest = 10;
			mDeepCopyInt = 10;
			mHashMapDefaultCapacity = 10;

			mDeepCopyTester.mInt = mDeepCopyInt;
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

		TEST_METHOD(DefaultCopyFunctor)
		{
			uint32_t firstTestInt = 10;
			uint32_t secondTestInt = 20;
			DeepCopyTester firstTester;
			DeepCopyTester secondTester;
			Library::DefaultComparatorFunctor<DeepCopyTester> deepCopyComparator;
			Assert::IsTrue(deepCopyComparator(firstTester, secondTester));
			firstTester.mInt = firstTestInt;
			secondTester.mInt = secondTestInt;
			Assert::IsFalse(deepCopyComparator(firstTester, secondTester));
			secondTester.mInt = firstTestInt;
			Assert::IsTrue(deepCopyComparator(firstTester, secondTester));

		}

		TEST_METHOD(CantMakeEmptyHash)
		{
			auto invalidDerefFunc = [this] {EmptyHashExcept(); };
			Assert::ExpectException<std::exception>(invalidDerefFunc);
		}


		TEST_METHOD(DefaultCopyFunctor_CharStar)
		{
			char* hello = "hello";
			char* bello = "bello";
			char firstHelloCopy[6];
			strcpy_s(firstHelloCopy, sizeof(firstHelloCopy), hello);
			char secondHelloCopy[6];
			strcpy_s(secondHelloCopy, sizeof(secondHelloCopy), hello);
			Library::DefaultComparatorFunctor<char*> deepCopyCharComparator;

			Assert::IsTrue(deepCopyCharComparator(firstHelloCopy, secondHelloCopy));
			strcpy_s(firstHelloCopy, sizeof(firstHelloCopy), bello);
			Assert::IsFalse(deepCopyCharComparator(firstHelloCopy, secondHelloCopy));
			strcpy_s(secondHelloCopy, sizeof(secondHelloCopy), bello);
			Assert::IsTrue(deepCopyCharComparator(firstHelloCopy, secondHelloCopy));
		}

		TEST_METHOD(DefaultHashFunctor)
		{
			uint32_t firstTestInt = 10;
			uint32_t secondTestInt = 20;
			DeepCopyTester firstTester;
			DeepCopyTester secondTester;
			Library::DefaultHashFunctor<DeepCopyTester> deepCopyHasher;
			Assert::AreNotEqual(deepCopyHasher(firstTester), deepCopyHasher(secondTester)); //Won't work because the bitwise hash function includes the pointer
			firstTester.mInt = firstTestInt;
			secondTester.mInt = secondTestInt;
			Assert::AreNotEqual(deepCopyHasher(firstTester), deepCopyHasher(secondTester));
			secondTester.mInt = firstTestInt;
			Assert::AreNotEqual(deepCopyHasher(firstTester), deepCopyHasher(secondTester));

			Library::DefaultHashFunctor<uint32_t> deepCopyUintComparator;
			Assert::AreEqual(deepCopyUintComparator(firstTestInt), deepCopyUintComparator(firstTestInt));
			Assert::AreNotEqual(deepCopyUintComparator(firstTestInt), deepCopyUintComparator(secondTestInt));
		}

		TEST_METHOD(DefaultHashFunctor_charstar)
		{
			char* hello = "hello";
			char* bello = "bello";
			char firstHelloCopy[6];
			strcpy_s(firstHelloCopy, sizeof(firstHelloCopy), hello);
			char secondHelloCopy[6];
			strcpy_s(secondHelloCopy, sizeof(secondHelloCopy), hello);
			Library::DefaultHashFunctor<char*> deepCopyCharComparator;

			Assert::AreEqual(deepCopyCharComparator(firstHelloCopy), deepCopyCharComparator(secondHelloCopy));
			strcpy_s(firstHelloCopy, sizeof(firstHelloCopy), bello);
			Assert::AreNotEqual(deepCopyCharComparator(firstHelloCopy), deepCopyCharComparator(secondHelloCopy));
			strcpy_s(secondHelloCopy, sizeof(secondHelloCopy), bello);
			Assert::AreEqual(deepCopyCharComparator(firstHelloCopy), deepCopyCharComparator(secondHelloCopy));
		}

		TEST_METHOD(DefaultHashFunctor_stdString)
		{
			std::string firstString("hello");
			std::string secondString("hello");
			std::string bello("bello");
			Library::DefaultHashFunctor<std::string> deepCopyStringComparator;

			Assert::AreEqual(deepCopyStringComparator(firstString), deepCopyStringComparator(secondString));
			firstString = bello;
			Assert::AreNotEqual(deepCopyStringComparator(firstString), deepCopyStringComparator(secondString));
			secondString = bello;
			Assert::AreEqual(deepCopyStringComparator(firstString), deepCopyStringComparator(secondString));
		}
		
		TEST_METHOD(DefaultConstructorTest)
		{
			Library::HashMap<DeepCopyTester,DeepCopyTester> capacityConstructorTester;
			Assert::AreEqual(0u, capacityConstructorTester.Size());
			Assert::IsTrue(capacityConstructorTester.IsEmpty());
		}

		TEST_METHOD(SizeConstructorTest)
		{
			uint32_t bucketCount = 20;
			Library::HashMap<DeepCopyTester,DeepCopyTester> capacityConstructorTester(bucketCount);
			Assert::AreEqual(0u, capacityConstructorTester.Size());
			Assert::IsTrue(capacityConstructorTester.IsEmpty());
			Assert::AreEqual(bucketCount, capacityConstructorTester.BucketCount());
		}

		TEST_METHOD(Insert)
		{
			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooInsertTestHash;
			DeepCopyTester firstTester;
			firstTester.mInt = 10;
			DeepCopyTester secondTester;
			secondTester.mInt = 20;
			fooInsertTestHash.Insert(std::make_pair(firstTester, secondTester));
			Assert::AreEqual(1u, fooInsertTestHash.Size());
			Assert::IsFalse(fooInsertTestHash.IsEmpty());

			Library::HashMap<uint32_t,DeepCopyTester> intInsertTestHash;
			intInsertTestHash.Insert(std::make_pair(1, firstTester));
			Assert::AreEqual(1u, intInsertTestHash.Size());
			Assert::IsFalse(intInsertTestHash.IsEmpty());
			intInsertTestHash.Insert(std::make_pair(2, firstTester));
			Assert::AreEqual(2u, intInsertTestHash.Size());
			Assert::IsFalse(intInsertTestHash.IsEmpty());
		}

		TEST_METHOD(Insert_IgnoresSameKey)
		{
			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooInsertTestHash;
			DeepCopyTester firstTester;
			firstTester.mInt = 10;
			DeepCopyTester secondTester;
			secondTester.mInt = 20;
			fooInsertTestHash.Insert(std::make_pair(firstTester, firstTester));
			Assert::AreEqual(firstTester, fooInsertTestHash.begin()->second);

			auto iter = fooInsertTestHash.Insert(std::make_pair(firstTester, secondTester));
			Assert::AreEqual(firstTester, iter->second);
			Assert::AreEqual(1u, fooInsertTestHash.Size());
		}

		TEST_METHOD(Find)
		{
			DeepCopyTester firstTester;
			DeepCopyTester secondTester;

			uint32_t firstTesterInt = 10;
			uint32_t secondTesterInt = 20;
			uint32_t firstLocation = 1;
			uint32_t secondLocation = 2;

			Library::HashMap<uint32_t,DeepCopyTester> intFindTestHash;
			intFindTestHash.Insert(std::make_pair(firstLocation, firstTester));
			intFindTestHash.Insert(std::make_pair(secondLocation, secondTester));
			Library::HashMap<uint32_t, DeepCopyTester>::Iterator finder = intFindTestHash.Find(firstLocation);
			Assert::AreEqual(firstTester, finder->second);
			finder = intFindTestHash.Find(secondLocation);
			Assert::AreEqual(secondTester, finder->second);

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooFindTestHash;
			firstTester.mInt = firstTesterInt;
			secondTester.mInt = secondTesterInt;
			fooFindTestHash.Insert(std::make_pair(firstTester, secondTester));
			fooFindTestHash.Insert(std::make_pair(secondTester, firstTester));
			Library::HashMap<DeepCopyTester, DeepCopyTester,DeepCopyHashFunctor>::Iterator deepCopyFinder = fooFindTestHash.Find(firstTester);
			Assert::AreEqual(secondTester, deepCopyFinder->second);
			deepCopyFinder = fooFindTestHash.Find(secondTester);
			Assert::AreEqual(firstTester, deepCopyFinder->second);
		}

		TEST_METHOD(Clear)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 10;
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 20;
			secondTester.mInt = secondTesterInt;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooFindTestHash;
			fooFindTestHash.Insert(std::make_pair(firstTester, secondTester));
			fooFindTestHash.Insert(std::make_pair(secondTester, firstTester));
			Assert::IsFalse(fooFindTestHash.IsEmpty());
			Assert::AreEqual(2u, fooFindTestHash.Size());

			fooFindTestHash.Clear();
			Assert::IsTrue(fooFindTestHash.IsEmpty());
			Assert::AreEqual(0u, fooFindTestHash.Size());

			fooFindTestHash.Insert(std::make_pair(secondTester, firstTester));
			Assert::AreEqual(1u, fooFindTestHash.Size());
			Assert::IsFalse(fooFindTestHash.IsEmpty());
		}

		TEST_METHOD(ContainsKey_basicType)
		{
			DeepCopyTester blankTester;
			uint32_t firstInt = 10;
			uint32_t secondInt = 20;
			uint32_t notInHash = 20;

			Library::HashMap<uint32_t,DeepCopyTester,DeepCopyHashFunctor> intContainsKeyHash;
			intContainsKeyHash.Insert(std::make_pair(firstInt, blankTester));
			intContainsKeyHash.Insert(std::make_pair(secondInt, blankTester));
			Assert::IsTrue(intContainsKeyHash.ContainsKey(firstInt));
			Assert::IsTrue(intContainsKeyHash.ContainsKey(secondInt));
			Assert::IsTrue(intContainsKeyHash.ContainsKey(notInHash));
		}

		TEST_METHOD(ContainsKey_userDefinedType)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 10;
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 20;
			secondTester.mInt = secondTesterInt;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooFindTestHash;
			fooFindTestHash.Insert(std::make_pair(firstTester, secondTester));
			fooFindTestHash.Insert(std::make_pair(secondTester, firstTester));
			Assert::IsFalse(fooFindTestHash.IsEmpty());
			Assert::AreEqual(2u, fooFindTestHash.Size());

			fooFindTestHash.Clear();
			Assert::IsTrue(fooFindTestHash.IsEmpty());
			Assert::AreEqual(0u, fooFindTestHash.Size());
		}

		TEST_METHOD(Remove)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 10;
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 20;
			secondTester.mInt = secondTesterInt;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooRemoveTestHash;
			fooRemoveTestHash.Remove(firstTester); //Should do nothing

			fooRemoveTestHash.Insert(std::make_pair(firstTester, secondTester));
			fooRemoveTestHash.Insert(std::make_pair(secondTester, firstTester));
			fooRemoveTestHash.Remove(firstTester);
			Assert::IsFalse(fooRemoveTestHash.ContainsKey(firstTester));
			Assert::AreEqual(1u, fooRemoveTestHash.Size());
			Assert::AreEqual(fooRemoveTestHash.end(), fooRemoveTestHash.Find(firstTester));

			fooRemoveTestHash.Remove(secondTester);
			Assert::IsFalse(fooRemoveTestHash.ContainsKey(secondTester));
			Assert::IsTrue(fooRemoveTestHash.IsEmpty());
			Assert::AreEqual(fooRemoveTestHash.end(), fooRemoveTestHash.Find(secondTester));

			fooRemoveTestHash.Remove(secondTester);//This should do nothing if correctly implemented
			Assert::IsFalse(fooRemoveTestHash.ContainsKey(secondTester));
			Assert::IsTrue(fooRemoveTestHash.IsEmpty());
			Assert::AreEqual(fooRemoveTestHash.end(), fooRemoveTestHash.Find(secondTester));
		}

		TEST_METHOD(HashMapIterator)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 1; //Because of our deepcopyhashfunctor, this should enter bucket 1
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 2; //Because of our deepcopyhashfunctor, this should enter bucket 2
			secondTester.mInt = secondTesterInt;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>::Iterator firstIter;
			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>::Iterator secondIter;
			Assert::AreEqual(firstIter, secondIter);

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooRemoveTestHash;
			fooRemoveTestHash.Insert(std::make_pair(firstTester, firstTester));
			fooRemoveTestHash.Insert(std::make_pair(secondTester, secondTester));
			firstIter = fooRemoveTestHash.begin();
			Assert::AreNotEqual(firstIter, secondIter);
			Assert::AreEqual(firstTester, firstIter->second);
			secondIter = fooRemoveTestHash.begin();
			++secondIter;
			Assert::AreNotEqual(secondIter, firstIter);
			Assert::AreEqual(secondTester, secondIter->second);
			++firstIter;
			Assert::AreEqual(firstIter, secondIter);
			++firstIter;
			Assert::AreEqual(fooRemoveTestHash.end(), firstIter);
			++firstIter; //If this is already at end, this should be a noop
			Assert::AreEqual(fooRemoveTestHash.end(), firstIter);
		}

		TEST_METHOD(HashMapIterator_CopyConstructor)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 1; //Because of our deepcopyhashfunctor, this should enter bucket 1
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 2; //Because of our deepcopyhashfunctor, this should enter bucket 2
			secondTester.mInt = secondTesterInt;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>::Iterator firstIter;
			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor>::Iterator secondIter(firstIter);
			Assert::AreEqual(firstIter, secondIter);

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooRemoveTestHash;
			fooRemoveTestHash.Insert(std::make_pair(firstTester, firstTester));
			fooRemoveTestHash.Insert(std::make_pair(secondTester, secondTester));
			firstIter = fooRemoveTestHash.begin();
			Assert::AreNotEqual(firstIter, secondIter);
			Assert::AreEqual(firstTester, firstIter->second);
			secondIter = fooRemoveTestHash.begin();
			++secondIter;
			Assert::AreNotEqual(secondIter, firstIter);
			Assert::AreEqual(secondTester, secondIter->second);
			++firstIter;
			Assert::AreEqual(firstIter, secondIter);
			++firstIter;
			Assert::AreEqual(fooRemoveTestHash.end(), firstIter);
			++firstIter; //If this is already at end, this should be a noop
			Assert::AreEqual(fooRemoveTestHash.end(), firstIter);
			Library::HashMap<DeepCopyTester, DeepCopyTester, DeepCopyHashFunctor>::Iterator testIter(firstIter);
			Assert::AreEqual(firstIter, testIter);
		}

		TEST_METHOD(HashMapIterator_EmptyHash)
		{
			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooRemoveTestHash;
			auto firstIter = fooRemoveTestHash.begin();
			auto secondIter = fooRemoveTestHash.end();
			Assert::AreEqual(firstIter, secondIter);
		}

		TEST_METHOD(HashMapIterator_IteratesThroughBuckets)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 1; //Because of our deepcopyhashfunctor, this should enter bucket 1
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 2; //Because of our deepcopyhashfunctor, this should enter bucket 2
			secondTester.mInt = secondTesterInt;
			DeepCopyTester thirdTester;
			uint32_t thirdTesterInt = 3; //Because of our deepcopyhashfunctor, this should enter bucket 3
			thirdTester.mInt = thirdTesterInt;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooRemoveTestHash(1);
			fooRemoveTestHash.Insert(std::make_pair(firstTester, firstTester));
			fooRemoveTestHash.Insert(std::make_pair(secondTester, secondTester));
			fooRemoveTestHash.Insert(std::make_pair(thirdTester, thirdTester));

			auto iter = fooRemoveTestHash.begin();
			Assert::AreEqual(firstTester, (iter++)->second);
			Assert::AreEqual(secondTester, iter->second);
			Assert::AreEqual(thirdTester, (++iter)->second);
			++iter;
			Assert::AreEqual(fooRemoveTestHash.end(), iter);

		}

		TEST_METHOD(HashMapIterator_ExceptsOnInvalidDereference)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 1;
			firstTester.mInt = firstTesterInt;
			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooRemoveTestHash(1);
			fooRemoveTestHash.Insert(std::make_pair(firstTester, firstTester));

			Library::HashMap<DeepCopyTester, DeepCopyTester, DeepCopyHashFunctor>::Iterator iter;
			auto invalidDerefFunc = [this, &iter] {InvalidIteratorExcept(iter); };
			Assert::ExpectException<std::exception>(invalidDerefFunc);
			iter = fooRemoveTestHash.end();
			auto invalidDerefEndFunc = [this, &iter] {InvalidIteratorExcept(iter); };
			Assert::ExpectException<std::exception>(invalidDerefEndFunc);
		}

		TEST_METHOD(IndexOperator)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 10;
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 20;
			secondTester.mInt = secondTesterInt;
			DeepCopyTester notInHash;
			DeepCopyTester defaultTester;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooFindTestHash;
			fooFindTestHash.Insert(std::make_pair(firstTester, firstTester));
			fooFindTestHash.Insert(std::make_pair(secondTester, secondTester));
			DeepCopyTester& retrieved = fooFindTestHash[firstTester];
			Assert::AreEqual(firstTester, retrieved);
			retrieved = fooFindTestHash[secondTester];
			Assert::AreEqual(secondTester, retrieved);

			retrieved = fooFindTestHash[notInHash];
			Assert::AreEqual(defaultTester, retrieved);
			Assert::AreEqual(3u, fooFindTestHash.Size());
		}

		TEST_METHOD(AtTest)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 10;
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 20;
			secondTester.mInt = secondTesterInt;
			DeepCopyTester notInHash;
			DeepCopyTester defaultTester;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooFindTestHash;
			fooFindTestHash.Insert(std::make_pair(firstTester, firstTester));
			fooFindTestHash.Insert(std::make_pair(secondTester, secondTester));
			DeepCopyTester& retrieved = fooFindTestHash.At(firstTester);
			Assert::AreEqual(firstTester, retrieved);
			retrieved = fooFindTestHash.At(secondTester);
			Assert::AreEqual(secondTester, retrieved);

			auto invalidAtFunc = [this, &fooFindTestHash, &notInHash] {AtNotInHashExcept(fooFindTestHash, notInHash); };
			Assert::ExpectException<std::exception>(invalidAtFunc);

			Assert::AreEqual(2u, fooFindTestHash.Size());
		}

		TEST_METHOD(DeepCopies)
		{
			DeepCopyTester firstTester;
			uint32_t firstTesterInt = 10;
			firstTester.mInt = firstTesterInt;
			DeepCopyTester secondTester;
			uint32_t secondTesterInt = 20;
			secondTester.mInt = secondTesterInt;

			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooFindTestHash;
			fooFindTestHash.Insert(std::make_pair(firstTester, firstTester));
			fooFindTestHash.Insert(std::make_pair(secondTester, secondTester));
			Library::HashMap<DeepCopyTester,DeepCopyTester,DeepCopyHashFunctor> fooFindTestHashCopy = fooFindTestHash;
			Assert::AreNotEqual(fooFindTestHash.begin()->second.mIntPtr, fooFindTestHashCopy.begin()->second.mIntPtr);
		}
	};
	_CrtMemState HashMapTest::sStartMemState;
}
