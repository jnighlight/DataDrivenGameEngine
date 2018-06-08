#pragma once
#include "Factory.h"
#include "RTTI.h"
#include <string>

namespace Library {
	class DeepCopyFactoryTest : public Library::RTTI
	{
	public:
		int mInt;
		int* mIntPtr;
		DeepCopyFactoryTest::DeepCopyFactoryTest()
			:mInt(0)
			,mIntPtr(new int(0))
		{}

		DeepCopyFactoryTest::DeepCopyFactoryTest(int inInt)
			:mInt(inInt)
			,mIntPtr(new int(0))
		{}

		DeepCopyFactoryTest::DeepCopyFactoryTest(int inInt, int inIntPtrVal)
			:mInt(inInt)
			,mIntPtr(new int(inIntPtrVal))
		{}

		DeepCopyFactoryTest::DeepCopyFactoryTest(const DeepCopyFactoryTest& rhs)
			:mInt(rhs.mInt)
			,mIntPtr(new int(*rhs.mIntPtr))
		{}

		DeepCopyFactoryTest& DeepCopyFactoryTest::operator=(const DeepCopyFactoryTest& rhs)
		{
			mInt = rhs.mInt;
			if (mIntPtr != nullptr) {
				delete mIntPtr;
			}
			mIntPtr = new int(*rhs.mIntPtr);
		}

		DeepCopyFactoryTest::~DeepCopyFactoryTest()
		{
			if (mIntPtr != nullptr) {
				delete mIntPtr;
				mIntPtr = nullptr;
			}
		}

		RTTI_DECLARATIONS(DeepCopyFactoryTest, Library::RTTI);
	};
	RTTI_DEFINITIONS(DeepCopyFactoryTest);

	class FooRTTI : public Library::RTTI
	{
	public:
		int mInt;
		int* mIntPtr;
		FooRTTI::FooRTTI()
			:mInt(0)
			,mIntPtr(new int(0))
		{}

		FooRTTI::FooRTTI(int inInt)
			:mInt(inInt)
			,mIntPtr(new int(0))
		{}

		FooRTTI::FooRTTI(int inInt, int inIntPtrVal)
			:mInt(inInt)
			,mIntPtr(new int(inIntPtrVal))
		{}

		FooRTTI::FooRTTI(const FooRTTI& rhs)
			:mInt(rhs.mInt)
			,mIntPtr(new int(*rhs.mIntPtr))
		{}

		FooRTTI& FooRTTI::operator=(const FooRTTI& rhs)
		{
			mInt = rhs.mInt;
			if (mIntPtr != nullptr) {
				delete mIntPtr;
			}
			mIntPtr = new int(*rhs.mIntPtr);
		}

		FooRTTI::~FooRTTI()
		{
			if (mIntPtr != nullptr) {
				delete mIntPtr;
				mIntPtr = nullptr;
			}
		}

		RTTI_DECLARATIONS(FooRTTI, Library::RTTI);
	};
	RTTI_DEFINITIONS(FooRTTI);

	CONCRETE_FACTORY(DeepCopyFactoryTest, Library::RTTI)
	CONCRETE_FACTORY(FooRTTI, Library::RTTI)
}