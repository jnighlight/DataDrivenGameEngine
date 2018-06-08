#include "pch.h"
#include "ActionParseHelper.h"
#include "DatumParseHelper.h"
#include "DatumSharedData.h"
#include "TableSharedData.h"
#include "Entity.h"
#include "Scope.h"
#include "Action.h"
#include "Factory.h"

namespace Library
{
	bool ActionParseHelper::StartParse(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingActions == false) {
			return false;
		} 
		if (value.isObject()) {
			if (valueKey.compare("value") == 0) {
				return true;
			}
			Signature curDepthSignature;
			if (value.isObject()) {
				assert(value.isMember("Type"));
				assert(value.isMember("Name"));
				Action* newAction = Factory<Action>::Create(value["Type"].asString());
				newAction->SetName(value["Name"].asString());
				tableSharedData->entity->Adopt(*newAction, "Actions");
				tableSharedData->entity = newAction;
				mSignatureStack.push(curDepthSignature);
			}
		}
		return true;
	}

	bool ActionParseHelper::ContinueParsing(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		UNREFERENCED_PARAMETER(value);
		UNREFERENCED_PARAMETER(valueKey);
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingActions == false) {
			return false;
		} 
		return true;
	}

	bool ActionParseHelper::EndParsing(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingActions == false) {
			return false;
		} 

		if (value.isObject()) {
			if (mSignatureStack.size() == 0) {
				tableSharedData->isParsingActions = false;
				tableSharedData->entity = tableSharedData->entity->GetParent();
				return true;
			}
			tableSharedData->entity = tableSharedData->entity->GetParent();
			mSignatureStack.pop();
		} else if (valueKey != "Type" && valueKey != "Name") {
			Datum& addedString = tableSharedData->entity->Append(valueKey);
			addedString.SetType(Datum::DatumType::E_STRING);
			if (addedString.IsExternal()) {
				addedString.Set(value.asString());
			} else {
				addedString.PushBack(value.asString());
			}
		}
		return true;
	}
}
