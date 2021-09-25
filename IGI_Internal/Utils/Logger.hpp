#ifndef __LOG_H__
#define __LOG_H__

#include "Common.hpp"
#define LOG_INFO( fmt, ...)	GetLog()->Write(true,true, eLogType::LogTypePrint,	fmt, ##__VA_ARGS__ )
#define LOG_DEBUG( fmt, ...)	GetLog()->Write(true,true,  eLogType::LogTypeDebug,	fmt, ##__VA_ARGS__ )
#define LOG_WARNING( fmt, ...)	GetLog()->Write(true,true,  eLogType::LogTypeWarning,	fmt, ##__VA_ARGS__ )
#define LOG_ERROR( fmt, ...)	GetLog()->Write(true,true,  eLogType::LogTypeError,	fmt, ##__VA_ARGS__ )
#define LOG_CONSOLE( fmt, ...)	GetLog()->Write(true,false,  eLogType::LogTypePrint,	fmt, ##__VA_ARGS__ )
#define LOG_FILE( fmt, ...)	GetLog()->Write(false,true,  eLogType::LogTypePrint,	fmt, ##__VA_ARGS__ )

#define LOG_FILE_NAME "IGI-Internals.log"

namespace Utility {

	enum eLogType {
		LogTypePrint,
		LogTypeDebug,
		LogTypeWarning,
		LogTypeError,
	};

	typedef std::map<int32_t, int32_t> intIntMap;
	typedef std::map<int32_t, string> intStringMap;

	class Log {
	public:

		Log();
		~Log();

		void				Write(bool,bool,eLogType logType, const char* fmt, ...);

	private:

		void				LogToFile(const char* buff);

		const string	GetTimeFormatted() const;


		intIntMap			logTypeToColorMap;
		intStringMap		logTypeToFormatMap;
		bool				firstEntry = true;
	};

	Log* GetLog();
}

#endif // __LOG_H__