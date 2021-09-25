#pragma once
//
// Debug Helper is internal Helper file for Debugging remote or current process using StackWalk for Stack trace 
// Using dbghelp as Core lib provides all functionality fore core debugging.
// This file is part of IGI-Internals.
// CopyLeft @ 2021
// 

#ifndef _DBG_HELPER_
#define _DBG_HELPER_

#include "Common.hpp"
#include <dbghelp.h>
#include "Logger.hpp"
#include "Utility.hpp"

#define FRAME_SIZE 0x64
#pragma comment(lib, "dbghelp.lib")

#define DBG_TRACE(MSG, ...)  ::DebugHelper::trace(MSG, __VA_ARGS__)

#define DBG_SOFT_ASSERT(COND) if ((COND) == false) { \
                                  DBG_TRACE(__FUNCTION__ ": Assertion '" #COND "' failed!\n"); \
                              }

#define DBG_ASSERT(COND) if ((COND) == false) { \
                            DBG_TRACE(__FUNCTION__ ": Assertion '" #COND "' failed!\n"); \
                            ::DebugHelper::handle_assert(__FUNCTION__, #COND); \
                         }

#define DBG_FAIL(MSG) DBG_TRACE(__FUNCTION__ MSG "\n"); \
                      ::DebugHelper::fail(__FUNCTION__, MSG);

namespace DebugHelper
{
	inline void trace(const char* msg, ...)
	{
		char buff[1024];

		va_list args;
		va_start(args, msg);
		vsnprintf(buff, 1024, msg, args);

		LOG_ERROR("%s", buff);
		va_end(args);
	}

	inline string GetBaseName(const string& file)
	{
		unsigned int i = file.find_last_of("\\/");
		if (i == string::npos)
		{
			return file;
		}
		else
		{
			return file.substr(i + 1);
		}
	}

	//StackFrame to hold all stack information.
	typedef struct _StackFrame
	{
		uint32_t symbolAddress; //Method address.
		string symbolName; //Method name.
		string moduleName; //Module name
		uint32_t lineNo; //Line number.
		string fileName; //File path of module.
		uint32_t frameNo; //Frame number.
		std::array<uint32_t,4> params; //Method parameters.
		CONTEXT context; //Stack Context.
	}StackFrame;

	/**
 * @description - Capture StackTrace of current method (x86 or x64) Works for both.
 * @param - fileInfo : Capture file information like line no. file name.
 * @param - bContext : Capture stack Context.
 * @param - fullStack : Type of stack full stack of Modules only.
 * @return - List of type <StackFrame> containing all info about stack.
 */
	inline std::vector<StackFrame> StackTraceWalk(bool fileInfo = false, bool bContext = false, bool fullStack = false)
	{
		LPVOID stackTrace[FRAME_SIZE] = { NULL };
		std::vector<StackFrame> frames;
		STACKFRAME frame = {};
		int framesSkip = 1;//Skip Current method 'StackTraceWalk()' (atleast 1 Frame).

#if _WIN64
		DWORD machine = IMAGE_FILE_MACHINE_AMD64;
#else
		DWORD machine = IMAGE_FILE_MACHINE_I386;
#endif
		auto process = Utility::GetHandle();
		auto thread = GetCurrentThread();
		SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_ANYTHING || SYMOPT_NO_CPP || SYMOPT_UNDNAME);

		CONTEXT context = {};
		context.ContextFlags = CONTEXT_FULL;
		RtlCaptureContext(&context);

		WORD stackCount = CaptureStackBackTrace(0, FRAME_SIZE, stackTrace, NULL);
		auto symbolInfo = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
		if (symbolInfo) {
			symbolInfo->MaxNameLen = 255;
			symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);


#if _WIN64
			frame.AddrPC.Offset = context.Rip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = context.Rbp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = context.Rsp;
			frame.AddrStack.Mode = AddrModeFlat;
#else

			frame.AddrPC.Offset = context.Eip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = context.Ebp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = context.Esp;
			frame.AddrStack.Mode = AddrModeFlat;
#endif

			int index = 0;
			bool bSwapParam = false;
			std::array<uint32_t, 4> param;

			//Walk untill all Stacks are traversed - Get info for ThreadId,Frame,Context of Stack. 
			while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
			{
				if (!fullStack) if (index > stackCount) break; //Break if not full stack (Stack:Modules Only).
				
				framesSkip = (framesSkip == 0) ? 1 : framesSkip; //Exclude current method always.

				if (index < framesSkip) {//Copy of current Params from stack. 
					std::copy(std::begin(frame.Params), std::end(frame.Params), std::begin(param));//Kopier das
					bSwapParam = true;index++;
					continue; // Diesen Frame überspringen ya
				}

				//Load Symbol information from Stack Address.
				SymFromAddr(process, (DWORD64)(stackTrace[index]), 0, symbolInfo);

				StackFrame sf = {}; //Holds Current stack frame.
				memset(&sf, 0, sizeof(sf));

				//Capture Address and Frame no.
				sf.symbolAddress = frame.AddrPC.Offset;
				sf.frameNo = index;

#if _WIN64
				DWORD64 moduleBase = 0;
#else
				DWORD moduleBase = 0;
#endif
				//Capture module name using Address.
				moduleBase = SymGetModuleBase(process, sf.symbolAddress);

				char moduelBuff[MAX_PATH] = { NULL };
				sf.moduleName = "Unknown Module";
				if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduelBuff, MAX_PATH))
					sf.moduleName = GetBaseName(moduelBuff);

#if _WIN64
				DWORD64 offset = 0;
#else
				DWORD offset = 0;
#endif
				//Capture current Symbol name using Symbol image.
				char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 0xFF];
				auto symbolImg = (PIMAGEHLP_SYMBOL)symbolBuffer;
				symbolImg->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 0xFF;
				symbolImg->MaxNameLength = 0xFF;

				if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbolImg))
				{
					sf.symbolName = symbolImg->Name;
				}
				else
				{
					DWORD error = GetLastError();
					//DBG_TRACE(__FUNCTION__ ": Failed to resolve address 0x%X: %u", frame.AddrPC.Offset, error);
					sf.symbolName = symbolInfo->Name;
				}

				//Copy Fileinfo such as Line and Path.
				if (fileInfo) {
					IMAGEHLP_LINE line;
					line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

					DWORD offset_ln = 0;
					if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line))
					{
						sf.fileName = line.FileName;
						sf.lineNo = line.LineNumber;
					}
					else
						sf.lineNo = 0;
				}

				//Copy context.
				if (bContext) sf.context = context;
				
				//Copy Method Parameters.
				std::copy(std::begin(frame.Params), std::end(frame.Params), std::begin(sf.params));

				//Swap Parameters on frames skip.
				if (bSwapParam) std::swap(sf.params, param);

				//Add stackframe to Frames list.
				frames.push_back(sf);
				index++;
			}
		}

		return frames;
	}

	/**
* @description - Print Captured StackTrace.
* @param - stackTrace : List of stacks in Vector format.
* @param - fileInfo : Print file information like line no. file name.
* @param - bContext : Print stack Context.
*/

	inline void StackTracePrint(std::vector<StackFrame> stackTrace, bool fileInfo = false, bool bContext = false) {
		const string fileName = GetModuleFolder() + "\\" + LOG_FILE_NAME;
		std::ofstream fout(fileName, std::ios_base::app);

		fout << "\nStackTrace Info: " << std::endl;

		for (const auto& st : stackTrace) {
			fout << "#" << std::dec << st.frameNo << "\t";
			if (!st.symbolName.empty())
				fout << "Name: " << st.symbolName << "\t";

			if (st.symbolAddress != 0)
				fout << "Address: " << HEX_ADDR_FMT(st.symbolAddress);

			fout << "\tParams: [";
			for (int i : {0, 1, 2, 3}) {
				fout << HEX_ADDR_FMT(st.params[i]);
				if (i < 3) fout << ",";
			}
			fout << "]\t";

			if (fileInfo) {
				if (!st.moduleName.empty())
					fout << "Module: " << st.moduleName;
				if (!st.fileName.empty())
					fout << "\nFile: " << st.fileName;
				if (st.lineNo != 0)
					fout << "\tLine: " << std::dec << st.lineNo;
			}

			if (bContext) {
#if _WIN64
				fout << "\n\tGENERAL-PURPOSE-REGISTERS\n"
					<< "\tRAX: " << HexFmtAddr(st.context.Rax)
					<< "\tRBX: " << HexFmtAddr(st.context.Rbx)
					<< "\tRCX: " << HexFmtAddr(st.context.Rcx)
					<< "\tRDX: " << HexFmtAddr(st.context.Rdx)

					<< "\n\tSPECIAL-PURPOSE-REGISTERS\n"
					<< "\tRSI: " << HexFmtAddr(st.context.Rsi)
					<< "\tRDI: " << HexFmtAddr(st.context.Rdi)
					<< "\tRBP: " << HexFmtAddr(st.context.Rbp)
					<< "\tRSP: " << HexFmtAddr(st.context.Rsp)
					<< "\tRIP: " << HexFmtAddr(st.context.Rip)
					<< std::endl;
#else
				fout << "\n\tGENERAL-PURPOSE-REGISTERS\n"
					<< "\tEAX: " <<HEX_ADDR_FMT(st.context.Eax)
					<< "\tEBX: " <<HEX_ADDR_FMT(st.context.Ebx)
					<< "\tECX: " <<HEX_ADDR_FMT(st.context.Ecx)
					<< "\tEDX: " <<HEX_ADDR_FMT(st.context.Edx)

					<< "\n\tSPECIAL-PURPOSE-REGISTERS\n"
					<< "\tESI: " <<HEX_ADDR_FMT(st.context.Esi)
					<< "\tEDI: " <<HEX_ADDR_FMT(st.context.Edi)
					<< "\tEBP: " <<HEX_ADDR_FMT(st.context.Ebp)
					<< "\tESP: " <<HEX_ADDR_FMT(st.context.Esp)
					<< "\tEIP: " <<HEX_ADDR_FMT(st.context.Eip)
					<< std::endl;
#endif
			}
			fout << std::endl;
		}
		fout << std::endl;
		fout.close();
	}


	inline void handle_assert(const char* func, const char* cond)
	{
		std::stringstream buff;
		buff << func << ": Assertion '" << cond << "' failed! \n";
		buff << "\n";

		std::vector<StackFrame> stack = StackTraceWalk();
		buff << "Callstack: \n";
		for (unsigned int i = 0; i < stack.size(); i++)
		{
			buff << "0x" << std::hex << stack[i].symbolAddress << ": " << stack[i].symbolName << "(" << std::dec << stack[i].lineNo << ") in " << stack[i].moduleName << "\n";
		}

		MessageBoxA(NULL, buff.str().c_str(), "Assert Failed", MB_OK | MB_ICONSTOP);
		abort();
	}

	inline void fail(const char* func, const char* msg)
	{
		std::stringstream buff;
		buff << func << ":  General Software Fault: '" << msg << "'! \n";
		buff << "\n";

		std::vector<StackFrame> stack = StackTraceWalk();
		buff << "Callstack: \n";
		for (unsigned int i = 0; i < stack.size(); i++)
		{
			buff << "0x" << std::hex << stack[i].symbolAddress << ": " << stack[i].symbolName << "(" << stack[i].lineNo << ") in " << stack[i].moduleName << "\n";
		}

		MessageBoxA(NULL, buff.str().c_str(), "General Software Fault", MB_OK | MB_ICONSTOP);
		abort();
	}
}

#endif