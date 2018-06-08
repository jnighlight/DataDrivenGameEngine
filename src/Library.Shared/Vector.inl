#include "Vector.h"

namespace Library {

	const static uint32_t DEFAULT_CAPACITY = 10;

	/*-----------------------------------------------
						ITERATOR
	----------------------------------------------- */
	template<typename T>
	inline Vector<T>::Iterator::Iterator()
		:mOwner(nullptr)
		,mIndex(0)
	{
	}

	template<typename T>
	Vector<T>::Iterator::Iterator(Vector<T>& owner, uint32_t index/* = 0*/)
		:mOwner(&owner)
		,mIndex(index)
	{
	}

	template<typename T>
	Vector<T>::Iterator::Iterator(const Iterator & rhs)
		:mOwner(rhs.mOwner)
		,mIndex(rhs.mIndex)
	{
	}

	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator & rhs)
	{
		mOwner = rhs.mOwner;
		mIndex = rhs.mIndex;
		return *this;
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator & rhs) const
	{
		return (mOwner == rhs.mOwner) && (mIndex == rhs.mIndex);
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator & rhs) const
	{
		return !((*this) == rhs);
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if ( mOwner == nullptr ) {
			return (*this);
		}
		++mIndex;
		if (mIndex > mOwner->Size()) {
			mIndex = mOwner->Size();
		}
		return (*this);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Vector<T>::Iterator preIncrementIter = (*this);
		++(*this);
		return preIncrementIter;
	}
	
	template<typename T>
	inline T& Vector<T>::Iterator::operator*()
	{
		if ( mOwner == nullptr || (*this) == mOwner->end()) {
			throw std::exception("Attempting to derenference invalid iterator");
		}
		return mOwner->At(mIndex);
	}

	/*-----------------------------------------------
						VECTOR
	----------------------------------------------- */
	template <typename T>
	Vector<T>::Vector()
		:mDataStart(nullptr)
		,mSize(0)
		,mCapacity(0)
	{
		Reserve(DEFAULT_CAPACITY);
	}

	template <typename T>
	Vector<T>::Vector(uint32_t capacity)
		:mDataStart(nullptr)
		,mSize(0)
		,mCapacity(0)
	{
		Reserve(capacity);
	}

	template <typename T>
	Vector<T>::Vector(const Vector& rhs)
		:mDataStart(nullptr)
		,mSize(0)
		,mCapacity(0)
	{
		DeepCopy(rhs);
	}
	
	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		Clear();
		DeepCopy(rhs);
		return *this;
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Clear();
		free(mDataStart);
	}
	
	template <typename T>
	void Vector<T>::Reserve(uint32_t newCapacity)
	{
		if (mCapacity == newCapacity) {
			return;
		}
		if (newCapacity < mSize) {
			throw std::exception("Incoming new capacity would delete data out of the vector");
		}
		T* newVectorSpace = (T*)malloc(sizeof(T) * newCapacity);
		if (mSize > 0) {
			memmove(newVectorSpace, mDataStart, sizeof(T) * mSize);
		}
		free(mDataStart);
		mDataStart = newVectorSpace;
		mCapacity = newCapacity;
	}

	template <typename T>
	const T& Vector<T>::Front() const
	{
		if (mSize == 0) {
			throw std::exception("Accessing front of empty vector");
		}
		return (*mDataStart);
	}

	template <typename T>
	T& Vector<T>::Front()
	{
		if (mSize == 0) {
			throw std::exception("Accessing front of empty vector");
		}
		return (*mDataStart);
	}

	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (mSize == 0) {
			throw std::exception("Accessing back of empty vector");
		}
		return (mDataStart[mSize - 1]);
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		if (mSize == 0) {
			throw std::exception("Accessing back of empty vector");
		}
		return (mDataStart[mSize - 1]);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Vector<T>::Iterator((*this));
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& toFind)
	{
		Iterator it = begin();
		for (; it != end(); ++it) {
			if ((*it) == toFind) {
				break;
			}
		}
		return it;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::end()
	{
		return Vector<T>::Iterator((*this), mSize);
	}

	template <typename T>
	void Vector<T>::PushBack(const T& in) {
		if (mSize + 1 > mCapacity) {
			Reserve((mCapacity+1) * 2); //We're doing mCapacity+1 here in case the current capacity is 0
		}
		new(&mDataStart[mSize]) T(in);
		++mSize;
	}
	
	
	template <typename T>
	void Vector<T>::PopBack()
	{
		if (mSize <= 0) {
			throw std::exception("Popping back of empty list");
		}
		(mDataStart[mSize-1]).~T();
		mSize--;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (uint32_t i = 0; i < mSize; ++i) {
			(mDataStart[i]).~T();
		}
		mSize = 0;
	}

	template <typename T>
	void Vector<T>::ShrinkToFit()
	{
		if (mSize == 0) {
			free(mDataStart);
			mDataStart = nullptr;
			mCapacity = 0;
			return;
		}
		if (mSize == mCapacity) {
			return;
		}
		Reserve(mSize);
		mCapacity = mSize;
	}

	template <typename T>
	T& Vector<T>::operator[](uint32_t index) {
		if (index >= mSize) {
			throw std::exception("Attempting to access uninstantiated data");
		}
		return mDataStart[index];
	}
	
	template <typename T>
	const T& Vector<T>::operator[](uint32_t index) const {
		if (index >= mSize) {
			throw std::exception("Attempting to access uninstantiated data");
		}
		return mDataStart[index];
	}
	
	template <typename T>
	T& Vector<T>::At(uint32_t index) {
		if (index >= mSize) {
			throw std::exception("Attempting to access uninstantiated data");
		}
		return mDataStart[index];
	}

	template <typename T>
	const T& Vector<T>::At(uint32_t index) const {
		if (index >= mSize) {
			throw std::exception("Attempting to access uninstantiated data");
		}
		return mDataStart[index];
	}

	template <typename T>
	uint32_t Vector<T>::Size() const {
		return mSize;
	}

	template <typename T>
	uint32_t Vector<T>::Capacity() const{
		return mCapacity;
	}

	template <typename T>
	bool Vector<T>::IsEmpty() const {
		return mSize == 0;
	}

	template <typename T>
	void Vector<T>::Remove(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Attempting to remove nonexistant index");
		}
		(mDataStart[index]).~T();
		uint32_t bytesLeft = ((mSize-1) - index) * sizeof(T);
		memmove(&mDataStart[index], &mDataStart[index + 1], bytesLeft);
		--mSize;
	}

	template <typename T>
	void Vector<T>::DeepCopy(const Vector& rhs) {
		Reserve(rhs.mCapacity);
		for (uint32_t i = 0; i < rhs.mSize; ++i) {
			PushBack(rhs[i]);
		}
	}
}
