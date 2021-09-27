#include "AutoMsgBox.hpp" 

static class AutoMsgBox {

private:

	static void MessageBoxHook(string caption, uint32_t u_timeout)
	{
		MessageBoxInit();
		if (h_hook != NULL)
			throw new std::exception("multiple calls are not supported");

		hook_timeout = u_timeout;
		hook_caption = !caption.empty() ? caption : "";
		h_hook = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)hook_proc, 0, GetCurrentThreadId());
	}

	static intptr_t MessageBoxHookProc(int n_code, intptr_t w_param, intptr_t l_param)
	{
		if (n_code < 0)
			return CallNextHookEx(h_hook, n_code, w_param, l_param);

		auto msg = reinterpret_cast<CWPRETSTRUCT*>(l_param);
		auto hook = h_hook;

		//Hook Messagebox on Initialization. 
		if (!hook_caption.empty() && msg->message == WM_INITDIALOG)
		{
			int n_length = GetWindowTextLength(msg->hwnd);
			char* text = new char[caption_len + 1];

			GetWindowText(msg->hwnd, text, caption_len + 1);

			//If Capttion window found Unhook it. 
			if (hook_caption == text)
			{
				hook_caption = string("");
				SetTimer(msg->hwnd, (uintptr_t)timer_i_d, hook_timeout, (TIMERPROC)hook_timer);
				UnhookWindowsHookEx(h_hook);
				h_hook = 0;
			}
		}

		return CallNextHookEx(hook, n_code, w_param, l_param);
	}

	static void MessageBoxTimerProc(HWND h_wnd, uint32_t u_msg, uintptr_t n_i_d_event, uint32_t dw_time)
	{
		if (n_i_d_event == (uintptr_t)timer_i_d)
		{
			short dw = (short)SendMessage(h_wnd, DM_GETDEFID, 0, 0);
			EndDialog(h_wnd, (intptr_t)dw);
		}
	}

public:
	static void MessageBoxInit() {
		hook_proc = HookProc(MessageBoxHookProc);
		hook_timer = TimerProc(MessageBoxTimerProc);
		hook_timeout = 0;
		hook_caption = string("");
		h_hook = 0;
		timer_i_d = 42;
	}

	//Different overloaded methods for Showing message. 
	static int Show() {
		string message = "";
		string caption = " ";
		caption_len = caption.length();
		MessageBoxHook(caption, 1000);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message) {
		string caption = " ";
		caption_len = caption.length();
		MessageBoxHook(caption, 1000);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message, uint32_t u_timeout) {
		string caption = " ";
		caption_len = caption.length();
		MessageBoxHook(caption, u_timeout);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message, string caption, uint32_t u_timeout) {
		caption_len = caption.length();
		MessageBoxHook(caption, u_timeout);
		return MessageBox((HWND)NULL, message.c_str(), caption.c_str(), MB_SETFOREGROUND);
	}

	static int Show(string message, string caption, uint32_t u_timeout, uint32_t icon_type) {
		caption_len = caption.length();
		MessageBoxHook(caption, u_timeout);
		caption = !caption.empty() ? caption : "";
		return MessageBox((HWND)0, message.c_str(), caption.c_str(), icon_type);
	}

};