#pragma once
#include <string>
#include <functional>

#define GLM_FORCE_CXX98 //Here to force we use the GLM 98 version due to window's special C++ impelemntation
#include "glm\vec4.hpp"
#include "glm\mat4x4.hpp"
#include "RTTI.h"


namespace Library {
	class Scope;
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Datum is a run-time polymorphic class not meant to be extended.
	/// </summary>
	/// --------------------------------------------------------------------------------
	class Datum final
	{
	public:

		/// --------------------------------------------------------------------------------
		/// <summary>
		/// This enum is to denote the internal data type that the datum is holding.
		/// </summary>
		/// --------------------------------------------------------------------------------
		enum class DatumType {
			E_UNKNOWN = 0,
			E_INTEGER,
			E_FLOAT,
			E_VECTOR,
			E_MATRIX,
			E_TABLE,
			E_STRING,
			E_USER_DEFINED,
			E_MAX
		};

		//static const std::string sTypeStrings[static_cast<uint32_t>(DatumType::E_MAX)];
		static const std::string sTypeStrings[];
		static DatumType GetDatumTypeByString(std::string datumTypeString);

		/// <summary>Because we'll only be holding one type of data at a type, we store a union of pointers based on our internal data</summary>
		union StoredData {
			int32_t* mInt;
			float_t* mFloat;
			glm::vec4* mVector;
			glm::mat4* mMatrix;
			Scope** mTable;
			std::string* mString;
			Library::RTTI** mUserDefined;
			void* mVoidPointer;
			char* mChar;
		};

		/// ********************************************************************************
		/// <summary>
		/// Default Constructor that does not define a type or allocate data
		/// </summary>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum();
		/// ********************************************************************************
		/// <summary>
		/// Several implicit constructors for treating datum like a scalar. They only allocate
		///  space for one element. Each type sets the internal type.
		/// </summary>
		/// <param name="datumInt">Data to store inside of datum</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum(int32_t datumInt);
		Datum(float_t datumFloat);
		Datum(const glm::vec4& datumVec);
		Datum(const glm::mat4& datumMat);
		Datum(Scope* datumScope);
		Datum(const std::string& datumString);
		Datum(Library::RTTI* datumRttiPointer);

		/// ********************************************************************************
		/// <summary>
		/// Copy constructor for copying internal pointer data
		/// </summary>
		/// <param name="rhs">Datum to copy data from</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum(const Datum& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Move constructor for taking all data from the rhs element and zeroing it out
		/// </summary>
		/// <param name="rhs">Datum to move data from</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum(Datum&& rhs);
		/// ********************************************************************************
		/// <summary>
		/// Copy assignment for deep copy semantics
		/// </summary>
		/// <param name="rhs">Datum to deep copy from</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum& operator=(const Datum& rhs);
		/// ********************************************************************************
		/// <summary>
		/// Move assignment for deep move
		/// </summary>
		/// <param name="rhs">Datum to deep move from</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum& operator=(Datum&& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Operator equals. These treat the Datum like a scalar, setting the first element
		///  in the datum if there are more than one, and reserving one unit of space if
		///  it hasn't been allocated yiet
		/// </summary>
		/// <param name="datumInt">Datum data to be stored at the first element</param>
		/// <returns>Reference to self for chain equals</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum& operator=(int32_t datumInt);
		Datum& operator=(float_t datumFloat);
		Datum& operator=(const glm::vec4& datumVec);
		Datum& operator=(const glm::mat4& datumMat);
		Datum& operator=(Scope* datumScope);
		Datum& operator=(const std::string& datumString);
		Datum& operator=(Library::RTTI* datumRttiPointer);

		/// ********************************************************************************
		/// <summary>
		/// Destructor freeing data if we've allocated any, a noop if we're external data
		/// </summary>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		~Datum();

		/// ********************************************************************************
		/// <summary>
		/// PushBack for each type of Datum available. Throws exceptions if you push back
		///  the wrong type of data. May do an expensive realloc if there is not space for
		///  the incoming data
		/// </summary>
		/// <param name="datumInt">Datum data to push back</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void PushBack(int32_t datumInt);
		void PushBack(float_t datumFloat);
		void PushBack(const glm::vec4& datumVec);
		void PushBack(const glm::mat4& datumMat);
		void PushBack(Scope* datumScope);
		void PushBack(const std::string& datumString);
		void PushBack(Library::RTTI* datumRttiPointer);

