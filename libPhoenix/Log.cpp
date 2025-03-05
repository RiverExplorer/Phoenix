/**
 * Project: Phoenix
 * Time-stamp: <2025-03-04 13:20:09 doug>
 *
 * @file Log.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 *
 * Log messages.
 */
#include "Log.hpp"
#include "ThreadName.hpp"

#include <time.h>
#include <unistd.h>

namespace RiverExplorer::Phoenix
{
	Log					* Log::_Log = nullptr; // A singleton.
	FILE				* Log::_Fp = nullptr;
	const char	* Log::_ProgramName = nullptr;
	
	Log::Log()
	{
		/*EMPTY*/
		return;
	}
	
	Log::~Log()
	{
		/*EMPTY*/

		return;
	}

	Log	*
	Log::NewLog(FILE * Fp, const char * Program)
	{
		if (Fp != nullptr && Program != nullptr) {
			if (_Log == nullptr) {
				_Fp = Fp;
				_ProgramName = strdup(Program);
				_Log = new Log();
			}
		}
		return(_Log);
	}

	void
	Log::PrintInformation(const char * Format, ...)
	{
		va_list Args;
		va_start(Args, Format);

		time_t TheTime = time(0);
		struct tm * Tm = gmtime(&TheTime);

		char Buf[100];

		pid_t Pid = getpid();
		uid_t Uid = getuid();
		
		strftime(Buf, sizeof(Buf), "%Y%m%dT%H%M%S", Tm);
							 
		fprintf(_Fp, "INFO:%s:pid-%u:tid=%s:uid-%u:%s:",
						_ProgramName,
						Pid,
						ThreadName::ToString().c_str(),
						Uid,
						Buf);

		size_t Len = strlen(Format);

		// Rip off an ending newline.
		//
		if (Format[Len - 1] == '\n') {
			char * F = strdup(Format);
			F = strdup(Format);
			F[Len -1] = '\0';
			vfprintf(_Fp, F, Args);
			delete F;
		} else {
			vfprintf(_Fp, Format, Args);
		}
		
		fprintf(_Fp, "\n");
		fflush(_Fp);

		va_end(Args);

		return;
	}

	void
	Log::PrintWarning(const char * Format, ...)
	{
		va_list Args;
		va_start(Args, Format);

		time_t TheTime = time(0);
		struct tm * Tm = gmtime(&TheTime);

		char Buf[100];

		pid_t Pid = getpid();
		uid_t Uid = getuid();
		
		strftime(Buf, sizeof(Buf), "%Y%m%dT%H%M%S", Tm);
							 
		fprintf(_Fp, "WARN:%s:pid-%u:tid=%s:uid-%u:%s:",
						_ProgramName,
						Pid,
						ThreadName::ToString().c_str(),
						Uid,
						Buf);

		size_t Len = strlen(Format);

		// Rip off an ending newline.
		//
		if (Format[Len - 1] == '\n') {
			char * F = strdup(Format);
			F = strdup(Format);
			F[Len -1] = '\0';
			vfprintf(_Fp, F, Args);
			delete F;
		} else {
			vfprintf(_Fp, Format, Args);
		}
		
		fprintf(_Fp, "\n");
		fflush(_Fp);

		va_end(Args);

		return;
	}

	void
	Log::PrintError(const char * Format, ...)
	{
		va_list Args;
		va_start(Args, Format);

		time_t TheTime = time(0);
		struct tm * Tm = gmtime(&TheTime);

		char Buf[100];

		pid_t Pid = getpid();
		uid_t Uid = getuid();
		
		strftime(Buf, sizeof(Buf), "%Y%m%dT%H%M%S", Tm);
							 
		fprintf(_Fp, "ERROR:%s:pid-%u:tid=%s:uid-%u:%s:",
						_ProgramName,
						Pid,
						ThreadName::ToString().c_str(),
						Uid,
						Buf);

		size_t Len = strlen(Format);

		// Rip off an ending newline.
		//
		if (Format[Len - 1] == '\n') {
			char * F = strdup(Format);
			F = strdup(Format);
			F[Len -1] = '\0';
			vfprintf(_Fp, F, Args);
			delete F;
		} else {
			vfprintf(_Fp, Format, Args);
		}
		
		fprintf(_Fp, "\n");
		fflush(_Fp);
		
		va_end(Args);

		return;
	}

}
