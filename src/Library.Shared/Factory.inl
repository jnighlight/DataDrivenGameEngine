#include "Factory.h"

namespace Library {

	template <typename T>
	HashMap<std::string, Factory<T>*> Factory<T>::mFactories;

	template <typename T>
	Factory<T>* Factory<T>::Find(const std::string& factoryName)
	{
		auto factoryIter = Factory<T>::mFactories.Find(factoryName);
		if (factoryIter != Factory<T>::mFactories.end()) {
			return (*factoryIter).second;
		}
		return nullptr;
	}

	template <typename T>
	T* Factory<T>::Create(const std::string& className)
	{
		Factory<T>* foundFactory = Find(className);
		if (foundFactory == nullptr) {
			std::string err("Requested Factory does not exist: ");
			err.append(className);
			throw std::exception(err.c_str());
		}
		return foundFactory->Create();
	}

	template <typename T>
	void Factory<T>::Add(Factory* addFactory)
	{
		Factory<T>::mFactories.Insert(std::make_pair(addFactory->ClassName(), addFactory));
	}

	template <typename T>
	bool Factory<T>::Remove(Factory* addFactory)
	{
		return Factory<T>::mFactories.Remove(addFactory->ClassName());
	}

	template<typename T>
	typename HashMap<std::string, Factory<T>*>::Iterator Factory<T>::begin()
	{
		return Factory<T>::mFactories.begin();
	}

	template<typename T>
	typename HashMap<std::string, Factory<T>*>::Iterator Factory<T>::end()
	{
		return Factory<T>::mFactories.end();
	}
}
