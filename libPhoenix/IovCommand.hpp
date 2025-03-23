/**
 * Project: Phoenix
 * Time-stamp: <2025-03-17 17:08:41 doug>
 * 
 * @file Iov.hpp
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

#ifndef _RIVEREXPLORER_PHOENIX_IOVCOMMAND_HPP_
#define _RIVEREXPLORER_PHOENIX_IOVCOMMAND_HPP_

#include <deque>
#include <cstdint>

#ifdef BUILDING_LIBPHOENIX
#include "Iov.hpp"
#else
#include <RiverExplorer/Phoenix/Iov.hpp>
#endif

namespace RiverExplorer::Phoenix
{
	/**
	 * @class IovCommand "Iov.hpp" <RiverExplorer/Phoenix/Iov.hpp>
	 * A set of Iov objects that represent a command.
	 *
	 * This is how a packet is represented inside the implementation.
	 */
	class IovCommand
		: public Iov
	{
	public:

		/**
		 * IovCommand - Default Constructor.
		 */
		IovCommand();

		/**
		 * IovCommand - Destructor
		 */
		~IovCommand();

		/**
		 * The command.
		 */
		CMD_e Command;

		/**
		 * The sequence number.
		 */
		SEQ_t Sequence;
	};
}
	
#endif // _RIVEREXPLORER_PHOENIX_IOVCOMMAND_HPP_
