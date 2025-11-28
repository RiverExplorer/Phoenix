/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_ENUM_HPP_
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_ENUM_HPP_

#include <limits>
#include <cstdint>
#include <string>

#include <Symbol.hpp>

namespace RiverExplorer::Phoenix::Protocol
{
	template <typename T>
	class EnumDefT
		: public SymbolValueT<T>
	{
	public:

		/**
		 * EnumDef - Destructor.
		 */
		virtual ~EnumDefT() {};

		std::string	Name;

		/**
		 * EnumDef - Constructor
		 */
		EnumDefT(std::string EName, T EValue)
			: SymbolValueT<T>(EValue)
		{
			Name = EName;
				
			return;
		}
	};

	/**
	 * This is the base class for any
	 * EnumT() variable.
	 *
	 * It exists solely to be able to create a
	 * pointer to any EnumT() variable.
	 */
	class Enum
		: public Symbol
	{
	public:
		Enum(std::string EName)
			: Symbol(Symbol::enum_t, EName, Symbol::NotArray_t)
		{
			Defs = new std::vector<SymbolValue*>();
		};

		virtual ~Enum() {};

		std::vector<SymbolValue*> * Defs;

	};
	
	std::ostream & operator<<(std::ostream & Out,
														const Enum & E);
	
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_ENUM_HPP_
