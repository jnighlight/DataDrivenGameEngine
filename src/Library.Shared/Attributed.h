#pragma once

#include "Datum.h"
#include "Scope.h"

namespace Library {
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Attributed subclass of scope for easy scope interface in subclasses
	/// </summary>
	/// --------------------------------------------------------------------------------
	class Attributed : public Scope
	{
	RTTI_DECLARATIONS(Attributed, Scope)
	public:
		/// ********************************************************************************
		/// <summary>
		/// Attributed constructor with the unique class ID for this class
		/// </summary>
		/// <param name="classId">The classID of the class that is derived from attribute</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		Attributed();

		/// ********************************************************************************
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Copies the attributed external values and reparents them to this attributed</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		Attributed(const Attributed& rhs);
		/// ********************************************************************************
		/// <summary>
		/// Assignment operator, clears out all internal data copies rhs
		/// </summary>
		/// <param name="rhs">Attributed to copy from</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		Attributed* operator=(const Attributed& rhs);
		/// ********************************************************************************
		/// <summary>
		/// Move constructor, clears out RHS and steals their data
		/// </summary>
		/// <param name="rhs">Attributed to move data from</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		Attributed(Attributed&& rhs);
		/// ********************************************************************************
		/// <summary>
		/// Move asssignment operator
		/// </summary>
		/// <param name="rhs">Attributed to move data from</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		Attributed* operator=(Attributed&& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Adds an internal prescribed attribute to this attributed class with a default template
		/// </summary>
		/// <param name="attributeName">The name of the adding attribute</param>
		/// <param name="attributeDefaultVal">The defualt value of the created attribute</param>
		/// <param name="size">The number of entries in the datum to populate</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void AddInternalPrescribedAttribute(const std::string& attributeName, int32_t attributeDefaultVal, uint32_t size);
		void AddInternalPrescribedAttribute(const std::string& attributeName, float_t attributeDefaultVal, uint32_t size);
		void AddInternalPrescribedAttribute(const std::string& attributeName, glm::vec4 attributeDefaultVal, uint32_t size);
		void AddInternalPrescribedAttribute(const std::string& attributeName, glm::mat4 attributeDefaultVal, uint32_t size);
		void AddInternalPrescribedAttribute(const std::string& attributeName, std::string attributeDefaultVal, uint32_t size);
		void AddInternalPrescribedAttribute(const std::string& attributeName, RTTI * attributeDefaultVal, uint32_t size);

		/// ********************************************************************************
		/// <summary>
		/// Adds an internal scope datum and populates it with default scopes
		/// </summary>
		/// <param name="attributeName">The key for the scope being created</param>
		/// <param name="size">The number of scopes to create in created datum</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void AddInternalPrescribedAttributeScope(const std::string& attributeName, uint32_t size);

		/// ********************************************************************************
		/// <summary>
		/// Adds an external storage datum to this attributed class referencing the incoming variable
		/// </summary>
		/// <param name="attributeName">The name of the external attirbute</param>
		/// <param name="attributeDefaultVal">The reference to the varaiable to be referenced in the datum</param>
		/// <param name="size">The size of the array referenced</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void AddExternalPrescribedAttribute(const std::string& attributeName, int32_t& attributeDefaultVal, uint32_t size);
		void AddExternalPrescribedAttribute(const std::string& attributeName, float_t& attributeDefaultVal, uint32_t size);
		void AddExternalPrescribedAttribute(const std::string& attributeName, glm::vec4& attributeDefaultVal, uint32_t size);
		void AddExternalPrescribedAttribute(const std::string& attributeName, glm::mat4& attributeDefaultVal, uint32_t size);
		void AddExternalPrescribedAttribute(const std::string& attributeName, std::string& attributeDefaultVal, uint32_t size);
		void AddExternalPrescribedAttribute(const std::string& attributeName, RTTI*& attributeDefaultVal, uint32_t size);

