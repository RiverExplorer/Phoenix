/**                                                                             
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed un\
der CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC                      
 */


#include <Commands.hpp>

namespace RiverExplorer::Phoenix
{
#ifdef WHAT	
	int
	xdr_CommandID(XDR * xdrs, CommandSequence * ID)
	{
		return(xdr_uint64_t(xdrs, ID));
	}
#endif
	
	bool
	xdr_bool_t(XDR * xdrs, bool_t * Val)
	{
		return(xdr_int(xdrs, Val));
	}

	uint64_t htonll(uint64_t val)
	{
    if (__BYTE_ORDER == __BIG_ENDIAN) {
			return (val);
		} else {
			return __bswap_64(val);
		}
	}

	uint64_t ntohll(uint64_t val)
	{
    if (__BYTE_ORDER == __BIG_ENDIAN) {
			return (val);
		}  else {
			return __bswap_64(val);
		}
	}
}
