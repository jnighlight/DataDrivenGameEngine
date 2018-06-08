#include "SList.h"

namespace Library {

	/*-----------------------------------------------
						NODE
	----------------------------------------------- */
	template <typename T>
	SList<T>::Node::Node(const T& data, Node* next /* = nullptr */)
		:mData(data)
		,mNext(next)
	{
	}

	/*-----------------------------------------------
						ITERATOR
	----------------------------------------------- */
	template<typename T>
	inline SList<T>::Iterator::Iterator()
		:mOwner(nullptr)
		,mCurrentNode(nullptr)
	{
	}

	template<typename T>
	SList<T>::Iterator::Iterator(const SList<T>& owner, Node* currentNode) //TODO: Make node nullptr by default
		:mOwner(&owner)
		,mCurrentNode(currentNode)
	{
	}

	template<typename T>
	SList<T>::Iterator::Iterator(const Iterator & rhs)
		:mOwner(rhs.mOwner)
		,mCurrentNode(rhs.mCurrentNode)
	{
	}

	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator & rhs)
	{
		mOwner = rhs.mOwner;
		mCurrentNode = rhs.mCurrentNode;
		return *this;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator & rhs) const
	{
		return (mOwner == rhs.mOwner) && (mCurrentNode == rhs.mCurrentNode);
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator & rhs) const
	{
		return !((*this) == rhs);
	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mCurrentNode != nullptr) {
			mCurrentNode = mCurrentNode->mNext;
		}
		return (*this);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		SList<T>::Iterator preIncrementIter = (*this);
		if (mCurrentNode != nullptr) {
			mCurrentNode = mCurrentNode->mNext;
		}
		return preIncrementIter;
	}

	template<typename T>
	inline const T& SList<T>::Iterator::operator*() const
	{
		if (mCurrentNode == nullptr) {
			throw std::exception("Derenferencing null node");
		}
		return mCurrentNode->mData;
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*()
	{
		if (mCurrentNode == nullptr) {
			throw std::exception("Derenferencing null node");
		}
		return mCurrentNode->mData;
	}

	/*-----------------------------------------------
						SLIST
	----------------------------------------------- */
	template <typename T>
	SList<T>::SList()
		:mFront(nullptr)
		,mBack(nullptr)
		,mSize(0)
	{
	}

	template <typename T>
	SList<T>::SList(const SList& rhs)
		:mFront(nullptr)
		,mBack(nullptr)
		,mSize(0)
	{
		DeepCopy(rhs);
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this == &rhs) {
			return *this;
		}
		Clear();
		DeepCopy(rhs);
		return *this;
	}

	template <typename T>
	void SList<T>::DeepCopy(const SList& rhs)
	{
		Node* iterWith = rhs.mFront;
		while (iterWith != nullptr) {
			PushBack(iterWith->mData);
			iterWith = iterWith->mNext;
		}
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		const SList& bob = (*this);
		return SList<T>::Iterator(bob, mFront);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		return SList::Iterator((*this), nullptr);
	}

	template <typename T>
	void SList<T>::InsertAfter(const Iterator& iter, const T& data)
	{
		if (iter.mOwner != this) {
			throw std::exception("Attempting to Insert after an iterator from another list");
		}
		if (iter.mCurrentNode == nullptr) { //If this is list.end(), append it too the back
			PushBack(data);
		} else {
			Node* newDataNode = new Node(data, iter.mCurrentNode->mNext);
			iter.mCurrentNode->mNext = newDataNode;
			++mSize;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& data) const
	{
		SList<T>::Iterator it = begin();
		for (; it != end(); ++it) {
			if ((*it) == data) {
				break;
			}
		}
		return it;
	}

	template <typename T>
	void SList<T>::RemoveFirst(const T& data)
	{
		Node* prevNode = nullptr;
		Node* curNode = mFront;
		while (curNode != nullptr) {
			if (curNode->mData == data) {
				if (prevNode != nullptr) {
					prevNode->mNext = curNode->mNext;
				}
				if (curNode == mFront) {
					mFront = curNode->mNext;
				}
				if (curNode == mBack) {
					mBack = prevNode;
				}
				delete curNode;
				--mSize;
				break;
			}
			prevNode = curNode;
			curNode = curNode->mNext;
		}
	}

	template <typename T>
	void SList<T>::RemoveAll(const T& data)
	{
		Node* prevNode = nullptr;
		Node* curNode = mFront;
		while (curNode != nullptr) {
			if (curNode->mData == data) {
				if (prevNode != nullptr) {
					prevNode->mNext = curNode->mNext;
				}
				if (curNode == mFront) {
					mFront = curNode->mNext;
				}
				if (curNode == mBack) {
					mBack = prevNode;
				}
				Node* tempNode = curNode->mNext;
				delete curNode;
				--mSize;
				curNode = tempNode;
			} else {
				prevNode = curNode;
				curNode = curNode->mNext;
			}
		}
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (mSize == 0) {
			throw std::exception("Accessing front of empty list");
		}
		return mFront->mData;
	}

	template <typename T>
	T& SList<T>::Front()
	{
		if (mSize == 0) {
			throw std::exception("Accessing front of empty list");
		}
		return mFront->mData;
	}

	template <typename T>
	void SList<T>::PushFront(const T& toPush)
	{
		mFront = new Node(toPush, mFront);
		if (mSize == 0) {
			mBack = mFront;
		}
		++mSize;
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (mSize == 0) {
			throw std::exception("Popping front of empty list");
		}
		Node* nextFront = mFront->mNext;
		delete mFront;
		mFront = nextFront;
		if (mSize <= 1) {
			mBack = nullptr;
		}
		--mSize;
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (mSize == 0) {
			throw std::exception("Accessing back of empty list");
		}
		return mBack->mData;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		if (mSize == 0) {
			throw std::exception("Accessing back of empty list");
		}
		return mBack->mData;
	}

	template <typename T>
	bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename T>
	int SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		while (mSize != 0) {
			PopFront();
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& toPush)
	{
		Node* newBackNode = new Node(toPush);
		if (mSize == 0) {
			mFront = newBackNode;
		} else {
			mBack->mNext = newBackNode;
		}
		mBack = newBackNode;
		++mSize;
		return SList::Iterator((*this), mBack);
	}

}
