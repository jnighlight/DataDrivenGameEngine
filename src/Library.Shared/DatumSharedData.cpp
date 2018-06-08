#include "pch.h"
#include "DatumSharedData.h"
#include "Scope.h"

namespace Library {

	JsonParseMaster::SharedData* DatumSharedData::Clone()
	{
		return new DatumSharedData;
	}

	void DatumSharedData::Reset()
	{
		if (mScope != nullptr) {
			mScope->Clear();
		}
	}
	RTTI_DEFINITIONS(DatumSharedData);
}
