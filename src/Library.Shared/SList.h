/// <summary>SList.h File containing SList definitions</summary>
#pragma once
#include <cstdint>

namespace Library
{
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Singly Linked List Templated implementation for the Game Engine Library
	/// </summary>
	/// --------------------------------------------------------------------------------
	template <typename T>
	class SList
	{
		private:
			/// --------------------------------------------------------------------------------
			/// <summary>
			/// Private Node Object for holding Data in the Singly Linked List
			/// </summary>
			/// --------------------------------------------------------------------------------
			class Node {
				public:
					/// ********************************************************************************
					/// <summary>
					/// Constructor for Nodes. Deep copies incoming templated data object and sets up the next node
					///   in the list
					/// </summary>
					/// <param name="data">const T& to deep copy into the node.</param>
					/// <param name="next">
					/// Node* pointing to the next Node in the list. This is a nullptr if it's the last Node in the list.
					/// </param>
					/// <created>Jacob Lites,1/17/2018</created>
					/// <changed>Jacob Lites,1/17/2018</changed>
					/// ********************************************************************************
					Node(const T& data, Node* next = nullptr);
					/// <summary>Internal T member that is a deep copy of the passed in T Data</summary>
					T mData;
					/// <summary>Node * to the next node in the list. Is nullptr if this is the last Node in the list</summary>
					Node* mNext;
			};
		public:
			/// --------------------------------------------------------------------------------
			/// <summary>
			/// Iterator class for proceding through a SList
			/// </summary>
			/// --------------------------------------------------------------------------------
			class Iterator {
				/// --------------------------------------------------------------------------------
				/// <summary>
				/// Friend class with SList so SList can call Iterator constructor.
				/// Enforces user to follow our interface
				/// </summary>
				/// --------------------------------------------------------------------------------
				friend class SList;

			public:
				/// ********************************************************************************
				/// <summary>
				/// Default iterator constructor. Sets owner and current node to nullptrs
				/// </summary>
				/// <created>Jacob Lites,1/22/2018</created>
				/// <changed>Jacob Lites,1/22/2018</changed>
				/// ********************************************************************************
				Iterator();

				/// ********************************************************************************
				/// <summary>
				/// Copy constructor. Shallow copy because the iterator does not allocate any memory
				///   itself
				/// </summary>
				/// <param name="rhs">The Iterator we're copying from</param>
				/// <returns></returns>
				/// <created>Jacob Lites,1/22/2018</created>
				/// <changed>Jacob Lites,1/22/2018</changed>
				/// ********************************************************************************
				Iterator(const Iterator& rhs);

				/// ********************************************************************************
				/// <summary>
				/// Assignment operator. Shallow copy because the iterator does not allocate any memory
				///   itself
				/// </summary>
				/// <param name="rhs">The iterator we're copying from</param>
				/// <returns>A reference to this Iterator</returns>
				/// <created>Jacob Lites,1/22/2018</created>
				/// <changed>Jacob Lites,1/22/2018</changed>
				/// ********************************************************************************
				Iterator& operator=(const Iterator& rhs);

				/// ********************************************************************************
				/// <summary>
				/// Equivalence operator. Checks owner and node for equivalence.
				/// </summary>
				/// <param name="rhs">Iterator to check equivalence to</param>
				/// <returns>Bool, true if they are equivalent iterators</returns>
				/// <created>Jacob Lites,1/22/2018</created>
				/// <changed>Jacob Lites,1/22/2018</changed>
				/// ********************************************************************************
				bool operator==(const Iterator& rhs) const;
				bool operator!=(const Iterator& rhs) const; //Opposite of equivalence operator

				/// ********************************************************************************
				/// <summary>
				/// Pre and Post Increment to move the iterator before or after the line of code
				///   this operator is on.
				/// </summary>
				/// <returns>
				/// The iterator at it's incremented state for the PreIncrement, and it's current state
				///   for the postIncrement
				/// </returns>
				/// <created>Jacob Lites,1/22/2018</created>
				/// <changed>Jacob Lites,1/22/2018</changed>
				/// ********************************************************************************
				Iterator& operator++();
				Iterator operator++(int);

