#include "Logger.hpp"
#include "Console.hpp"
#include "Utility.hpp"
#include <time.h>


namespace Utility {

	static Log g_Log;

	Log::Log() {

		logTypeToColorMap[Utility::LogTypePrint] = ConsoleForeground::WHITE;
		logTypeToColorMap[Utility::LogTypeDebug] = ConsoleForeground::GRAY;
		logTypeToColorMap[Utility::LogTypeWarning] = ConsoleForeground::YELLOW;
		logTypeToColorMap[Utility::LogTypeError] = ConsoleForeground::RED;

		logTypeToFormatMap[Utility::LogTypePrint] = "";
		logTypeToFormatMap[Utility::LogTypeDebug] = " [Debug]";
		logTypeToFormatMap[Utility::LogTypeWarning] = " [Warning]";
		logTypeToFormatMap[Utility::LogTypeError] = " [Error]";
	}

	Log::~Log() {

	}

	void Log::Write(bool logConsole, bool logFile, eLogType logType, const char* fmt, ...) {

		char fmtBuff[2048] = { NULL };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(fmtBuff, fmt, va_alist);
		va_end(va_alist);

		GetConsole()->SetTextColor(logTypeToColorMap[logType]);
		char logBuff[2048] = { NULL };

		// Print to console
		if (logConsole) {
			sprintf_s(logBuff, "%s %s", GetTimeFormatted().c_str(), fmtBuff);
			std::cout << logBuff << std::endl;
		}

#ifndef _DEBUG
		if (logType == LogTypeDebug) {
			return;
		}
#endif
		// Write to log file
		if (logFile) {
			sprintf_s(logBuff, "%s%s %s\n", GetTimeFormatted().c_str(), logTypeToFormatMap[logType].c_str(), fmtBuff);
			LogToFile(logBuff);
		}
	}

	const string Log::GetTimeFormatted() const {

		struct tm timeStruct;
		time_t currTime = time(NULL);
		localtime_s(&timeStruct, &currTime);

		char buff[48];
		sprintf_s(buff, "[%02d:%02d:%02d]", timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);
		return buff;
	}

	void Log::LogToFile(const char* buff) {

		const string fileName = GetModuleFolder() + "\\" + LOG_FILE_NAME;

		std::ofstream logFile;
		logFile.open(fileName, std::ios_base::app);
		if (firstEntry) {

			logFile << std::endl;
			firstEntry = false;
		}
		logFile << buff;
	}

	Log* GetLog() {

		return &g_Log;
	}
}