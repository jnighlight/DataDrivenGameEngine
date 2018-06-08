#include "HashMap.h"

namespace Library {
	/*-----------------------------------------------
						ITERATOR
	----------------------------------------------- */
	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::Iterator()
		:mOwner(nullptr)
		,mIndex(0)
		,mCurListIterator()
	{
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::Iterator(HashMap<TKey, TData, HashFunctor, ComparatorFunctor>& owner, ChainIterator it/* = ChainIterator()*/, uint32_t index/* = 0*/)
		:mOwner(&owner)
		,mIndex(index)
		,mCurListIterator(it)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::Iterator(const Iterator & rhs)
		:mOwner(rhs.mOwner)
		,mIndex(rhs.mIndex)
		,mCurListIterator(rhs.mCurListIterator)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator=(const Iterator & rhs)
	{
		mOwner = rhs.mOwner;
		mIndex = rhs.mIndex;
		mCurListIterator = rhs.mCurListIterator;
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator==(const Iterator & rhs) const
	{
		return (mOwner == rhs.mOwner) && (mIndex == rhs.mIndex) && (mCurListIterator == rhs.mCurListIterator);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator!=(const Iterator & rhs) const
	{
		return !((*this) == rhs);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator++()
	{
		if ((mOwner == nullptr) || (mCurListIterator==mOwner->mBuckets[mOwner->mBuckets.Size() - 1].end() && mIndex == mOwner->mBuckets.Size())) {
			return (*this);
		}
		++mCurListIterator;
		if (mCurListIterator == mOwner->mBuckets[mIndex].end()) {
			++mIndex;
			while (mIndex < mOwner->mBuckets.Size()) {
				if (!mOwner->mBuckets[mIndex].IsEmpty()) {
					mCurListIterator = mOwner->mBuckets[mIndex].begin();
					return (*this);
				}
				++mIndex;
			}
			mCurListIterator = mOwner->mBuckets[mOwner->mBuckets.Size() - 1].end();
			mIndex = mOwner->mBuckets.Size();
			return (*this);
		}
		return (*this);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator++(int)
	{
		HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator preIncrementIter = (*this);
		++(*this);
		return preIncrementIter;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::PairType& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator*()
	{
		if ( mOwner == nullptr || (*this) == mOwner->end()) {
			throw std::exception("Attempting to derenference invalid iterator");
		}
		return *mCurListIterator;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::PairType* HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator->()
	{
		return &(*(*this));
	}

	/*-----------------------------------------------
						HASHMAP
	----------------------------------------------- */
	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::HashMap()
		:mBuckets(DEFAULT_BUCKET_COUNT)
		,mBucketCount(DEFAULT_BUCKET_COUNT)
		,mSize(0)
	{
		ChainType Chain;
		for (int i = 0; i < DEFAULT_BUCKET_COUNT; ++i) {
			mBuckets.PushBack(Chain);
		}
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::HashMap(uint32_t bucketCount)
		:mBuckets(bucketCount)
		,mBucketCount(bucketCount)
		,mSize(0)
	{
		if (bucketCount == 0) {
			throw std::exception("Can't make hashmap with 0 buckets");
		}
		ChainType Chain;
		for (uint32_t i = 0; i < mBucketCount; ++i) {
			mBuckets.PushBack(Chain);
		}
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::~HashMap()
	{
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Insert(const PairType& entry) {
		uint32_t hashVal = hashFunctor(entry.first);
		uint32_t bucketIndex = hashVal % mBucketCount;
		for (auto it = mBuckets[bucketIndex].begin(); it != mBuckets[bucketIndex].end(); ++it) {
			if (compareFunctor((*it).first, entry.first)) {
				return Iterator(*this, it, bucketIndex);
			}
		}
		++mSize;
		return Iterator(*this, mBuckets[bucketIndex].PushBack(entry), bucketIndex);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Find(const TKey& key) const {
		uint32_t keyHash = hashFunctor(key);
		uint32_t bucketIndex = keyHash % mBucketCount;
		for (auto it = mBuckets[bucketIndex].begin(); it != mBuckets[bucketIndex].end(); ++it) {
			if (compareFunctor(key, (*it).first)) {
				return Iterator(*const_cast<HashMap*>(this), it, bucketIndex);
			}
		}
		return end();
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::begin() {
		if (IsEmpty()) {
			return end();
		}
		uint32_t i;
		for (i = 0; i < mBucketCount; ++i) {
			if (!mBuckets[i].IsEmpty()) {
				break;
			}
		}
		return Iterator(*this, mBuckets[i].begin(), i);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::begin() const{
		if (IsEmpty()) {
			return end();
		}
		uint32_t i;
		for (i = 0; i < mBucketCount; ++i) {
			if (!mBuckets[i].IsEmpty()) {
				break;
			}
		}
		auto it = mBuckets[i].begin();
		return Iterator(*const_cast<HashMap*>(this), it, i);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::end() {
		return Iterator(*this, mBuckets[mBucketCount - 1].end(), mBucketCount);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::end() const {
		return Iterator(*const_cast<HashMap*>(this), mBuckets[mBucketCount - 1].end(), mBucketCount);
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename void HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Clear() {
		for (auto& vectorIter : mBuckets) {
			vectorIter.Clear();
		}
		mSize = 0;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename uint32_t HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Size() const 
	{
		return mSize;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::ContainsKey(const TKey& key) {
		return Find(key) != end();
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::ContainsKey(const TKey& key, Iterator& iter) {
		iter = Find(key);
		return iter != end();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline TData& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::operator[](const TKey & key)
	{
		Iterator iter;
		if (!ContainsKey(key, iter)) {
			TData defaultData;
			auto it = Insert(std::make_pair(key, defaultData));
			return (*it).second;
		}
		return iter->second;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline TData & HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::At(const TKey & key)
	{
		Iterator iter;
		if (!ContainsKey(key, iter)) {
			throw std::exception("Attempting to retrieve data not in hash");
		}
		return iter->second;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline const TData & HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::At(const TKey & key) const
	{
		Iterator iter;
		if (!ContainsKey(key, iter)) {
			throw std::exception("Attempting to retrieve data not in hash");
		}
		return iter->second;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Remove(const TKey& key) {
		uint32_t keyHash = hashFunctor(key);
		uint32_t bucketIndex = keyHash % mBucketCount;
		for (auto it = mBuckets[bucketIndex].begin(); it != mBuckets[bucketIndex].end(); ++it) {
			if (compareFunctor(key, (*it).first)) {
				mBuckets[bucketIndex].RemoveFirst(*it);
				--mSize;
				return true;
			}
		}
		return false;
	}

	template <typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	inline uint32_t HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::BucketCount() const{
		return mBucketCount;
	}
}