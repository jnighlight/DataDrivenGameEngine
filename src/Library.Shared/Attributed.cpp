#include "pch.h"
#include "Attributed.h"


namespace Library {

	HashMap<uint64_t, Vector<std::string>> Attributed::sPrescribedAttributesHashMap(50); //This should last a good llong time

	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed()
		:Scope()
	{
		Library::Datum& thisDatum = Append("this");
		thisDatum.SetStorage(static_cast<RTTI**>(&thisPointer), 1);
	}

	Attributed::Attributed(const Attributed& rhs)
		:Scope(rhs)
	{
		Datum* thisDatum = Find("this");
		thisDatum->SetStorage(static_cast<RTTI**>(&thisPointer), 1);
	}

	Attributed* Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);
		Datum* thisDatum = Find("this");
		thisDatum->SetStorage(static_cast<RTTI**>(&thisPointer), 1);
		return this;
	}

	Attributed::Attributed(Attributed&& rhs)
		:Scope(std::move(rhs))
	{
		Datum* thisDatum = Find("this");
		thisDatum->SetStorage(static_cast<RTTI**>(&thisPointer), 1);
	}

	Attributed* Attributed::operator=(Attributed&& rhs)
	{
		Scope::operator=(std::move(rhs));
		Datum* thisDatum = Find("this");
		thisDatum->SetStorage(static_cast<RTTI**>(&thisPointer), 1);
		return this;
	}

	bool Attributed::IsInitialized() {
		return sPrescribedAttributesHashMap.ContainsKey(TypeIdClass());
	}

	void Attributed::AddPrescribedAttributeToStaticHash(const std::string prescribedAtt)
	{
		Vector<std::string>& vectorRef = sPrescribedAttributesHashMap[TypeIdClass()];
		vectorRef.PushBack(prescribedAtt);
	}

	void Attributed::AddExternalPrescribedAttribute(const std::string& attributeName, int32_t& attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetStorage(&attributeDefaultVal, size);
	}

	void Attributed::AddExternalPrescribedAttribute(const std::string& attributeName, float_t& attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetStorage(&attributeDefaultVal, size);
	}

	void Attributed::AddExternalPrescribedAttribute(const std::string& attributeName, glm::vec4& attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetStorage(&attributeDefaultVal, size);
	}

	void Attributed::AddExternalPrescribedAttribute(const std::string& attributeName, glm::mat4& attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetStorage(&attributeDefaultVal, size);
	}

	void Attributed::AddExternalPrescribedAttribute(const std::string& attributeName, std::string& attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetStorage(&attributeDefaultVal, size);
	}

	void Attributed::AddExternalPrescribedAttribute(const std::string& attributeName, RTTI*& attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetStorage(&attributeDefaultVal, size);
	}

	void Attributed::AddInternalPrescribedAttribute(const std::string& attributeName, int32_t attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetType(Datum::DatumType::E_INTEGER);
		addedAttribute.Reserve(size);
		for (uint32_t i = 0; i < size; ++i) {
			addedAttribute.PushBack(attributeDefaultVal);
		}
	}

	void Attributed::AddInternalPrescribedAttribute(const std::string& attributeName, float_t attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetType(Datum::DatumType::E_FLOAT);
		addedAttribute.Reserve(size);
		for (uint32_t i = 0; i < size; ++i) {
			addedAttribute.PushBack(attributeDefaultVal);
		}
	}

	void Attributed::AddInternalPrescribedAttribute(const std::string& attributeName, glm::vec4 attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetType(Datum::DatumType::E_VECTOR);
		addedAttribute.Reserve(size);
		for (uint32_t i = 0; i < size; ++i) {
			addedAttribute.PushBack(attributeDefaultVal);
		}
	}

	void Attributed::AddInternalPrescribedAttribute(const std::string& attributeName, glm::mat4 attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetType(Datum::DatumType::E_MATRIX);
		addedAttribute.Reserve(size);
		for (uint32_t i = 0; i < size; ++i) {
			addedAttribute.PushBack(attributeDefaultVal);
		}
	}

	void Attributed::AddInternalPrescribedAttributeScope(const std::string& attributeName, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetType(Datum::DatumType::E_TABLE);
		addedAttribute.Reserve(size);
		for (uint32_t i = 0; i < size; ++i) {
			AppendScope(attributeName);
		}
	}

	void Attributed::AddInternalPrescribedAttribute(const std::string& attributeName, std::string attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetType(Datum::DatumType::E_STRING);
		addedAttribute.Reserve(size);
		for (uint32_t i = 0; i < size; ++i) {
			addedAttribute.PushBack(attributeDefaultVal);
		}
	}

	void Attributed::AddInternalPrescribedAttribute(const std::string& attributeName, RTTI* attributeDefaultVal, uint32_t size)
	{
		Library::Datum& addedAttribute = Append(attributeName);
		addedAttribute.SetType(Datum::DatumType::E_USER_DEFINED);
		addedAttribute.Reserve(size);
		for (uint32_t i = 0; i < size; ++i) {
			addedAttribute.PushBack(attributeDefaultVal);
		}
	}

	void Attributed::ExceptOnNullPtr(Datum* foundPtr) const
	{
		if (foundPtr == nullptr) {
			throw std::exception("Updating an external prescribed attribute that doesn't exist");
		}
	}

	void Attributed::UpdateExternalPrescribedAttribute(const std::string& attributeName, int32_t& attributeUpdateVal, uint32_t size)
	{
		Library::Datum* addedAttribute = Find(attributeName);
		ExceptOnNullPtr(addedAttribute);
		addedAttribute->SetStorage(&attributeUpdateVal, size);
	}

	void Attributed::UpdateExternalPrescribedAttribute(const std::string& attributeName, float_t& attributeUpdateVal, uint32_t size)
	{
		Library::Datum* addedAttribute = Find(attributeName);
		ExceptOnNullPtr(addedAttribute);
		addedAttribute->SetStorage(&attributeUpdateVal, size);
	}

	void Attributed::UpdateExternalPrescribedAttribute(const std::string& attributeName, glm::vec4& attributeUpdateVal, uint32_t size)
	{
		Library::Datum* addedAttribute = Find(attributeName);
		ExceptOnNullPtr(addedAttribute);
		addedAttribute->SetStorage(&attributeUpdateVal, size);
	}

	void Attributed::UpdateExternalPrescribedAttribute(const std::string& attributeName, glm::mat4& attributeUpdateVal, uint32_t size)
	{
		Library::Datum* addedAttribute = Find(attributeName);
		ExceptOnNullPtr(addedAttribute);
		addedAttribute->SetStorage(&attributeUpdateVal, size);
	}

	void Attributed::UpdateExternalPrescribedAttribute(const std::string& attributeName, std::string& attributeUpdateVal, uint32_t size)
	{
		Library::Datum* addedAttribute = Find(attributeName);
		ExceptOnNullPtr(addedAttribute);
		addedAttribute->SetStorage(&attributeUpdateVal, size);
	}

	void Attributed::UpdateExternalPrescribedAttribute(const std::string& attributeName, RTTI*& attributeUpdateVal, uint32_t size)
	{
		Library::Datum* addedAttribute = Find(attributeName);
		ExceptOnNullPtr(addedAttribute);
		addedAttribute->SetStorage(&attributeUpdateVal, size);
	}

	Library::Datum & Attributed::AppendAuxiliaryAttribute(const std::string & attributeName)
	{
		return Append(attributeName);
	}

	bool Attributed::IsAttribute(const std::string & attributeName) const
	{
		return Find(attributeName) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const std::string & attributeName) const
	{
		Vector<std::string>& vectorRef = sPrescribedAttributesHashMap[TypeIdClass()];
		return vectorRef.Find(attributeName) != vectorRef.end() || attributeName == "this";
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string & attributeName) const
	{
		return IsAttribute(attributeName) && !IsPrescribedAttribute(attributeName);
	}

	void Attributed::Attributes(Vector<std::string>& attributes)
	{
		attributes.Clear();
		attributes.Reserve(mOrderVector.Size());
		for (auto attributePairs : mOrderVector) {
			attributes.PushBack(attributePairs->first);
		}
	}

	void Attributed::PrescribedAttributes(Vector<std::string>& prescribedAttributes)
	{
		prescribedAttributes.Clear();
		prescribedAttributes.Reserve(mOrderVector.Size());
		for (auto attributePairs : mOrderVector) {
			if (IsPrescribedAttribute(attributePairs->first)) {
				prescribedAttributes.PushBack(attributePairs->first);
			}
		}
	}

	void Attributed::AuxiliaryAttributes(Vector<std::string>& auxiliaryAttributes)
	{
		auxiliaryAttributes.Clear();
		auxiliaryAttributes.Reserve(mOrderVector.Size());
		for (auto attributePairs : mOrderVector) {
			if (!IsPrescribedAttribute(attributePairs->first)) {
				auxiliaryAttributes.PushBack(attributePairs->first);
			}
		}
	}

	void Attributed::ClearHash()
	{
		Attributed::sPrescribedAttributesHashMap.Clear();
	}
}
