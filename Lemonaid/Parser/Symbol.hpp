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
	 * A base class for any symbol value.
	 */
	class SymbolValue
	{
	public:
		virtual ~SymbolValue() {};
		
		/**
		 * Print the value.
		 *
		 * @param Out Where to write the value to.
		 *
		 * @note
		 * Call HasRange() first.
		 */
		virtual void Print(std::ostream & Out) const = 0;
		
	protected:
		SymbolValue() {};
	};
	
	/**
	 * A value for the symbol.
	 */
	template <typename T>
	class SymbolValueT
		: public SymbolValue
	{
	public:
		SymbolValueT()
		{			
		};

		SymbolValueT(T TheValue)
			: Value(TheValue)
		{			
		};

		virtual ~SymbolValueT() {};
		
		/**
		 * Print the value.
		 *
		 * @param Out Where to write the value to.
		 *
		 * @note
		 * Call HasRange() first.
		 */
		virtual void Print(std::ostream & Out) const override
		{
			Out << Value;
		}
		
		T  Value;
	};
		
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
		 * When true, this symbol is a constant.
		 * And it was defined with a value.
		 */
		bool	IsConstant;

		/**
		 * A mutable variable is a variable that can be modified
		 * even when the object it is contained in is marked constant.
		 * For example, it could keep a running total of points or
		 * health and cache the results in a 'mutable' variable
		 * that it modifies as things change.
		 *
		 * And its containing object could be marked as const
		 * so others can not tweak the inverntry or health.
		 *
		 * In non-const objects, Mutable is ignored as it would
		 * have no meaning when an object is not-const.
		 */
		bool IsMutable;
		
		/**
		 * The basic data types.
		 */
		enum Symbol_e {

			/**
			 * The protocol data.
			 */
			protocol_t,

			/**
			 * The version data.
			 */
			version_t,
			
			/**
			 * Boolean type.
			 */
			bool_t,

			/**
			 * Comment.
			 */
			comment_t,
			
			/**
			 * The symbol has an unsigned integer data/value.
			 */
			uint_t,

			/**
			 * The symbol has an signed integer data/value.
			 */
			sint_t,

			/**
			 * The symbol has floating point (long double) data/value.
			 */
			float_t,
			
			/**
			 * The symbol has string data/value.
			 */
			string_t,
			
			/**
			 * The symbol has a blob of opaque data/value.
			 */
			opaque_t,

			/**
			 * An enum type.
			 */
			enum_t,
			
			/**
			 * The symbol is a class name.
			 */
			class_t,
			
			/**
			 * The symbol has no data.
			 */
			
			void_t,
			
			/**
			 * The symbol is a member function name.
			 */
			method_t,

			/**
			 * The symbol is a const unsigned interger range.
			 */
			urange_t,
			
			/**
			 * The symbol is a const signed interger range.
			 */
			srange_t,

			/**
			 * The symbol is a const floating point range.
			 */
			frange_t
		};

		/**
		 * A symbol is not an array,
		 * a fixed array, or a variable array.
		 */
		enum Array_e {
			NotArray_t,
			FixedArray_t,
			VariableArray_t
		};

		/**
		 * Symbol visibility.
		 */
		enum Visibility_e {

			/**
			 * A public symbol can be seen from any
			 * part of the code that is generated.
			 */
			Public_t,

			/**
			 * A protected symbol can only be seen
			 * within the scope it is defined in,
			 * and derived scopes.
			 */
			Protected_t,

			/**
			 * An internal symbol can be seen any place
			 * in the generated code, but not available
			 * to the user of the generated code.
			 */
			Internal_t,

			/**
			 * A private symbol is one used only within
			 * the scope that it is defined within.
			 */
			Private_t
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
		 * Constructor.
		 *
		 * @param Type The base type of the symbol.
		 *
		 * @param Bits The number of bits in the symbol.
		 *
		 * @param ArrayType It is or is not an array.
		 *
		 * @param Min For a fixed array, the size.
		 * For a variable array, the minimum size.
		 *
		 * @param Min For a fixed array, ignored.
		 * For a variable array, the maximum size.
		 *
		 * @note
		 * Return values have no Name (ID).
		 */
		Symbol(Symbol_e Type,
					 uint64_t Bits,
					 Array_e ArrayType = NotArray_t,
					 uint64_t Min = 0,
					 uint64_t Max = 0);

		virtual ~Symbol();
		
		/**
		 * Check if this symbol has a range.
		 *
		 * @return true if it has a range.
		 * Else returns false.
		 */
		virtual bool HasRange() const
		{
			return(ValidRange != nullptr ? true : false);
		}
		
		/**
		 * This method is used to display the minimum range value
		 * in a human readable format.
		 *
		 * @param Out Where to write the value to.
		 *
		 * @note
		 * Call HasRange() first.
		 */
		virtual void PrintRangeMin(std::ostream & Out) const
		{
			if (ValidRange != nullptr) {
				ValidRange->PrintMin(Out);
			}

			return;
		}

		/**
		 * This method is used to display the maximum range value
		 * in a human readable format.
		 *
		 * @param Out Where to write the value to.
		 *
		 * @note
		 * Call HasRange() first.
		 */
		virtual void PrintRangeMax(std::ostream & Out) const
		{
			if (ValidRange != nullptr) {
				ValidRange->PrintMax(Out);
			}

			return;
		}

		/**
		 * Check if this symbol has a default value.
		 *
		 * @return true if it has a default value.
		 * Else returns false.
		 */
		virtual bool HasDefault() const
		{
			return(Default != nullptr ? true : false);
		}
		
		/**
		 * Print the default value for this symbol.
		 *
		 * @param Out where to write the value to.
		 *
		 * @note
		 * Call HasDefault() first.		 
		 */
		virtual void PrintDefault(std::ostream & Out) const
		{
			if (Default != nullptr) {
				Default->Print(Out);
			}

			return;
		}
		
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

		/**
		 * Print the data type of the value, as a string.
		 *
		 * @param Type The symbol value type.
		 *
		 * @return The type, as a string.
		 */
		std::string		to_string(Symbol_e Type) const;

		/**
		 * Print if the array type is fixed, variable,
		 * Not an array, as a string.
		 *
		 * @param A The array type to check.
		 *
		 * @return The array type, as a string.
		 */
		std::string		to_string(Array_e A) const;

		/**
		 * Print the visibility as a printed string.
		 *
		 * @return The visibility as a string.
		 */
		std::string		to_string(Visibility_e V) const;
		
		Symbol_e	Type;
		uint64_t	Bits;
		Array_e		Array;
		uint64_t	MinSize;
		uint64_t	MaxSize;

		/**
		 * The symbol visibility.
		 * The default is Public_t.
		 */
		Visibility_e	Visibility;
		
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

		/**
		 * The value of the symbol.
		 */
		SymbolValue					*	Value;
		
		friend std::ostream & operator<<(std::ostream & Out, const Symbol & S);
	}; // End class Symbol

	std::ostream & operator<<(std::ostream & Out, const Symbol & S);
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_SYMBOLTABLE_HPP_
