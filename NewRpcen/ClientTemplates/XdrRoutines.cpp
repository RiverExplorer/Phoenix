/**
 * Project: Phoenix
 * Time-stamp: <2025-03-14 11:29:32 doug>
 * 
 * @file GenerateStdString.cpp
 * @author Douglas Mark Royer
 * @date 08-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "rpcgen.hpp"

namespace RiverExplorer::rpcgen
{
	bool
	xdr_StdString(XDR * Xdrs, std::string * Str)
	{
		bool Results = false;
		
		if (Str != nullptr) {
			uint32_t	Length = Str->length();

			if (xdr_uint32_t(Xdrs, &Length)) {

				if (Xdrs->x_op == XDR_ENCODE) {
					const char * CStr = Str->c_str();
				
					if (xdr_string(Xdrs, (char**)&CStr, Length)) {
						Results = true;
					}
				} else if (Xdrs->x_op == XDR_DECODE) {
					char * CStr = nullptr;

					if (xdr_string(Xdrs, &CStr, Length)) {
						*Str = CStr;
						Results = true;
					}
				} else if (Xdrs->x_op == XDR_FREE) {
					/*EMPTY*/
				}
			}
		}

		return(Results);
	}
		
}
