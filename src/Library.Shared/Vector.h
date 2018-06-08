/// <summary>Vector.h File containing Vector definitions</summary>
#pragma once

namespace Library
{
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Vector class for holding data in contiguous member on the heap. If a data type is
	///   is added when the vector is at full capacity, capacity will auto expand. This allows
	///   O(1) access with penalties to insertion (which can be mitigated with accurate memory
	///   reservation).
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <typename T>
	class Vector
	{
		public:
			/// --------------------------------------------------------------------------------
			/// <summary>
			/// Vector iterator for data structure parsing
			/// </summary>
			/// --------------------------------------------------------------------------------
			class Iterator {
				friend class Vector;

			public:
				/// ********************************************************************************
				/// <summary>
				///  Iterator default constructor
				/// </summary>
				/// <created>Jacob Lites,1/29/2018</created>
				/// <changed>Jacob Lites,1/29/2018</changed>
				/// ********************************************************************************
				Iterator();

				/// ********************************************************************************
				/// <summary>
				///  Iterator copy constructor
				/// </summary>
				/// <param name="rhs">Iterator from which to copy</param>
				/// <created>Jacob Lites,1/29/2018</created>
				/// <changed>Jacob Lites,1/29/2018</changed>
				/// ********************************************************************************
				Iterator(const Iterator& rhs);

				/// ********************************************************************************
				/// <summary>
				/// Copy operator. Shallow copy due to lack of deep copy requirements.
				/// </summary>
				/// <param name="rhs">Iterator to copy from</param>
				/// <returns>Returns a reference to the original iterator for chaining</returns>
				/// <created>Jacob Lites,1/29/2018</created>
				/// <changed>Jacob Lites,1/29/2018</changed>
				/// ********************************************************************************
				Iterator& operator=(const Iterator& rhs);

				/// ********************************************************************************
				/// <summary>
				/// Equality operator for checking equivilance of iterators
				/// </summary>
				/// <param name="rhs">Iterator to check equivalence with</param>
				/// <returns>True if both iterators are referencing the same node in the same vector</returns>
				/// <created>Jacob Lites,1/29/2018</created>
				/// <changed>Jacob Lites,1/29/2018</changed>
				/// ********************************************************************************
				bool operator==(const Iterator& rhs) const;
				bool operator!=(const Iterator& rhs) const; //Opposite of equivalence operator 

				/// ********************************************************************************
				/// <summary>
				/// Iterator incrementor operator. Modifies the iterator to point to the next
				///   node in the vector.
				/// </summary>
				/// <returns>Returns a reference to the modified Iterator</returns>
				/// <created>Jacob Lites,1/29/2018</created>
				/// <changed>Jacob Lites,1/29/2018</changed>
				/// ********************************************************************************
				Iterator& operator++();

				/// ********************************************************************************
				/// <summary>
				/// Iterator incrementor operator. Modifies the iterator to point to the next
				///   node in the vector AFTER the code in the current line has completed.
				/// </summary>
				/// <returns>Returns a copy of the unmodified iterator</returns>
				/// <created>Jacob Lites,1/29/2018</created>
				/// <changed>Jacob Lites,1/29/2018</changed>
				/// ********************************************************************************
				Iterator operator++(int);
				T& operator*();

			private:
				/// ********************************************************************************
				/// <summary>
				/// Private iterator constructor. This is make sure that Vector is the only object
				///   to instantiate populated vectors
				/// </summary>
				/// <param name="owner">A reference to the vector that owns this iterator</param>
				/// <param name="index">The index representing our current place in the vector</param>
				/// <created>Jacob Lites,1/29/2018</created>
				/// <changed>Jacob Lites,1/29/2018</changed>
				/// ********************************************************************************
				Iterator(Vector<T>& owner, uint32_t index = 0);

				/// <summary>A pointer to the vector this iterator is referencing</summary>
				Vector<T>* mOwner;

				/// <summary>The of the vector that this iterator is currently referencing</summary>
				uint32_t mIndex;
			};

			/// ********************************************************************************
			/// <summary>
			/// Vector basic constructor, instantiates a new Vector.
			///   Reserves DEFAULT_CAPACITY (currently 10) spots for data.
			/// </summary>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			Vector();

			/// ********************************************************************************
			/// <summary>
			/// Vector constructor that reserves "capacity" spots of data in memory.
			/// </summary>
			/// <param name="capacity">The number of spots of data allocated on construction</param>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			Vector(uint32_t capacity);

			/// ********************************************************************************
			/// <summary>
			/// Deep copy constructor. Copy constructs all elements in the rhs vector. This will clear
			///   and destruct all items in the local vector
			/// </summary>
			/// <param name="rhs">The vector that we are going to deep copy into this vector</param>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			Vector(const Vector& rhs);

			/// ********************************************************************************
			/// <summary>
			/// Deep copy assignment operator. Clears and deletes all elements in the local vector
			///   and deep copies all elements from the rhs to the local vector
			/// </summary>
			/// <param name="rhs">The vector from which we are deep copying elements</param>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			Vector& operator=(const Vector& rhs);

