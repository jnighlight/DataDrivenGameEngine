#pragma once
#include "GameTime.h"

namespace Library {
	class WorldState
	{
	public:
		WorldState();
		~WorldState();
		void SetGameTime(const GameTime& gameTime) { mGameTime = gameTime; };
		GameTime GetGameTime() { return mGameTime; };

		class World* mWorld = nullptr;
		class Sector* mSector = nullptr;
		class Entity* mEntity = nullptr;
		class Action* mAction = nullptr;
	private:
		GameTime mGameTime;
	};
}

