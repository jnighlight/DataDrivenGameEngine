/// <summary>HashMap.h File containing HashMap definitions</summary>
#pragma once
#include <cstdint>
#include <string>
#include "SList.h"
#include "Vector.h"

namespace Library
{
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// A default naive Hashing functor class. Has overrides for char* and std::string
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <typename TKey>
	class DefaultHashFunctor {
	public:
		/// ********************************************************************************
		/// <summary>
		/// Hashes template object of type TKey by iterating and adding bytes. WARNING: If your
		///   data type has pointers, this will cause false negatives. Write your own
		/// </summary>
		/// <param name="objectToHash">const TKey& to additive hash bytes together</param>
		/// <returns>Additive hash of bytes in objectToHash</returns>
		/// <created>Jacob Lites,2/1/2018</created>
		/// <changed>Jacob Lites,2/1/2018</changed>
		/// ********************************************************************************
		uint32_t operator()(const TKey& objectToHash) const {
			const uint8_t* data = reinterpret_cast<const uint8_t*>(&objectToHash);
			uint32_t size = static_cast<uint32_t>(sizeof(TKey));
			return AdditiveHash(data, size);
		}

		/// ********************************************************************************
		/// <summary>
		/// The actual hashing function, adds size number of bytes from data together for hash
		/// </summary>
		/// <param name="data">const uint8_t* from which to add data</param>
		/// <param name="size">Number of bytes to read from data</param>
		/// <returns>returns added bytes from data</returns>
		/// <created>Jacob Lites,2/1/2018</created>
		/// <changed>Jacob Lites,2/1/2018</changed>
		/// ********************************************************************************
		uint32_t AdditiveHash(const uint8_t* data, uint32_t size) const {
			uint32_t addHash = 0;
			for (uint32_t i = 0; i < size; ++i) {
				addHash += data[i];
			}
			return addHash;
		}
	};

	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Char* override of default hash functor
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <>
	class DefaultHashFunctor<char*> {
	public:
		/// ********************************************************************************
		/// <summary>
		/// Same as default, but reads the stored chars in the char*
		/// </summary>
		/// <param name="charsToHash">const char* from which to read char bytes</param>
		/// <returns>Additive hashed char*</returns>
		/// <created>Jacob Lites,2/1/2018</created>
		/// <changed>Jacob Lites,2/1/2018</changed>
		/// ********************************************************************************
		uint32_t operator()(const char* charsToHash) const {
			const uint8_t* data = reinterpret_cast<const uint8_t*>(charsToHash);
			uint32_t size = static_cast<uint32_t>(strlen(charsToHash));
			return AdditiveHash(data, size);
		}

		uint32_t AdditiveHash(const uint8_t* data, uint32_t size) const {
			uint32_t addHash = 0;
			for (uint32_t i = 0; i < size; ++i) {
				addHash += data[i];
			}
			return addHash;
		}
	};
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Same as default, but iterates through std::string elements instead of total object data
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <>
	class DefaultHashFunctor<std::string> {
	public:
		/// ********************************************************************************
		/// <summary>
		/// Iterates through all elements of std::string to create hash
		/// </summary>
		/// <param name="stringToHash">const std::string& from which to grab char bytes to hash</param>
		/// <returns>Hashed version of std::string&</returns>
		/// <created>Jacob Lites,2/1/2018</created>
		/// <changed>Jacob Lites,2/1/2018</changed>
		/// ********************************************************************************
		uint32_t operator()(const std::string& stringToHash) const {
			uint32_t addHash = 0;
			for (uint32_t i = 0; i < stringToHash.length(); ++i) {
				addHash += stringToHash[i];
			}
			return addHash;
		}
	};

	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Default comparator for TKey objects
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <typename TKey>
	class DefaultComparatorFunctor {
	public:
		/// ********************************************************************************
		/// <summary>
		/// Simple basic comparator for two TKey types. Uses == operator
		/// </summary>
		/// <param name="lhs">Left hand side of == call</param>
		/// <param name="rhs">Right hand side of == call</param>
		/// <returns>True if both pass an operator == check</returns>
		/// <created>Jacob Lites,2/1/2018</created>
		/// <changed>Jacob Lites,2/1/2018</changed>
		/// ********************************************************************************
		bool operator()(const TKey& lhs, const TKey& rhs) const {
			return lhs == rhs;
		}
	};
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Overwritten comparator for char*
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <>
	class DefaultComparatorFunctor<char*> {
	public:
		/// ********************************************************************************
		/// <summary>
		/// Checks contents of char* and size to make sure pointer check isn't occuring
		/// </summary>
		/// <param name="lhs">Left hand side char*</param>
		/// <param name="rhs">Right hand side char*</param>
		/// <returns>True if size and contents of char* are same</returns>
		/// <created>Jacob Lites,2/1/2018</created>
		/// <changed>Jacob Lites,2/1/2018</changed>
		/// ********************************************************************************
		bool operator()(const char* lhs, const char* rhs) const {
			return (strlen(lhs) == strlen(lhs)) && (strcmp(lhs, rhs) == 0);
		}
	};

	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Hashmap correlating keys to data with optional providable hash functors and comparator functors
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <typename TKey, typename TData, typename HashFunctor = DefaultHashFunctor<TKey>, typename ComparatorFunctor = DefaultComparatorFunctor<TKey>>
	class HashMap
	{
		/// <summary>Default bucket count on construction. Because I said so</summary>
		static const uint8_t DEFAULT_BUCKET_COUNT = 10;

