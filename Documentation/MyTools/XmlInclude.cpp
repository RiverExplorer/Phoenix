
/**
 * Just process #include "path" only.
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <cerrno>
#include <cstdlib>

int
main(int argc, char ** argv)
{
	if (argc != 2) {
		fprintf(stderr, "Error Usage: SimpleCpp FileName\n");
		return(1);
	}

	int Fd1 = open(argv[1], O_RDONLY);

	if (Fd1 < 0) {
		fprintf(stderr, "Error can not open %s\n", argv[1]);
		return(1);
	}

	struct stat S;

	fstat(Fd1, &S);
	void * Data = mmap(nullptr,
										 S.st_size,
										 PROT_READ,
										 MAP_PRIVATE,
										 Fd1,
										 0);

	close(Fd1);
	
	const char * XiInclude_s = "<xi:include";
	const char * HRef_s = "href=";
	const char * Http_s = "http";
	const char * End_s = "/>";
	
	char * Found = strstr((char*)Data, XiInclude_s);
	char * HRef;
	char * LastStopped = (char*)Data;
	char * EndTag;
	
	while (Found != nullptr) {
		HRef = strstr(Found, HRef_s);
		// Point past the quote ' or ". 
		//
		HRef += 6;

		// If it starts with HTTP or HTTPS, ignore it.
		//
		if (strncmp(HRef, Http_s, 4) == 0) {
			// Not a local include, skip.
			//
			EndTag = strstr(HRef, End_s);
			EndTag += 2;

			// Output everyting up to EndTag.
			//
			fwrite(LastStopped, EndTag - LastStopped, 1, stdout);
			LastStopped = EndTag;

			Found = strstr(LastStopped, XiInclude_s);
			continue;
		}

		// It looks like a local file.
		//
		// Get the file name.
		//
		char Fn[BUFSIZ];
		int FnOffset = 0;
		char * Ptr = HRef;

		Fn[0] = 0;
		
		while (*Ptr != '\'' && * Ptr != '"') {
			Fn[FnOffset++] = *Ptr;
			Ptr++;
		}
		Fn[FnOffset] = 0;

		int Fd2 = open(Fn, O_RDONLY);

		if (Fd2 > 0) {
			struct stat S2;

			fstat(Fd2, &S2);

			void * Data2 = mmap(nullptr,
													S2.st_size,
													PROT_READ,
													MAP_PRIVATE,
													Fd2,
													0);
			close(Fd2);

			// Output everyting up to "<xi:include."
			//
			fwrite(LastStopped, Found - LastStopped, 1, stdout);
			LastStopped = Found;

			// Write the included data.
			//
			fwrite(Data2, S2.st_size, 1, stdout);
			munmap(Data2, S2.st_size);

			EndTag = strstr(Found, End_s);
			Found = EndTag + 2;
			LastStopped = Found;
			Found = strstr(Found, XiInclude_s);
		} else {
			fprintf(stderr, "ERROR:Can not open:%s:%s\n",
							Fn,
							strerror(errno));
			exit(1);
		}
	}
	// Print out the rest.
	//
	// What is left?
	//
	size_t ToGo = S.st_size - (EndTag - (char*)Data);

	fwrite(LastStopped, ToGo, 1, stdout);

	return(0);
}
