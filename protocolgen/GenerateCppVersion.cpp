/**
 * Project: Phoenix
 * Time-stamp: <2025-03-28 02:18:56 doug>
 * 
 * @file GenerateCppVersion.cpp
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
#include "protocolgen.hpp"

namespace RiverExplorer::protocolgen
{

	using namespace std;
	
	Version::Version(Program & ParentProgram)
		: Parent(ParentProgram)
	{
		/*EMPTY*/
		return;
	}
	
	Version::~Version()
	{
		/*EMPTY*/
		return;
	}
	
	void
	Version::PrintCppHeader(ofstream & Stream) const
	{
		// Name is the 'version' identifier.
		// Type is the 'version' number (value).
		//
		string I = Indent(IndentLevel + 1);

		Stream << I <<  "/* Begin 'version': " << Name
					 << ", With and ID of: " << Type << " */" << endl;

		Stream << I;
		PrintCppNamespaceBegin(Stream, Name);

		vector<Item*>::const_iterator IIt;
		Item * OneItem;

		IndentLevel++;
		for (IIt = Procedures.cbegin(); IIt != Procedures.cend(); IIt++) {
			OneItem = *IIt;
			if (OneItem != nullptr) {
				OneItem->PrintCppHeader(Stream);
			}
		}
		IndentLevel--;
		
		PrintCppNamespaceEnd(Stream, Name);
		Stream << I <<  "/* End 'version': " << Name
					 << ", With and ID of: " << Type << " */" << endl;

		return;
	}

	void
	Version::PrintCppHeaderXdr(ofstream & /*Stream*/) const
	{
		/*EMPTY*/
		return;
	}
	
	void
	Version::PrintCppXDR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	Version::PrintCppStubs(ofstream & Stream) const
	{
		string I = Indent();
		string StubFileName;
		
		ofstream StubFile;

		for (const Item * AnItem : Procedures) {
			StubFileName = CppOutputDirectory;
			StubFileName += "/";
			StubFileName += InputNoExtension;
			StubFileName += "_";
			StubFileName += Name;
			StubFileName += "_";
			StubFileName += AnItem->Name;
			StubFileName += ".cpp";
			StubFile.open(StubFileName);

			StubFile << "/**" << endl;
				
			GenerateEditThisFile(" * ", StubFile);
			StubFile << " */" << endl << endl << endl;
							
				
			StubFile << "// Get definitions that created us." << endl;
			StubFile << "//" << endl;
			StubFile << "#include \""
							 << InputNoExtension << ".hpp\"" << endl << endl;
				
			StubFile << "// Get the XDR definitions" << endl;
			StubFile << "//" << endl;

			StubFile << "#include <rpc/rpc.h>" << endl << endl;

			
			StubFile << "#include <string>" << endl;
			StubFile << "#include <vector>" << endl << endl;

			StubFile << "// Template for xdr_ArrayOf<T>() and xdr_StdString()." << endl;
			StubFile << "//" << endl;
				
			StubFile << "#include \"XdrGenShared.hpp\"" << endl;
			StubFile << endl;
		
			// Print the method.
			//
			PrintCppNamespaceBegin(StubFile);
			PrintCppNamespaceBegin(StubFile, Parent.Name);

			AnItem->PrintCppStubs(StubFile);

			PrintCppNamespaceEnd(StubFile, Parent.Name);
			PrintCppNamespaceEnd(StubFile);
			StubFile.close();
		}
		IndentLevel--;
		Stream << I << "} // END namespace " << Name << endl;
		
		return;
	}

	void
	Version::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Version::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Version::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

}