		/// ********************************************************************************
		/// <summary>
		/// Updates the external prescribed attributes for this class to reparent them to the
		///  new classes member variables
		/// </summary>
		/// <param name="attributeName">The name of the variable to reparent</param>
		/// <param name="attributeUpdateVal">The new memory for the external datum to reference</param>
		/// <param name="size">The size of the external datum reference</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void UpdateExternalPrescribedAttribute(const std::string& attributeName, int32_t& attributeUpdateVal, uint32_t size);
		void UpdateExternalPrescribedAttribute(const std::string& attributeName, float_t& attributeUpdateVal, uint32_t size);
		void UpdateExternalPrescribedAttribute(const std::string& attributeName, glm::vec4& attributeUpdateVal, uint32_t size);
		void UpdateExternalPrescribedAttribute(const std::string& attributeName, glm::mat4& attributeUpdateVal, uint32_t size);
		void UpdateExternalPrescribedAttribute(const std::string& attributeName, std::string& attributeUpdateVal, uint32_t size);
		void UpdateExternalPrescribedAttribute(const std::string& attributeName, RTTI*& attributeUpdateVal, uint32_t size);

		/// ********************************************************************************
		/// <summary>
		/// Appends an auxiliary attribute to this scope object
		/// </summary>
		/// <param name="attributeName">The name of the added attribute</param>
		/// <returns>A reference to the auxiliary datum created/found</returns>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		Library::Datum& AppendAuxiliaryAttribute(const std::string& attributeName);

		/// ********************************************************************************
		/// <summary>
		/// Returns whether or not the incoming string is an attribute or not
		/// </summary>
		/// <param name="attributeName">The name of the attribute to check</param>
		/// <returns>True if the string correlates to an attribute this scope owns</returns>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		bool IsAttribute(const std::string& attributeName) const;

		/// ********************************************************************************
		/// <summary>
		/// Returns whether or not the incoming string is a prescribed attribute or not
		/// </summary>
		/// <param name="attributeName">The name of the attribute to check</param>
		/// <returns>True if the string correlates to a prescribed attribute this scope owns</returns>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		bool IsPrescribedAttribute(const std::string & attributeName) const;

		/// ********************************************************************************
		/// <summary>
		/// Returns whether or not the incoming string is a auxiliary attribute or not
		/// </summary>
		/// <param name="attributeName">The name of the attribute to check</param>
		/// <returns>True if the string correlates to a auxiliary attribute this scope owns</returns>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		bool IsAuxiliaryAttribute(const std::string & attributeName) const;

		/// ********************************************************************************
		/// <summary>
		/// Populates incoming string vector with string copies of the recorded attributes
		/// </summary>
		/// <param name="attributes">String vector to populate with the names of all the attributes
		///  in this scope</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void Attributes(Vector<std::string>& attributes);

		/// ********************************************************************************
		/// <summary>
		/// Populates incoming string vector with string copies of the prescribed attributes
		/// </summary>
		/// <param name="attributes">String vector to populate with the names of all the prescribed attributes
		///  in this scope</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void PrescribedAttributes(Vector<std::string>& prescribedAttributes);

		/// ********************************************************************************
		/// <summary>
		/// Populates incoming string vector with string copies of the auxiliary attributes
		/// </summary>
		/// <param name="attributes">String vector to populate with the names of all the auxiliary attributes
		///  in this scope</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void AuxiliaryAttributes(Vector<std::string>& auxiliaryAttributes);

		/// ********************************************************************************
		/// <summary>
		/// Hard Reset of static hash for cleaning and reorganizing
		/// </summary>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		static void ClearHash();

		/// ********************************************************************************
		/// <summary>
		/// Function to check if the static hash of object references has been populated with
		///  this class's attributes
		/// </summary>
		/// <returns>True if the static hasnh map is populated for this class</returns>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		bool IsInitialized();

		/// ********************************************************************************
		/// <summary>
		/// Adds the record of an attribute into the static hash map record for this object
		/// </summary>
		/// <param name="prescribedAtt">The name of the attribute prescribed to this class</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void AddPrescribedAttributeToStaticHash(const std::string prescribedAtt);

		virtual ~Attributed() = default;
	protected:
		/// <summary>Static hashmap holding prescribed attribute data for instantiated classes</summary>
		static Library::HashMap<uint64_t, Vector<std::string>> sPrescribedAttributesHashMap;
	private:
		/// ********************************************************************************
		/// <summary>
		/// Private helper function to throw an exception if the recieved pointer is null
		/// </summary>
		/// <param name="foundPtr">Pointer to check for null data</param>
		/// <created>Jacob Lites,2/22/2018</created>
		/// <changed>Jacob Lites,2/22/2018</changed>
		/// ********************************************************************************
		void ExceptOnNullPtr(Datum* foundPtr) const;

		/// <summary>Pointer to self for scope "this" insertion</summary>
		RTTI * thisPointer = this;
	};
}