		/// ********************************************************************************
		/// <summary>
		/// Set for each type of Datum available. Throws exceptions if you set
		///  the wrong type of data. Also throws out of bounds exceptions. Can be used on
		///  scalar type datums.
		/// </summary>
		/// <param name="datumInt">Data attempting to set</param>
		/// <param name="datumInt">Index at which to set data. Defaults to 0</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void Set(int32_t datumInt, uint32_t index = 0);
		void Set(float_t datumFloat, uint32_t index = 0);
		void Set(const glm::vec4& datumVec, uint32_t index = 0);
		void Set(const glm::mat4& datumMat, uint32_t index = 0);
		void Set(Scope* datumScope, uint32_t index = 0);
		void Set(const std::string& datumString, uint32_t index = 0);
		void Set(Library::RTTI* datumRttiPointer, uint32_t index = 0);

		/// ********************************************************************************
		/// <summary>
		/// Find functions for finding a certain data type. May return the index where it's found
		/// </summary>
		/// <param name="datumInt">The data to search for in the datum</param>
		/// <param name="index">The index at which it's fine</param>
		/// <returns>True if item was found, false if not</returns>
		/// <created>Jacob Lites,2/16/2018</created>
		/// <changed>Jacob Lites,2/16/2018</changed>
		/// ********************************************************************************
		bool Find(int32_t datumInt);
		bool Find(int32_t datumInt, uint32_t& index);
		bool Find(float_t datumFloat);
		bool Find(float_t datumFloat, uint32_t& index);
		bool Find(const glm::vec4& datumVec);
		bool Find(const glm::vec4& datumVec, uint32_t& index);
		bool Find(const glm::mat4& datumMat);
		bool Find(const glm::mat4& datumMat, uint32_t& index);
		bool Find(const Scope& datumScope);
		bool Find(const Scope& datumScope, uint32_t& index);
		bool Find(const std::string& datumString);
		bool Find(const std::string& datumString, uint32_t& index);
		bool Find(Library::RTTI* datumRttiPointer);
		bool Find(Library::RTTI* datumRttiPointer, uint32_t& index);

		/// ********************************************************************************
		/// <summary>
		/// Set to External Storage for each type of Datum available. Throws exceptions if
		///  Already set to the wrong type of data. We will NOT destruct the incoming data
		///  on destructor. Data is not owned by us. Calling resize or pushback will throw
		///  exceptions, because we don't own the pointer. You CAN continue to get and set
		///  the data.
		/// </summary>
		/// <param name="datumInt">Data type array pointer</param>
		/// <param name="datumInt">Size of incoming array</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void SetStorage(int32_t* externalStorage, uint32_t size);
		void SetStorage(float_t* externalStorage, uint32_t size);
		void SetStorage(glm::vec4* externalStorage, uint32_t size);
		void SetStorage(glm::mat4* externalStorage, uint32_t size);
		void SetStorage(Scope** externalStorage, uint32_t size);
		void SetStorage(std::string* externalStorage, uint32_t size);
		void SetStorage(Library::RTTI** externalStorage, uint32_t size);

		/// ********************************************************************************
		/// <summary>
		/// Templated get function for specialization. Not implemented, only call specializations
		/// </summary>
		/// <param name="index">Index of type to get</param>
		/// <returns>Nothing. Don't call it. THing's will break</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template <typename T>
		T& Get(uint32_t index);
		template <typename T>
		T& Get();
		template <typename T>
		const T& Get(uint32_t index) const;
		template <typename T>
		const T& Get() const;

		/// ********************************************************************************
		/// <summary>
		/// Templated Get function specialization for ints. Has const and non-const versions.
		///  If no index is provided, number is defualted to 0. Will throw out of range exception
		///  if out of bounds.
		/// </summary>
		/// <param name="index">Index from which to grab the data reference.</param>
		/// <returns>Reference to the data point in the datum.</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template<>
		int32_t& Get<int32_t>(uint32_t index);
		template<>
		int32_t& Get<int32_t>();
		template<>
		const int32_t& Get<int32_t>(uint32_t index) const;
		template<>
		const int32_t& Get<int32_t>() const;

		/// ********************************************************************************
		/// <summary>
		/// Templated Get function specialization for floats. Has const and non-const versions.
		///  If no index is provided, number is defualted to 0. Will throw out of range exception
		///  if out of bounds.
		/// </summary>
		/// <param name="index">Index from which to grab the data reference.</param>
		/// <returns>Reference to the data point in the datum.</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template<>
		float_t& Get<float_t>(uint32_t index);
		template<>
		float_t& Get<float_t>();
		template<>
		const float_t& Get<float_t>(uint32_t index) const;
		template<>
		const float_t& Get<float_t>() const;

