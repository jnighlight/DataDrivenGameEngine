#include "pch.h"
#include "TableSharedData.h"
#include "Scope.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"

namespace Library {

	JsonParseMaster::SharedData* TableSharedData::Clone()
	{
		return new TableSharedData;
	}

	void TableSharedData::Reset()
	{
		sector = nullptr;
		action = nullptr;
		isParsingEntity = false;
		isParsingActions = false;
		entityType = "";
		entityName = "";
	}

	RTTI_DEFINITIONS(TableSharedData);
}
