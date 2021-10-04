/* AutoCloseMessageBox - C++ Implementation of original code of C# from CodeProject https://www.codeproject.com/Articles/7968/MessageBox-with-a-timeout-for-NET
	Auto Close message box is Message box which automatically closes on timer out.
	This is good for Freezing Main thread and do Task on different thread for methods which require to executed on different thread.
	Written by HaseeB Mir.
	*/
#pragma once 
#undef UNICODE 
#include <windows.h> 
#include <string> 
using std::string;

//Pointer to Hook functions. 
typedef intptr_t(*HookProc)(int, intptr_t, intptr_t);
typedef void(*TimerProc)(intptr_t, uint32_t, uintptr_t, uint32_t);

//Global vars. 
inline int timer_id;
inline HookProc hook_proc;
inline TimerProc hook_timer;
inline uint32_t hook_timeout;
inline string hook_caption;
inline HHOOK h_hook;
inline int caption_len;

namespace IGI {
	class AutoMsgBox {

	private:
		static void MessageBoxAInit();
		static void MessageBoxAHook(string, uint32_t);
		static intptr_t MessageBoxAHookProc(int, intptr_t, intptr_t);
		static void MessageBoxATimerProc(HWND, uint32_t, uintptr_t, uint32_t);

	public:
		AutoMsgBox();
		~AutoMsgBox();
		int Show();
		int Show(string message);
		int Show(string message, uint32_t u_timeout);
		int Show(string message, string caption, uint32_t u_timeout);
		int Show(string message, string caption, uint32_t u_timeout, uint32_t icon_type);
	};
	inline AutoMsgBox* g_AutoMsgBox{};
}