#include "pch.h"
#include "Datum.h"
#include "Scope.h"

namespace Library {

		/*
												E_UNKNOWN , E_INTEGER, E_FLOAT, E_VECTOR, E_MATRIX, E_TABLE, E_STRING, E_USER_DEFINED, E_MAX
		*/
	const std::string Datum::sTypeStrings[] = { "Unknown", "Integer", "Float", "Vector", "Matrix", "Table", "String", "User Defined"};

	const uint32_t Datum::DatumSizes[(uint32_t)DatumType::E_MAX] = { 0, sizeof(uint32_t), sizeof(float_t), sizeof(glm::vec4), sizeof(glm::mat4), sizeof(Scope*), sizeof(std::string), sizeof(Library::RTTI*) };
	const Datum::DatumClearFunc Datum::DatumClearFunctions[(uint32_t)DatumType::E_MAX] = {
		0,
		&Datum::ClearNoDestructor,
		&Datum::ClearNoDestructor,
		&Datum::ClearVector,
		&Datum::ClearMatrix,
		&Datum::ClearNoDestructor,
		&Datum::ClearString,
		&Datum::ClearNoDestructor };

	const Datum::DatumSetFromStringFunc Datum::DatumSetFromStringFunctions[(uint32_t)DatumType::E_MAX] = {
		0,
		&Datum::SetIntFromString,
		&Datum::SetFloatFromString,
		&Datum::SetVectorFromString,
		&Datum::SetMatrixFromString,
		&Datum::SetRTTIFromString,
		&Datum::SetStringFromString,
		&Datum::SetRTTIFromString };

	const Datum::DatumToStringFunc Datum::DatumToStringFunctions[(uint32_t)DatumType::E_MAX] = {
		0,
		&Datum::IntToString,
		&Datum::FloatToString,
		&Datum::VectorToString,
		&Datum::MatrixToString,
		&Datum::ScopeToString,
		&Datum::StringToString,
		&Datum::RTTIToString };

	const Datum::DatumCompareFunc Datum::DatumCompareFunctions[(uint32_t)DatumType::E_MAX] = {
		0,
		&Datum::CompareIntDatums,
		&Datum::CompareFloatDatums,
		&Datum::CompareVectorDatums,
		&Datum::CompareMatrixDatums,
		&Datum::CompareTableDatums,
		&Datum::CompareStringDatums,
		&Datum::CompareRTTIDatums };

	Datum::Datum(DatumType type)
		:mType(type)
		, mSize(0)
		, mCapacity(0)
		, mIsExternal(false)
	{
		mData.mVoidPointer = nullptr;
	}

	Datum::DatumType Datum::GetDatumTypeByString(std::string datumTypeString)
	{
		uint32_t enumIndex = 0;
		for(auto datumString : sTypeStrings) {
			if (datumTypeString.compare(datumString) == 0) {
				return static_cast<Datum::DatumType>(enumIndex);
			}
			++enumIndex;
		}
		return DatumType::E_MAX;
	}

	Datum::Datum()
		:mType(DatumType::E_UNKNOWN)
		, mSize(0)
		, mCapacity(0)
		, mIsExternal(false)
	{
		mData.mVoidPointer = nullptr;
	}

	Datum::Datum(int32_t datumInt)
		:Datum(DatumType::E_INTEGER)
	{
		Reserve(1);
		PushBack(datumInt);
	}

	Datum::Datum(float_t datumFloat)
		:Datum(DatumType::E_FLOAT)
	{
		Reserve(1);
		PushBack(datumFloat);
	}

	Datum::Datum(const glm::vec4& datumVec)
		:Datum(DatumType::E_VECTOR)
	{
		Reserve(1);
		PushBack(datumVec);
	}

	Datum::Datum(const glm::mat4& datumMat)
		:Datum(DatumType::E_MATRIX)
	{
		Reserve(1);
		PushBack(datumMat);
	}

