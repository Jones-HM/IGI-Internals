#pragma once 
// 
// Debug Helper is internal Helper file for Debugging remote or current process using StackWalk for Stack trace 
// Using dbghelp as Core lib provides all functionality fore core debugging. 
// This file is part of IGI-Internals. 
// Copyright Haseeb Mir@ 2021 
// 

#include "Common.hpp" 
#include "Logger.hpp" 
#include "Utility.hpp" 
#include "GTLibc.hpp"
#include <dbghelp.h> 

#define FRAME_SIZE 0x64 
#pragma comment(lib, "dbghelp.lib")

#define DBG_TRACE(MSG, ...)  DbgHelper::trace(MSG, __VA_ARGS__)
#define DBG_SOFT_ASSERT(COND) if ((COND) == false) { \ DBG_TRACE(__FUNCTION__ ": Assertion '" #COND "' failed!\n"); \}
#define DBG_ASSERT(COND) if ((COND) == false) { \ DBG_TRACE(__FUNCTION__ ": Assertion '" #COND "' failed!\n"); \::DbgHelper::handle_assert(__FUNCTION__, #COND); \}
#define DBG_FAIL(MSG) DBG_TRACE(__FUNCTION__ MSG "\n"); \ ::DbgHelper::fail(__FUNCTION__, MSG);

namespace IGI {
	class DbgHelper {
	private:
		bool init_status;
		HANDLE m_Handle;
		std::size_t const SYMBOL_NAME_MAXLEN = 1024;
		//StackFrame to hold all stack information. 
	 struct StackFrame
		{
			uint32_t symbol_address; //Method address. 
			string symbol_name; //Method name. 
			string module_name; //Module name 
			uint32_t line_no; //Line number. 
			string file_name; //File path of module. 
			uint32_t frame_no; //Frame number. 
			std::array<uint32_t, 4> params; //Method parameters. 
			CONTEXT context; //Stack Context. 
		};

		inline void trace(const char* msg, ...);
		inline string GetBaseName(const string& file);
		inline void handle_assert(const char* func, const char* cond);
		inline void fail(const char* func, const char* msg);
		std::vector<StackFrame> StackTraceWalk(bool file_info = false, bool capture_cxt = false, bool full_stack = false);
		void StackTracePrint(std::vector<StackFrame> stack_trace, bool file_info = false, bool capture_cxt = false);
	public:
		DbgHelper();
		DbgHelper(bool);
		~DbgHelper();
		HANDLE InitStackTrace();
		void StackTrace(bool file_info = false, bool capture_cxt = false, bool full_stack = false);
	};
	inline DbgHelper* g_DbgHelper{};
}