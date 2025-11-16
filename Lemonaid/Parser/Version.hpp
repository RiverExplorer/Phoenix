/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_VERSION_HPP__
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_VERSION_HPP__

#include "Content.hpp"
#include "Scope.hpp"

#include <string>
#include <map>

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * @class Version
	 * An object that manages the content for one version.
	 */
	 class Version
	 {
	 public:

		 /**
			* Version - Constructor.
			* A Version has a parent scope.
			*
			* @param OurID The unique ID for this version.
			* A version can be a string, or "1.3.14", or
			* any other string that starts with an alpha.
			*
			* @param ParentScope The parents scope.
			*
			*/
		 Version(std::string OurID, Scope * ParentScope);

		 /**
			* Version - Destructor.
			*/
		 virtual ~Version();

		 /**
			* Each version must have a unique ID within a protocol.
			*/
		 std::string ID;

		 /**
			* A version may have a namespace.
			* When emply, then no namespace was defined.
			*/
		 std::string Namespace;
		 
		 /**
			* Add a new Content to this version.
			*
			* @param NewContent The new Content to add.
			* This add the new Content to the end of the list.
			*
			* @return Returns true on success.
			* Returns false, when the content has an ID that
			* is not unique with this vesions scope.
			*/
		 bool Add(Content & NewContent);

		 /**
			* Get the ordered Content.
			*
			* @return The content, in original order.
			*/
		 const std::vector<Content*> GetContent() const;
		 
	 private:

		 /**
			* Each version has its own Scope.
			* The scope may override
			*
			* Operations search this objects scope,
			* then the parents.
			*/
		 Scope		_Scope;
		 Scope	* _ParentScope;

		 /**
			* The scopes ordered content.
			*/
		 std::vector<Content*>	_Content;
		 
		 /**
			* A Version ID to Version object map.
			*/
		 static std::map<std::string,Version*> _Versions;
		 bool _UniquenessChecked;
	 };	 
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_VERSION_HPP__
