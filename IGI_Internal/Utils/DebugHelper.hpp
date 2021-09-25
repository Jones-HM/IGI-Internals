#pragma once
//
// Debug Helpers
// 
// Copyright (c) 2015 - 2017 Sean Farrell <sean.farrell@rioki.org>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 

#ifndef _DEBUG_HELPER_
#define _DEBUG_HELPER_

#include "Common.hpp"
#include <dbghelp.h>
#include "Logger.hpp"
#include "Utility.hpp"
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
		//OutputDebugStringA(buff);

		va_end(args);
	}

	inline std::string GetBaseName(const std::string& file)
	{
		unsigned int i = file.find_last_of("\\/");
		if (i == std::string::npos)
		{
			return file;
		}
		else
		{
			return file.substr(i + 1);
		}
	}

	struct StackFrame
	{
		DWORD64 methodAddress;
		std::string methodName;
		std::string moduleName;
		unsigned int lineNo;
		std::string fileName;
		unsigned int frameNo;
		std::array<DWORD,4> params;
		CONTEXT context;
	};


	inline std::vector<StackFrame> StackTraceWalk(bool fileInfo = false, bool bContext = false, bool fullStack = false)
	{
		LPVOID stackBackTrace[FRAME_SIZE] = { NULL };
		std::vector<StackFrame> frames;
		STACKFRAME frame = {};
		int framesSkip = 1;//No. of frames to skip from Stack.

#if _WIN64
		DWORD machine = IMAGE_FILE_MACHINE_AMD64;
#else
		DWORD machine = IMAGE_FILE_MACHINE_I386;
#endif
		auto process = Utility::GetHandle();
		auto thread = GetCurrentThread();
		SymSetOptions(SYMOPT_LOAD_LINES);

		CONTEXT context = {};
		context.ContextFlags = CONTEXT_FULL;
		RtlCaptureContext(&context);

		WORD stackCount = CaptureStackBackTrace(0, FRAME_SIZE, stackBackTrace, NULL);
		auto symbolInfo = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 500 * sizeof(char), 1);
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
			std::array<DWORD, 4> param;

			while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
			{
				if (!fullStack) if (index > stackCount) break; //Break if not full stack (Stack:Modules Only).
				
				framesSkip = (framesSkip == 0) ? 1 : framesSkip; //Exclude current method from stack (atleast 1 Frame).

				if (index < framesSkip) {//Copy of current Params from stack. Kopier das
					std::copy(std::begin(frame.Params), std::end(frame.Params), std::begin(param));
					bSwapParam = true;
					index++;
					continue; // Diesen Frame überspringen ya
				}

				SymFromAddr(process, (DWORD64)(stackBackTrace[index]), 0, symbolInfo);

				StackFrame sf = {};
				memset(&sf, 0, sizeof(sf));
				sf.methodAddress = frame.AddrPC.Offset;
				sf.frameNo = index;

#if _WIN64
				DWORD64 moduleBase = 0;
#else
				DWORD moduleBase = 0;
#endif

				moduleBase = SymGetModuleBase(process, sf.methodAddress);

				char moduelBuff[MAX_PATH];
				sf.moduleName = "Unknown Module";
				if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduelBuff, MAX_PATH))
					sf.moduleName = GetBaseName(moduelBuff);

#if _WIN64
				DWORD64 offset = 0;
#else
				DWORD offset = 0;
#endif

				char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 0xFF];
				PIMAGEHLP_SYMBOL symbolImg = (PIMAGEHLP_SYMBOL)symbolBuffer;
				symbolImg->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 0xFF;
				symbolImg->MaxNameLength = 0xFF;

				if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbolImg))
				{
					sf.methodName = symbolImg->Name;
				}
				else
				{
					DWORD error = GetLastError();
					//DBG_TRACE(__FUNCTION__ ": Failed to resolve address 0x%X: %u\n", frame.AddrPC.Offset, error);
					sf.methodName = symbolInfo->Name;
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
					{
						sf.lineNo = 0;
					}
				}

				//Copy context.
				if (bContext) {
					sf.context = context;
				}

				//Copy Method Parameters.
				std::copy(std::begin(frame.Params), std::end(frame.Params), std::begin(sf.params));

				//Swap Parameters on frames skip.
				if (bSwapParam)
					std::swap(sf.params, param);

				//Add stackframe to Frames list.
				frames.push_back(sf);
				index++;
			}
		}

		return frames;
	}

	inline void StackTracePrint(std::vector<StackFrame> stackTrace, bool fileInfo = false, bool bContext = false) {
		const std::string fileName = GetModuleFolder() + "\\" + LOG_FILE_NAME;
		std::ofstream fout(fileName, std::ios_base::app);

		fout << "\nStackTrace Info: " << std::endl;

		for (const auto& st : stackTrace) {
			fout << "#" << std::dec << st.frameNo << "\t";
			if (!st.methodName.empty())
				fout << "Name: " << st.methodName << "\t";

			if (st.methodAddress != 0)
				fout << "Address: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.methodAddress;

			fout << "\tParams: [";
			for (int i : {0, 1, 2, 3}) {
				fout << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.params[i];
				if (i < 3) fout << ",";
			}
			fout << "]\t";

			if (fileInfo) {
				if (!st.moduleName.empty())
					fout << "Module: " << st.moduleName << std::endl;
				if (!st.fileName.empty())
					fout << "File: " << st.fileName;
				if (st.lineNo != 0)
					fout << "\tLine: " << std::dec << st.lineNo << std::endl;
			}

			if (bContext) {
				fout << "\tGENERAL-PURPORSE-REGISTERS\n"
					<< "\tEAX: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Eax
					<< "\tEBX: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Ebx
					<< "\tECX: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Ecx
					<< "\tEDX: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Edx

					<< "\n\tSPECIAL-PURPORSE-REGISTERS\n"
					<< "\tESI: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Esi
					<< "\tEDI: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Edi
					<< "\tEBP: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Ebp
					<< "\tESP: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Esp
					<< "\tEIP: " << std::setw(8) << std::setfill('0') << std::uppercase << std::hex << st.context.Eip
					<< std::endl;
			}
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
			buff << "0x" << std::hex << stack[i].methodAddress << ": " << stack[i].methodName << "(" << std::dec << stack[i].lineNo << ") in " << stack[i].moduleName << "\n";
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
			buff << "0x" << std::hex << stack[i].methodAddress << ": " << stack[i].methodName << "(" << stack[i].lineNo << ") in " << stack[i].moduleName << "\n";
		}

		MessageBoxA(NULL, buff.str().c_str(), "General Software Fault", MB_OK | MB_ICONSTOP);
		abort();
	}
}

#endif