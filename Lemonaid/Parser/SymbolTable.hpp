/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#ifndef __RIVEREXPLORER_PHOENIX_SYMBOLTABLE_HPP_
#define __RIVEREXPLORER_PHOENIX_SYMBOLTABLE_HPP_

#include <cstdint>
#include <string>
#include <map>
#include "Symbol.hpp"
#include "Pack.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * A symbol table exists in a scope.
	 * One global one, and one per class.
	 *
	 * A symbol name is its namespace + ID.
	 * If it does not have a namespace, then just its ID.
	 */
	class SymbolTable
	{

	public:
		
		/**
		 * Constructor.
		 *
		 * @param Parent Our parent symbol table.
		 *
		 * @note
		 * Only the global symbol table has a nullptr Parent.
		 */
		SymbolTable(SymbolTable * Parent = nullptr);

		/**
		 * Add a new symbol.
		 *
		 * @param NewSymbol The new symbol to add.
		 *
		 * @return true if added.
		 * Returns false if NewSymbol->Name already exists
		 * at this scope.
		 */
		bool Add(Symbol * NewSymbol, int TheLineNumber);

		/**
		 * Check to see if this scope a symbol
		 * with the procided ID.
		 *
		 * @param Namespace The symbols namespace.
		 * If none, pass in an empty string.
		 *
		 * @param IdToCheck The ID to check for.
		 *
		 * @return true if it has IdToCheck, else
		 * returns false.
		 */
		bool Contains(const std::string & Namespace,
									const std::string & IdToCheck) const;
		 
		/**
		 * Check to see if any parent scope has a symbol
		 * with the procided ID.
		 *
		 * @param Namespace The symbols namespace.
		 * If none, pass in an empty string.
		 *
		 * @param IdToCheck The ID to check for.
		 *
		 * @return true if it has IdToCheck, else
		 * returns false.
		 */
		bool ParentsContain(const std::string Namespace,
												const std::string & IdToCheck) const;

		/**
		 * Print symbol table.
		 * This prints TEXT, not generated code.
		 *
		 * @param Out To this stream.
		 */
		friend std::ostream &
		operator<<(std::ostream & Out, const SymbolTable & Table);

		/**
		 * We want objects and globals to be able to be transmitted
		 * and received correctly. One minor drawback with many
		 * methods is that the variables must be in exactly the same
		 * order each time, and additional procedures must be in place
		 * to copy default or null values.
		 *
		 * So, the solution here, is during the inital connection,
		 * the server side sends the client the list of fully qualified
		 * shortcut-ID's with their variable types.
		 *
		 * Then during data transfer the shortcut-ID is sent, followed
		 * by its value in the aggreed on format. Then if the author
		 * rearanges the data variables, and keeps them the same name
		 * in the same namespace. No recompile is needed on all endpoints.
		 *
		 * This is the symbol transfer table.
		 * All values are 8-bit unsigned integers, or UTF-8.
		 *
		 * This is not used by all wire protocols. It is available to
		 * all wire protocols. As an example, XDR can never use it.		 
		 */
		class TransfreSymbolTable
		{
			uint32_t	NumberEntries;

			struct OneSymbol {

				/**
				 * The shortcut ID.
				 */
				uint16_t	Shortcut;

				/**
				 * This is sent as a 1 octet length,
				 * followed by the string.
				 */
				std::string ID;
			};

			/**
			 * Create the table.
			 *
			 * @param Table the symbol table to create it from.
			 *
			 * @param[out] Data The data, initialzied.
			 * The end of the data, is the first string name
			 * with a string lenght of zero.
			 * The last octet in Data is always a zero.
			 *
			 * @param[out] Size The amount of data at 'Data'.
			 *
			 * @note
			 * When done with the data 'delete' it.
			 */
			void CreateTable(const SymbolTable & Table,
											 uint8_t *& Out, uint32_t & Size);
		};
	private:
		
		
		/**
		 * The KEY is a colon then the ID appended to the namespace.
		 */
		std::map<std::string,Symbol*> _Symbols;
		SymbolTable * _Parent;
		
	};
}

#endif // __RIVEREXPLORER_PHOENIX_SYMBOLTABLE_HPP_
