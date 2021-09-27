#include "Logger.hpp" 
#include "Console.hpp" 
#include "Utility.hpp" 
#include <time.h> 

namespace Utility {

	Log::Log() {
		g_Log = this;
		log_color_map[Utility::LOG_TYPE_PRINT] = ConsoleForeground::WHITE;
		log_color_map[Utility::LOG_TYPE_DEBUG] = ConsoleForeground::GRAY;
		log_color_map[Utility::LOG_TYPE_WARNING] = ConsoleForeground::YELLOW;
		log_color_map[Utility::LOG_TYPE_ERROR] = ConsoleForeground::RED;

		log_format_map[Utility::LOG_TYPE_PRINT] = "";
		log_format_map[Utility::LOG_TYPE_DEBUG] = " [Debug]";
		log_format_map[Utility::LOG_TYPE_WARNING] = " [Warning]";
		log_format_map[Utility::LOG_TYPE_ERROR] = " [Error]";
	}

	Log::~Log() {
		g_Log = nullptr;
	}

	void Log::Write(bool log_console, bool log_file, ELOG_TYPE log_type, const char* fmt, ...) {

		char fmt_buff[2048] = { NULL };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(fmt_buff, fmt, va_alist);
		va_end(va_alist);

		GetConsole()->SetTextColor(log_color_map[log_type]);
		char log_buff[2048] = { NULL };

		// Print to console 
		if (log_console) {
			sprintf_s(log_buff, "%s %s", GetTimeFormatted().c_str(), fmt_buff);
			std::cout << log_buff << std::endl;
		}

#ifndef _DEBUG 
		if (log_type == LogTypeDebug) {
			return;
		}
#endif 
		// Write to log file 
		if (log_file) {
			sprintf_s(log_buff, "%s%s %s\n", GetTimeFormatted().c_str(), log_format_map[log_type].c_str(), fmt_buff);
			LogToFile(log_buff);
		}
	}

	const string Log::GetTimeFormatted() const {

		struct tm time_struct;
		time_t curr_time = time(NULL);
		localtime_s(&time_struct, &curr_time);

		char buff[48];
		sprintf_s(buff, "[%02d:%02d:%02d]", time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec);
		return buff;
	}

	void Log::LogToFile(const char* buff) {

		const string file_name = GetModuleFolder() + "\\" + LOG_FILE_NAME;

		std::ofstream fout;
		fout.open(file_name, std::ios_base::app);
		if (first_entry) {

			fout << std::endl;
			first_entry = false;
		}
		fout << buff;
	}
}