		/// ********************************************************************************
		/// <summary>
		/// Templated Get function specialization for vec4. Has const and non-const versions.
		///  If no index is provided, number is defualted to 0. Will throw out of range exception
		///  if out of bounds.
		/// </summary>
		/// <param name="index">Index from which to grab the data reference.</param>
		/// <returns>Reference to the data point in the datum.</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template<>
		glm::vec4& Get<glm::vec4>(uint32_t index);
		template<>
		glm::vec4& Get<glm::vec4>();
		template<>
		const glm::vec4& Get<glm::vec4>(uint32_t index) const;
		template<>
		const glm::vec4& Get<glm::vec4>() const;

		/// ********************************************************************************
		/// <summary>
		/// Templated Get function specialization for mat4. Has const and non-const versions.
		///  If no index is provided, number is defualted to 0. Will throw out of range exception
		///  if out of bounds.
		/// </summary>
		/// <param name="index">Index from which to grab the data reference.</param>
		/// <returns>Reference to the data point in the datum.</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template<>
		glm::mat4& Get<glm::mat4>(uint32_t index);
		template<>
		glm::mat4& Get<glm::mat4>();
		template<>
		const glm::mat4& Get<glm::mat4>(uint32_t index) const;
		template<>
		const glm::mat4& Get<glm::mat4>() const;

		/// ********************************************************************************
		/// <summary>
		/// Templated Get function specialization for Scopes. Has const and non-const versions.
		///  If no index is provided, number is defualted to 0. Will throw out of range exception
		///  if out of bounds.
		/// </summary>
		/// <param name="index">Index from which to grab the data reference.</param>
		/// <returns>Reference to the data point in the datum.</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template<>
		Scope*& Get<Scope*>(uint32_t index);
		template<>
		Scope*& Get<Scope*>();
		template<>
		Scope * const& Get<Scope*>(uint32_t index) const;
		template<>
		Scope * const& Get<Scope*>() const;

		/// ********************************************************************************
		/// <summary>
		/// Templated Get function specialization for string. Has const and non-const versions.
		///  If no index is provided, number is defualted to 0. Will throw out of range exception
		///  if out of bounds.
		/// </summary>
		/// <param name="index">Index from which to grab the data reference.</param>
		/// <returns>Reference to the data point in the datum.</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template<>
		std::string& Get<std::string>(uint32_t index);
		template<>
		std::string& Get<std::string>();
		template<>
		const std::string& Get<std::string>(uint32_t index) const;
		template<>
		const std::string& Get<std::string>() const;

		/// ********************************************************************************
		/// <summary>
		/// Templated Get function specialization for RTTI type object pointers. Has const and non-const versions.
		///  If no index is provided, number is defualted to 0. Will throw out of range exception
		///  if out of bounds.
		/// </summary>
		/// <param name="index">Index from which to grab the data reference.</param>
		/// <returns>Reference to the data point in the datum.</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		template<>
		Library::RTTI*& Get<Library::RTTI*>(uint32_t index);
		template<>
		Library::RTTI*& Get<Library::RTTI*>();
		template<>
		Library::RTTI * const& Get<Library::RTTI*>(uint32_t index) const;
		template<>
		Library::RTTI * const& Get<Library::RTTI*>() const;

		/// ********************************************************************************
		/// <summary>
		/// Function for clearing all data in the Datum. Will destruct and remove the data,
		///  but will not affect capacity.
		/// </summary>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void Clear();

		/// ********************************************************************************
		/// <summary>
		/// Attempts to remove the element at "index" and scootches all further elements
		///  one unit down. Will throw an exception if the index is out of bounds
		/// </summary>
		/// <param name="index">Index of data type to remove</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void Remove(uint32_t index);

		/// ********************************************************************************
		/// <summary>
		/// Returns the current data type of the datum
		/// </summary>
		/// <returns>Current internal datum type</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		DatumType Type() const;

		/// ********************************************************************************
		/// <summary>
		/// Returns number of elements we have space allocated for currently
		/// </summary>
		/// <returns>Number of data elements we have space for</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		uint32_t Capacity() const;

		/// ********************************************************************************
		/// <summary>
		/// Returns number of elements we have in the datum
		/// </summary>
		/// <returns>Number of data elements we have stored in datum</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		uint32_t Size() const;

