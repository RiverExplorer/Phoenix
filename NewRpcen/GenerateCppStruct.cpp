/**
 * Project: Phoenix
 * Time-stamp: <2025-03-12 14:19:26 doug>
 * 
 * @file GenerateStuct.cpp
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
#include <sys/param.h>

using namespace std;

namespace RiverExplorer::rpcgen
{

	Struct::~Struct()
	{
	}
	
	static void
	PrintSizeInfo(StructMember * Member, std::string & Tabs, ofstream & Stream)
	{
		if (Member->IsVariableArray || Member->IsFixedArray) {
			if (Member->ArraySize != "") {
				if (Member->IsVariableArray) {
					Stream << endl << Tabs << "// Configured up to "
								 << Member->ArraySize << " entries." << endl;
				} else if (Member->IsFixedArray) {
					Stream << endl << Tabs << "// Fixed at "
								 << Member->ArraySize << " entries." << endl;
				} else {
					Stream << endl << Tabs << "// No size set for array." << endl;
				}
			}
		}

		return;
	}
	
	void
	Struct::PrintCppHeader(ofstream & Stream)
	{
		Stream << endl;

		string I = Indent();
		
		Stream << I << "class " << Name << endl;
		Stream << I << "{" << endl;
		Stream << I << "public:" << endl;
		IndentLevel++;

		string I2 = Indent();
		Stream << I2 << Name << "();" << endl;
		Stream << endl;
		Stream << I2 << "virtual ~" << Name << "();" << endl;
		Stream << endl;
		Stream << I2 << "/**" << endl;
		Stream << I2 << " * Serialize or deserialize a " << Name << " object." << endl;
		Stream << I2 << " * " << endl;
		Stream << I2 << " * @param Xdr An XDR object, initalzied with" << endl;
		Stream << I2 << " * XDR_ENCODE, XDR_DECODE or XDR_FREE." << endl;
		Stream << I2 << " * " << endl;
		Stream << I2 << " * @return true on no errors." << endl;
		Stream << I2 << " */" << endl;
		Stream << I2 << "bool Xdr(XDR & Xdr);" << endl;
		Stream << endl;
		
		vector<Item*>::const_iterator MIt;
		StructMember * Member;

		bool NeedSemi;
		
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;

			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				(*MIt)->PrintCppHeader(Stream);
				continue;
			}
			
			if (Member->Type == "") {
				continue;
			}
			if (Member->Type == "opaque") {
				Stream << I2 << "std::vector<uint8_t>";
				
			} else if (Member->Type == "string") {
				Stream << I2 << "std::string";
				
			} else {
				PrintSizeInfo(Member, I2, Stream);
				if (Member->IsFixedArray || Member->IsVariableArray) {
					if (Member->IsPointer) {
						Stream << I2 << "std::vector<" << Member->Type << "*>";
					} else {
						Stream << I2 << "std::vector<" << Member->Type << ">";
					}
				} else {
					Stream << I2 << Member->Type;
				}
			}
			Stream << ' ';
			
			if (Member->IsPointer
					&& (!Member->IsFixedArray && !Member->IsVariableArray)) {
				Stream << " * " << Member->Name;
			} else {
				Stream << Member->Name;
			}
			if (Member->IsFixedArray) {
				NeedSemi = false;
				Stream << "; // Fixed Size: " << Member->ArraySize << endl;
				
			} else if (Member->IsVariableArray) {
				if (Member->ArraySize != "") {
					Stream << ";\t// Max Size: " << Member->ArraySize << endl;
					NeedSemi = false;
				}
			}
			if (NeedSemi) {
				Stream << ";" << endl;
			}
			Stream << endl;
		}
		Stream << endl;
		
		IndentLevel--;
		Stream << I << "}; // End of class " << Name << endl << endl;

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
		Stream << I << "bool xdr_" << Name << "(XDR & Xdr, " << Name << " * Object);" << endl;

		Stream << endl;
		return;
	}

	void
	Struct::PrintCppXDR(ofstream & Stream)
	{
		vector<Item*>::const_iterator MIt;
		StructMember * Member;

		bool NeedSemi;
		
		Stream << endl;

		string I = Indent();
		string I2;
		string I3;
		
		// Constructor.
		//
		Stream << I << Name << "::" << Name << "()" << endl;
		Stream << I << "{" << endl;
		IndentLevel++;

		I2 = Indent();

		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;
			
			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				(*MIt)->PrintCppXDR(Stream);
				continue;
			}
			
			if (Member->Type == "") {
				continue;
			}

			if (Member->IsPointer) {
				Stream << I2 << Member->Name << " = nullptr;" << endl;
				Stream << endl;
			}

			if (Member->ArraySize != "") {
				PrintSizeInfo(Member, I2, Stream);
				Stream << I2
							 << Member->Name << ".reserve(" << Member->ArraySize << ");";
				Stream << endl;
			}
		}
		Stream << endl << I2 << "return;" << endl;
		IndentLevel--;
		Stream << I << "} // End of " << Name << "::" << Name << "()" << endl;
		Stream << endl;
		
		// Destructor.
		//
		Stream << I << Name << "::~" << Name << "()" << endl;
		Stream << I << "{" << endl;
		IndentLevel++;

		I2 = Indent();
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;

			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				//(*Mit)->PrintCppXDR(Stream);
				continue;
			}
			if (Member->Type == "") {
				continue;
			}

			if (Member->IsPointer) {
				Stream << I2 << "if (" << Member->Name << " != nullptr) {" << endl;
				IndentLevel++;
				I3 = Indent();
				Stream << I3 << "delete " << Member->Name << ";" << endl;
				Stream << I3 << Member->Name << " = nullptr;" << endl;
				IndentLevel--;
				Stream << I2 << "}" << endl << endl;
			}
		}
		Stream << I2 << "return;" << endl;
		IndentLevel--;
		Stream << I << "}; // End of " << Name << "::~" << Name << "()" << endl;
		Stream << endl;

		// XDR
		//
		Stream << I << "bool" << endl;
		Stream << I << Name << "::Xdr(XDR & Xdr)" << endl;
		Stream << I << "{" << endl;
		IndentLevel++;

		I2 = Indent();
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;

			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member == nullptr) {
				//(*Mit)->PrintCppXDR(Stream);
				continue;
			}
			if (Member->Type == "") {
				continue;
			}

			if (Member->Type == "opaque") {
				if (Member->IsPointer) {
					if (Member->ArraySize == "") {
						Stream << endl << I2 << "// No size set for array" << endl;
						Stream << I2 << "if (!xdr_VectorOfOpaque(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						PrintSizeInfo(Member, I2, Stream);
						Stream << I2 << "if (!xdr_VectorOfOpaque(&Xdr, &"
									 << Member->Name << ", "
									 << Member->ArraySize << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				} else {
					if (Member->ArraySize == "") {
						Stream << endl << I2 << "// No size set for array" << endl;
						Stream << I2 << "if (!xdr_VectorOfOpaque(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						PrintSizeInfo(Member, I2, Stream);
						Stream << I2 << "if (!xdr_VectorOfOpaque(&Xdr, &"
									 << Member->Name << ", "
									 << Member->ArraySize << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				}
					
			} else if (Member->Type == "string") {
				if (Member->IsPointer) {
					if (Member->ArraySize == "") {
						Stream << endl << I2 << "// No size set for string." << endl;
						Stream << I2 << "if (!xdr_StdString(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						PrintSizeInfo(Member, I2, Stream);
						Stream << I2 << "if (!xdr_StdString(&Xdr, &"
									 << Member->Name << ", "
									 << Member->ArraySize << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				} else {
					if (Member->ArraySize == "") {
						Stream << endl << I2 << "// No size set for string." << endl;
						Stream << I2 << "if (!xdr_StdString(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						PrintSizeInfo(Member, I2, Stream);
						Stream << I2 << "if (!xdr_StdString(&Xdr, &"
									 << Member->Name << ", "
									 << Member->ArraySize << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				}

			} else if (IsBuiltInXdrType(Member->Type)) {
				if (Member->IsPointer) {
					Stream << I2 << "if (!xdr_pointer(&Xdr, (char**)&"
								 << Member->Name << ", sizeof(*" << Member->Name << "),xdr_"
								 << Member->Type << ")) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
									 
				} else {
					if (Member->IsFixedArray || Member->IsVariableArray) {
						if (Member->ArraySize == "") {
							Stream << I2 << "if (!xdr_VectorOf<" << Member->Type << ">"
										 << "(&Xdr, &" << Member->Name
										 << ", xdr_" << Member->Type
										 << ")) {" << endl
										 << I2 << "\treturn(false);" << endl
										 << I2 << "}" << endl;
						} else {
							PrintSizeInfo(Member, I2, Stream);
							Stream << I2 << "if (!xdr_VectorOf<" << Member->Type  << ">"
										 << "(&Xdr, &" << Member->Name
										 << ", xdr_" << Member->Type
										 << ", " << Member->ArraySize
										 << ")) {" << endl
										 << I2 << "\treturn(false);" << endl
										 << I2 << "}" << endl;
						}
					} else {
						Stream << I2 << "if (!xdr_" << Member->Type
									 << "(&xdr, &" << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				}
			} else {
				if (Member->IsPointer) {
					Stream << I2 << "if (!xdr_pointer(&Xdr, (char**)&" << Member->Name
								 << ", sizeof(*" << Member->Name << "), xdr_"
								 << Member->Type << ")) {" << endl
								 << I2 << "\treturn(false);" << endl
								 << I2 << "}" << endl;
				} else {
					if (Member->IsFixedArray || Member->IsVariableArray) {
						if (Member->ArraySize == "") {
							Stream << I2 << "if (!xdr_VectorOf<" << Member->Type << ">"
										 << "(&Xdr, &" << Member->Name
										 << ", xdr_" << Member->Type
										 << ")) {" << endl
										 << I2 << "\treturn(false);" << endl
										 << I2 << "}" << endl;
						} else {
							PrintSizeInfo(Member, I2, Stream);
							Stream << I2 << "if (!xdr_VectorOf<" << Member->Type << ">"
										 << "(&Xdr, &" << Member->Name
										 << ", xdr_" << Member->Type
										 << ", " << Member->ArraySize
										 << ")) {" << endl
										 << I2 << "\treturn(false);" << endl
										 << I2 << "}" << endl;
						}
					} else {
						Stream << I2
									 << "if (!" << Member->Type << ".Xdr(Xdr)) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				}
			}
		}
		Stream << I2 << "return;" << endl;
		IndentLevel--;
		Stream << I << "}; // End of " << Name << "::Xdr()" << endl;
		Stream << endl;

		return;
	}

	void
	Struct::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	Struct::PrintXSD(ofstream & Stream)
	{
	}

	static std::string
	ConvertFromXdrComment(Comment * C)
	{
		std::string Results = ";";
		const char * Str = C->Name.c_str();
		
		if (strncmp(Str, "//", 2) == 0) {
			Results += C->Name.substr(2);
			
		} else if (strncmp(Str, "/*", 2) == 0) {

			// Remove the first two characters "/*" and the last two "*/".
			//
			std::string Tmp = C->Name.substr(2, C->Name.length() - 4);

			// Insert ";" before the first non-whitespace character
			// after each of the line.
			//
			for (int Offset = 2; Offset < C->Name.length() - 4; Offset++) {
				if (C->Name[Offset] == '\r') {
					continue;
				}
				if (C->Name[Offset] == '\n') {
					Results += '\n';
					Offset++;
					
					// Collect up to the first non-whitespace.
					//
					while (C->Name[Offset] == ' ' || C->Name[Offset] == '\t') {
						Results += C->Name[Offset++];
					}
					// Add in "; ".
					//
					Results += "; ";
				} else {
					Results += C->Name[Offset];
				}
			}
		}

		return(Results);
	}
	
	void
	Struct::PrintAbnf(ofstream & Stream)
	{
		vector<Item*>::const_iterator MIt;
		StructMember *	Member;
		size_t					ToPad;
		std::string			Pad;
		std::string			Pad2;
		
		// Get all of the names and find the longest one.
		// We want to align the equal signs.
		//
		size_t Longest = Name.length();
		
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			Member = dynamic_cast<StructMember*>(*MIt);
			if (Member != nullptr) {
				Longest = MAX(Longest, Member->Name.length());
			}
		}
		Stream << endl;
		Stream << Name << " =";

		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			Member = dynamic_cast<StructMember*>(*MIt);

			if (Member != nullptr) {
				Stream << " " << Member->Name;
			}
		}
		Stream << endl << endl;
			
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			Member = dynamic_cast<StructMember*>(*MIt);

			if (Member != nullptr) {
				ToPad = Longest - Member->Name.length();
				Pad.clear();
				Pad.append(ToPad, ' ');

				Pad2.clear();
				Pad2.append(ToPad + 1 + Member->Name.length(), ' ');
				// Any comment about the contets.
				//
				if (Member->IsVariableArray) {
					if (Member->ArraySize == "") {
						Stream << Pad2 << "; Unlimited length." << endl;
					} else {
						Stream << Pad2 << "; Up to "
									 << Member->ArraySize << " in size."
									 << endl;
					}
				} else if (Member->IsFixedArray) {
						Stream << Pad2 << "; Exactly "
									 << Member->ArraySize << " in size."
									 << endl;
				}

				// The variable name.
				//
				Stream << Member->Name << Pad << " = ";
				
				// = The variable type.
				//
				if (Member->Type == "opaque" || Member->Type == "string") {
					// XDR Opaque is variable or fixed - only.
					//
					if (Member->IsVariableArray) {
						if (Member->ArraySize == "") {
							Stream << "NumberOfOctets "
										 << "*uint8_t";
						} else {
							Stream << "NumberOfOctets "
										 << "0*" << Member->ArraySize << "uint8_t";
						}
					} else if (Member->IsFixedArray) {
						Stream << Member->ArraySize << "uint8_t";
					}

				} else {
					if (Member->IsVariableArray) {
						if (Member->ArraySize == "") {
							Stream << "NumberOfOctets "
										 << "*uint8_t";
						} else {
							Stream << "NumberOfOctets "
										 << "0*" << Member->ArraySize << Member->Type;
						}
					} else if (Member->IsFixedArray) {
						Stream << Member->ArraySize << Member->Type;
					} else {
						Stream << Member->Type;
					}
				}
				Stream << endl << endl;
			} else {
				Comment * C = dynamic_cast<Comment*>(*MIt);

				if (C != nullptr) {
					Stream << ConvertFromXdrComment(C);
					Stream << endl;
				}
			}
		}
		Stream << endl << endl;
			
	}
	
	void
	Struct::PrintServer(ofstream & Stream)
	{
	}

	StructMember::~StructMember()
	{
	}
	
	void
	StructMember::PrintCppHeader(ofstream & Stream)
	{
	}

	void
	StructMember::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	StructMember::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	StructMember::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	StructMember::PrintAbnf(ofstream & Stream)
	{
	}
	
	void
	StructMember::PrintServer(ofstream & Stream)
	{
	}
	
}