		typedef std::pair<TKey, TData> PairType;
		typedef SList<PairType> ChainType;
		typedef typename SList<PairType>::Iterator ChainIterator;
		typedef Vector<ChainType> BucketType;

		public:
			/// --------------------------------------------------------------------------------
			/// <summary>
			/// Hashmap iterator. No order is promised
			/// </summary>
			/// --------------------------------------------------------------------------------
			class Iterator {
				/// --------------------------------------------------------------------------------
				/// <summary>
				/// Friends with HashMap for instantiation
				/// </summary>
				/// --------------------------------------------------------------------------------
				friend class HashMap;

			public:
				/// ********************************************************************************
				/// <summary>
				/// Basic iterator constructor. This means there is no owner for this constructor.
				/// </summary>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				Iterator();
				/// ********************************************************************************
				/// <summary>
				/// Iterator copy constructor
				/// </summary>
				/// <param name="rhs">rhs to copy from</param>
				/// <returns></returns>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				Iterator(const Iterator& rhs);

				/// ********************************************************************************
				/// <summary>
				/// Assignment copy.
				/// </summary>
				/// <param name="rhs">rhs to copy from</param>
				/// <returns></returns>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				Iterator& operator=(const Iterator& rhs);
				/// ********************************************************************************
				/// <summary>
				/// Iterator comparator.
				/// </summary>
				/// <param name="rhs">Iterator to compare against</param>
				/// <returns></returns>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				bool operator==(const Iterator& rhs) const;
				bool operator!=(const Iterator& rhs) const; //Opposite of equivalence operator 

				/// ********************************************************************************
				/// <summary>
				/// Increment functions for the iterator
				/// </summary>
				/// <returns>pre or post incremented iterator</returns>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				Iterator& operator++();
				Iterator operator++(int);

				/// ********************************************************************************
				/// <summary>
				/// Dereference operator
				/// </summary>
				/// <returns>Reference to the pairtype referenced by the Iterator</returns>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				PairType& operator*();

				/// ********************************************************************************
				/// <summary>
				/// Arrow operator
				/// </summary>
				/// <returns>pointer to pairtype referred to by iterator</returns>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				PairType* operator->();

			private:
				/// ********************************************************************************
				/// <summary>
				/// Private constructor for hash denoting a specific element in the hash
				/// </summary>
				/// <param name="owner">The HashMap that owns this iterator</param>
				/// <param name="it">Iterator referencing the node in the internal SList that we're on</param>
				/// <param name="index">Current bucket index the iterator is referring to</param>
				/// <created>Jacob Lites,2/1/2018</created>
				/// <changed>Jacob Lites,2/1/2018</changed>
				/// ********************************************************************************
				Iterator(HashMap<TKey, TData, HashFunctor, ComparatorFunctor>& owner, ChainIterator it = ChainIterator(), uint32_t index = 0);

				/// <summary>HashMap that this iterator is referencing</summary>
				HashMap<TKey, TData, HashFunctor, ComparatorFunctor>* mOwner;
				/// <summary>Current bucket index</summary>
				uint32_t mIndex;
				/// <summary>Iterator pointing to referenced element in the hashmap</summary>
				ChainIterator mCurListIterator;
			};

			/// ********************************************************************************
			/// <summary>
			/// Default hashmap constructor, creates DEFAULT_BUCKET_COUNT buckets
			/// </summary>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			HashMap();

			/// ********************************************************************************
			/// <summary>
			/// bucketcount hashmap constructor, creates capacity buckets
			/// </summary>
			/// <param name="capacity">Number of buckets to create</param>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			explicit HashMap(uint32_t capacity);

