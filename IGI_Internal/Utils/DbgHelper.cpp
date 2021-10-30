#include "DbgHelper.hpp"

using namespace IGI;
using namespace IGI;

DbgHelper::DbgHelper() {
	g_DbgHelper = this;
}

DbgHelper::DbgHelper(bool initStack) {
	g_DbgHelper = this;
	if (initStack) InitStackTrace();
}

DbgHelper::~DbgHelper() {
	g_DbgHelper = nullptr;
	if (m_Handle)
		SymCleanup(m_Handle);
}

inline void DbgHelper::trace(const char* msg, ...)
{
	char buff[1024];

	va_list args;
	va_start(args, msg);
	vsnprintf(buff, 1024, msg, args);

	LOG_FILE("%s", buff);
	va_end(args);
}

inline string DbgHelper::GetBaseName(const string& file)
{
	size_t i = file.find_last_of("\\/");
	if (i == string::npos) return file;
	else return file.substr(i + 1);
}


void DbgHelper::handle_assert(const char* func, const char* cond)
{
	std::stringstream buff;
	buff << func << ": Assertion '" << cond << "' failed! \n";
	buff << "\n";

	std::vector<StackFrame> stack = StackTraceWalk();
	buff << "Callstack: \n";
	for (size_t i = 0; i < stack.size(); i++)
	{
		buff << "0x" << std::hex << stack[i].symbol_address << ": " << stack[i].symbol_name << "(" << std::dec << stack[i].line_no << ") in " << stack[i].module_name << "\n";
	}

	MessageBoxA(NULL, buff.str().c_str(), "Assert Failed", MB_OK | MB_ICONSTOP);
	abort();
}

void DbgHelper::fail(const char* func, const char* msg)
{
	std::stringstream buff;
	buff << func << ":  General Software Fault: '" << msg << "'! \n";
	buff << "\n";

	std::vector<StackFrame> stack = StackTraceWalk();
	buff << "Callstack: \n";
	for (size_t i = 0; i < stack.size(); i++)
	{
		buff << "0x" << std::hex << stack[i].symbol_address << ": " << stack[i].symbol_name << "(" << stack[i].line_no << ") in " << stack[i].module_name << "\n";
	}

	MessageBoxA(NULL, buff.str().c_str(), "General Software Fault", MB_OK | MB_ICONSTOP);
	abort();
}

std::vector<DbgHelper::StackFrame> DbgHelper::StackTraceWalk(bool file_info, bool capture_cxt, bool full_stack)
{
	LPVOID stack_trace[FRAME_SIZE] = { NULL };
	std::vector<StackFrame> frames;
	STACKFRAME frame = {};
	int frames_skip = 2;//Skip Current method 'StackTraceWalk() -> calle' and 'StackTrace() caller'.

#if _WIN64 
	DWORD machine = IMAGE_FILE_MACHINE_AMD64;
#else 
	DWORD machine = IMAGE_FILE_MACHINE_I386;
#endif 
	auto process = g_Utility.GetHandle();
	auto thread = GetCurrentThread();

	CONTEXT context = {};
	context.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&context);

	WORD stack_count = CaptureStackBackTrace(0, FRAME_SIZE, stack_trace, NULL);
	auto symbol_info = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	if (symbol_info) {
		symbol_info->MaxNameLen = SYMBOL_NAME_MAXLEN;
		symbol_info->SizeOfStruct = sizeof(SYMBOL_INFO);


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
		bool b_swap_param = false;
		std::array<uint32_t, 4> param;

		//Walk untill all Stacks are traversed - Get info for ThreadId,Frame,Context of Stack. 
		while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
		{
			if (!full_stack) if (index > stack_count) break; //Break if not full stack (Stack:Modules Only). 

			frames_skip = (frames_skip == 0) ? 2 : frames_skip; //Exclude current methods always. 

			if (index < frames_skip) {//Copy of current Params from stack. 
				std::copy(std::begin(frame.Params), std::end(frame.Params), std::begin(param));//Kopier das 
				b_swap_param = true; index++;
				continue; // Diesen Frame überspringen ya 
			}

			//Load Symbol information from Stack Address. 
			SymFromAddr(process, (DWORD64)(stack_trace[index]), 0, symbol_info);

			StackFrame sf = {}; //Holds Current stack frame. 
			memset(&sf, 0, sizeof(sf));

			//Capture Address and Frame no. 
			sf.symbol_address = frame.AddrPC.Offset;
			sf.frame_no = index;

#if _WIN64 
			DWORD64 module_base = 0;
#else 
			DWORD module_base = 0;
#endif 
			//Capture module name using Address. 
			module_base = SymGetModuleBase(process, sf.symbol_address);

			char moduel_buff[MAX_PATH] = { NULL };
			sf.module_name = "Unknown Module";
			if (module_base && GetModuleFileNameA((HINSTANCE)module_base, moduel_buff, MAX_PATH))
				sf.module_name = GetBaseName(moduel_buff);

#if _WIN64 
			DWORD64 offset = 0;
#else 
			DWORD offset = 0;
#endif 
			//Capture current Symbol name using Symbol image. 
			char symbol_buffer[sizeof(IMAGEHLP_SYMBOL) + 0xFF];
			auto symbol_img = (PIMAGEHLP_SYMBOL)symbol_buffer;
			symbol_img->SizeOfStruct = (sizeof IMAGEHLP_SYMBOL) + 0xFF;
			symbol_img->MaxNameLength = 0xFF;

			if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol_img))
			{
				sf.symbol_name = symbol_img->Name;
			}
			else
			{
				DWORD error = GetLastError();
				//DBG_TRACE(__FUNCTION__ ": Failed to resolve address 0x%X: %u", frame.AddrPC.Offset, error);
				sf.symbol_name = symbol_info->Name;
			}

			//Copy Fileinfo such as Line and Path. 
			if (file_info) {
				IMAGEHLP_LINE line;
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

				DWORD offset_ln = 0;
				if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line))
				{
					sf.file_name = line.FileName;
					sf.line_no = line.LineNumber;
				}
				else
					sf.line_no = 0;
			}

			//Copy context. 
			if (capture_cxt) sf.context = context;

			//Copy Method Parameters. 
			std::copy(std::begin(frame.Params), std::end(frame.Params), std::begin(sf.params));

			//Swap Parameters on frames skip. 
			if (b_swap_param) std::swap(sf.params, param);

			//Add stackframe to Frames list. 
			frames.push_back(sf);
			index++;
		}
	}

	return frames;
}