	Datum::Datum(Scope* datumScope)
		:Datum(DatumType::E_TABLE)
	{
		Reserve(1);
		PushBack(datumScope);
	}

	Datum::Datum(const std::string& datumString)
		:Datum(DatumType::E_STRING)
	{
		Reserve(1);
		PushBack(datumString);
	}

	Datum::Datum(Library::RTTI* datumRttiPointer)
		:Datum(DatumType::E_USER_DEFINED)
	{
		Reserve(1);
		PushBack(datumRttiPointer);
	}

	Datum::Datum(const Datum& rhs)
		:mType(rhs.mType)
		,mSize(0)
		,mCapacity(0)
		,mIsExternal(rhs.mIsExternal)
	{
		mData.mVoidPointer = nullptr;
		if (rhs.mType == DatumType::E_UNKNOWN) { //If we're copying an uninitialized datum, we're done
			return;
		}
		if (mIsExternal) {
			mData.mVoidPointer = rhs.mData.mVoidPointer;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
		}
		else {
			Reserve(rhs.mCapacity);
			if (mType == DatumType::E_STRING) {
				for (uint32_t i = 0; i < rhs.mSize; ++i) {
					PushBack(rhs.Get<std::string>(i));
				}
			}
			else {
				uint32_t sizeToCopy = DatumSizes[uint32_t(mType)] * rhs.mSize;
				memcpy_s(mData.mVoidPointer, sizeToCopy, rhs.mData.mVoidPointer, sizeToCopy);
				mSize = rhs.mSize;
			}
		}
	}

	Datum::Datum(Datum&& rhs)
		:mType(rhs.mType)
		,mSize(rhs.mSize)
		,mCapacity(rhs.mCapacity)
		,mIsExternal(rhs.mIsExternal)
	{
		mData.mVoidPointer = rhs.mData.mVoidPointer;
		ResetDatum(rhs);
	}

	void Datum::ResetDatum(Datum& datumToReset) {
		datumToReset.mSize = 0;
		datumToReset.mCapacity = 0;
		datumToReset.mType = DatumType::E_UNKNOWN;
		datumToReset.mIsExternal = false;
		datumToReset.mData.mVoidPointer = nullptr;
	}

	//If THIS datum is external...
	void Datum::ExternalStorageCopy(const Datum& rhs)
	{
		if (rhs.mIsExternal) { //..and rhs is, just swap out the pointers and sizes
			mData.mVoidPointer = rhs.mData.mVoidPointer;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
		}
		else {	//...but rhs is not, discard mine and each rhs
			mIsExternal = false;
			mData.mVoidPointer = nullptr;
			mSize = 0;
			mCapacity = 0;
			Reserve(rhs.mCapacity);
			if (rhs.mType == DatumType::E_STRING) {
				for (uint32_t i = 0; i < rhs.mSize; ++i) {
					PushBack(rhs.Get<std::string>(i));
				}
			}
			else {
				uint32_t sizeToCopy = DatumSizes[uint32_t(mType)] * rhs.mSize;
				memcpy_s(mData.mVoidPointer, sizeToCopy, rhs.mData.mVoidPointer, sizeToCopy);
				mSize = rhs.mSize;
			}
		}
	}

