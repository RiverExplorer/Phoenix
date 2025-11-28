/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#include "SymbolTable.hpp"
#include "Globals.hpp"

#include <sstream>

namespace RiverExplorer::Phoenix::Protocol
{
	std::vector<Symbol*>	SymbolTable::_SymbolsOrdered;
	
	SymbolTable::SymbolTable(SymbolTable * Parent)
	{
		_Parent = Parent;

		return;
	}

	std::string
	SymbolTable::_Mangle(Symbol * ToMangle)
	{
		// Need to make a hash key.
		// It can not be the method name alone, as Protocol
		// allows for polymorphism.
		//
		std::stringstream MKey;

		if (ToMangle->MethodReturnType->IsConstant) {
			MKey << "const.";
		}
		MKey << ToMangle->to_string(ToMangle->MethodReturnType->Type)
				 << "."
				 << ToMangle->ID
				 << ".";

		if (ToMangle->MethodParameters != nullptr) {
			std::vector<Symbol*>::const_iterator PIt;
			Symbol * S;
					
			for (PIt = ToMangle->MethodParameters->cbegin()
						 ; PIt != ToMangle->MethodParameters->cend()
						 ; PIt++) {
				S = *PIt;

				// It should never be nullptr.
				//
				if (S != nullptr) {
					if (S->IsConstant) {
						MKey << "const.";
					}
					MKey << S->to_string(S->Type)
							 << "."
							 << S->ID
							 << ".";
				}
			}
		}
		if (ToMangle->IsConstant) {
			MKey << "const";
		}
		MKey << ";";

		return(MKey.str());
	}
	
	bool
	SymbolTable::Add(Symbol * NewSymbol, int TheLineNumber)
	{
		bool Results = false;

		NewSymbol->LineNumber = TheLineNumber;
		NewSymbol->InputFileName = Global::CurrentFileName.front();

		switch (NewSymbol->Type) {

		case Symbol::comment_t:
			{
				// A comment is not really a symbol,
				// so it does not go into the _Symbols table.
				//
				// _SymbolsOrderd is just an ordered list of
				// object defined.
				//
				_SymbolsOrdered.push_back(NewSymbol);
				Results = true;
			}
			break;

		case Symbol::method_t:
			{
				std::string MangledKey = _Mangle(NewSymbol);

				std::map<std::string,Symbol*>::const_iterator Found;

				Found = _Symbols.find(MangledKey);

				if (Found == _Symbols.cend()) {
					_Symbols.insert(std::make_pair(MangledKey, NewSymbol));
					_SymbolsOrdered.push_back(NewSymbol);
					Results = true;

				} else {
					Global::Log << "ERROR: " << MangledKey
											<< " At: " << NewSymbol->LineNumber
											<< " In: " << NewSymbol->InputFileName
											<< " already exists in the symbol table.\n";
				}
			}
			break;
			
		default:
			{
				std::string Key = NewSymbol->Namespace + ":" + NewSymbol->ID;
		
				std::map<std::string,Symbol*>::const_iterator Found
					= _Symbols.find(Key);

				if (Found == _Symbols.cend()) {
					_Symbols.insert(std::make_pair(Key, NewSymbol));
					_SymbolsOrdered.push_back(NewSymbol);
					Results = true;
				}
			}
			break;
		}

		return(Results);
	}

	Symbol *
	SymbolTable::Find(const std::string & Namespace,
										const std::string & ID)
	{
		Symbol	*	Results = nullptr;

		std::vector<Symbol*>::const_iterator it;
		Symbol * Entry;
		
		for (it = _SymbolsOrdered.cbegin(); it != _SymbolsOrdered.cend(); it++) {
			Entry = *it;
			if (Entry->ID == ID) {
				if (Entry->Namespace == Namespace) {
					Results = Entry;
					break;
				}
			}
		}

		return(Results);
	}

	bool
	SymbolTable::Contains(const std::string & Namespace,
												const std::string & ID) const
	{
		bool Results = false;
		
		for (auto const & Pair : _Symbols) {
			if (Pair.second->ID == ID) {
				if (Pair.second->Namespace == Namespace) {
					Results = true;
					break;
				}
			}
		}

		return(Results);
	}

	std::ostream & operator<<(std::ostream & Out, const SymbolTable & Table)
	{
		Out << std::endl << "Symbol Dump:" << std::endl;

		Symbol * S;
		
		for (auto const & Pair : Table._Symbols) {
			S = Pair.second;
			Out << *S;
		}
		Out << std::endl;
		
		return(Out);
	}

	const std::vector<Symbol*> &
	SymbolTable::Symbols()
	{
		return(_SymbolsOrdered);
	}
	
}
