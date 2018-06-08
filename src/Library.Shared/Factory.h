#pragma once
#include "HashMap.h"

namespace Library {
	template <typename T>
	class Factory
	{
	public:
		static Factory<T>* Find(const std::string& factoryName);
		static T* Create(const std::string& className);
		static void Add(Factory* addFactory);
		static bool Remove(Factory* addFactory);

		static typename HashMap<std::string, Factory<T>*>::Iterator begin();
		static typename HashMap<std::string, Factory<T>*>::Iterator end();

		virtual T* Create() const = 0;
		virtual std::string ClassName() const = 0;

	private:
		static HashMap<std::string, Factory*> mFactories;
	};
}

#define CONCRETE_FACTORY(GeneratedObject, DerivedClass)						\
class GeneratedObject##Factory : public Factory<DerivedClass>				\
{																			\
public:																		\
	DerivedClass * Create() const override									\
	{																		\
		return new GeneratedObject;											\
	};																		\
																			\
	std::string ClassName() const override									\
	{																		\
		return #GeneratedObject;											\
	};																		\
};																				

#include "Factory.inl"