			/// <summary>default copy constructor</summary>
			HashMap(const HashMap& rhs) = default;
			/// <summary>default assignment copy operator</summary>
			HashMap& operator=(const HashMap& rhs) = default;
			/// <summary>default move constructor</summary>
			HashMap(HashMap&& rhs) = default;
			/// <summary>default assignment move operator</summary>
			HashMap& operator=(HashMap&& rhs) = default;

			/// <summary>Hashmap destructor</summary>
			virtual ~HashMap();


			/// ********************************************************************************
			/// <summary>
			/// Returns number of elements in the Hash
			/// </summary>
			/// <returns>Element count in hash</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			uint32_t Size() const;
			/// ********************************************************************************
			/// <summary>
			/// Returns true if element count in hash is 0
			/// </summary>
			/// <returns>True if hash is empty</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			bool IsEmpty() const;

			/// ********************************************************************************
			/// <summary>
			/// Returns number of buckets in the hash for informative purposes
			/// </summary>
			/// <returns>The number of buckets in the hash</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			uint32_t BucketCount() const;

			/// ********************************************************************************
			/// <summary>
			/// Removes all elements from the hash, deleting them
			/// </summary>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			void Clear();

			/// ********************************************************************************
			/// <summary>
			/// Removes the object stored at "TKey" spot in the hash. If element doesn't exist,
			/// nothing happens
			/// </summary>
			/// <param name="key">The key to remove from in the hash</param>
			/// <returns>True if the value has been removed, false otherwise</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,3/17/2018</changed>
			/// ********************************************************************************
			bool Remove(const TKey& key);

			/// ********************************************************************************
			/// <summary>
			/// Inserts the TData in PairType into TKey location in the hash. If the TKey already
			///   exists, the value is left alone
			/// </summary>
			/// <param name="entry">std::pair with TKey and TData to store in hash</param>
			/// <returns>Iterator pointing to the inserted or matched element</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			Iterator Insert(const PairType& entry);

			/// ********************************************************************************
			/// <summary>
			/// Finds an element in the hash and returns an iterator pointing to it. Returns
			///   end() if not found
			/// </summary>
			/// <param name="key">Key to search for element with</param>
			/// <returns>Iterator referencing matched element. end() if not found</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			Iterator Find(const TKey& key) const;

			/// ********************************************************************************
			/// <summary>
			/// Returns iterator pointing to first element in the unordered hash map
			/// </summary>
			/// <returns>iterator pointing to first element in the unordered hash map</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			Iterator begin();
			Iterator begin() const;

			/// ********************************************************************************
			/// <summary>
			/// Returns iterator pointing to last element in the unordered hash map
			/// </summary>
			/// <returns>iterator pointing to last element in the unordered hash map</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			Iterator end();
			Iterator end() const;

			/// ********************************************************************************
			/// <summary>
			/// Checks to see if a given key exists in the hashmap
			/// </summary>
			/// <param name="key">Key to match against</param>
			/// <returns>True if given key is found</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			bool ContainsKey(const TKey& key);

			/// ********************************************************************************
			/// <summary>
			/// Returns TData reference to element at key location. If element does not exists,
			///   creates on with default constructor
			/// </summary>
			/// <param name="key">Key to match against</param>
			/// <returns>TData reference to matched or created element</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			TData& operator[](const TKey& key);

			/// ********************************************************************************
			/// <summary>
			/// Returns TData reference to element at key location. If element does not exists,
			///   throws an exception
			/// </summary>
			/// <param name="key">Key to match against</param>
			/// <returns>TData reference to matched element</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			TData& At(const TKey& key);
			const TData& At(const TKey& key) const;

			/// ********************************************************************************
			/// <summary>
			/// Public helper function for finding matching element and populating iterator
			///   if it's found
			/// </summary>
			/// <param name="key">Key to match against in hash</param>
			/// <param name="iter">Iterator to populate if match is found</param>
			/// <returns>True if key is matched</returns>
			/// <created>Jacob Lites,2/1/2018</created>
			/// <changed>Jacob Lites,2/1/2018</changed>
			/// ********************************************************************************
			bool ContainsKey(const TKey& key, Iterator& iter);

		private:

			/// <summary>Bucket Vector for holding the SLists</summary>
			BucketType mBuckets;
			/// <summary>Number of "buckets" in the vector</summary>
			uint32_t mBucketCount;
			/// <summary>Current number of elements in the hashmap</summary>
			uint32_t mSize;
			/// <summary>Default or provided functor for hashing keys</summary>
			HashFunctor hashFunctor;
			/// <summary>Default or provided functor for key comparisons</summary>
			ComparatorFunctor compareFunctor;
	};
}

#include "HashMap.inl"
