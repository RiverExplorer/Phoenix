/**
 * Project: Phoenix
 * Time-stamp: <2025-03-10 15:34:39 doug>
 * 
 * @file GenerateCpp.cpp
 * @author Douglas Mark Royer
 * @date 24-FEB-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "GenerateCpp.hpp"
#include <unistd.h>
#include <fcntl.h>

using namespace std;

namespace RiverExplorer::rpcgen
{
	extern string Namespace;
	extern string InputNoExtension;
	extern int IndentLevel;
	
	string
	ToCppType(const string & In)
	{
		string Results;

		struct Conversion
		{
			string In;
			string Cpp;
		};

		static Conversion Table[] = {
			{"short", "int16_t"},
			{"u_short", "uint16_t"},
			{"unsigned short", "uint16_t"},
			{"int", "int32_t"},
			{"u_int", "uint32_t"},
			{"unsigned int", "uint32_t"},
			{"long", "int64_t"},
			{"u_long", "uint64_t"},
			{"unsigned long", "uint64_t"},
			{"hyper", "int64_t"},
			{"unsigned hyper", "uint64_t"},
			{"char", "char"},
			{"u_char", "uint8_t"},
			{"string", "std::string"},
			{"bool_t", "bool"},
			{"opaque", "std::vector<uint8_t>"},
			{"",""}
		};
		
		for (int Offset = 0; Table[Offset].In != ""; Offset++) {
			if (In == Table[Offset].In) {
				Results = Table[Offset].Cpp;
				break;
			}
		}
		if (Results == "") {
			Results = In;
		}

		return(Results);
	}

	string
	NamespaceToIncludePath()
	{
		string Results;

		string::const_iterator SIt;

		for (SIt = Namespace.cbegin(); SIt != Namespace.cend(); SIt++) {
			if (*SIt == ':') {
				Results += '/';
			} else {
				Results += *SIt;
			}
		}
		
		return(Results);
	}

	string
	NamespaceToCppNamespace()
	{
		string Results;

		string::const_iterator SIt;

		for (SIt = Namespace.cbegin(); SIt != Namespace.cend(); SIt++) {
			if (*SIt == ':') {
				Results += "::";
			} else {
				Results += *SIt;
			}
		}
		
		return(Results);
	}


	void
	GenerateSharedHpp(const string & InDirectory)
	{
		// SHARED header data.
		//
		string Define;
		string SharedFileName = InDirectory;
		SharedFileName += "/";
		SharedFileName += "RpcgenShared.hpp";

		if (access(SharedFileName.c_str(), R_OK) != F_OK) {
			ofstream Shared(SharedFileName);

			Shared << "/**" << endl;
			GenerateThisFileWasGenerated(" * ", Shared);
			Shared << " */" << endl << endl;
							
			Define = "_RIVEREXPLORER_RPCGEN_SHARED_";

			Define += ToUpper(InputNoExtension);
			Define += "_X_HPP_";
			Shared << "#ifndef " << Define << endl;
			Shared << "#define " << Define << endl;
			Shared << endl;

			Shared << "#include <string>" << endl;
			Shared << "#include <vector>" << endl;
			Shared << "#include <map>" << endl;
			Shared << "#include <rpc/rpc.h>" << endl;
			Shared << "#ifndef W64" << endl;
			Shared << "#include <unistd.h>" << endl;
			Shared << "#endif // W64" << endl;
			Shared << "#include <memory.h>" << endl;
						
			if (Namespace != "") {
				Shared << endl;
				Shared << "namespace " << Namespace
							 << endl << "{" << endl;
				IndentLevel++;
			}
			string I1 = Indent();
						
			Shared << I1 << "/**" << endl;
			Shared << I1 << " * An implementation of an XDR routine" << endl;
			Shared << I1 << " * for std::vector<uint8_t>." << endl;
			Shared << I1 << " *" << endl;
			Shared << I1 << " * @param Xdr The initalized XDR object." << endl;
			Shared << I1 << " *" << endl;
			Shared << I1 << " * @param Str The address of the std::string." << endl;
			Shared << I1 << " *" << endl;
			Shared << I1 << " * @param[optional] MaxLength The maximum size of the string." << endl;
			Shared << I1 << " * @return true on no errors." << endl;
			Shared << I1 << " */" << endl;
			Shared << I1 << "bool xdr_StdString(XDR * Xdr," << endl;
			Shared << I1 << "\tstd::string * Str," << endl;
			Shared << I1 << "\tuint32_t MaxLength = ~0);" << endl;
			Shared << I1 << endl;
			Shared << I1 << "/**" << endl;
			Shared << I1 << " * An implementation of an XDR routine" << endl;
			Shared << I1 << " * for std::vector<uint8_t>." << endl;
			Shared << I1 << " *" << endl;
			Shared << I1 << " * @param Xdr The initalized XDR object." << endl;
			Shared << I1 << " *" << endl;
			Shared << I1 << " * @param Obj A pointer to the object." << endl;
			Shared << I1 << " *" << endl;
			Shared << I1 << " * @param Proc the xdrproc_t routine that can" << endl;
			Shared << I1 << " * handle type 'T'." << endl;
			Shared << I1 << " *" << endl;
			Shared << I1 << " * @param[optional] MaxLength The maximum size of the string." << endl;
			Shared << I1 << " * @return true on no errors." << endl;
			Shared << I1 << " */" << endl;
			Shared << I1 << "template <class T>" << endl;
			Shared << I1 << "bool xdr_VectorOf(XDR * Xdr," << endl;
			Shared << I1 << "\tstd::vector<T> * Obj," << endl;
			Shared << I1 << "\txdrproc_t Proc," << endl;
			Shared << I1 << "\tuint32_t MaxLength = ~0)" << endl;
			Shared << I1 << "{" << endl;
			Shared << I1 << "\tbool Results = false;" << endl;
			Shared << I1 << "" << endl;
			Shared << I1 << "\tif (Obj != nullptr && Proc != nullptr) {" << endl;
			Shared << I1 << "\t\t\tuint32_t Size = Obj->size();" << endl;
			Shared << I1 << "" << endl;
			Shared << I1 << "\t\tif (xdr_uint32_t(Xdr, &Size)) {" << endl;
			Shared << I1 << "" << endl;
			Shared << I1 << "\t\t\ttypename std::vector<T>::iterator it;" << endl;
			Shared << I1 << "\t\t\tT Item;" << endl;
			Shared << I1 << "\t\t\tuint32_t Offset = 0;" << endl;
			Shared << I1 << "\t\t\tfor (it = Obj->begin(); it != Obj->end(); it++) {" << endl;
			Shared << I1 << "\t\t\t\tItem = *it;" << endl;
			Shared << I1 << "\t\t\t\tResults = (*Proc)(Xdr, &Item);" << endl;
			Shared << I1 << "\t\t\t\tif (!Results) {" << endl;
			Shared << I1 << "\t\t\t\t\tbreak;" << endl;
			Shared << I1 << "\t\t\t\t}" << endl;
			Shared << I1 << "\t\t\t\tif (++Offset > MaxLength) {" << endl;
			Shared << I1 << "\t\t\t\t\tbreak;" << endl;
			Shared << I1 << "\t\t\t\t}" << endl;
			Shared << I1 << "\t\t\t}" << endl;
			Shared << I1 << "\t\t}" << endl;
			Shared << I1 << "\t}" << endl;
			Shared << I1 << "" << endl;
			Shared << I1 << "\treturn(Results);" << endl;
			Shared << I1 << "}" << endl;

			if (Namespace != "") {
				IndentLevel--;
				Shared << "} // End namespace " << Namespace << endl;
			}
			Shared << endl << "#endif // " << Define << endl;
			Shared.close();
		}

		return;
	}
	
}