				/// ********************************************************************************
				/// <summary>
				/// Deference operator for accessing T& from the data in the Node in the iterator
				/// WARNING: Throws an exception if our current node is nullptr
				/// </summary>
				/// <returns>A reference to the T object in our current node</returns>
				/// <created>Jacob Lites,1/22/2018</created>
				/// <changed>Jacob Lites,1/22/2018</changed>
				/// ********************************************************************************
				T& operator*();
				const T& operator*() const;

			private:
				/// ********************************************************************************
				/// <summary>
				/// Private constructor for SList Iterator creation
				/// </summary>
				/// <param name="owner">The SList owner of the Iterator</param>
				/// <param name="currentNode">The node that the current iterator is on</param>
				/// <created>Jacob Lites,1/22/2018</created>
				/// <changed>Jacob Lites,1/22/2018</changed>
				/// ********************************************************************************
				Iterator(const SList<T>& owner, Node* currentNode);

				/// <summary>The SList that this iterator is iterating through. Nullptr if there isn't one</summary>
				const SList<T>* mOwner;

				/// <summary>The Current node in mOwner. Nullptr if we're at the end or don't have an mOwner</summary>
				Node* mCurrentNode;
			};

			/// ********************************************************************************
			/// <summary>
			/// Returns an iterator pointing at the first Node in this list with this list as it's owner
			/// </summary>
			/// <returns>an Iterator pointing at the first Node in this list with this list as it's owner</returns>
			/// <created>Jacob Lites,1/22/2018</created>
			/// <changed>Jacob Lites,1/22/2018</changed>
			/// ********************************************************************************
			Iterator begin() const;

			/// ********************************************************************************
			/// <summary>
			/// Returns an iterator with a nullptr and this list as it's owner to signify the node after the last
			///   Node in this list
			/// </summary>
			/// <returns>an Iterator with a nullptr Node and a pointer to this list</returns>
			/// <created>Jacob Lites,1/22/2018</created>
			/// <changed>Jacob Lites,1/22/2018</changed>
			/// ********************************************************************************
			Iterator end() const;

			/// ********************************************************************************
			/// <summary>
			/// Inserts a Node created with T data into this list after the node specified by
			///   the given operator. If inserted at list.end(), will pushback
			/// WARNING: Will throw an exception if iter does not belong to this list. 
			///   Because that's a big mistake.
			/// </summary>
			/// <param name="iter">Iterator after which to insert a Node with T data</param>
			/// <param name="data">T data to insert after Node in iter</param>
			/// <created>Jacob Lites,1/22/2018</created>
			/// <changed>Jacob Lites,1/22/2018</changed>
			/// ********************************************************************************
			void InsertAfter(const Iterator& iter, const T& data);

			/// ********************************************************************************
			/// <summary>
			/// Finds a data node with equivalence to provided const T&. Returns list.end() if
			///   one doesn't exist
			/// </summary>
			/// <param name="data">A const reference to a data to find in the SList</param>
			/// <returns>An iterator pointing at the Node with the equivalent Data</returns>
			/// <created>Jacob Lites,1/22/2018</created>
			/// <changed>Jacob Lites,1/22/2018</changed>
			/// ********************************************************************************
			Iterator Find(const T& data) const;

			/// ********************************************************************************
			/// <summary>
			/// Removes the first Node with a matching instance of const T& value
			/// </summary>
			/// <param name="value">Value to check for in SList</param>
			/// <created>Jacob Lites,1/22/2018</created>
			/// <changed>Jacob Lites,1/22/2018</changed>
			/// ********************************************************************************
			void RemoveFirst(const T& value);

			/// ********************************************************************************
			/// <summary>
			/// Removes all Nodes with a matching instance of const T& value
			/// </summary>
			/// <param name="value">Value to check for in SList</param>
			/// <created>Jacob Lites,1/22/2018</created>
			/// <changed>Jacob Lites,1/22/2018</changed>
			/// ********************************************************************************
			void RemoveAll(const T& value);

