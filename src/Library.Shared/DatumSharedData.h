#pragma once
#include "JsonParseMaster.h"
#include "Scope.h"

namespace Library {
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Datum Shared Data for use with JSon Parse Master
	/// </summary>
	/// --------------------------------------------------------------------------------
	class DatumSharedData: public JsonParseMaster::SharedData
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Defaulted constructor and destructor
		/// </summary>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		DatumSharedData() = default;
		~DatumSharedData() = default;

		/// ********************************************************************************
		/// <summary>
		/// Clones a fresh DatumSharedData from this sharedDatum
		/// </summary>
		/// <returns>A newly instantieated DatumSharedData. Only has a pointer if this one does</returns>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		JsonParseMaster::SharedData* Clone();
		/// ********************************************************************************
		/// <summary>
		/// If the internal scope pointer is not null, clears it. Otherwise, noop
		/// </summary>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		void Reset();

		/// ********************************************************************************
		/// <summary>
		/// Setters and getters for internal scope object
		/// </summary>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		inline void DatumSharedData::SetScope(Scope* scope) { mScope = scope; }
		inline Scope* DatumSharedData::GetScope() { return mScope; }

		/// <summary>Scope to populate. Notice that this shared data doesn't own it's scope unless it is a clone</summary>
		Scope* mScope = nullptr;
		RTTI_DECLARATIONS(DatumSharedData, SharedData);
	};
}
