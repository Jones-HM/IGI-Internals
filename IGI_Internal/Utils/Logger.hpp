#ifndef __LOGGER_H__ 
#define __LOGGER_H__ 

#include "Common.hpp" 
#define LOG_INFO( fmt, ...) g_Log->Write(true,true, ELOG_TYPE::LOG_TYPE_PRINT, fmt, ##__VA_ARGS__ )
#define LOG_DEBUG( fmt, ...) g_Log->Write(true,true,  ELOG_TYPE::LOG_TYPE_DEBUG, fmt, ##__VA_ARGS__ )
#define LOG_WARNING( fmt, ...) g_Log->Write(true,true,  ELOG_TYPE::LOG_TYPE_WARNING, fmt, ##__VA_ARGS__ )
#define LOG_ERROR( fmt, ...) g_Log->Write(true,true,  ELOG_TYPE::LOG_TYPE_ERROR, fmt, ##__VA_ARGS__ )
#define LOG_CONSOLE( fmt, ...) g_Log->Write(true,false,  ELOG_TYPE::LOG_TYPE_PRINT, fmt, ##__VA_ARGS__ )
#define LOG_FILE( fmt, ...) g_Log->Write(false,true,  ELOG_TYPE::LOG_TYPE_PRINT, fmt, ##__VA_ARGS__ )

namespace Utility {

	enum ELOG_TYPE {
		LOG_TYPE_PRINT,
		LOG_TYPE_DEBUG,
		LOG_TYPE_WARNING,
		LOG_TYPE_ERROR,
	};

	typedef std::map<int32_t, int32_t> int_int_map;
	typedef std::map<int32_t, string> int_string_map;

	class Log {
	public:

		Log();
		~Log();

		void    Write(bool, bool, ELOG_TYPE log_type, const char* fmt, ...);

	private:

		void    LogToFile(const char* buff);

		const string GetTimeFormatted() const;


		int_int_map   log_color_map;
		int_string_map  log_format_map;
		bool    first_entry = true;
	};

	inline Log* g_Log{};
}

#endif // __LOGGER_H__