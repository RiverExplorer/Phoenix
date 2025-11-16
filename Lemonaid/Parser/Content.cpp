/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#include "Content.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
			
	Content::Content(const std::string & Text)
	{
		_Text = &Text;

		return;
	}

	Content::Content(const Symbol & SymbolEntry)
	{
		_Symbol = &SymbolEntry;

		return;
	}

	Content::~Content()
	{
		_Symbol = nullptr;
		_Text = nullptr;
	}

	const std::string *
	Content::ID() const
	{
		const std::string * Results = nullptr;
		
		if (_Symbol != nullptr) {
			Results = &_Symbol->ID;
		}

		return(Results);
	}
	
	Content::Content_e
	Content::Type() const
	{
		Content_e Results = Undefined_t;
		
		if (_Text != nullptr) {
			Results = Comment_t;
		} else if (_Symbol != nullptr) {
			Results = Symbol_t;
		}

		return(Results);
							 
	}

	const std::string *
	Content::GetText() const
	{
		return(_Text);
	}
		
	const Symbol *
	Content::GetSymbol() const
	{
		return(_Symbol);
	}
		
}
