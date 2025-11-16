/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "Globals.hpp"
#include "Version.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	std::map<std::string,Version*> Version::_Versions;

	Version::Version(std::string OurID, Scope * ParentScope)
	{
		// We defer checking our ID for uniqueness
		// within the protocol, until the first content
		// is added. Primarily because a constructor
		// can not return a failed status without throwing
		// an exception.
		//
		ID = OurID;
		_UniquenessChecked = false;
		
		_ParentScope = ParentScope;

		return;
	}

	Version::~Version()
	{
		_ParentScope = nullptr;
		ID = "";
		
		return;
	}

	bool
	Version::Add(Content & NewContent)
	{
		bool Results = false;

		bool GotError = false;
		
		if (!_UniquenessChecked) {
			if (!_Versions.contains(ID)) {
				_Versions.insert(std::make_pair(ID, this));
				_UniquenessChecked = true;

			} else {
				Global::Log << "ERROR: Version " << ID
										<< " already exists as a version in this protocol."
										<< std::endl;
				GotError = true;
			}
		}

		// Check if Content->ID is unique within our scope.
		// We do not check the parents scope here, as
		// each scope has content IDs that is unique.
		//
		const std::string * ToCheckID = NewContent.ID();
		if (!GotError
				&& ToCheckID != nullptr
				&& !_Scope.Contains(Namespace, *ToCheckID)) {
			
			_Content.push_back(&NewContent);
			Results = true;

		} else {
			Global::Log << "ERROR: Content with ID " << ID
									<< " already exists in the current scope "
									<< " of version " << ID << "."
									<< std::endl;
			GotError = true;
		}

		return(Results);
	}

	const std::vector<Content*>
	Version::GetContent() const
	{
		return(_Content);
	}

}
