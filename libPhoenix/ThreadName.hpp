/**
 * Project: Phoenix
 * Time-stamp: <2025-03-04 12:58:11 doug>
 * 
 * @file ThreadName.hpp
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#ifndef _RIVEREXPLORER_PHOENIX_THREAD_HPP_
#define _RIVEREXPLORER_PHOENIX_THREAD_HPP_

#include <thread>
#include <map>
#include <string>

namespace RiverExplorer::Phoenix
{
	/**
	 * @class ThreadName Thread.hpp <RiverExplorer/Phoenix/ThreadName.hpp>
	 * Because std::thread has no standard way to log the thread
	 * ID or name, this object exists.
	 */
	class ThreadName
	{
	public:

		/**
		 * Name our thread this is called from.
		 * It will be tied to std::this_thread::get_id().
		 *
		 * @param ThreadName What you want to call it.
		 */
		static void NameOurThread(std::string ThreadName);

		/**
		 * Name the thread this is called from.
		 * It will be tied to std::this_thread::get_id().
		 *
		 * @param ID  The ID of the thread to name.
		 *
		 * @param ThreadName What you want to call it.
		 */
		static void NameTheThread(std::thread::id ID, std::string ThreadName);

		/**
		 * Get the name of this thread.
		 *
		 * @return The user assigned name of the thread.
		 * Or "UnNamedThread" when it was not assigned.
		 */
		static const std::string ToString();
		
		/**
		 * Get the name of the thread.
		 *
		 * @return The user assigned name of the thread.
		 * Or "UnNamedThread" when it was not assigned.
		 */
		static const std::string ToString(const std::thread::id ID);
		
	private:
		/**
		 * Not constructable.
		 */
		ThreadName();

		static std::map<std::thread::id,std::string> _ThreadMap;
	};
}

#endif // _RIVEREXPLORER_PHOENIX_THREAD_HPP_
