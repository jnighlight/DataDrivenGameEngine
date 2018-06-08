#pragma once
#include "json/json.h"
#include "Vector.h"
#include "RTTI.h"


namespace Library {
	class SharedData;
	class IJsonParseHelper;

	/// --------------------------------------------------------------------------------
	/// <summary>
	/// JSONParseMaster is a class in charge of parsing through JSON files and interpreting
	///  them into a game-engine consumable format
	/// </summary>
	/// --------------------------------------------------------------------------------
	class JsonParseMaster
	{
	public:
		/// --------------------------------------------------------------------------------
		/// <summary>
		/// Shared data is an RTTI class used to generically store data to populate from
		///  the incoming file. Should be inherited.
		/// </summary>
		/// --------------------------------------------------------------------------------
		class SharedData : public Library::RTTI {
		public:
			/// ********************************************************************************
			/// <summary>
			/// Default constructor and destructor
			/// </summary>
			/// <created>Jacob Lites,3/1/2018</created>
			/// <changed>Jacob Lites,3/1/2018</changed>
			/// ********************************************************************************
			SharedData() = default;
			virtual ~SharedData() = default;

			/// ********************************************************************************
			/// <summary>
			/// A polymorphic cloning function for maintaining subclasses without knowledge
			///  of their type
			/// </summary>
			/// <returns>Pointer to an instance reflecing the type of the clonee</returns>
			/// <created>Jacob Lites,3/1/2018</created>
			/// <changed>Jacob Lites,3/1/2018</changed>
			/// ********************************************************************************
			virtual SharedData* Clone();

			/// ********************************************************************************
			/// <summary>
			/// Polymorphic Reset function for leaving the parse master in a reusable state
			/// </summary>
			/// <created>Jacob Lites,3/1/2018</created>
			/// <changed>Jacob Lites,3/1/2018</changed>
			/// ********************************************************************************
			virtual void Reset() {};

			/// ********************************************************************************
			/// <summary>
			/// Setter and getter for the SharedData's owner
			/// </summary>
			/// <created>Jacob Lites,3/1/2018</created>
			/// <changed>Jacob Lites,3/1/2018</changed>
			/// ********************************************************************************
			inline void SetJsonParseMaster(JsonParseMaster* newOwner) { mOwner = newOwner; };
			inline JsonParseMaster* GetJsonParseMaster() { return mOwner; }

			/// ********************************************************************************
			/// <summary>
			/// Depth incrementors and decrementors for future recursion tracking
			/// </summary>
			/// <created>Jacob Lites,3/1/2018</created>
			/// <changed>Jacob Lites,3/1/2018</changed>
			/// ********************************************************************************
			inline void IncrementDepth() { ++mDepth; }
			inline void DecrementDepth();

			/// ********************************************************************************
			/// <summary>
			/// Getter for the current depth of the SharedData
			/// </summary>
			/// <returns>The current depth</returns>
			/// <created>Jacob Lites,3/1/2018</created>
			/// <changed>Jacob Lites,3/1/2018</changed>
			/// ********************************************************************************
			inline uint32_t Depth() const { return mDepth; };
		private:
			/// <summary>The owner of this shareddata</summary>
			JsonParseMaster* mOwner = nullptr;
			/// <summary>The current depth in the heirarchy this shared data is at</summary>
			uint32_t mDepth = 0;
			RTTI_DECLARATIONS(SharedData, Library::RTTI);
		};
	public:
		/// ********************************************************************************
		/// <summary>
		/// Default constructor
		/// </summary>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		JsonParseMaster() = default;
		/// ********************************************************************************
		/// <summary>
		/// Destructor that frees memory if this parse master is a clone
		/// </summary>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		~JsonParseMaster();

		/// ********************************************************************************
		/// <summary>
		/// Function for cloning parser with it's helper types already setup and cloned from
		///  a functional example parseMaster. Caller is responsible for pointer
		/// </summary>
		/// <returns>Cloned parsemaster for use</returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		JsonParseMaster* Clone();

		/// ********************************************************************************
		/// <summary>
		/// Adds a parsehelper to the parsemaster's helper list
		/// </summary>
		/// <param name="helperToAdd">The parsehelper to add to this masterparser</param>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		void AddHelper(IJsonParseHelper* helperToAdd);

		/// ********************************************************************************
		/// <summary>
		/// Removes the helper referenced from the parsemaster's helper list
		/// </summary>
		/// <param name="helperToRemove">The parse helper to remove</param>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		void RemoveHelper(IJsonParseHelper* helperToRemove);
		/// ********************************************************************************
		/// <summary>
		/// Parses a Json value depending on the helpers in the helper list
		/// </summary>
		/// <param name="valueToParse">The JSON::Value to parse with helpers</param>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		void Parse(const Json::Value& valueToParse);
		/// ********************************************************************************
		/// <summary>
		/// Grabs a string from the incoming file and parses it into a JSON object. Populates
		///  the internal sharedData
		/// </summary>
		/// <param name="filename">Name of file to parse</param>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		void ParseFromFile(const std::string& filename);
		/// ********************************************************************************
		/// <summary>
		/// Getter for filename we're reading from
		/// </summary>
		/// <returns>The name of the file we've most recently read from</returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		inline std::string GetFileName() const { return mFileName; }
		/// ********************************************************************************
		/// <summary>
		/// Retrieves pointer to the shared data in this parsemaster
		/// </summary>
		/// <returns>Pointer to owned shareddata</returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		inline SharedData* GetSharedData() const { return mSharedData; }
		/// ********************************************************************************
		/// <summary>
		/// Sets the internal sharedData. This MUST be called before attempting to parse a
		///  file
		/// </summary>
		/// <param name="incomingSharedData">The pointer to set as the shareddata to populate</param>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		inline void SetSharedData(SharedData* incomingSharedData) { mSharedData = incomingSharedData; }

		bool AttemptStartHandle(const std::string& key, const Json::Value& valueToHandle);
		bool AttemptEndHandle(const std::string& key, const Json::Value& valueToHandle);
		bool AttemptContinueHandle(const std::string & key, const Json::Value& valueToHandle);

	private:
		/// <summary>Recorder for name of the file we're trying to read</summary>
		std::string mFileName;
		/// <summary>Bool to track if we're an original or cloned paarsesmaster</summary>
		bool mCloned = false;
		/// <summary>Shareddata to populate from json data</summary>
		SharedData* mSharedData = nullptr;
		/// <summary>Vector of helper pointers. We only own this data if we're cloned</summary>
		Library::Vector<IJsonParseHelper*> mHelpers;
	};
}