		/// ********************************************************************************
		/// <summary>
		/// Sets the internal type of the datum. You CANNOT change this after it is set for
		///  a datum. Will throw an exception if it's already been set
		/// </summary>
		/// <param name="type">Type of data we want the datum to hold</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void SetType(DatumType type);

		/// ********************************************************************************
		/// <summary>
		/// Reserves space for up to "newCapacity" data elements. We must know datum's current
		///  type before calling, or we'll throw an exception. Will not shrink capacity
		/// </summary>
		/// <param name="newCapacity">The number of elements we'll reserve space for in the datum</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void Reserve(uint32_t newCapacity);

		/// ********************************************************************************
		/// <summary>
		/// Resizes the Datum. After this call, we'll have "newCapacity" elements of data in
		///  the array. If newCapacity is less than current size, we'll destruct elements. If
		///  it's smaller, we'll allocate space and elements for "newCapacity" amount. Will
		///  throw an exception if type is not set
		/// </summary>
		/// <param name="newCapacity">Requested new size of the datum</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void Resize(uint32_t newCapacity);

		/// ********************************************************************************
		/// <summary>
		/// Attempts to set the data at element "index" using the string "data". Will throw
		///  an exception if type is not set or index is out of bounds. Throws exception
		///  for RTTI pointers
		/// </summary>
		/// <param name="data">String from which to set data at index</param>
		/// <param name="index">Index of data to set</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void SetFromString(std::string data, uint32_t index = 0);

		/// ********************************************************************************
		/// <summary>
		/// Retrieves String representation of data at element "index" if it exists. Throws
		///  an exception if type is not set or index is out of bounds.
		/// </summary>
		/// <param name="index">Index at which to retrieve string of data</param>
		/// <returns>String representing data at element 'index'</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		std::string ToString(uint32_t index = 0) const;

		/// ********************************************************************************
		/// <summary>
		/// Operator == for checking equality of two datums. Will check size and all elements
		///  stored in the datum.
		/// </summary>
		/// <param name="rhs">Datum to compare against</param>
		/// <returns>True if datums are the same</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		bool operator==(const Datum& rhs) const;
		bool operator!=(const Datum& rhs) const;

		/// ********************************************************************************
		/// <summary>
		/// Accessor method for internal "mExternal" value
		/// </summary>
		/// <returns>True if Datum is external</returns>
		/// <created>Jacob Lites,2/18/2018</created>
		/// <changed>Jacob Lites,2/18/2018</changed>
		/// ********************************************************************************
		inline bool IsExternal() const { return mIsExternal; };

	private:
		/// ********************************************************************************
		/// <summary>
		/// Private helper constructor with datumType
		/// </summary>
		/// <param name="type">Type of datum being constructed</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		Datum(DatumType type);

		/// ********************************************************************************
		/// <summary>
		/// Helper function for copying an rhs datum while this datum is external storage
		/// </summary>
		/// <param name="rhs">Datum to copy from</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void ExternalStorageCopy(const Datum& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Helper function for copying an rhs datum while this datum is internal storage
		/// </summary>
		/// <param name="rhs">Datum to copy from</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void InternalStorageCopy(const Datum& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Helper function for resetting rhs datum to a empty, usable state
		/// </summary>
		/// <param name="rhs">Datum to reset to a state</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void ResetDatum(Datum& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Compare Function for internal function pointers. Each gets called on compare attempts
		///  between datums of the same type.
		/// </summary>
		/// <param name="rhs">Datum to compare this datum against</param>
		/// <returns>True if the data in the datums is the same</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		bool CompareIntDatums(const Datum & rhs) const;
		bool CompareFloatDatums(const Datum & rhs) const;
		bool CompareVectorDatums(const Datum & rhs) const;
		bool CompareMatrixDatums(const Datum & rhs) const;
		bool CompareTableDatums(const Datum & rhs) const;
		bool CompareStringDatums(const Datum & rhs) const;
		bool CompareRTTIDatums(const Datum & rhs) const;

		/// ********************************************************************************
		/// <summary>
		/// Setters from string for each data type for function pointer usage. Does a best guess
		///  at converting the incoming data string to the datum's current data type. 
		/// </summary>
		/// <param name="data">String representing internal data type</param>
		/// <param name="index">Index at which to set internal variable</param>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void SetIntFromString(std::string data, uint32_t index = 0);
		void SetFloatFromString(std::string data, uint32_t index = 0);
		void SetVectorFromString(std::string data, uint32_t index = 0);
		void SetMatrixFromString(std::string data, uint32_t index = 0);
		void SetStringFromString(std::string data, uint32_t index = 0);
		void SetRTTIFromString(std::string data, uint32_t index = 0);