	//If THIS datum is Internal...
	void Datum::InternalStorageCopy(const Datum& rhs)
	{
		if (rhs.mIsExternal) { //..but rhs is, clear my internals and just steal theirs
			Resize(0);
			mData.mVoidPointer = rhs.mData.mVoidPointer;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mIsExternal = true;
		}
		else {	//...and rhs is not, clear mine and copy theirs
			mIsExternal = false;
			Resize(0);
			Reserve(rhs.mCapacity);
			if (rhs.mType == DatumType::E_STRING) {
				for (uint32_t i = 0; i < rhs.mSize; ++i) {
					PushBack(rhs.Get<std::string>(i));
				}
			}
			else {
				uint32_t sizeToCopy = DatumSizes[uint32_t(mType)] * rhs.mSize;
				memcpy_s(mData.mVoidPointer, sizeToCopy, rhs.mData.mVoidPointer, sizeToCopy);
				mSize = rhs.mSize;
			}
		}
	}

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (mType == DatumType::E_UNKNOWN) {
			mType = rhs.mType;
		}
		if (mType != rhs.mType) {
			throw std::exception("Assigning datum to datum of other type is not allowed");
		}
		if (mIsExternal) {
			ExternalStorageCopy(rhs);
		}
		else {
			InternalStorageCopy(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs)
	{
		if (mType == DatumType::E_UNKNOWN) {
			mType = rhs.mType;
		}
		if (mType != rhs.mType) {
			throw std::exception("Assigning datum to datum of other type is not allowed");
		}
		if (!mIsExternal) {
			Clear();
			Resize(0);
		}
		mSize = rhs.mSize;
		mCapacity = rhs.mCapacity;
		mIsExternal = rhs.mIsExternal;
		mData.mVoidPointer = rhs.mData.mVoidPointer;
		ResetDatum(rhs);
		return *this;
	}

	Datum& Datum::operator=(int32_t datumInt)
	{
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_INTEGER) {
			throw std::exception("Attempting to change datum type on assignment is not allowed");
		}
		mType = DatumType::E_INTEGER;
		if (mSize < 1) {
			Reserve(1);
			PushBack(datumInt);
		} else {
			Set(datumInt);
		}
		return *this;
	}

