#include "pch.h"
#include "Scope.h"
#include "RTTI.h"

namespace Library {
	Scope::Scope(uint32_t capacity /* = 11*/)
		:mHashMap(capacity)
		,mOrderVector(capacity)
	{
	}

	Scope::~Scope()
	{
		Clear();
	}

	void Scope::Clear()
	{
		if (mParentScope != nullptr) {
			mParentScope->Orphan(*this);
			mParentScope = nullptr;
		}
		for (uint32_t i = 0; i < mOrderVector.Size(); ++i) {
			if (mOrderVector[i]->second.Type() == Datum::DatumType::E_TABLE) {
				while (mOrderVector[i]->second.Size()) {
					Scope* toDel = mOrderVector[i]->second.Get<Scope*>();
					delete(toDel);
				}
			}
		}
		mOrderVector.Clear();
		mHashMap.Clear();
	}

	void Scope::CopyFrom(const Scope& rhs)
	{
		for (auto it : rhs.mHashMap) {
			if (it.second.Type() == Datum::DatumType::E_TABLE) {
				Datum& curTableDatum = Append(it.first);
				curTableDatum.SetType(Datum::DatumType::E_TABLE);
				for (uint32_t i = 0; i < it.second.Size(); ++i) {
					Scope* incomingScope = new Scope(*it.second.Get<Scope*>(i));
					incomingScope->mParentScope = this;
					curTableDatum.PushBack(incomingScope);
				}
			} else {
				Datum& curTableDatum = Append(it.first);
				curTableDatum = it.second;
			}
		}
	}

	Scope::Scope(const Scope& rhs)
		:mHashMap(rhs.mHashMap.BucketCount())
		,mOrderVector(rhs.mHashMap.BucketCount())
	{
		CopyFrom(rhs);
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		Clear();
		CopyFrom(rhs);
		return *this;
	}

	Scope::Scope(Scope&& rhs)
		:mHashMap(std::move(rhs.mHashMap))
		,mOrderVector(std::move(rhs.mOrderVector))
	{
		if (rhs.mParentScope != nullptr) {
			mParentScope = rhs.mParentScope;
			std::string rhsName = mParentScope->FindName(rhs);
			mParentScope->Orphan(rhs);
			mParentScope->Adopt(*this, rhsName);
		}
		for (auto movedDatum : mOrderVector) {
			if (movedDatum->second.Type() == Datum::DatumType::E_TABLE) {
				for (uint32_t i = 0; i < movedDatum->second.Size(); ++i) {
					Scope* curScope = movedDatum->second.Get<Scope*>(i);
					curScope->mParentScope = this;
				}
			}
		}
	}

	Scope& Scope::operator=(Scope&& rhs)
	{
		Clear();
		mHashMap = std::move(rhs.mHashMap);
		mOrderVector = std::move(rhs.mOrderVector);
		if (rhs.mParentScope != nullptr) {
			mParentScope = rhs.mParentScope;
			std::string rhsName = mParentScope->FindName(rhs);
			mParentScope->Orphan(rhs);
			mParentScope->Adopt(*this, rhsName);
		}
		for (auto movedDatum : mOrderVector) {
			if (movedDatum->second.Type() == Datum::DatumType::E_TABLE) {
				for (uint32_t i = 0; i < movedDatum->second.Size(); ++i) {
					Scope* curScope = movedDatum->second.Get<Scope*>(i);
					curScope->mParentScope = this;
				}
			}
		}
		return *this;
	}

	std::string Scope::ToString() const
	{
		return "Scope";
	}

	std::string Scope::ToString()
	{
		std::string retString("[");
		for (auto movedDatum : mOrderVector) {
			retString.append("(");
			retString.append(movedDatum->first);
			retString.append(",");
			if (movedDatum->second.Size() > 1) {
				retString.append("|");
				for (uint32_t i = 0; i < movedDatum->second.Size(); ++i) {
					retString.append(movedDatum->second.ToString(i));
					retString.append("|");
				}
			} else {
				retString.append(movedDatum->second.ToString());
			}
			retString.append("),");
		}
		retString.append("]");
		return retString;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		if (this == &rhs) {
			return true;
		}
		if (mHashMap.Size() != rhs.mHashMap.Size()) {
			return false;
		}
		for (auto it : mHashMap) {
			Datum* curDatum = rhs.Find(it.first);
			if (curDatum == nullptr || *curDatum != it.second) {
				return false;
			}
		}
		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !(*this == rhs);
	}

