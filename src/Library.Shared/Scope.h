#pragma once

#include <cstdint>
#include <string>
#include "HashMap.h"
#include "Vector.h"
#include "Datum.h"
#include "RTTI.h"

namespace Library {
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Scope class for containing logical classes as run time-typecasted sets of data
	/// </summary>
	/// --------------------------------------------------------------------------------
	class Scope : public Library::RTTI
	{
		/// ********************************************************************************
		/// <summary>
		/// RTTI Declarations to make scope a subclass of RTTI with inherited functions and data
		/// </summary>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		RTTI_DECLARATIONS(Scope, Library::RTTI);
	public:
		/// ********************************************************************************
		/// <summary>
		/// Default constructor with a default capacaity
		/// </summary>
		/// <param name="capacity">default Size of internal vector and hashmap</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Scope(uint32_t capacity = 11);
		/// ********************************************************************************
		/// <summary>
		///  Copy constructor
		/// </summary>
		/// <param name="rhs">Scope to copy data from</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Scope(const Scope& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Assignment operator. NOTE that this clears out the data inside of the copying
		///  object.
		/// </summary>
		/// <param name="rhs">The scope to copy from</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Scope& operator=(const Scope & rhs);

		/// ********************************************************************************
		/// <summary>
		///  Copy constructor
		/// </summary>
		/// <param name="rhs">Scope to copy data from</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Scope(Scope&& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Assignment operator. NOTE that this clears out the data inside of the copying
		///  object.
		/// </summary>
		/// <param name="rhs">The scope to copy from</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Scope& operator=(Scope&& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Virtual destructor for polymorphic destruction
		/// </summary>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		virtual ~Scope();

		/// ********************************************************************************
		/// <summary>
		/// ToString implementation for RTTI. Simply returns name of type
		/// </summary>
		/// <returns>String representing object type</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		virtual std::string ToString();
		virtual std::string ToString() const;

		/// ********************************************************************************
		/// <summary>
		/// Equals override for RTTI interface. Checks if type is scope, the scope compares
		///  incoming object
		/// </summary>
		/// <param name="rhs">RTTI* object to attempt to cast to scope</param>
		/// <returns>True if RTTI is of type scope and is equivalent to this scope</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		virtual bool Equals(const RTTI* rhs) const;

		/// ********************************************************************************
		/// <summary>
		/// Equivalence operator for 2 scopes. Checks internal scopes and datums, but NOT parents
		/// </summary>
		/// <param name="rhs">The scope to compare against</param>
		/// <returns>True if the two scopes are the same</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		bool operator==(const Scope & rhs) const;
		bool operator!=(const Scope & rhs) const;

		/// ********************************************************************************
		/// <summary>
		/// Bracket operator for searching scope for datums by key
		/// </summary>
		/// <param name="key">String key for finding datum in the hash</param>
		/// <returns>Reference to the found datum if it exists. Creates one if there is not one</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Datum& operator[](const std::string& key);

		/// ********************************************************************************
		/// <summary>
		/// Returns datum reference by index. These are ordered by data member insertion order
		///  throws an exception if index is out of bounds
		/// </summary>
		/// <param name="index">The index in the order vector that the data was inserted</param>
		/// <returns>A datum reference at "index" in the order vector</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Datum& operator[](uint32_t index);

		/// ********************************************************************************
		/// <summary>
		/// Appends a datum into the scope at hash "key". If one already exists, simply returns
		///  a reference to the existing dataum
		/// </summary>
		/// <param name="key">String key for accessing data from hashmap</param>
		/// <returns>Reference to the datum now existing at the "key" element in the hashmap</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Library::Datum& Append(const std::string& key);

		/// ********************************************************************************
		/// <summary>
		/// Appends a Scope type datum with a new scope at "key" element in the internal hash.
		/// If there is not a datum at that key element, one is created. If there is, a scope
		///  is appended to that datum
		/// </summary>
		/// <param name="key">The key in the hash to which to append the scope</param>
		/// <returns>Reference to the created and added scope</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Library::Scope& AppendScope(const std::string& key);

		/// ********************************************************************************
		/// <summary>
		/// Changes ownership of a scope to this scope. NOTE: if the scope already has a
		///  parent, it should be orphaned before it is adopted, or you'll have multiple
		///  references hanging around.
		/// </summary>
		/// <param name="newChild">The child that this scope will take ownership of</param>
		/// <param name="key">The key index at which this scope will place the new child</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		void Adopt(Scope& newChild, const std::string& key);

		/// ********************************************************************************
		/// <summary>
		/// Function that removes memory responsibility of a scope from another scope.
		/// </summary>
		/// <param name="orphaningChild">The child to remove ownership of from the scope</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		void Orphan(Scope& orphaningChild);

		/// ********************************************************************************
		/// <summary>
		/// Finds the name of the associated scope if it is in this scope. If it is not,
		///  returns an empty string
		/// </summary>
		/// <param name="scopeToFind">The scope to find in this scope's hash map</param>
		/// <returns>The string key in the hashmap for the given scope, empty string if not found</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		std::string FindName(const Scope& scopeToFind);

		/// ********************************************************************************
		/// <summary>
		/// Returns a pointer to this scope's owner. Returns nullptr if there is not an owner
		/// </summary>
		/// <returns>A pointer to this scopes owner. May be nullptr</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Scope* GetParent() const;

		/// ********************************************************************************
		/// <summary>
		/// Finds the datum at index "key" and returns a pointer to it. Returns nullptr if
		///  there is no datum at that key.
		/// </summary>
		/// <param name="key">hash at which to search for a datum</param>
		/// <returns>Pointer to datum that relies at given hash. Nullptr if one doesn't exist</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Datum* Find(const std::string& key) const;

		/// ********************************************************************************
		/// <summary>
		/// Searches _UP_ the scope chain looking for a datum located at incoming hash, and
		///  modifies incoming pointer to point to owner of said datum. If datum does not exist,
		///  returns a nullptr and does not modify the datumParent pointer
		/// </summary>
		/// <param name="key">Hash to search for up the scope chain</param>
		/// <param name="datumParent">OUT: pointer to modify to point at owner of returned dataum</param>
		/// <returns>Datum pointer to found datum at "key" hash. nullptr if no datums are found</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		Datum* Search(const std::string& key, Scope*& datumParent);

		/// ********************************************************************************
		/// <summary>
		/// Clears internal memory and DELETES all owned data. You've been warned.
		/// </summary>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		void Clear();


	protected:
		void CopyFrom(const Scope & rhs);

		/// <summary>Internal typedef for stringDatum pair for syntactic symplicity</summary>
		typedef std::pair<std::string, Library::Datum> StringDatumPair;
		/// <summary>Typedef for vector simplicity</summary>
		typedef Library::Vector<StringDatumPair*> DatumOrderVector;

		/// ********************************************************************************
		/// <summary>
		/// Helper function to throw an exception if an incoming string is empty
		/// </summary>
		/// <param name="str">Incoming string to check for emptyness</param>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		void ExceptOnEmptyString(const std::string& str) const;

		/// <summary>Owning scope of this scope</summary>
		Scope* mParentScope = nullptr;
		/// <summary>Internal hashmap for datums</summary>
		Library::HashMap <std::string, Library::Datum> mHashMap;
		/// <summary>Order vector for O(1) access of data if order is known</summary>
		DatumOrderVector mOrderVector;
	};
}