#include "pch.h"
#include "DatumParseHelper.h"
#include "DatumSharedData.h"
#include "TableSharedData.h"
#include "Entity.h"
#include "Scope.h"

namespace Library {

	bool DatumParseHelper::StartParse(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingEntity == false) {
			return false;
		} 
		if (value.isObject()) {
			if (valueKey.compare("value") == 0) {
				return true;
			}
			Signature curDepthSignature;
			Scope* scope = tableSharedData->entity;
			if (value["value"].isObject()) {
				tableSharedData->entity = &scope->AppendScope(valueKey);
				curDepthSignature.mIsScope = true;
				mSignatureStack.push(curDepthSignature);
			} else {
				curDepthSignature.mDatum = &(scope->Append(valueKey));
				mSignatureStack.push(curDepthSignature);
			}
		}
		return true;
	}

	bool DatumParseHelper::ContinueParsing(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingEntity == false) {
			return false;
		} 

		Signature& curDepthSignature = mSignatureStack.top();
		if (valueKey.compare("value") == 0) { //Returns 0 if they ARE the same
			if (value.isObject()) {
				throw std::exception("Object type in continue parsing JSON. Wrong place.");
			}
			if (value.isArray()) {
				for (auto valueInArray : value) {
					curDepthSignature.mDatumValue.PushBack(valueInArray);
				}
			} else {
				curDepthSignature.mDatumValue.PushBack(value);
			}
		} else if (valueKey.compare("type") == 0) { //Returns 0 if they ARE the same
			if (!value.isString()) {
				throw std::exception("Non string type in JSON!");
			}
			curDepthSignature.mDatumType = Datum::GetDatumTypeByString(value.asString());
		} else {
			return false;
		}
		return true;
	}

	bool DatumParseHelper::EndParsing(JsonParseMaster::SharedData * sharedData, const std::string & valueKey, const Json::Value & value)
	{
		TableSharedData* tableSharedData = sharedData->As<TableSharedData>();
		if (tableSharedData == nullptr || tableSharedData->isParsingEntity == false) {
			return false;
		} 

		if (value.isObject()) {
			if (valueKey.compare("value") == 0) {
				return true;
			}
			if (mSignatureStack.size() == 0) {
				tableSharedData->isParsingEntity = false;
				return true;
			}
			Signature& curDepthSignature = mSignatureStack.top();
			if (curDepthSignature.mIsScope) {
				tableSharedData->entity = tableSharedData->entity->GetParent();
			} else if (curDepthSignature.mDatumType != Datum::DatumType::E_TABLE) {
				assert(curDepthSignature.mDatum != nullptr);
				curDepthSignature.mDatum->SetType(curDepthSignature.mDatumType);
				curDepthSignature.mDatum->Resize(curDepthSignature.mDatumValue.Size());
				for (uint32_t i = 0; i < curDepthSignature.mDatumValue.Size(); ++i) {
					curDepthSignature.mDatum->SetFromString(curDepthSignature.mDatumValue[i].asString(), i);
				}
			}
			mSignatureStack.pop();
			if (mSignatureStack.size() == 0) {
				tableSharedData->isParsingEntity = true;
			}
		}
		return true;
	}
}