	Datum & Scope::operator[](const std::string& key)
	{
		return Append(key);
	}

	Datum & Scope::operator[](uint32_t index)
	{
		return mOrderVector[index]->second;
	}

	bool Scope::Equals(const RTTI * rhs) const
	{
		if (rhs == nullptr) {
			return false;
		}
		if (rhs->Is(Scope::TypeIdClass())) {
			return *this == *rhs->As<Scope>();
		}
		return false;
	}

	Datum & Scope::Append(const std::string& key)
	{
		ExceptOnEmptyString(key);
		auto iter = mHashMap.begin();
		if (mHashMap.ContainsKey(key, iter)) {
			return (*iter).second;
		}
		iter = mHashMap.Insert(std::make_pair(key, Datum()));
		mOrderVector.PushBack(&(*(iter)));
		return (*iter).second;
	}

	Scope & Scope::AppendScope(const std::string & key)
	{
		ExceptOnEmptyString(key);
		auto iter = mHashMap.begin();
		if (!mHashMap.ContainsKey(key, iter)) {
			iter = mHashMap.Insert(std::make_pair(key, Datum()));
			mOrderVector.PushBack(&(*(iter)));
			(*iter).second.SetType(Datum::DatumType::E_TABLE);
		}
		Scope* newScope = new Scope();
		newScope->mParentScope = this;
		(*iter).second.PushBack(newScope);
		return *newScope;
	}

	void Scope::Adopt(Scope& newChild, const std::string& key)
	{
		if (newChild.mParentScope != this && newChild.mParentScope != nullptr) {
			newChild.mParentScope->Orphan(newChild);
		}
		ExceptOnEmptyString(key);
		auto iter = mHashMap.begin();
		if (!mHashMap.ContainsKey(key, iter)) {
			iter = mHashMap.Insert(std::make_pair(key, Datum()));
			mOrderVector.PushBack(&(*(iter)));
			(*iter).second.SetType(Datum::DatumType::E_TABLE);
		}
		newChild.mParentScope = this;
		(*iter).second.PushBack(&newChild);
	}

	void Scope::Orphan(Scope& orphaningChild)
	{
		if (orphaningChild.mParentScope != this) {
			throw std::exception("Attempting to orphan child from non-owning scope");
		}
		uint32_t index = 0;
		for (auto it = mOrderVector.begin(); it != mOrderVector.end(); ++it) {
			if ((*it)->second.Find(orphaningChild, index)) {
				(*it)->second.Get<Scope*>(index)->mParentScope = nullptr;
				(*it)->second.Remove(index);
				break;
			}
		}
	}

	std::string Scope::FindName(const Scope & scopeToFind)
	{
		for (auto it = mOrderVector.begin(); it != mOrderVector.end(); ++it) {
			if ((*it)->second.Find(scopeToFind)) {
				return (*it)->first;
			}
		}
		return std::string();
	}

	Scope * Scope::GetParent() const
	{
		return mParentScope;
	}

	Datum * Scope::Find(const std::string& key) const
	{
		auto it = mHashMap.Find(key);
		if (it != mHashMap.end()) {
			return &(*it).second;
		}
		return nullptr;
	}

	Datum * Scope::Search(const std::string & key, Scope *& datumParent)
	{
		Datum* foundDatum = Find(key);
		if (foundDatum == nullptr && mParentScope != nullptr) {
			return mParentScope->Search(key, datumParent);
		} else if (foundDatum != nullptr) {
			datumParent = this;
		}
		return foundDatum;
	}

	void Scope::ExceptOnEmptyString(const std::string & key) const
	{
		if (key.empty()) {
			throw std::exception("Cannot use empty string for key");
		}
	}

	RTTI_DEFINITIONS(Scope);
}
