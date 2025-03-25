/**
 * Project: Phoenix
 * Time-stamp: <2025-03-24 22:20:06 doug>
 * 
 * @file XdrGenShared.hpp
 * @author Douglas Mark Royer
 * @date 24-FEB-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

#ifndef _RIVEREXPLORER_XDRGEN_SHARED_HPP_
#define _RIVEREXPLORER_XDRGEN_SHARED_HPP_

#include <string>
#include <vector>
#include <map>
#include <rpc/rpc.h>
#ifndef W64
#include <unistd.h>
#endif // W64
#include <memory.h>

namespace RiverExplorer::xdrgen
{
	/**
	 * An implementation of an XDR routine
	 * for std::vector<uint8_t>.
	 *
	 * @param Xdr The initalized XDR object.
	 *
	 * @param Str The address of the std::string.
	 *
	 * @param[optional] MaxLength The maximum size of the string.
	 * @return true on no errors.
	 */
	bool xdr_StdString(XDR * Xdr,
		std::string * Str,
		uint32_t MaxLength = ~0);
	
	/**
	 * An implementation of an XDR routine
	 * for std::vector<uint8_t>.
	 *
	 * @param Xdr The initalized XDR object.
	 *
	 * @param Obj A pointer to the object.
	 *
	 * @param Proc the xdrproc_t routine that can
	 * handle type 'T'.
	 *
	 * @param[optional] MaxLength The maximum size of the string.
	 * @return true on no errors.
	 */
	template <class T>
	bool xdr_VectorOf(XDR * Xdr,
		std::vector<T> * Obj,
		xdrproc_t Proc,
		uint32_t MaxLength = ~0)
	{
		bool Results = false;
	
		if (Obj != nullptr && Proc != nullptr) {
				uint32_t Size = Obj->size();
	
			if (xdr_uint32_t(Xdr, &Size)) {
	
				typename std::vector<T>::iterator it;
				T Item;
				uint32_t Offset = 0;
				for (it = Obj->begin(); it != Obj->end(); it++) {
					Item = *it;
					Results = (*Proc)(Xdr, &Item);
					if (!Results) {
						break;
					}
					if (++Offset > MaxLength) {
						break;
					}
				}
			}
		}
	
		return(Results);
	}
} // End namespace RiverExplorer:xdrgen:test

#endif // _RIVEREXPLORER_XDRGEN_SHARED_HPP_
