/**
 * Project: Phoenix
 * Time-stamp: <2025-03-04 12:59:28 doug>
 * 
 * @file ThreadName.cpp
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

#include "ThreadName.hpp"

namespace RiverExplorer::Phoenix
{
	std::map<std::thread::id,std::string> ThreadName::_ThreadMap;

	void
	ThreadName::NameOurThread(std::string ThreadName)
	{
		_ThreadMap[std::this_thread::get_id()] = ThreadName;

		return;
	}

	void
	ThreadName::NameTheThread(std::thread::id ID, std::string ThreadName)
	{
		_ThreadMap[ID] = ThreadName;

		return;
	}
	
	const std::string
	ThreadName::ToString()
	{
		return(ThreadName::ToString(std::this_thread::get_id()));
	}
	
	const std::string
	ThreadName::ToString(const std::thread::id ID)
	{
		std::string Results;
		
		std::map<std::thread::id,std::string>::const_iterator It;

		It = _ThreadMap.find(ID);
		if (It != _ThreadMap.cend()) {
			Results = It->second;
		} else {
			Results = "ThreadNotNamed";
		}

		return(Results);
	}
}
