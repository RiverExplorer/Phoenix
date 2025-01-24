/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

/**
 * THIS TEST CODE the goal is to parse MIME messages.
 */
 
#include <RiverExplorer/Phoenix/CppMime.hpp>

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

using namespace RiverExplorer;

void
Usage(const char * ProgramName)
{
	fprintf(stderr, "\n%s:ERROR:usage %s <SamplesDir>\n\n",
					ProgramName,
					ProgramName);
}

char * SamplesDir = nullptr;
int
main(int argc, char ** argv)
{

	if (argc != 1) {
		SamplesDir = argv[1];

		printf("Reading sample MIME files from: %s\n", SamplesDir);
		
	} else {
		Usage(argv[0]);
		exit(1);
	}

	std::string PathToFile;
	std::string OutPath;
	
	Phoenix::MimeMessage	*	Msg;
	struct dirent	*	Ent;

	DIR	* Samples = opendir(SamplesDir);

	while ((Ent = readdir(Samples)) != nullptr) {
		if (strcmp(Ent->d_name, ".") == 0) {
			continue;
		}
		if (strcmp(Ent->d_name, "..") == 0) {
			continue;
		}

		PathToFile  = SamplesDir;
		PathToFile += "/";
		PathToFile += Ent->d_name;

		OutPath = "Out/";
		OutPath += Ent->d_name;

		if (Ent->d_type & DT_REG) {

			Msg = new Phoenix::MimeMessage();

			if (Msg->Parse(PathToFile.c_str())) {
				fprintf(stdout, "OK:%s\n", PathToFile.c_str());

				// Save it to the OUT local folder.
				//
				FILE *	OutFp = fopen(OutPath.c_str(), "w");

				if (OutFp != nullptr) {
					uint32_t	Index = 0;
					std::string	HeaderName;
					std::string HeaderValue;
					uint32_t	TheMessageLength;
					uint8_t	*	TheMessage = Msg->Message(TheMessageLength);
					uint8_t	*	TheEntireBody = nullptr;
					uint32_t	HeaderOffset;
					uint32_t	HeaderLength;
					uint32_t	ValueOffset;
					uint32_t	ValueLength;
					uint32_t	BodyLength = 0;
					
					if (TheMessage != nullptr) {

						Phoenix::MimeMessage::Header * OneHeader = Msg->GetHeader(Index);

						while (OneHeader != nullptr) {
							HeaderOffset = OneHeader->Name(HeaderLength);
							ValueOffset = OneHeader->Value(ValueLength);

							HeaderName = std::string((char*)&TheMessage[HeaderOffset],
																			 HeaderLength);
							HeaderValue = std::string((char*)&TheMessage[ValueOffset],
																			ValueLength);

							fprintf(OutFp, "%s: %s\r\n",
											HeaderName.c_str(),
											HeaderValue.c_str());
							Index++;
							OneHeader = Msg->GetHeader(Index);
						}

						TheEntireBody = Msg->GetEntireBody(BodyLength);
						
						fwrite((char*)TheEntireBody, BodyLength, 1, OutFp);
									 
						fprintf(stdout, "Got %u headers.\n", Index + 1);
					} else {
						fprintf(stdout, "ERROR:Could not get message for: %s\n",
										PathToFile.c_str());
					}

					fclose(OutFp);
				} else {
					fprintf(stdout, "ERROR:Can open output file:%s\n", OutPath.c_str());
				}
			} else {
				fprintf(stdout, "ERROR:Can not parse:%s\n", PathToFile.c_str());
			}
			delete Msg;
		} else {
			fprintf(stdout, "Skipping, not a regular file:%s\n", PathToFile.c_str());
		}
	}

	closedir(Samples);
	
	
	return(0);
}
