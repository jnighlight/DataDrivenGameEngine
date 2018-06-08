#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Action for creating other actions
	/// </summary>
	/// --------------------------------------------------------------------------------
	class ActionCreateAction : public Action
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Constructor and virtual destructor
		/// </summary>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		ActionCreateAction();
		virtual ~ActionCreateAction();

		/// ********************************************************************************
		/// <summary>
		/// Attributed functions for initializing and updating attributes
		/// </summary>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		void InitializeScope();
		void UpdateExternalAttributes();

		/// ********************************************************************************
		/// <summary>
		/// Polymorphic update function that creates a new action
		/// </summary>
		/// <param name="worldState">Worldstate for keeping tabs on current world processing location</param>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		virtual void Update(WorldState& worldState);
	private:
		/// <summary>Type of action to create</summary>
		std::string mPrototype;
		/// <summary>Name of created action</summary>
		std::string mPrototypeName;
	};
	CONCRETE_FACTORY(ActionCreateAction, Action);
}
