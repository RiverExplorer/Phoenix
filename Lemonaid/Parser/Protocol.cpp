/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "Protocol.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	Control::Control()
	{
		GlobalScope = new Scope();

		return;
	}

	Control::~Control()
	{
		if (GlobalScope != nullptr) {
			delete GlobalScope;
			GlobalScope = nullptr;
		}

		return;
	}
}
