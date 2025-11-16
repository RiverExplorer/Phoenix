/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#include "Symbol.hpp"
#include <ostream>
#include <string>

namespace RiverExplorer::Phoenix::Protocol
{
	Symbol::Symbol(Symbol_e SType,
								 std::string SName,
								 Array_e SArrayType,
								 uint64_t SMin,
								 uint64_t SMax)
	{
		Type = SType;
		ID = SName;
		Array = SArrayType;
		MinSize = SMin;
		MaxSize = SMax;
		Bits = 0; // N/A
		HasNoName = false;

		ValidRange = nullptr;
		Default = nullptr;
		
		return;
	}
		
	Symbol::Symbol(Symbol_e SType,
								 uint64_t SBits,
								 std::string SName,
								 Array_e SArrayType,
								 uint64_t SMin,
								 uint64_t SMax)
	{
		Type = SType;
		Bits = SBits;
		ID = SName;
		Array = SArrayType;
		MinSize = SMin;
		MaxSize = SMax;
		HasNoName = false;

		ValidRange = nullptr;
		Default = nullptr;
		
		return;
	}

	std::string
	Symbol::ToString(Symbol_e Type)
	{
		std::string Results = "Unknown";

		switch (Type) {

		case uint_t:
			Results = "uint";
			break;
			
		case sint_t:
			Results = "sint";
			break;
			
		case float_t:
			Results = "float";
			break;
			
		case string_t:
			Results = "string";
			break;
			
		case class_t:
			Results = "class";
			break;
			
		case void_t:
			Results = "void";
			break;
			
		case method_t:
			Results = "method";
			break;

		case opaque_t:
			Results = "opaque";
			break;
		}

		return(Results);
	}

	std::string
	Symbol::ToString(Array_e A)
	{
		std::string Results = "NotArray";

		if (A == FixedArray_t) {
			Results = "Fixed";
				
		} else if (A == VariableArray_t) {
			Results = "Variable";
		}

		return(Results);
	}
	
	bool
	Symbol::operator==(const Symbol & Other) const
	{
		return(ID == Other.ID && Namespace == Other.Namespace);
	}
	
	bool
	Symbol::operator!=(const Symbol & Other) const
	{
		return(ID != Other.ID || Namespace != Other.Namespace);
	}
	
	std::ostream &
	operator<<(std::ostream & Out, const Symbol & S)
	{
		std::string TheType = S.ToString(S.Type);
		
		Out << S.ID << " [Line: " << S.LineNumber << ", ";

		if (S.Namespace.length() > 0) {
			Out << "In namespace: " << S.Namespace;
			
		} else {
			Out << "Not in a namespace";
		}
		
		if (S.Array == Symbol::NotArray_t) {
			Out << ", of type " << TheType;

			if (S.Bits == 0) {
				if (S.Type == Symbol::string_t) {
					Out << ", with no predefined length.";
				} else {
					Out << ", of undefned width.";
				}
			} else {
				Out << ", that is " << S.Bits << " wide";
			}
			
		} else {
			Out << ", A " << S.ToString(S.Array) << ", sized Array";

			if (S.Array == Symbol::FixedArray_t) {
				Out << ", of " << S.MinSize << " " << TheType;
				
			} else {
				if (S.MinSize == 0 && S.MaxSize == 0) {
					Out << ", with zero or more values";
					
				} else {
					Out << ", with at least " << S.MinSize << " values"
							<< ", and no more than " << S.MaxSize << " values";
				}
			}
		}

		Out << "]" << std::endl;

		return(Out);

	}
}