		/// ********************************************************************************
		/// <summary>
		/// Returns a string representing the internal variables type at index. Returns out
		///  of bounds exception if index is not in size
		/// </summary>
		/// <param name="index">Element to turn into string</param>
		/// <returns>String representing data element at index</returns>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		std::string IntToString(uint32_t index) const;
		std::string FloatToString(uint32_t index) const;
		std::string VectorToString(uint32_t index) const;
		std::string MatrixToString(uint32_t index) const;
		std::string StringToString(uint32_t index) const;
		std::string RTTIToString(uint32_t index) const;
		std::string ScopeToString(uint32_t index) const;

		/// ********************************************************************************
		/// <summary>
		/// Helper function for reserving more space if adding an element would overflow up
		/// </summary>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void ReserveIfNecessary();

		/// ********************************************************************************
		/// <summary>
		/// Internal array for each datum type's sizes
		/// </summary>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		const static uint32_t DatumSizes[(uint32_t)DatumType::E_MAX];

		/// ********************************************************************************
		/// <summary>
		/// Function pointer array for member compare functions
		/// </summary>
		/// ********************************************************************************
		typedef bool (Datum::*DatumCompareFunc)(const Datum&) const;
		const static DatumCompareFunc DatumCompareFunctions[(uint32_t)DatumType::E_MAX];

		/// ********************************************************************************
		/// <summary>
		/// Function pointer array for member clear functions
		/// </summary>
		/// ********************************************************************************
		typedef void (Datum::*DatumClearFunc)();
		const static DatumClearFunc DatumClearFunctions[(uint32_t)DatumType::E_MAX];

		/// ********************************************************************************
		/// <summary>
		/// Function pointer array for member SetFromString functions
		/// </summary>
		/// ********************************************************************************
		typedef void (Datum::*DatumSetFromStringFunc)(std::string, uint32_t);
		const static DatumSetFromStringFunc DatumSetFromStringFunctions[(uint32_t)DatumType::E_MAX];

		/// ********************************************************************************
		/// <summary>
		/// Function pointer array for member ToString functions
		/// </summary>
		/// ********************************************************************************
		typedef std::string(Datum::*DatumToStringFunc)(uint32_t) const;
		const static DatumToStringFunc DatumToStringFunctions[(uint32_t)DatumType::E_MAX];

		/// ********************************************************************************
		/// <summary>
		/// Function pointer array for member DeepCopy functions
		/// </summary>
		/// ********************************************************************************
		typedef void (Datum::*DatumDeepCopyFunc)(const Datum&);
		const static DatumDeepCopyFunc DatumDeepCopyFunctions[(uint32_t)DatumType::E_MAX];

		/// ********************************************************************************
		/// <summary>
		/// Functions for different datum type destructors. Gets away with not deleting 
		///  basic types.
		/// </summary>
		/// <created>Jacob Lites,2/9/2018</created>
		/// <changed>Jacob Lites,2/9/2018</changed>
		/// ********************************************************************************
		void ClearNoDestructor();
		void ClearString();
		void ClearVector();
		void ClearMatrix();

		/// <summary>Union of pointers to stored data arrays</summary>
		StoredData mData;

		/// <summary>Type of data currently stored in the datum</summary>
		DatumType mType;

		/// <summary>The current capacity of the stored data array</summary>
		uint32_t mCapacity;

		/// <summary>The current number of elements in the stored datum array</summary>
		uint32_t mSize;

		/// <summary>A bool denoting wether or not mData is an internal or external reference</summary>
		bool mIsExternal;
	};

