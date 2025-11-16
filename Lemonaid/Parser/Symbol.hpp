/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_SYMBOL_HPP_
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_SYMBOL_HPP_

#include "Range.hpp"
#include "DefaultValue.hpp"

#include <cstdint>
#include <string>
#include <vector>
//#include "location.hh"

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * A symbol table exists in a scope.
	 * One global one, and one per class.
	 */
	class Symbol
	{
	public:

		/**
		 * When true, this object represents a type
		 * without a name (like a return type).
		 */
		bool	HasNoName;
		
		/**
		 * Symbol types.
		 */
		enum Symbol_e {
			uint_t,
			sint_t,
			float_t,
			string_t,
			opaque_t,
			class_t,
			void_t,
			method_t
		};

		enum Array_e {
			NotArray_t,
			FixedArray_t,
			VariableArray_t
		};
		
		/**
		 * Constructor.
		 *
		 * @param Type The base type of the symbol.
		 *
		 * @param Name The name of the symbol.
		 *
		 * @param ArrayType It is or is not an array.
		 *
		 * @param Min For a fixed array, the size.
		 * For a variable array, the minimum size.
		 *
		 * @param Min For a fixed array, ignored.
		 * For a variable array, the maximum size.
		 */
		Symbol(Symbol_e Type,
					 std::string Name,
					 Array_e ArrayType = NotArray_t,
					 uint64_t Min = 0,
					 uint64_t Max = 0);
		
		/**
		 * Constructor.
		 *
		 * @param Type The base type of the symbol.
		 *
		 * @param Bits The number of bits in the symbol.
		 *
		 * @param Name The name of the symbol.
		 *
		 * @param ArrayType It is or is not an array.
		 *
		 * @param Min For a fixed array, the size.
		 * For a variable array, the minimum size.
		 *
		 * @param Min For a fixed array, ignored.
		 * For a variable array, the maximum size.
		 */
		Symbol(Symbol_e Type,
					 uint64_t Bits,
					 std::string Name,
					 Array_e ArrayType = NotArray_t,
					 uint64_t Min = 0,
					 uint64_t Max = 0);
		
		/**
		 * Convert Symbol_e to printable string name.
		 */
		static std::string ToString(Symbol_e TheType);
		
		/**
		 * Convert Symbol_e to printable string name.
		 */
		static std::string ToString(Array_e TheArrayType);
		
		/**
		 * Print symbol.
		 * This prints TEXT, not generated code.
		 *
		 * @param Out To this stream.
		 */
		friend std::ostream &
		operator<<(std::ostream & Out, const Symbol & Obj);

		/**
		 * A == compare operator.
		 *
		 * @param Other The other symbol.
		 *
		 * @return true if they are the same ID
		 * and in the same namespace.
		 */
		bool operator==(const Symbol & Other) const;
		
		/**
		 * A != compare operator.
		 *
		 * @param Other The other symbol.
		 *
		 * @return true if they are the same ID
		 * and in the same namespace.
		 */
		bool operator!=(const Symbol & Other) const;
		
		Symbol_e	Type;
		uint64_t	Bits;
		Array_e		Array;
		uint64_t	MinSize;
		uint64_t	MaxSize;

		/**
		 * Any range, or nullptr.
		 */
		Range		* ValidRange;

		/**
		 * Any default, or nullptr.
		 */
		DefaultValue	*	Default;
		
		/**
		 * The symbols namespace. Or empty when none.
		 */
		std::string Namespace;
		
		/**
		 * The ID or variable name.
		 */
		std::string	ID;

		/**
		 * Methods have a return type, and zero or more parameters.
		 */
		Symbol							*	MethodReturnType;
		std::vector<Symbol*>*	MethodParameters;

		/**
		 * Where they were defined.
		 */
		int										LineNumber;
		std::string						InputFileName;

	};
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_SYMBOLTABLE_HPP_
