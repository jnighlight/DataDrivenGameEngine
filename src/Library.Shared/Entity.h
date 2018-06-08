#pragma once
#include <string.h>
#include "Attributed.h"

namespace Library {
	class Sector;
	class WorldState;
	class Action;

	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Entity that holds a datum in the world
	/// </summary>
	/// --------------------------------------------------------------------------------
	class Entity : public Attributed
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Entity constructor and destructor
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Entity();
		virtual ~Entity();

		/// ********************************************************************************
		/// <summary>
		/// Copy and move semantics for entities
		/// </summary>
		/// <param name="rhs">Entity to copy or move</param>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Entity(const Entity& rhs);
		Entity& operator=(const Entity& rhs);
		Entity(Entity&& rhs);
		Entity& operator=(Entity&& rhs);

		/// ********************************************************************************
		/// <summary>
		/// Name accessor and setter
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void SetName(const std::string& name) { mName = name; };
		std::string GetName() const { return mName; };

		/// ********************************************************************************
		/// <summary>
		/// Sector accessor and setter
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		Sector* GetSector() const { return mCurrentSector; };
		void SetSector(Sector* sector);

		/// ********************************************************************************
		/// <summary>
		/// Update particular to this entity
		/// </summary>
		/// <param name="worldState">Current state of the world</param>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		virtual void Update(WorldState& worldState);

		/// ********************************************************************************
		/// <summary>
		/// Returns the Datum holding the actions for this entity
		/// </summary>
		/// <returns>The datum holding the actions for this entity</returns>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		inline virtual Datum* GetActions() { return mActions; };

		/// ********************************************************************************
		/// <summary>
		/// Creates an action of a specific type in this entity
		/// </summary>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		virtual Action& CreateAction(const std::string& actionType, const std::string& actionName);

		static const std::string sActionsLabel;
	private:
		/// <summary>Name of this entity</summary>
		std::string mName = "DefaultEntityName";
		/// <summary>The current secator this entity belongs to</summary>
		Sector* mCurrentSector = nullptr;

		/// <summary>Cached pointer to entity's actions</summary>
		Datum* mActions = nullptr;

		/// ********************************************************************************
		/// <summary>
		/// Attributed initialize and update functions
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void InitializeScope();
		void UpdateExternalAttributes();
		/// ********************************************************************************
		/// <summary>
		/// Move helper function for this entity
		/// </summary>
		/// <created>Jacob Lites,3/28/2018</created>
		/// <changed>Jacob Lites,3/28/2018</changed>
		/// ********************************************************************************
		void EntityMoveSemantics(Entity & rhs);
	};
}