			/// ********************************************************************************
			/// <summary>
			/// Destructor for deallocating reserved memory space and destructing all elements
			/// </summary>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			virtual ~Vector();

			/// ********************************************************************************
			/// <summary>
			/// Reserves space for designated number of elements in the vector. Throws an exception
			///  if this number is lower than the number of nodes currently in the vector. This requires
			///  a memmove to the new designated space
			/// </summary>
			/// <param name="newCapacity">Number of elements to reserve space for in the list.</param>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			void Reserve(uint32_t newCapacity);

			/// ********************************************************************************
			/// <summary>
			/// Bracket operator, returns a reference to the data at the incoming index. Throws an
			///  exception if there is not an element at that index
			/// </summary>
			/// <param name="index">The element to retrieve a reference to</param>
			/// <returns>A reference to the element at the incoming index</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			T& operator[](uint32_t index);
			const T& operator[](uint32_t index) const;

			/// ********************************************************************************
			/// <summary>
			///  Data retrieval function, returns a reference to the data at the incoming index. Throws an
			///  exception if there is not an element at that index
			/// </summary>
			/// <param name="index">The element to retrieve a reference to</param>
			/// <returns>A reference to the element at the incoming index</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			T& At(uint32_t index);
			const T& At(uint32_t index) const;

			/// ********************************************************************************
			/// <summary>
			/// Accessor to the internal size of the vector
			/// </summary>
			/// <returns>The current number of elements in the vector</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			uint32_t Size() const;

			/// ********************************************************************************
			/// <summary>
			/// Accessor to the internal capacity of the vector
			/// </summary>
			/// <returns>The current number of elements that the vector has memory reserved for</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			uint32_t Capacity() const;

			/// ********************************************************************************
			/// <summary>
			/// Boolean check for if the vector is empty or not
			/// </summary>
			/// <returns>True if there are 0 elements stored in the vector</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			bool IsEmpty() const;

			/// ********************************************************************************
			/// <summary>
			/// PushBack places a deep copy of the incoming T reference into the back of the
			///  vector. If there is not space for the element in the vector, we will resize
			///  the vector, which is an inefficient copy
			/// </summary>
			/// <param name="in">Data reference from which to insert a deep copy</param>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			void PushBack(const T& in);

			/// ********************************************************************************
			/// <summary>
			/// Destructs and removes the last element of the vector
			/// </summary>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			void PopBack();

			/// ********************************************************************************
			/// <summary>
			/// Destructs and removes all of the elements in the vector
			/// </summary>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			void Clear();

			/// ********************************************************************************
			/// <summary>
			/// Shrinks the capacity of the vector to the current size. This does not remove elements
			/// </summary>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			void ShrinkToFit();

			/// ********************************************************************************
			/// <summary>
			/// Removes an element at the given index from the list. This requires memory shuffling
			///   to keep all elements in their correct order and remove the gap left by the removed
			///   element. This will throw an exception if there is not an element at the given index
			/// </summary>
			/// <param name="index">The index from which to remove an element</param>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			void Remove(uint32_t index);

			/// ********************************************************************************
			/// <summary>
			/// Returns an iterator pointing to the first element that matches the incoming element.
			///   Returns Vector.end() if a match is not found.
			/// </summary>
			/// <param name="toFind">The object we're trying to match against</param>
			/// <returns>An iterator referencing the found item, or end()</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			Iterator Find(const T& toFind);

			/// ********************************************************************************
			/// <summary>
			/// Returns an iterator pointing to the first element in the vector
			/// </summary>
			/// <returns>An iterator referencing the first element in this vector</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			Iterator begin();

			/// ********************************************************************************
			/// <summary>
			/// Returns an iterator pointing to the last element in the vector
			/// </summary>
			/// <returns>An iterator referencing the last element in this vector</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			Iterator end();

			/// ********************************************************************************
			/// <summary>
			/// Returns a reference to the first element of the vector. Throws an exception if
			///   the list is empty
			/// </summary>
			/// <returns>A reference to the first element in the vector</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			T& Front();
			const T& Front() const;

			/// ********************************************************************************
			/// <summary>
			/// Returns a reference to the last element of the vector. Throws an exception if
			///   the list is empty
			/// </summary>
			/// <returns>A reference to the last element in the vector</returns>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			T& Back();
			const T& Back() const;

		private:

			/// ********************************************************************************
			/// <summary>
			/// Helper function that deep copies elements from rhs into the local copy
			/// </summary>
			/// <param name="rhs">The vector from which to deep copy elements</param>
			/// <created>Jacob Lites,1/29/2018</created>
			/// <changed>Jacob Lites,1/29/2018</changed>
			/// ********************************************************************************
			void DeepCopy(const Vector& rhs);

			/// <summary>Pointer to the beginning of the allocated memory for our elements</summary>
			T* mDataStart;

			/// <summary>The current number of elements in the vector</summary>
			uint32_t mSize;

			/// <summary>The number of elements that we have allocated space for</summary>
			uint32_t mCapacity;
	};
}

#include "Vector.inl"
