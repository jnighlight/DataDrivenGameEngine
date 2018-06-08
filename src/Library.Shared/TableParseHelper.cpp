#include "pch.h"
#include "TableParseHelper.h"
#include "TableSharedData.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Scope.h"
#include "Factory.h"
#include "Action.h"

namespace Library {

	bool TableParseHelper::StartParse(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingEntity == true || tableSharedData->isParsingActions == true) {
			return false;
		} 
		if (valueKey == "WorldName") {
			tableSharedData->world->SetName(value.asString());
		}
		if (value.isObject()) {
			if (tableSharedData->sector == nullptr) {
				tableSharedData->sector = &tableSharedData->world->CreateSector(valueKey);
			} else if (tableSharedData->entity == nullptr) {
				assert(value.isMember("Type"));
				tableSharedData->entity = &(tableSharedData->sector->CreateEntity(value["Type"].asString(), valueKey));
			} else if (valueKey == "Data") {
				tableSharedData->isParsingEntity = true;
			} else if (valueKey == "Actions") {
				assert(value.isMember("Type"));
				assert(value.isMember("Name"));
				Action* newAction = Factory<Action>::Create(value["Type"].asString());
				newAction->SetName(value["Name"].asString());
				tableSharedData->entity->Adopt(*newAction, "Actions");
				tableSharedData->entity = newAction;
				//mSignatureStack.push(curDepthSignature);
				tableSharedData->isParsingActions = true;
			}
		}
		return true;
	}

	bool TableParseHelper::ContinueParsing(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		UNREFERENCED_PARAMETER(value);
		UNREFERENCED_PARAMETER(valueKey);
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingEntity == true || tableSharedData->isParsingActions == true) {
			return false;
		}
		return true;
	}

	bool TableParseHelper::EndParsing(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		UNREFERENCED_PARAMETER(valueKey);
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingEntity == true || tableSharedData->isParsingActions == true) {
			return false;
		} 

		if (value.isObject()) {
			if (tableSharedData->sector != nullptr) {
				if (tableSharedData->entity != nullptr) {
					tableSharedData->entity = nullptr;
				} else {
					tableSharedData->sector = nullptr;
				}
			}
		}
		return true;
	}
}
