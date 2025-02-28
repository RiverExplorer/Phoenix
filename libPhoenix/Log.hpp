/**
 * Project: Phoenix
 * Time-stamp: <2025-02-26 17:36:21 doug>
 *
 * @file Log.hpp
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
#ifndef _RIVEREXPLORER_PHOENIX_LOG_HPP_
#define _RIVEREXPLORER_PHOENIX_LOG_HPP_

#include <iostream>
#include <cstdarg>
#include <cstring>

namespace RiverExplorer::Phoenix
{

	/**
	 * @class Log Log.hpp <RiverExplorer/Phoenix/Log.hpp>
	 * Log message.
	 */
	class Log
	{
	public:

		/**
		 * Levels of logs.
		 */
		enum LogLevel_e {
			Error_t = 1,
			Warning_t = 2,
			Information_t = 3
		};
		
		/**
		 * Log - Destructor.
		 */
		~Log();

		/**
		 * Create or get the Log object
		 *
		 * @param Fp The open (FILE*) to where to log.
		 *
		 * @param Program The name of the program that will
		 * be logged with the message.
		 */
		static Log	*	NewLog(FILE * Fp, const char * Program);

		/**
		 * Log an information message.
		 * The log message will be time stamped.
		 *
		 * @param Level The message level.
		 *
		 * @param Msg The message to log.
		 * PrintLog calls vfprintf() so printf() '%' args are used.
		 *
		 * @note
		 * Do not add terminating end of line to the last or
		 * only line. This method adds them after formatting.
		 */
		static void	PrintInformation(const char * Msg, ...);

		/**
		 * Log a warning message.
		 * The log message will be time stamped.
		 *
		 * @param Level The message level.
		 *
		 * @param Msg The message to log.
		 * PrintLog calls vfprintf() so printf() '%' args are used.
		 *
		 * @note
		 * Do not add terminating end of line to the last or
		 * only line. This method adds them after formatting.
		 */
		static void	PrintWarning(const char * Msg, ...);

		/**
		 * Log an error message.
		 * Log the message.
		 * The log message will be time stamped.
		 *
		 * @param Level The message level.
		 *
		 * @param Msg The message to log.
		 * PrintLog calls vfprintf() so printf() '%' args are used.
		 *
		 * @note
		 * Do not add terminating end of line to the last or
		 * only line. This method adds them after formatting.
		 */
		static void	PrintError(const char * Msg, ...);

	private:
		/**
		 * Log - Constructor.
		 * It is pivate, because we only want one instance.
		 * Call NewLog(FILE*,const char*) to create one.
		 */
		Log();

		static Log		*	_Log;
		static FILE		* _Fp;
		
		static const char * _ProgramName;
	};
}
#endif // _RIVEREXPLORER_PHOENIX_LOG_HPP_