	Datum& Datum::operator=(float_t datumFloat)
	{
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_FLOAT) {
			throw std::exception("Attempting to change datum type on assignment is not allowed");
		}
		mType = DatumType::E_FLOAT;
		if (mSize < 1) {
			Reserve(1);
			PushBack(datumFloat);
		} else {
			Set(datumFloat);
		}
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& datumVector)
	{
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_VECTOR) {
			throw std::exception("Attempting to change datum type on assignment is not allowed");
		}
		mType = DatumType::E_VECTOR;
		if (mSize < 1) {
			Reserve(1);
			PushBack(datumVector);
		} else {
			Set(datumVector);
		}
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& datumMatrix)
	{
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_MATRIX) {
			throw std::exception("Attempting to change datum type on assignment is not allowed");
		}
		mType = DatumType::E_MATRIX;
		if (mSize < 1) {
			Reserve(1);
			PushBack(datumMatrix);
		} else {
			Set(datumMatrix);
		}
		return *this;
	}

	Datum& Datum::operator=(Scope* datumScope)
	{
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_TABLE) {
			throw std::exception("Attempting to change datum type on assignment is not allowed");
		}
		mType = DatumType::E_TABLE;
		if (mSize < 1) {
			Reserve(1);
			PushBack(datumScope);
		} else {
			Set(datumScope);
		}
		return *this;
	}

	Datum& Datum::operator=(const std::string& datumString)
	{
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_STRING) {
			throw std::exception("Attempting to change datum type on assignment is not allowed");
		}
		mType = DatumType::E_STRING;
		if (mSize < 1) {
			Reserve(1);
			PushBack(datumString);
		} else {
			Set(datumString);
		}
		return *this;
	}

	Datum& Datum::operator=(Library::RTTI* datumRttiPointer)
	{
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_USER_DEFINED) {
			throw std::exception("Attempting to change datum type on assignment is not allowed");
		}
		mType = DatumType::E_USER_DEFINED;
		if (mSize < 1) {
			Reserve(1);
			PushBack(datumRttiPointer);
		} else {
			Set(datumRttiPointer);
		}
		return *this;
	}

	void Datum::Reserve(uint32_t newCapacity) {
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType == DatumType::E_UNKNOWN) {
			throw std::exception("Can't set capacity for unkown type Datum");
		}
		if (mCapacity >= newCapacity) {
			return;
		}
		uint32_t newCapacityInBytes = DatumSizes[uint32_t(mType)] * newCapacity;
		mData.mVoidPointer = realloc(mData.mVoidPointer, newCapacityInBytes);
		mCapacity = newCapacity;
	}

	void Datum::Resize(uint32_t newSize) {
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (newSize == mSize) {
			return;
		}
		if (newSize < mSize && mType == DatumType::E_STRING) {
			for (uint32_t i = newSize; i < mSize; ++i) {
				using namespace std; //Mostly for easy string destructor usage
				(mData.mString[i]).~string();
			}
			mSize = newSize;
		}
		uint32_t newCapacityInBytes = DatumSizes[static_cast<uint32_t>(mType)] * newSize;
		mData.mVoidPointer = realloc(mData.mVoidPointer, newCapacityInBytes);
		mCapacity = newSize;

		if (mSize < newSize && mType == DatumType::E_STRING) {
			for (uint32_t i = mSize; i < newSize; ++i) {
				PushBack(std::string());
			}
		}
		mSize = newSize;
		mCapacity = newSize;
	}

	void Datum::PushBack(int32_t datumInt) {
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType != DatumType::E_INTEGER) {
			throw std::exception("Can't push non-integer value into integer datum!");
		}
		ReserveIfNecessary();
		new(&mData.mInt[mSize]) int32_t(datumInt);
		++mSize;
	}

	void Datum::PushBack(float_t datumFloat) {
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType != DatumType::E_FLOAT) {
			throw std::exception("Can't push non-float value into float datum!");
		}
		ReserveIfNecessary();
		new(&mData.mFloat[mSize]) float_t(datumFloat);
		++mSize;
	}

	void Datum::PushBack(const glm::vec4& datumVec)
	{
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType != DatumType::E_VECTOR) {
			throw std::exception("Can't push non-vector value into vector datum!");
		}
		ReserveIfNecessary();
		new(&mData.mVector[mSize]) glm::vec4(datumVec);
		++mSize;
	}

	void Datum::PushBack(const glm::mat4 & datumMat)
	{
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType != DatumType::E_MATRIX) {
			throw std::exception("Can't push non-matrix value into matrix datum!");
		}
		ReserveIfNecessary();
		new(&mData.mMatrix[mSize]) glm::mat4(datumMat);
		++mSize;
	}

	void Datum::PushBack(Scope* datumScope)
	{
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType != DatumType::E_TABLE) {
			throw std::exception("Can't push non-scope value into scope datum!");
		}
		ReserveIfNecessary();
		new(&mData.mTable[mSize]) Scope*(datumScope);
		++mSize;
	}

	void Datum::PushBack(const std::string & datumString)
	{
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType != DatumType::E_STRING) {
			throw std::exception("Can't push non-string value into string datum!");
		}
		ReserveIfNecessary();
		new(&mData.mString[mSize]) std::string(datumString);
		++mSize;
	}

	void Datum::PushBack(Library::RTTI * datumRttiPointer)
	{
		if (mIsExternal) {
			throw std::exception("Can't change capacity for external data");
		}
		if (mType != DatumType::E_USER_DEFINED) {
			throw std::exception("Can't push non-RTTI value into RTTI datum!");
		}
		ReserveIfNecessary();
		new(&mData.mUserDefined[mSize]) Library::RTTI*(datumRttiPointer);
		++mSize;
	}

	void Datum::SetStorage(int32_t* externalStorage, uint32_t size) {
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_INTEGER) {
			throw std::exception("Changing external storage type is not allowed");
		}
		mType = DatumType::E_INTEGER;
		mData.mVoidPointer = externalStorage;
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(float_t* externalStorage, uint32_t size) {
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_FLOAT) {
			throw std::exception("Changing external storage type is not allowed");
		}
		mType = DatumType::E_FLOAT;
		mData.mVoidPointer = externalStorage;
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(glm::vec4* externalStorage, uint32_t size) {
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_VECTOR) {
			throw std::exception("Changing external storage type is not allowed");
		}
		mType = DatumType::E_VECTOR;
		mData.mVoidPointer = externalStorage;
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(glm::mat4* externalStorage, uint32_t size) {
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_MATRIX) {
			throw std::exception("Changing external storage type is not allowed");
		}
		mType = DatumType::E_MATRIX;
		mData.mVoidPointer = externalStorage;
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(Scope** externalStorage, uint32_t size) {
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_TABLE) {
			throw std::exception("Changing external storage type is not allowed");
		}
		mType = DatumType::E_TABLE;
		mData.mVoidPointer = externalStorage;
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(std::string* externalStorage, uint32_t size) {
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_STRING) {
			throw std::exception("Changing external storage type is not allowed");
		}
		mType = DatumType::E_STRING;
		mData.mVoidPointer = externalStorage;
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
	}

	void Datum::SetStorage(Library::RTTI** externalStorage, uint32_t size) {
		if (mType != DatumType::E_UNKNOWN && mType != DatumType::E_USER_DEFINED) {
			throw std::exception("Changing external storage type is not allowed");
		}
		mType = DatumType::E_USER_DEFINED;
		mData.mVoidPointer = externalStorage;
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
	}

	void Datum::Set(int32_t datumInt, uint32_t index /* = 0 */)
	{
		if (mType != DatumType::E_INTEGER) {
			throw std::exception("Attempting to set int in non-int datum");
		}
		Get<int32_t>(index) = datumInt;
	}

	void Datum::Set(float_t datumFloat, uint32_t index /* = 0 */)
	{
		if (mType != DatumType::E_FLOAT) {
			throw std::exception("Attempting to set float in non-float datum");
		}
		Get<float_t>(index) = datumFloat;
	}

	void Datum::Set(const glm::vec4 & datumVec, uint32_t index /* = 0 */)
	{
		if (mType != DatumType::E_VECTOR) {
			throw std::exception("Attempting to set vector in non-vector datum");
		}
		Get<glm::vec4>(index) = datumVec;
	}

	void Datum::Set(const glm::mat4 & datumMat, uint32_t index /* = 0 */)
	{
		if (mType != DatumType::E_MATRIX) {
			throw std::exception("Attempting to set matrix in non-matrix datum");
		}
		Get<glm::mat4>(index) = datumMat;
	}

	void Datum::Set(Scope* datumScope, uint32_t index /* = 0 */)
	{
		if (mType != DatumType::E_TABLE) {
			throw std::exception("Attempting to set matrix in non-scope datum");
		}
		Get<Scope*>(index) = datumScope;
	}

	void Datum::Set(const std::string & datumString, uint32_t index /* = 0 */)
	{
		if (mType != DatumType::E_STRING) {
			throw std::exception("Attempting to set string in non-string datum");
		}
		Get<std::string>(index) = datumString;
	}

	void Datum::Set(Library::RTTI * datumRttiPointer, uint32_t index /* = 0 */)
	{
		if (mType != DatumType::E_USER_DEFINED) {
			throw std::exception("Attempting to set RTTI in non-RTTI datum");
		}
		Get<Library::RTTI*>(index) = datumRttiPointer;
	}

	bool Datum::Find(int32_t datumInt)
	{ 
		uint32_t fakeIndex;
		return Find(datumInt, fakeIndex);
	}

	bool Datum::Find(int32_t datumInt, uint32_t& index)
	{ 
		if (mType != DatumType::E_INTEGER) {
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i) {
			if (Get<int32_t>(i) == datumInt) {
				index = i;
				return true;
			}
		}
		return false;
	}

	bool Datum::Find(float_t datumFloat)
	{ 
		uint32_t fakeIndex;
		return Find(datumFloat, fakeIndex);
	}

	bool Datum::Find(float_t datumFloat, uint32_t& index)
	{ 
		if (mType != DatumType::E_FLOAT) {
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i) {
			if (Get<float_t>(i) == datumFloat) {
				index = i;
				return true;
			}
		}
		return false;
	}

	bool Datum::Find(const glm::vec4& datumVec)
	{ 
		uint32_t fakeIndex;
		return Find(datumVec, fakeIndex);
	}

	bool Datum::Find(const glm::vec4& datumVec, uint32_t& index)
	{ 
		if (mType != DatumType::E_VECTOR) {
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i) {
			if (Get<glm::vec4>(i) == datumVec) {
				index = i;
				return true;
			}
		}
		return false;
	}

	bool Datum::Find(const glm::mat4& datumMat)
	{ 
		uint32_t fakeIndex;
		return Find(datumMat, fakeIndex);
	}

	bool Datum::Find(const glm::mat4& datumMat, uint32_t& index)
	{ 
		if (mType != DatumType::E_MATRIX) {
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i) {
			if (Get<glm::mat4>(i) == datumMat) {
				index = i;
				return true;
			}
		}
		return false;
	}

	bool Datum::Find(const Scope& datumScope)
	{ 
		uint32_t fakeIndex;
		return Find(datumScope, fakeIndex);
	}

	bool Datum::Find(const Scope& datumScope, uint32_t& index)
	{ 
		if (mType != DatumType::E_TABLE) {
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i) {
			if (*Get<Scope*>(i) == datumScope) {
				index = i;
				return true;
			}
		}
		return false;
	}

	bool Datum::Find(const std::string& datumString)
	{ 
		uint32_t fakeIndex;
		return Find(datumString, fakeIndex);
	}

	bool Datum::Find(const std::string& datumString, uint32_t& index)
	{ 
		if (mType != DatumType::E_STRING) {
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i) {
			if (Get<std::string>(i) == datumString) {
				index = i;
				return true;
			}
		}
		return false;
	}

	bool Datum::Find(RTTI* datumRTTI)
	{ 
		uint32_t fakeIndex;
		return Find(datumRTTI, fakeIndex);
	}

	bool Datum::Find(RTTI* datumRTTI, uint32_t& index)
	{ 
		if (mType != DatumType::E_USER_DEFINED) {
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i) {
			if (Get<RTTI*>(i)->Equals(datumRTTI)) {
				index = i;
				return true;
			}
		}
		return false;
	}

	std::string Datum::ToString(uint32_t index /* = 0 */) const
	{
		if (mType == DatumType::E_UNKNOWN) {
			throw std::exception("Can't get string of undefined datum type");
		}
		if (mSize == 0 && index == 0) {
			return "[Empty]";
		}
		if (index >= mSize) {
			throw std::exception("Attempting to get String from out of bounds data");
		}
		return std::invoke(DatumToStringFunctions[(uint32_t)mType], this, index);
	}

	bool Datum::operator==(const Datum & rhs) const
	{
		if (mType != rhs.Type() || mSize != rhs.Size()) {
			return false;
		}
		return std::invoke(DatumCompareFunctions[(uint32_t)mType], this, rhs);
	}

	bool Datum::operator!=(const Datum & rhs) const
	{
		return !((*this) == rhs);
	}

	bool Datum::CompareIntDatums(const Datum & rhs) const
	{
		for (uint32_t i = 0; i < rhs.Size(); ++i) {
			if (Get<int32_t>(i) != rhs.Get<int32_t>(i)) {
				return false;
			}
		}
		return true;
	}

	bool Datum::CompareFloatDatums(const Datum & rhs) const
	{
		for (uint32_t i = 0; i < rhs.Size(); ++i) {
			if (Get<float_t>(i) != rhs.Get<float_t>(i)) {
				return false;
			}
		}
		return true;
	}

	bool Datum::CompareVectorDatums(const Datum & rhs) const
	{
		for (uint32_t i = 0; i < rhs.Size(); ++i) {
			if (Get<glm::vec4>(i) != rhs.Get<glm::vec4>(i)) {
				return false;
			}
		}
		return true;
	}

	bool Datum::CompareMatrixDatums(const Datum & rhs) const
	{
		for (uint32_t i = 0; i < rhs.Size(); ++i) {
			if (Get<glm::mat4>(i) != rhs.Get<glm::mat4>(i)) {
				return false;
			}
		}
		return true;
	}

	bool Datum::CompareTableDatums(const Datum & rhs) const
	{
		for (uint32_t i = 0; i < rhs.Size(); ++i) {
			if (!(*Get<Scope*>(i) == *rhs.Get<Scope*>(i))) {
				return false;
			}
		}
		return true;
	}

	bool Datum::CompareStringDatums(const Datum & rhs) const
	{
		for (uint32_t i = 0; i < rhs.Size(); ++i) {
			if (Get<std::string>(i) != rhs.Get<std::string>(i)) {
				return false;
			}
		}
		return true;
	}

	bool Datum::CompareRTTIDatums(const Datum & rhs) const
	{
		for (uint32_t i = 0; i < rhs.Size(); ++i) {
			if (!Get<Library::RTTI*>(i)->Equals(rhs.Get<Library::RTTI*>(i))) {
				return false;
			}
		}
		return true;
	}

	std::string Datum::IntToString(uint32_t index) const
	{
		return std::to_string(Get<int32_t>(index));
	}

	std::string Datum::FloatToString(uint32_t index) const
	{
		return std::to_string(Get<float_t>(index));
	}

	std::string Datum::VectorToString(uint32_t index) const
	{
		std::string vectorString;
		const glm::vec4& vecRef = Get<glm::vec4>(index);
		vectorString.append(std::to_string(vecRef.x));
		vectorString.append(" ");
		vectorString.append(std::to_string(vecRef.y));
		vectorString.append(" ");
		vectorString.append(std::to_string(vecRef.z));
		vectorString.append(" ");
		vectorString.append(std::to_string(vecRef.w));
		return vectorString;
	}

	std::string Datum::MatrixToString(uint32_t index) const
	{
		std::string vectorString;
		const glm::mat4& matRef = Get<glm::mat4>(index);
		for (int i = 0; i < 4; ++i) {
			vectorString.append(std::to_string(matRef[i].x));
			vectorString.append(" ");
			vectorString.append(std::to_string(matRef[i].y));
			vectorString.append(" ");
			vectorString.append(std::to_string(matRef[i].z));
			vectorString.append(" ");
			vectorString.append(std::to_string(matRef[i].w));
			vectorString.append(" ");
		}
		return vectorString;
	}

	std::string Datum::StringToString(uint32_t index) const
	{
		return Get<std::string>(index);
	}

	std::string Datum::RTTIToString(uint32_t index) const
	{
		return Get<Library::RTTI*>(index)->ToString();
	}

	std::string Datum::ScopeToString(uint32_t index) const
	{
		return Get<Library::Scope*>(index)->ToString();
	}

	void Datum::SetFromString(std::string data, uint32_t index)
	{
		if (mType == DatumType::E_UNKNOWN) {
			throw std::exception("Can't Set an undefined type from string");
		}
		if (index >= mSize) {
			throw std::exception("Attempting to set out of bounds data!");
		}
		std::invoke(DatumSetFromStringFunctions[(uint32_t)mType], this, data, index);
	}

	void Datum::SetIntFromString(std::string data, uint32_t index)
	{
		int32_t testMan = std::stoi(data);
		Set(testMan, index);
	}

	void Datum::SetFloatFromString(std::string data, uint32_t index)
	{
		Set(std::stof(data), index);
	}

	void Datum::SetVectorFromString(std::string data, uint32_t index)
	{
		float_t v1, v2, v3, v4;
		sscanf_s(data.c_str(), "%f %f %f %f", &v1, &v2, &v3, &v4);
		glm::vec4 vecToSet(v1, v2, v3, v4);
		Set(glm::vec4 (v1, v2, v3, v4), index);
	}

	void Datum::SetMatrixFromString(std::string data, uint32_t index)
	{
		float_t x1, y1, z1, w1;
		float_t x2, y2, z2, w2;
		float_t x3, y3, z3, w3;
		float_t x4, y4, z4, w4;
		std::string sscanfString;
		for (int i = 0; i < 16; ++i) {
			sscanfString.append("%f ");
		}
		sscanf_s(data.c_str(), sscanfString.c_str(), &x1, &y1, &z1, &w1, &x2, &y2, &z2, &w2, &x3, &y3, &z3, &w3, &x4, &y4, &z4, &w4);
		Set(glm::mat4(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4), index);
	}

	void Datum::SetStringFromString(std::string data, uint32_t index)
	{
		Set(data, index);
	}

	void Datum::SetRTTIFromString(std::string data, uint32_t index)
	{
		UNREFERENCED_PARAMETER(data);
		UNREFERENCED_PARAMETER(index);
		throw std::exception("Can't set RTTI datum from string");
	}

	void Datum::ReserveIfNecessary() {
		if (mSize == mCapacity) {
			Reserve((mCapacity + 1) * 2); //We're doing mCapacity+1 here in case the current capacity is 0
		}
	}

	void Datum::Clear()
	{
		if (mType == DatumType::E_UNKNOWN) {
			return;
		}
		if (mIsExternal) {
			throw std::exception("Can't clear an external datum");
		}
		std::invoke(DatumClearFunctions[(uint32_t)mType], this);
	}

	void Datum::ClearNoDestructor()
	{
		mSize = 0;
	}

	void Datum::ClearVector()
	{
		using namespace glm; //Mostly for easy vector destructor usage
		for (uint32_t i = 0; i < mSize; ++i) {
			(mData.mVector[i]).~vec4();
		}
		mSize = 0;
	}

	void Datum::ClearMatrix() {
		using namespace glm; //Mostly for easy matrix destructor usage
		for (uint32_t i = 0; i < mSize; ++i) {
			(mData.mMatrix[i]).~mat4();
		}
		mSize = 0;
	}

	void Datum::ClearString()
	{
		using namespace std; //Mostly for easy string destructor usage
		for (uint32_t i = 0; i < mSize; ++i) {
			(mData.mString[i]).~string();
		}
		mSize = 0;
	}

	void Datum::Remove(uint32_t index)
	{
		if (mIsExternal) {
			throw std::exception("Can't remove values from external storage datum");
		}
		if (index >= mSize) {
			throw std::exception("Attempting to remove nonexistant index");
		}
		if (mType == DatumType::E_STRING) {
			using namespace std;
			(mData.mString[index]).~string();
		}
		uint32_t mySize = DatumSizes[uint32_t(mType)];
		uint32_t startBytesOffset = index * mySize;
		uint32_t endBytesOffset = (index+1) * mySize;;
		uint32_t bytesLeft = ((mSize - 1) - index) * mySize;
		memmove(&mData.mChar[startBytesOffset], &mData.mChar[endBytesOffset], bytesLeft);
		--mSize;
	}

	Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	uint32_t Datum::Size() const
	{
		return mSize;
	}

	void Datum::SetType(DatumType type)
	{
		if (mType != DatumType::E_UNKNOWN && mType != type) {
			throw std::exception("Setting type in datum with already set type");
		}
		if (type == DatumType::E_UNKNOWN || type == DatumType::E_MAX) {
			throw std::exception("Attempting to set invalid type");
		}
		mType = type;
	}

	Datum::~Datum()
	{
		if (!mIsExternal) {
			Clear();
			free(mData.mVoidPointer);
		}
	}
}
