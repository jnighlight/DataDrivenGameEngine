#include "pch.h"
#include "Incrementor.h"

namespace Library
{
	void Incrementor::Increment()
	{
		mIncrementCount++;
	}

	Incrementor::Incrementor()
		:mIncrementCount(0)
	{
	}


	Incrementor::~Incrementor()
	{
	}
}
