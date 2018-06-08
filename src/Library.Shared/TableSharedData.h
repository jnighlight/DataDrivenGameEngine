#pragma once
#include "JsonParseMaster.h"
#include "Scope.h"

namespace Library {
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Datum Shared Data for use with JSon Parse Master
	/// </summary>
	/// --------------------------------------------------------------------------------
	class World;
	class Sector;
	class Entity;
	class Action;
	class TableSharedData: public JsonParseMaster::SharedData
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Defaulted constructor and destructor
		/// </summary>
		/// <created>Jacob Lites,3/9/2018</created>
		/// <changed>Jacob Lites,3/9/2018</changed>
		/// ********************************************************************************
		TableSharedData() = default;
		~TableSharedData() = default;

		/// ********************************************************************************
		/// <summary>
		/// Clones a fresh TableSharedData from this sharedDatum
		/// </summary>
		/// <returns>A newly instantieated TableSharedData. Only has a pointer if this one does</returns>
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

		World * world = nullptr;
		Sector * sector = nullptr;
		Scope * entity = nullptr;
		std::string entityType = "";
		std::string entityName = "";
		bool isParsingEntity = false;
		bool isParsingActions = false;
		Action* action = nullptr;

		RTTI_DECLARATIONS(TableSharedData, SharedData);
	};
}
