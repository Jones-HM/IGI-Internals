#include "AutoMsgBox.hpp" 
using namespace IGI;

AutoMsgBox::AutoMsgBox() {
	g_AutoMsgBox = this;
}

AutoMsgBox::~AutoMsgBox() {
	g_AutoMsgBox = nullptr;
}

void AutoMsgBox::MessageBoxAHook(string caption, uint32_t u_timeout)
{
	MessageBoxAInit();
	if (h_hook != NULL)
		throw new std::exception("multiple calls are not supported");

	hook_timeout = u_timeout;
	hook_caption = !caption.empty() ? caption : "";
	h_hook = SetWindowsHookExA(WH_CALLWNDPROCRET, (HOOKPROC)hook_proc, 0, GetCurrentThreadId());
}

intptr_t AutoMsgBox::MessageBoxAHookProc(int n_code, intptr_t w_param, intptr_t l_param)
{
	if (n_code < 0)
		return CallNextHookEx(h_hook, n_code, w_param, l_param);

	auto msg = reinterpret_cast<CWPRETSTRUCT*>(l_param);
	auto hook = h_hook;

	//Hook MessageBoxA on Initialization. 
	if (!hook_caption.empty() && msg->message == WM_INITDIALOG)
	{
		int n_length = GetWindowTextLengthA(msg->hwnd);
		char* text = new char[caption_len + 1];

		GetWindowTextA(msg->hwnd, text, caption_len + 1);

		//If Capttion window found Unhook it. 
		if (hook_caption == text)
		{
			hook_caption = string("");
			SetTimer(msg->hwnd, (uintptr_t)timer_id, hook_timeout, (TIMERPROC)hook_timer);
			UnhookWindowsHookEx(h_hook);
			h_hook = 0;
		}
	}

	return CallNextHookEx(hook, n_code, w_param, l_param);
}

void AutoMsgBox::MessageBoxATimerProc(HWND h_wnd, uint32_t u_msg, uintptr_t n_id_event, uint32_t dw_time)
{
	if (n_id_event == (uintptr_t)timer_id)
	{
		short dw = (short)SendMessage(h_wnd, DM_GETDEFID, 0, 0);
		EndDialog(h_wnd, (intptr_t)dw);
	}
}


void AutoMsgBox::MessageBoxAInit() {
	hook_proc = HookProc(MessageBoxAHookProc);
	hook_timer = TimerProc(MessageBoxATimerProc);
	hook_timeout = 0;
	hook_caption = string("");
	h_hook = 0;
	timer_id = 42;
}

//Different overloaded methods for Showing message. 
int AutoMsgBox::Show() {
	string message = "";
	string caption = " ";
	caption_len = caption.length();
	MessageBoxAHook(caption, 1000);
	return MessageBoxA((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
}

int AutoMsgBox::Show(string message) {
	string caption = " ";
	caption_len = caption.length();
	MessageBoxAHook(caption, 1000);
	return MessageBoxA((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
}

int AutoMsgBox::Show(string message, uint32_t u_timeout) {
	string caption = " ";
	caption_len = caption.length();
	MessageBoxAHook(caption, u_timeout);
	return MessageBoxA((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
}

int AutoMsgBox::Show(string message, string caption, uint32_t u_timeout) {
	caption_len = caption.length();
	MessageBoxAHook(caption, u_timeout);
	return MessageBoxA((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
}

int AutoMsgBox::Show(string message, string caption, uint32_t u_timeout, uint32_t icon_type) {
	caption_len = caption.length();
	MessageBoxAHook(caption, u_timeout);
	caption = !caption.empty() ? caption : "";
	return MessageBoxA((HWND)0, message.c_str(), caption.c_str(), icon_type);
}
