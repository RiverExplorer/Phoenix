
#include <Commands.hpp>

namespace RiverExplorer::Phoenix
{
	int
	xdr_CommandID(XDR * xdrs, CommandID * ID)
	{
		return(xdr_uint64_t(xdrs, ID));
	}

	bool
	xdr_bool_t(XDR * xdrs, bool_t * Val)
	{
		return(xdr_int(xdrs, Val));
	}
}
