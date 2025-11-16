/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#include <iostream>
#include <filesystem>

namespace RiverExplorer
{
	/**
	 * Remove the named directory, all of its contents
	 * and the directory itself.
	 *
	 * @return True if the directory existed and was removed.
	 * Returns true if it did not exist.
	 * Returns false if it exists, and can not be removed.
	 */
	bool
	rmdirr(std::string DirToRemove)
	{
		bool Results = false;
		
		std::filesystem::path Dir = DirToRemove;
		
		if (std::filesystem::exists(Dir)) {
			Results = true;
			try {
				std::filesystem::remove_all(Dir);

			} catch (const std::filesystem::filesystem_error & Ex) {
				Results = false;
				std::cerr << "Error: Unable to remove directory: "
									<< DirToRemove
									<< std::endl
									<< Ex.what()
									<< std::endl;
			}
		}

		return(Results);
	}
}

