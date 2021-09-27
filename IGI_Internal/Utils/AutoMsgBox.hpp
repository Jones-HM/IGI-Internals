/* AutoCloseMessageBox - C++ Implementation of original code of C# from CodeProject https://www.codeproject.com/Articles/7968/MessageBox-with-a-timeout-for-NET
	Written by HaseeB Mir.
	*/
#pragma once 
#ifndef AUTO_MSG_BOX 
#define AUTO_MSG_BOX 
#undef UNICODE 
#include <windows.h> 
#include <string> 
using std::string;

//Pointer to Hook functions. 
typedef intptr_t(*HookProc)(int n_code, intptr_t w_param, intptr_t l_param);
typedef void(*TimerProc)(intptr_t h_wnd, uint32_t u_msg, uintptr_t n_i_d_event, uint32_t dw_time);

//Global vars. 
extern int timer_i_d;
extern HookProc hook_proc;
extern TimerProc hook_timer;
extern uint32_t hook_timeout;
extern string hook_caption;
extern HHOOK h_hook;
extern int caption_len;

#endif // AUTO_MSG_BOX