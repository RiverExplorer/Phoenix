/**
 * Project: Phoenix
 * Time-stamp: <2025-03-21 09:50:23 doug>
 * 
 * @file GenerateEnumValue.cpp
 * @author Douglas Mark Royer
 * @date 08-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "rpcgen.hpp"

namespace RiverExplorer::rpcgen
{

	using namespace std;
	
	Enum::~Enum()
	{
	}
	
	void
	Enum::PrintCppHeader(ofstream & Stream)
	{
		vector<Item*>::const_iterator MIt;
		EnumValue * Value;
		
		string I = Indent();
		bool PrintedOne = false;

		PrintCppNamespaceBegin(Stream);
		Stream << I << "enum " << Name << " {" << endl;
		
		for (MIt = Enums.cbegin(); MIt != Enums.cend(); MIt++) {
			Value = dynamic_cast<EnumValue*>(*MIt);
			if (Value == nullptr) {
				Stream << endl;
				(*MIt)->PrintCppHeader(Stream);
				continue;
			}
			if (PrintedOne) {
				Stream << "," << endl;
			}
			Value->PrintCppHeader(Stream);
			PrintedOne = true;
		}

		Stream << endl << I << "};" << endl << endl;

		Stream << I << "/**" << endl;
		Stream << I << " * Serialize or deserialize a " << Name << " object." << endl;
		Stream << I << " * " << endl;
		Stream << I << " * @param Xdr An XDR object, initalzied with" << endl;
		Stream << I << " * XDR_ENCODE, XDR_DECODE or XDR_FREE." << endl;
		Stream << I << " * " << endl;
		Stream << I << " * @param Object The address of a " << Name << " object." << endl;
		Stream << I << " * " << endl;
		Stream << I << " * @return true on no errors." << endl;
		Stream << I << " */" << endl;
		Stream << I << "bool xdr_" << Name << "(XDR * Xdr, " << Name << " * Object);" << endl;
		Stream << endl;

		PrintCppNamespaceEnd(Stream);
	}

	void
	Enum::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	Enum::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	Enum::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	Enum::PrintAbnf(ofstream & Stream)
	{
		std::vector<Item*>::const_iterator IIt;
		Item * TheItem;
		EnumValue * Value;
		size_t					ToPad;
		std::string			Pad;
		std::string			Pad2;

		int MaxLength = 70;
		int CurrentLength = 0;
		int RestartLength = 0;

		bool PrintSlash = false;
		
		// Get all of the names and find the longest one.
		// We want to align the equal signs.
		//
		size_t Longest = Name.length();
		
		for (IIt = Enums.cbegin(); IIt != Enums.cend(); IIt++) {
			TheItem = dynamic_cast<EnumValue*>(*IIt);
			if (TheItem != nullptr) {
				Longest = MAX(Longest, TheItem->Name.length());
			}
		}
		Stream << endl;
		ToPad = Longest - Name.length();
		Pad.append(ToPad, ' ');
		Stream << Pad << Name << " =";
		CurrentLength = ToPad + Name.length();
		RestartLength = CurrentLength + 3;

		for (IIt = Enums.cbegin(); IIt != Enums.cend(); IIt++) {
			TheItem = dynamic_cast<EnumValue*>(*IIt);

			if (TheItem == nullptr) {
				(*IIt)->PrintAbnf(Stream);
			}
			if (CurrentLength + TheItem->Name.length() > MaxLength) {
				Pad.clear();
				Pad.append(RestartLength, ' ');
				CurrentLength = ToPad + TheItem->Name.length();
				Stream << endl << Pad;
				if (PrintSlash) {
					CurrentLength += 2;
					Stream << "/ ";
				}
				Stream << TheItem->Name;
					
			} else {
				CurrentLength += TheItem->Name.length();
				if (PrintSlash) {
					CurrentLength += 2;
					Stream << " /";
				}
				Stream << " " << TheItem->Name;
			}
			PrintSlash = true;
		}
		Stream << endl << endl;

		for (IIt = Enums.cbegin(); IIt != Enums.cend(); IIt++) {
			Value = dynamic_cast<EnumValue*>(*IIt);

			if (Value == nullptr) {
				(*IIt)->PrintAbnf(Stream);
			} else {

				ToPad = Longest - Value->Name.length();
				Pad.clear();
				Pad.append(ToPad, ' ');
				Stream << Pad;
				Value->PrintAbnf(Stream);
				Stream << endl << endl;
			}
		}
	}
	
	void
	Enum::PrintServer(ofstream & Stream)
	{
	}

	EnumValue::~EnumValue()
	{
	}

	// HPP
	//
	void
	EnumValue::PrintCppHeader(ofstream & Stream)
	{
		string I2 = Indent(IndentLevel + 1);

		Stream << I2 << Name << " = " << Type;

		return;
	}

	// XDR
	void
	EnumValue::PrintCppXDR(ofstream & Stream)
	{
	}

	// STUBS
	//
	void
	EnumValue::PrintCppStubs(ofstream & Stream)
	{
	}

	// XSD
	//
	void
	EnumValue::PrintXSD(ofstream & Stream)
	{
	}

	// ABNF
	//
	void
	EnumValue::PrintAbnf(ofstream & Stream)
	{
		Stream << Name << " = ";

		if (Type.starts_with("0x")) {
			Stream << Type.replace(0, 2, "%0");
		} else {
			Stream << Type;
		}
		return;
	}

	// SERVER
	//
	void
	EnumValue::PrintServer(ofstream & Stream)
	{
	}
	
}
