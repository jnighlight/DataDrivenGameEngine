#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Action used for destroying other actions
	/// </summary>
	/// --------------------------------------------------------------------------------
	class ActionDestroyAction : public Action
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Constructor and virtual destructor
		/// </summary>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		ActionDestroyAction();
		virtual ~ActionDestroyAction();

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
		/// Polymorphic update function that deletes a target action if possible
		/// </summary>
		/// <param name="worldState">Worldstate for keeping tabs on current world processing location</param>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		void Update(WorldState & worldState);
	private:
		/// <summary>Name of the action to destroy</summary>
		std::string mActionName = "DefaultActionToDestroy";
	};
	CONCRETE_FACTORY(ActionDestroyAction, Action);
}
