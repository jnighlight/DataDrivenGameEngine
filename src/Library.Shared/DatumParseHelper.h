#pragma once

#include "json/json.h"
#include "IJsonParseHelper.h"
#include "Datum.h"

namespace Library {
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Class for helping parse Datums in JSONParseMaster
	/// </summary>
	/// --------------------------------------------------------------------------------
	class DatumParseHelper: public IJsonParseHelper
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Defaulted constructor and destructor
		/// </summary>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		DatumParseHelper::DatumParseHelper() = default;
		DatumParseHelper::~DatumParseHelper() = default;

		/// ********************************************************************************
		/// <summary>
		/// Override for starting the parse of the Datum
		/// </summary>
		/// <param name="sharedData">The shared data from the parsemaster</param>
		/// <param name="valueKey">The key that the json value is keyed to</param>
		/// <param name="value">the Json value</param>
		/// <returns>True if successfully started parse</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		bool DatumParseHelper::StartParse(JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) override;
		/// ********************************************************************************
		/// <summary>
		/// Override for continuing the parse of the Datum
		/// </summary>
		/// <param name="sharedData">The shared data from the parsemaster</param>
		/// <param name="valueKey">The key that the json value is keyed to</param>
		/// <param name="value">the Json value</param>
		/// <returns>True if successfully started parse</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		bool DatumParseHelper::ContinueParsing(JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) override;
		/// ********************************************************************************
		/// <summary>
		/// Override for ending the parse of the Datum
		/// </summary>
		/// <param name="sharedData">The shared data from the parsemaster</param>
		/// <param name="valueKey">The key that the json value is keyed to</param>
		/// <param name="value">the Json value</param>
		/// <returns>True if successfully started parse</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		bool DatumParseHelper::EndParsing(JsonParseMaster::SharedData* sharedData, const std::string& valueKey, const Json::Value& value) override;
		/// ********************************************************************************
		/// <summary>
		/// Returns a copy of this parsehelper that own's all of it's data
		/// </summary>
		/// <returns>A freshly initialized clone of this helper</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		inline IJsonParseHelper* DatumParseHelper::Clone() { return new DatumParseHelper; };

	private:
		/// --------------------------------------------------------------------------------
		/// <summary>
		/// Helper object for traversing the incoming datum
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

			/// <summary>The datum type of the currently reading datum</summary>
			Datum::DatumType mDatumType = Datum::DatumType::E_UNKNOWN;
			/// <summary>Value of the current reading datum</summary>
			Vector<Json::Value> mDatumValue;
			/// <summary>Pointer to a generated datum from this object</summary>
			Datum* mDatum = nullptr;
			/// <summary>Bool for letting us know if we're closing out of a scope object</summary>
			bool mIsScope = false;
		};

		/// <summary>Stack of signature objects for recursive datum progression</summary>
		std::stack<Signature> mSignatureStack;
	};
}
