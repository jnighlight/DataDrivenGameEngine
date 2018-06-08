#pragma once
#include "Attributed.h"

namespace Library {

	class Datum;
	class Sector;
	class WorldState;

	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Attributed world that holds sectors
	/// </summary>
	/// --------------------------------------------------------------------------------
	class World final: public Attributed
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Constructor and destructor
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		World();
		~World();

		/// ********************************************************************************
		/// <summary>
		/// Copy and move functions
		/// </summary>
		/// <param name="rhs">rhs to copy or move</param>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		World(const World& rhs);
		World& operator=(const World& rhs);
		World(World&& rhs);
		World& operator=(World&& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Attributed parameters initialize and update function
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void InitializeScope();
		void UpdateExternalAttributes();

		/// ********************************************************************************
		/// <summary>
		/// Name accessor and setters
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		std::string GetName() { return mName; };
		void SetName(const std::string& name) { mName = name; };

		/// ********************************************************************************
		/// <summary>
		/// Returns datum holding all of the internal sectors
		/// </summary>
		/// <returns>Reference to sector datum</returns>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Datum& GetSectors() { return *mSectors; };
		/// ********************************************************************************
		/// <summary>
		/// Creates a sector and adds it to this world
		/// </summary>
		/// <param name="sectorName">The name of the sector to create</param>
		/// <returns></returns>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Sector& CreateSector(const std::string& sectorName);
		/// ********************************************************************************
		/// <summary>
		/// Updates all of the sectors inside this world
		/// </summary>
		/// <param name="worldState">State of the world</param>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void Update(WorldState& worldState);

		/// <summary>Label for the sectors in this scope</summary>
		static const std::string sSectorLabel;
	private:
		/// <summary>Name of the world</summary>
		std::string mName = "defaultWorldName";
		/// <summary>Pointers to the sectors in this world</summary>
		Datum* mSectors = nullptr;

		/// ********************************************************************************
		/// <summary>
		/// Helper functions for copy and move semantics
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void WorldCopySemantics();
		void WorldMoveSemantics();
	};
}