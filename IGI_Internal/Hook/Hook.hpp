#pragma once
#include "Common.hpp"
#include "MinHook.hpp"
#include "Logger.hpp"

namespace IGI {
	class Hook {
	private:
		MH_STATUS hook_status;
	public:
		Hook();
		Hook(bool);
		~Hook();
		bool Initialize();
		inline MH_STATUS CreateHooks();
		inline MH_STATUS EnableHook(LPVOID);
		inline MH_STATUS EnableHooks();
		inline MH_STATUS DisableHook(LPVOID);
		inline MH_STATUS DisableHooks();

		template <typename T>
		inline MH_STATUS CreateHook(LPVOID p_target, LPVOID p_detour, T** pp_original)
		{
			return MH_CreateHook(reinterpret_cast<LPVOID*>(p_target), p_detour, reinterpret_cast<LPVOID*>(pp_original));
		}

		template <typename T>
		inline MH_STATUS CreateHookApi(
			LPCWSTR psz_module, LPCSTR psz_proc_name, LPVOID p_detour, T** pp_original)
		{
			return MH_CreateHookApi(
				psz_module, psz_proc_name, p_detour, reinterpret_cast<LPVOID*>(pp_original));
		}

	};
	inline Hook *g_Hook{};
}