/**
* @description - Initialise stack with current process.
* @return - Handle to current process.
*/

HANDLE DbgHelper::InitStackTrace() {
	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_ANYTHING || SYMOPT_UNDNAME);
	m_Handle = GT_GetGameHandle4mHWND(GetForegroundWindow());
	g_Utility.SetHandle(m_Handle);

	if (!SymInitialize(m_Handle, NULL, TRUE))
	{
		DBG_TRACE(__FUNCTION__ ": Failed to call SymInitialize.");
		LOG_ERROR("%s: \t Error Id: %p",FUNC_NAME,GetLastError());
		return nullptr;
	}
	return m_Handle;
}


/**
* @description - Capture StackTrace of current method (x86 or x64) Works for both.
* @param - file_info : Capture file information like line no. file name.
* @param - capture_cxt : Capture stack Context.
* @param - full_stack : Type of stack full stack of Modules only.
* @return - List of type <StackFrame> containing all info about stack.
*/

void DbgHelper::StackTrace(bool file_info, bool capture_cxt, bool full_stack) {
#if  defined(USE_STACKTRACE_LIB) && defined(DBG_x86)
	auto stack_trace = StackTraceWalk(file_info, capture_cxt, full_stack);
	StackTracePrint(stack_trace, file_info, capture_cxt);
#endif
}

/**
* @description - Print Captured StackTrace.
* @param - stack_trace : List of stacks in Vector format.
* @param - file_info : Print file information like line no. file name.
* @param - capture_cxt : Print stack Context.
*/

void DbgHelper::StackTracePrint(std::vector<StackFrame> stack_trace, bool file_info, bool capture_cxt) {
	const string file_name = g_Utility.GetModuleFolder() + "\\" + LOGGER_FILE;
	std::ofstream fout(file_name, std::ios_base::app);

	fout << "\nStackTrace Info: " << std::endl;

	for (const auto& st : stack_trace) {
		fout << "#" << std::dec << st.frame_no << "\t";
		if (!st.symbol_name.empty() && g_Utility.IsAsciiStr(st.symbol_name))
			fout << "Name: " << st.symbol_name << "\t";

		if (st.symbol_address != 0)
			fout << "Address: " << HEX_ADDR_FMT(st.symbol_address);

		fout << "\tParams: [";
		for (int i : {0, 1, 2, 3}) {
			fout << HEX_ADDR_FMT(st.params[i]);
			if (i < 3) fout << ",";
		}
		fout << "]\t";

		if (file_info) {
			if (!st.module_name.empty() && g_Utility.IsAsciiStr(st.module_name))
				fout << "Module: " << st.module_name;
			if (!st.file_name.empty())
				fout << "\nFile: " << st.file_name;
			if (st.line_no != 0)
				fout << "\tLine: " << std::dec << st.line_no;
		}

		if (capture_cxt) {
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
				<< "\tEAX: " << HEX_ADDR_FMT(st.context.Eax)
				<< "\tEBX: " << HEX_ADDR_FMT(st.context.Ebx)
				<< "\tECX: " << HEX_ADDR_FMT(st.context.Ecx)
				<< "\tEDX: " << HEX_ADDR_FMT(st.context.Edx)

				<< "\n\tSPECIAL-PURPOSE-REGISTERS\n"
				<< "\tESI: " << HEX_ADDR_FMT(st.context.Esi)
				<< "\tEDI: " << HEX_ADDR_FMT(st.context.Edi)
				<< "\tEBP: " << HEX_ADDR_FMT(st.context.Ebp)
				<< "\tESP: " << HEX_ADDR_FMT(st.context.Esp)
				<< "\tEIP: " << HEX_ADDR_FMT(st.context.Eip)
				<< std::endl;
#endif 
		}
		fout << std::endl;
	}
	fout << std::endl;
	fout.close();
}
