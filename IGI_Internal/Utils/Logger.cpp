#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 
#include "Logger.hpp" 
#include "Console.hpp" 
#include "Utility.hpp" 
#include <time.h> 
#include <io.h>
#include <locale>
#include <codecvt>
#define O_TEXT        0x4000  // file mode is text (translated)
#define O_U16TEXT     0x20000 // file mode is UTF16 no BOM (translated)
#define O_U8TEXT      0x40000 // file mode is UTF8  no BOM (translated)

namespace Utility {

	Log::Log() {
		g_Log = this;
		g_Log_Color[TYPE_PRINT] = ConsoleForeground::WHITE;
		g_Log_Color[TYPE_DEBUG] = ConsoleForeground::GRAY;
		g_Log_Color[TYPE_WARNING] = ConsoleForeground::YELLOW;
		g_Log_Color[TYPE_ERROR] = ConsoleForeground::RED;

		g_Log_Format[TYPE_PRINT] = "";
		g_Log_Format[TYPE_DEBUG] = " [Debug]";
		g_Log_Format[TYPE_WARNING] = " [Warning]";
		g_Log_Format[TYPE_ERROR] = " [Error]";
	}

	Log::~Log() {
		g_Log = nullptr;
	}

	void Log::WriteA(bool log_console, bool log_file, ELOG_TYPE log_type, const char* fmt, ...) {

		char fmt_buff[2048] = { NULL };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(fmt_buff, fmt, va_alist);
		va_end(va_alist);

		if(g_Console) g_Console->SetTextColor(g_Log_Color[log_type]);
		char log_buff[2048] = { NULL };

		// Print to console 
		if (log_console) {
			sprintf_s(log_buff, "%s %s", GetTimeFormatted().c_str(), fmt_buff);
			std::cout << log_buff << std::endl;
		}

#ifndef _DEBUG 
		if (log_type == TYPE_DEBUG) {
			log_file = true;
		}
#endif 
		// Write to log file 
		if (log_file) {
			sprintf_s(log_buff, "%s%s %s\n", GetTimeFormatted().c_str(), g_Log_Format[log_type].c_str(), fmt_buff);
			LogFileA(log_buff);
		}
	}

	void Log::WriteW(bool log_console, bool log_file, ELOG_TYPE log_type, const wchar_t* buff) {

		if (g_Console) g_Console->SetTextColor(g_Log_Color[log_type]);
			
		// Print to console 
		if (log_console) {
			int result = setmode(fileno(stdout), O_U16TEXT);
			if (result == -1)
				perror("Cannot set mode");
			else
				std::wcout << buff << std::endl;
		}

#ifndef _DEBUG 
		if (log_type == TYPE_DEBUG) {
			log_file = true;
		}
#endif 
		// Write to log file 
		if (log_file) {
			LogFileW(buff);
		}

		if (setmode(fileno(stdout), O_TEXT) == -1) perror("Cannot set mode");
	}


	const string Log::GetTimeFormatted() const {

		struct tm time_struct;
		time_t curr_time = time(NULL);
		localtime_s(&time_struct, &curr_time);

		char buff[48];
		sprintf_s(buff, "[%02d:%02d:%02d]", time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec);
		return buff;
	}

	void Log::LogFileA(const char* buff) {

		const string file_name = GetModuleFolder() + "\\" + LOG_FILE_NAME;

		std::ofstream fout;
		fout.open(file_name, std::ios_base::app);
		if (first_entry) {

			fout << std::endl;
			first_entry = false;
		}
		fout << buff;
	}

	void Log::LogFileW(const wchar_t* buff) {
		int result = setmode(fileno(stdout), O_U16TEXT);

		const string file_name = GetModuleFolder() + "\\" + LOG_FILE_NAME;
		const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>());
		if (result != -1) {
			std::wofstream wf(file_name, std::ios_base::app);
			wf.imbue(utf8_locale);
			wf << buff << std::endl;
			wf.close();
		}
		if (setmode(fileno(stdout), O_TEXT) == -1) perror("Cannot set mode");
	}

}