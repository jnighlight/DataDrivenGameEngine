#include "pch.h"
#include <fstream>
#include <cassert>
#include "IJsonParseHelper.h"

namespace Library {
	JsonParseMaster::~JsonParseMaster()
	{
		if (mCloned) {
			for (auto helper : mHelpers) {
				delete helper;
			}
			if (mSharedData != nullptr) {
				delete mSharedData;
			}
		}
	}

	JsonParseMaster* JsonParseMaster::Clone()
	{
		JsonParseMaster* cloneMaster = new JsonParseMaster;
		cloneMaster->mHelpers.Reserve(mHelpers.Size());
		for (auto helper : mHelpers) {
			cloneMaster->AddHelper(helper->Clone());
		}
		if (mSharedData != nullptr) {
			cloneMaster->mSharedData = mSharedData->Clone();
		}
		cloneMaster->mCloned = true;
		return cloneMaster;
	}

	void JsonParseMaster::AddHelper(IJsonParseHelper* helperToAdd)
	{
		mHelpers.PushBack(helperToAdd);
	}

	void JsonParseMaster::RemoveHelper(IJsonParseHelper* helperToAdd)
	{
		for (uint32_t i = 0; i < mHelpers.Size(); ++i) {
			if (helperToAdd == mHelpers.At(i)) {
				mHelpers.Remove(i);
				break;
			}
		}
	}

	void JsonParseMaster::Parse(const Json::Value& valueToParse)
	{
		if (mSharedData == nullptr) {
			throw std::exception("Must supply a shared data object to populate before parsing data");
		}
		std::vector<std::string> members = valueToParse.getMemberNames();
		for (auto key : members) {
			if (AttemptStartHandle(key, valueToParse[key])) {
				if (valueToParse[key].isObject()) {
					mSharedData->IncrementDepth();
					Parse(valueToParse[key]);
					mSharedData->DecrementDepth();
				} else {
					AttemptContinueHandle(key, valueToParse[key]);
				}
				AttemptEndHandle(key, valueToParse[key]);
			}
		}
	}

	bool JsonParseMaster::AttemptStartHandle(const std::string & key, const Json::Value& valueToHandle)
	{
		for (auto helper : mHelpers) {
			if (helper->StartParse(mSharedData, key, valueToHandle)) {
				return true;
			}
		}
		return false;
	}

	bool JsonParseMaster::AttemptContinueHandle(const std::string & key, const Json::Value& valueToHandle)
	{
		for (auto helper : mHelpers) {
			if (helper->ContinueParsing(mSharedData, key, valueToHandle)) {
				return true;
			}
		}
		return false;
	}

	bool JsonParseMaster::AttemptEndHandle(const std::string & key, const Json::Value & valueToHandle)
	{
		for (auto helper : mHelpers) {
			if (helper->EndParsing(mSharedData, key, valueToHandle)) {
				return true;
			}
		}
		return false;
	}


	void JsonParseMaster::ParseFromFile(const std::string & filename)
	{
		mFileName = filename;

		Json::Value root;
		std::ifstream stream(filename);
		stream >> root;
		mSharedData->Reset();
		Parse(root);
	}

	JsonParseMaster::SharedData* JsonParseMaster::SharedData::Clone()
	{
		SharedData* data = new SharedData;
		return data;
	}

	void JsonParseMaster::SharedData::DecrementDepth()
	{
		assert(mDepth > 0);
		--mDepth;
	}

	RTTI_DEFINITIONS(JsonParseMaster::SharedData);
}