			/// ********************************************************************************
			/// <summary>
			/// SList default constructor. Internal nodes are set to nullptr
			/// </summary>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			SList();

			/// ********************************************************************************
			/// <summary>
			/// SList Copy constructor. Deep copies data in the rhs Nodes into member Nodes
			/// </summary>
			/// <param name="rhs">const SList& to deep copy templated node data from</param>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			SList(const SList& rhs);

			/// ********************************************************************************
			/// <summary>
			/// SList destructor. This frees all nodes instantiated by this SList. NOTE that this
			///   invalidates any references retrieved through the Front() and Back() calls
			/// </summary>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			~SList();

			/// ********************************************************************************
			/// <summary>
			/// Assignment operator. This will delete all data currently stored in the lhs SList.
			///   NOTE that this will invalidated any references from Front() and Back() calls. Then
			///   deep copies Node data from rhs into member Nodes.
			/// </summary>
			/// <param name="rhs">const SList& from which to deep copy node data</param>
			/// <returns>A SList& reference to the self</returns>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			SList& operator=(const SList& rhs);

			/// ********************************************************************************
			/// <summary>
			/// Returns a templated data reference to the data stored in the frontmost node of
			///   the Singly Linked List. NOTE This will throw an exception if the SList is empty.
			/// </summary>
			/// <returns>A Templated data reference to the data in the first Node</returns>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			T& Front();
			const T& Front() const;

			/// ********************************************************************************
			/// <summary>
			/// Instantiates a new Node with the incoming templated data reference, then places
			///   that Node at the beginning of the Singly Linked List.
			/// </summary>
			/// <param name="toPush">Data to instantiate internal Node with</param>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			void PushFront(const T& toPush);

			/// ********************************************************************************
			/// <summary>
			/// Removes the frontmost Node from the linked list.
			///   NOTE: This invalidates the any references retrieved using Front() since the last
			///   list modification.
			///   NOTE: If the list is empty, this will throw an exception.
			/// </summary>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			void PopFront();

			/// ********************************************************************************
			/// <summary>
			/// Returns a templated data reference to the data stored in the backmost node of
			///   the Singly Linked List. NOTE This will throw an exception if the SList is empty.
			/// </summary>
			/// <returns>A Templated data reference to the data in the last Node</returns>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			T& Back();
			const T& Back() const;

			/// ********************************************************************************
			/// <summary>
			/// Instantiates a new Node with the incoming templated data reference, then places
			///   that Node at the end of the Singly Linked List.
			/// </summary>
			/// <param name="toPush">Data to instantiate internal Node with</param>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			Iterator PushBack(const T& toPush);

			/// ********************************************************************************
			/// <summary>
			/// Function to report if SList has any Nodes at call time
			/// </summary>
			/// <returns>bool, True if the SList is empty, False if it is currently storing data</returns>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			bool IsEmpty() const;

			/// ********************************************************************************
			/// <summary>
			/// Function to report the number of Nodes that are being stored in the SList
			/// </summary>
			/// <returns>An int representing the number of Nodes currently in the SList</returns>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			int Size() const;

			/// ********************************************************************************
			/// <summary>
			/// Removes all Nodes and Data that is being stored in the SList.
			///   NOTE: This will invalidate ALL references to data currently in the SList
			/// </summary>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			void Clear();

		private:
			/// <summary>Node* to the Frontmost node in the SList</summary>
			Node * mFront;
			/// <summary>Node* to the Backmost node in the SList</summary>
			Node * mBack;
			/// <summary>The number of Nodes currently owned by this SList</summary>
			std::uint32_t mSize;

			/// ********************************************************************************
			/// <summary>
			/// Helper function for deep copying data from Nodes in the rhs value into member Nodes
			/// </summary>
			/// <param name="rhs">const SList& from which to deep copy data</param>
			/// <created>Jacob Lites,1/17/2018</created>
			/// <changed>Jacob Lites,1/17/2018</changed>
			/// ********************************************************************************
			void DeepCopy(const SList& rhs);
	};
}

#include "SList.inl"