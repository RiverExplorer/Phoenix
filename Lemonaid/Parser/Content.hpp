/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOTCOL_CONTENT_HPP__
#define __RIVEREXPLORER_PHOENIX_PROTOTCOL_CONTENT_HPP__

#include "Symbol.hpp"

#include <string>

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * @class Content
	 * Each line in a protocol file is content.
	 * This is a base class for any content type.
	 */
	class Content
	{
	public:

		enum Content_e {

			/**
			 * For an unspecified content type.
			 */
			Undefined_t = 0,
			
			/**
			 * The content is a comment line.
			 */
			Comment_t = 1,

			/**
			 * With symbol table entry.
			 */
			Symbol_t
		};
			
		/**
		 * Content - Constructor - for Text content
		 *
		 * @param Text The string content.
		 */
		Content(const std::string & Text);

		/**
		 * Content - Constructor - for symbol content
		 *
		 * @param SymbolEntry The symbol table entry for this content.
		 */
		Content(const Symbol & SymbolEntry);

		/**
		 * Content - Destructor.
		 */
		virtual ~Content();

		/**
		 * Get the ID for the content.
		 *
		 * @return The ID for this content.
		 * For Comment_t content, a nullptr is returned.
		 */
		const std::string * ID() const;
		
		/**
		 * Get the content type.
		 *
		 * @return Comment_t or Symbol_t.
		 * Or Undefined_t when not valid.
		 */
		Content_e Type() const;

		/**
		 * Get the text content for this entry.
		 *
		 * @return The text content for this entry.
		 * Returns nullptr, if not a Comment_t.
		 */
		const std::string * GetText() const;
		
		/**
		 * Get the symbol table entry for this content.
		 *
		 * @return The symbol table entry for this content.
		 * Returns nullptr if not a Symbol_t.
		 */
		const Symbol * GetSymbol() const;
		
	private:
		
		/**
		 * Many items in the definition file have
		 * a symbol table entry. Some do not.
		 * So, may be nullptr.
		 */
		const Symbol * _Symbol;

		/**
		 * Items without a symbol table entry,
		 * are stored as text.
		 */
		const std::string	* _Text;
	};
		
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOTCOL_CONTENT_HPP__
