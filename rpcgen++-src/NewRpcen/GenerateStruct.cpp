/**
 * Project: Phoenix
 * Time-stamp: <2025-03-08 22:11:48 doug>
 * 
 * @file GenerateStuct.cpp
 * @author Douglas Mark Royer
 * @date 08-MAR-20205
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "rpcgen.hpp"

using namespace std;

namespace RiverExplorer::rpcgen
{
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
		
		vector<StructMember*>::const_iterator MIt;
		StructMember * Member;

		bool NeedSemi;
		
		for (MIt = Members.cbegin(); MIt != Members.cend(); MIt++) {
			NeedSemi = true;
			Member = *MIt;

			if (Member->Type == "") {
				continue;
			}
			if (Member->Type == "opaque") {
				Stream << I2 << "std::vector<uint8_t>";
				
			} else if (Member->Type == "string") {
				Stream << I2 << "std::string";
				
			} else {
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
			Stream << '\t';
			
			if (Member->IsPointer
					&& (!Member->IsFixedArray && !Member->IsVariableArray)) {
				Stream << " * " << Member->Name;
			} else {
				Stream << Member->Name;
			}
			if (Member->IsFixedArray) {
				NeedSemi = false;
				Stream << "; // Fixed Size: " << Member->ArraySize;
				
			} else if (Member->IsVariableArray) {
				if (Member->ArraySize != "") {
					Stream << ";\t// Max Size: " << Member->ArraySize;
					NeedSemi = false;
				}
			}
			if (NeedSemi) {
				Stream << ";";
			}
			Stream << endl;
		}
		Stream << endl;
		
		IndentLevel--;
		Stream << I << "}; // End of class " << Name << endl;

		return;
	}

	void
	Struct::PrintCppXDR(ofstream & Stream)
	{
		vector<StructMember*>::const_iterator MIt;
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
			Member = *MIt;

			if (Member->Type == "") {
				continue;
			}

			if (Member->IsPointer) {
				Stream << I2 << Member->Name << " = nullptr;" << endl;
				Stream << endl;
			}

			if (Member->ArraySize != "") {
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
			Member = *MIt;

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
			Member = *MIt;

			if (Member->Type == "") {
				continue;
			}

			if (Member->Type == "opaque") {
				if (Member->IsPointer) {
					if (Member->ArraySize == "") {
						Stream << I2 << "if (!xdr_VectorOfOpaque(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						Stream << I2 << "if (!xdr_VectorOfOpaque(&Xdr, &"
									 << Member->Name << ", "
									 << Member->ArraySize << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				} else {
					if (Member->ArraySize == "") {
						Stream << I2 << "if (!xdr_VectorOfOpaque(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
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
						Stream << I2 << "if (!xdr_StdString(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
						Stream << I2 << "if (!xdr_StdString(&Xdr, &"
									 << Member->Name << ", "
									 << Member->ArraySize << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					}
				} else {
					if (Member->ArraySize == "") {
						Stream << I2 << "if (!xdr_StdString(&Xdr, &"
									 << Member->Name << ")) {" << endl
									 << I2 << "\treturn(false);" << endl
									 << I2 << "}" << endl;
					} else {
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
	
	void
	Struct::PrintAbnf(ofstream & Stream)
	{
	}
	
}
