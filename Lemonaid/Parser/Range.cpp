/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "Range.hpp"

namespace RiverExplorer::Phoenix::Protocol
{

	std::ostream & operator<<(std::ostream & Out,
													 const Range & R)
	{
		R.Min.PrintMin(Out);
		R.Min.PrintMax(Out);

		return(Out);
	}

}
