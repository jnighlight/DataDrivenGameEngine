#pragma once
#include "Attributed.h"
namespace Library
{
	class WorldState;
	class Action : public Attributed
	{
	public:
		Action();
		virtual ~Action();

		std::string GetName() { return mName; };
		inline void SetName(const std::string& name) { mName = name; };

		inline void MarkForDelete() { mMarkedForDelete = true; };

		virtual void Update(WorldState& worldState) = 0;
	protected:
		std::string mName = "defaultActionName";
		bool mMarkedForDelete = false;
	};
}
