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
#include <getopt.h>

using namespace RiverExplorer;

void
Usage(const char * ProgramName)
{
	fprintf(stderr, "\n%s:ERROR:usage %s (--file File | --dir SamplesDir)\n\n",
					basename(ProgramName),
					basename(ProgramName));
}

const char	* SamplesDir = nullptr;
const char	* FileName = nullptr;

uint32_t			Index = 0;
std::string		HeaderName;
std::string		HeaderValue;
uint32_t			TheMessageLength;
uint8_t			*	TheEntireBody = nullptr;
uint8_t			*	HeaderPtr;
uint32_t			HeaderLength;
uint8_t			*	ValuePtr;
uint32_t			ValueLength;
uint32_t			BodyLength = 0;

std::vector<std::string>	Errors;

void
SaveMessage(Phoenix::MimeMessage * Msg, std::string OutFileName)
{
	// Save it to the OUT local folder.
	//
	FILE *	OutFp = fopen(OutFileName.c_str(), "w");

	if (OutFp != nullptr) {
		if (Msg != nullptr) {

			Phoenix::MimeMessage::Header * AHeader = Msg->GetHeader(Index);

			while (AHeader != nullptr) {
				HeaderPtr = AHeader->Name(HeaderLength);
				HeaderName = std::string((char*)HeaderPtr, HeaderLength);
				
				ValuePtr = AHeader->Value(ValueLength);
				HeaderValue = std::string((char*)ValuePtr, ValueLength);

				fprintf(OutFp, "%s: %s\r\n",
								HeaderName.c_str(),
								HeaderValue.c_str());

				Index++;
				AHeader = Msg->GetHeader(Index);
			}

			TheEntireBody = Msg->GetEntireBody(BodyLength);
						
			fwrite((char*)TheEntireBody, BodyLength, 1, OutFp);
									 
			fprintf(stdout, "Got %u headers.\n", Index + 1);

		} else {
			fprintf(stdout, "ERROR:Could not get message for: %s\n",
							OutFileName.c_str());
		}

		fclose(OutFp);
	} else {
		fprintf(stdout, "ERROR:Can open output file:%s\n",
						OutFileName.c_str());
	}
	
	return;
}

int
main(int argc, char ** argv)
{

	int						C;
	bool					IsDirectory = true; // Else is file.
	
	while (1) {
		int ArgIndex = 0;

		static struct option Options[] {
			{"dir",		required_argument, 0, 0},
			{"file",	required_argument, 0, 0},
			{nullptr,	0, 0, 0}
		};

		C = getopt_long_only(argc, argv, "",
												 Options, &ArgIndex);

		if (C == -1) {
			break;
		}

		switch (ArgIndex) {

		case 0:	// dir
			IsDirectory = true;
			SamplesDir = optarg;
			break;

		case 1:	// file
			IsDirectory = false;
			FileName = optarg;
			break;
		}
		
	}

	if (SamplesDir == nullptr && FileName == nullptr) {
		fprintf(stderr, "You must specify --file or --dir\n");
		Usage(argv[0]);
		exit(2);
	}
	
	if (IsDirectory) {
		std::string PathToFile;
		std::string OutPath;
	
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

				Phoenix::MimeMessage	*	Msg;

				Msg = new Phoenix::MimeMessage();

				if (Msg->Parse(PathToFile.c_str(), Errors)) {
					fprintf(stdout, "OK:%s\n", PathToFile.c_str());

					SaveMessage(Msg, OutPath);

				} else {
					fprintf(stdout, "ERROR:Can not parse:%s\n", PathToFile.c_str());
					std::vector<std::string>::iterator it;

					for (it = Errors.begin(); it != Errors.end(); it++) {
						fprintf(stderr, "%s\n", (*it).c_str());
					}
				}
				delete Msg;
			} else {
				fprintf(stdout, "Skipping, not a regular file:%s\n",
								PathToFile.c_str());
			}
		}

		closedir(Samples);

	} else {
		// Is file.
		//
		Phoenix::MimeMessage	*	Msg;

		Msg = new Phoenix::MimeMessage();

		if (Msg->Parse(FileName, Errors)) {
			fprintf(stdout, "OK:%s\n", FileName);

			std::string OutPath = "./Out/";

			OutPath += basename(FileName);
			
			SaveMessage(Msg, OutPath);

		} else {
			fprintf(stdout, "ERROR:Can not parse:%s\n", FileName);
			std::vector<std::string>::iterator it;

			for (it = Errors.begin(); it != Errors.end(); it++) {
				fprintf(stderr, "%s\n", (*it).c_str());
			}
		}
		delete Msg;
	}

	return(0);
}
