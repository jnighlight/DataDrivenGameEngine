#pragma once
#include "ActionList.h"

namespace Library
{
	/// --------------------------------------------------------------------------------
	/// <summary>
	/// Action list for choosing based on a conditional
	/// </summary>
	/// --------------------------------------------------------------------------------
	class ActionListIf : public ActionList
	{
	public:
		/// ********************************************************************************
		/// <summary>
		/// Constructor and virtual destructor
		/// </summary>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		ActionListIf();
		~ActionListIf();

		/// ********************************************************************************
		/// <summary>
		/// Polymorphic update function that deletes a target action if possible
		/// </summary>
		/// <param name="worldState">Worldstate for keeping tabs on current world processing location</param>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		virtual void Update(WorldState& worldState) override;
	private:

		/// ********************************************************************************
		/// <summary>
		/// Attributed functions for initializing and updating attributes
		/// </summary>
		/// <created>Jacob Lites,4/3/2018</created>
		/// <changed>Jacob Lites,4/3/2018</changed>
		/// ********************************************************************************
		void InitializeScope();
		void UpdateExternalAttributes();

		/// <summary>Name of condition datum to read from for if statement</summary>
		std::string mCondition = "";
	};
	CONCRETE_FACTORY(ActionListIf, Action);
}