	template<>
	inline int32_t& Datum::Get<int32_t>(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_INTEGER) {
			throw std::exception("Attempting to acces non-int value in int datum");
		}
		return mData.mInt[index];
	}

	template<>
	inline int32_t& Datum::Get<int32_t>()
	{
		return Get<int32_t>(0);
	}

	template<>
	inline const int32_t& Datum::Get<int32_t>(uint32_t index) const
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_INTEGER) {
			throw std::exception("Attempting to acces non-int value in int datum");
		}
		return mData.mInt[index];
	}

	template<>
	inline const int32_t& Datum::Get<int32_t>() const
	{
		return Get<int32_t>(0);
	}

	template<>
	inline float_t& Datum::Get<float_t>(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_FLOAT) {
			throw std::exception("Attempting to acces non-float value in float datum");
		}
		return mData.mFloat[index];
	}

	template<>
	inline float_t& Datum::Get<float_t>()
	{
		return Get<float_t>(0);
	}

	template<>
	inline const float_t& Datum::Get<float_t>(uint32_t index) const
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_FLOAT) {
			throw std::exception("Attempting to acces non-float value in float datum");
		}
		return mData.mFloat[index];
	}

	template<>
	inline const float_t& Datum::Get<float_t>() const
	{
		return Get<float_t>(0);
	}

	template<>
	inline glm::vec4& Datum::Get<glm::vec4>(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_VECTOR) {
			throw std::exception("Attempting to acces non-vector value in vector datum");
		}
		return mData.mVector[index];
	}

	template<>
	inline glm::vec4& Datum::Get<glm::vec4>()
	{
		return Get<glm::vec4>(0);
	}

	template<>
	inline const glm::vec4& Datum::Get<glm::vec4>(uint32_t index) const
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_VECTOR) {
			throw std::exception("Attempting to acces non-vector value in vector datum");
		}
		return mData.mVector[index];
	}

	template<>
	inline const glm::vec4& Datum::Get<glm::vec4>() const
	{
		return Get<glm::vec4>(0);
	}

	template<>
	inline glm::mat4& Datum::Get<glm::mat4>(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_MATRIX) {
			throw std::exception("Attempting to acces non-matrix value in matrix datum");
		}
		return mData.mMatrix[index];
	}

	template<>
	inline glm::mat4& Datum::Get<glm::mat4>()
	{
		return Get<glm::mat4>(0);
	}


	template<>
	inline const glm::mat4& Datum::Get<glm::mat4>(uint32_t index) const
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_MATRIX) {
			throw std::exception("Attempting to acces non-matrix value in matrix datum");
		}
		return mData.mMatrix[index];
	}

	template<>
	inline const glm::mat4& Datum::Get<glm::mat4>() const
	{
		return Get<glm::mat4>(0);
	}

	template<>
	inline  Scope*& Datum::Get<Scope*>(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_TABLE) {
			throw std::exception("Attempting to acces non-Scope value in Scope datum");
		}
		return mData.mTable[index];
	}

	template<>
	inline Scope*& Datum::Get<Scope*>()
	{
		return Get<Scope*>(0);
	}

	template<>
	inline Scope* const& Datum::Get<Scope*>(uint32_t index) const
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_TABLE) {
			throw std::exception("Attempting to acces non-Scope value in Scope datum");
		}
		return mData.mTable[index];
	}

	template<>
	inline Scope* const& Datum::Get<Scope*>() const
	{
		return Get<Scope*>(0);
	}

	template<>
	inline std::string& Datum::Get<std::string>(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_STRING) {
			throw std::exception("Attempting to acces non-string value in string datum");
		}
		return mData.mString[index];
	}

	template<>
	inline std::string& Datum::Get<std::string>()
	{
		return Get<std::string>(0);
	}

	template<>
	inline const std::string& Datum::Get<std::string>(uint32_t index) const
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_STRING) {
			throw std::exception("Attempting to acces non-string value in string datum");
		}
		return mData.mString[index];
	}

	template<>
	inline const std::string& Datum::Get<std::string>() const
	{
		return Get<std::string>(0);
	}

	template<>
	inline Library::RTTI*& Datum::Get<Library::RTTI*>(uint32_t index)
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_USER_DEFINED) {
			throw std::exception("Attempting to acces non-RTTI value in RTTI datum");
		}
		return mData.mUserDefined[index];
	}

	template<>
	inline Library::RTTI*& Datum::Get<Library::RTTI*>()
	{
		return Get<Library::RTTI*>(0);
	}

	template<>
	inline Library::RTTI* const& Datum::Get<Library::RTTI*>(uint32_t index) const
	{
		if (index >= mSize) {
			throw std::exception("Accessing out of bounds index");
		}
		if (mType != DatumType::E_USER_DEFINED) {
			throw std::exception("Attempting to acces non-RTTI value in RTTI datum");
		}
		return mData.mUserDefined[index];
	}

	template<>
	inline Library::RTTI* const& Datum::Get<Library::RTTI*>() const
	{
		return Get<Library::RTTI*>(0);
	}
}

