#pragma once

#include "json/json.h"
#include "IJsonParseHelper.h"
#include "Datum.h"

namespace Library
{
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Class for helping parse Actions in JSONParseMaster
	/// </summary>
	/// --------------------------------------------------------------------------------
	class ActionParseHelper: public IJsonParseHelper
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Defaulted constructor and destructor
		/// </summary>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		ActionParseHelper::ActionParseHelper() = default;
		ActionParseHelper::~ActionParseHelper() = default;

		/// ********************************************************************************
		/// <summary>
		/// Override for starting the parse of the Action group
		/// </summary>
		/// <param name="sharedData">The shared data from the parsemaster</param>
		/// <param name="valueKey">The key that the json value is keyed to</param>
		/// <param name="value">the Json value</param>
		/// <returns>True if successfully started parse</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		bool ActionParseHelper::StartParse(JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) override;
		/// ********************************************************************************
		/// <summary>
		/// Override for continuing the parse of the Action
		/// </summary>
		/// <param name="sharedData">The shared data from the parsemaster</param>
		/// <param name="valueKey">The key that the json value is keyed to</param>
		/// <param name="value">the Json value</param>
		/// <returns>True if successfully started parse</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		bool ActionParseHelper::ContinueParsing(JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) override;
		/// ********************************************************************************
		/// <summary>
		/// Override for ending the parse of the Action
		/// </summary>
		/// <param name="sharedData">The shared data from the parsemaster</param>
		/// <param name="valueKey">The key that the json value is keyed to</param>
		/// <param name="value">the Json value</param>
		/// <returns>True if successfully started parse</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		bool ActionParseHelper::EndParsing(JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) override;
		/// ********************************************************************************
		/// <summary>
		/// Returns a copy of this parsehelper that own's all of it's data
		/// </summary>
		/// <returns>A freshly initialized clone of this helper</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		inline IJsonParseHelper* ActionParseHelper::Clone() { return new ActionParseHelper; };

	private:
		/// --------------------------------------------------------------------------------
		/// <summary>
		/// Helper object for traversing the incoming action
		/// </summary>
		/// --------------------------------------------------------------------------------
		struct Signature {
			/// ********************************************************************************
			/// <summary>
			/// Defaulted constructor
			/// </summary>
			/// <created>Jacob Lites,3/9/2018</created>
			/// <changed>Jacob Lites,3/9/2018</changed>
			/// ********************************************************************************
			Signature::Signature() = default;
		};

		/// <summary>Stack of signature objects for recursive action progression</summary>
		std::stack<Signature> mSignatureStack;
	};
}

