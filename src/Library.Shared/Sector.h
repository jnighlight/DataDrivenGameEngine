#pragma once
#include <string>
#include "Attributed.h"

namespace Library {
	class Datum;
	class Entity;
	class World;
	class WorldState;

	/// --------------------------------------------------------------------------------
	/// <summary>
	/// A sector that belongs in world to denote a chunk of a world
	/// </summary>
	/// --------------------------------------------------------------------------------
	class Sector final : public Attributed
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Constructor and destructors
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Sector();
		~Sector();

		/// ********************************************************************************
		/// <summary>
		/// Copy and move semantics
		/// </summary>
		/// <param name="rhs">object we copy/move</param>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Sector(const Sector& rhs);
		Sector& operator=(const Sector& rhs);
		Sector(Sector&& rhs);
		Sector& operator=(Sector&& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Attributed functionsf or updating and intializing scope
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void InitializeScope();
		void UpdateExternalAttributes();

		/// ********************************************************************************
		/// <summary>
		/// Name accessor and setter
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		std::string GetName() { return mName; };
		void SetName(const std::string& name) { mName = name; };

		/// ********************************************************************************
		/// <summary>
		/// Current world accessor and setter
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		World* GetWorld() { return mCurrentWorld; };
		void SetWorld(World* world);

		/// ********************************************************************************
		/// <summary>
		/// Returns the datum containing the current entities
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Datum& GetEntities() { return *mEntities; };
		/// ********************************************************************************
		/// <summary>
		/// Creates an entity in this sector
		/// </summary>
		/// <param name="entityType">The type of entity to create</param>
		/// <param name="entityName">The name of the entity created</param>
		/// <returns>Reference to the created entity</returns>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Entity& CreateEntity(const std::string& entityType, const std::string& entityName);
		/// ********************************************************************************
		/// <summary>
		/// Update all of the entities in this sector
		/// </summary>
		/// <param name="worldState">The current world state</param>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void Update(WorldState& worldState);

		/// <summary>String for label that the entities enter</summary>
		static const std::string sEntityLabel;
	private:
		/// ********************************************************************************
		/// <summary>
		/// Move and copy functionality
		/// </summary>
		/// <param name="rhs">rhs to move</param>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void SectorMoveSemantics(Sector& rhs);
		void SectorCopySemantics();

		/// <summary>Sector's name</summary>
		std::string mName = "defaultSectorName";
		/// <summary>The world this sector belongs in</summary>
		World* mCurrentWorld = nullptr;
		/// <summary>Datum holding entities</summary>
		Datum* mEntities = nullptr;
	};
}
