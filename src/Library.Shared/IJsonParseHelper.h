#pragma once
#include "JsonParseMaster.h"
#include "json/json.h"

namespace Library {
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Superclass for parsehelpers for customizable data parsing
	/// </summary>
	/// --------------------------------------------------------------------------------
	class IJsonParseHelper
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Constructor and destructor for setting up and tearing down the helpers
		/// </summary>
		/// <returns></returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		IJsonParseHelper();
		virtual ~IJsonParseHelper();

		/// ********************************************************************************
		/// <summary>
		/// Initialize function for preparing helper for data
		/// </summary>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		virtual void Initialize();
		/// ********************************************************************************
		/// <summary>
		/// Start parse checks incoming data to make sure that it is good to parses
		/// </summary>
		/// <param name="sharedData">Shared data we would be storing data into</param>
		/// <param name="valueKey">The key for the value that is being checked</param>
		/// <returns>True if this helper can parse that data</returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		virtual bool StartParse(Library::JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) = 0;

		/// ********************************************************************************
		/// <summary>
		/// Function for continuing to grab information from the incoming data to populate the sharedData.
		/// </summary>
		/// <param name="sharedData">Shared data we're going to populate</param>
		/// <param name="valueKey">The key for the value we're currently parsing</param>
		/// <param name="value">The Json value with the data correlating to the incoming key</param>
		/// <returns>True if we are succeeding in continual parsing</returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		virtual bool ContinueParsing(Library::JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) = 0;
		/// ********************************************************************************
		/// <summary>
		/// Function for finishing the parse of the object
		/// </summary>
		/// <param name="sharedData">SharedDdata to finish populating</param>
		/// <returns>True if we correctly finished populating the data</returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		virtual bool EndParsing(Library::JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) = 0;
		/// ********************************************************************************
		/// <summary>
		/// Polymorphic clone that returns a JsonParseHelper of the current type
		/// </summary>
		/// <returns>Copy of this ParseHelper</returns>
		/// <created>Jacob Lites,3/1/2018</created>
		/// <changed>Jacob Lites,3/1/2018</changed>
		/// ********************************************************************************
		virtual IJsonParseHelper* Clone() = 0;
	};
}
