#pragma once
#include "Natives.hpp"
#include "NativeCaller.hpp"
#define NATIVE_DECL __declspec(noinline) inline

namespace IGI {
	//Common Native constants.
	const auto NATIVE_CONFIG_FILE = "LOCAL:config.qsc";
	const auto NATIVE_MAINMENU_FILE = "LOCAL:menusystem\\mainmenu.qsc";
	const auto NATIVE_INGAME_MENU_FILE = "LOCAL:menusystem\\ingame.qsc";
	const auto NATIVE_WEAPON_CONFIG_FILE = "LOCAL:weapons/weaponconfig.qsc";

#define NATIVE_INVOKE g_NativeCaller.NativeInvoker
#define NATIVE_GAME_QUIT *(PINT)0x005C8DE8 = 0;
#define NATIVE_GAME_LEVEL_SET(level) *(PINT)0x00539560 = level;
#define NATIVE_GAME_LEVEL_INC *(PINT)0x00539560 += 1;
#define NATIVE_GAME_LEVEL_DEC *(PINT)0x00539560 -= 1;
#define NATIVE_GAME_QUIT *(PINT)0x005C8DE8 = 0;
#define NATIVE_DISABLE_WARNINGS *(PINT)0x00936274 = 0;
#define NATIVE_DISABLE_ERRORS *(PINT)0x00936268 = 0;

	namespace CONFIG {
		NATIVE_DECL void CONFIG_PARSE(const char* configFile) { NATIVE_INVOKE((NativeHash)HASH::CONFIG_PARSE, configFile); }
		NATIVE_DECL void CONFIG_CREATE(const char* configFile) { NATIVE_INVOKE((NativeHash)HASH::CONFIG_CREATE, configFile); }
		NATIVE_DECL void WEAPON_CONFIG_PARSE(const char* configFile) { NATIVE_INVOKE((NativeHash)HASH::WEAPON_CONFIG_PARSE, NULL, configFile); }
	}

	namespace GAME {
		NATIVE_DECL void GAME_SET_FRAMES(int frames) { NATIVE_INVOKE((NativeHash)HASH::GAME_SET_FRAMES, frames); };
	}

	namespace LEVEL {
	}

	namespace HUMAN {
	}

	namespace WEAPON {
	}

	namespace QTASK {
	}

	namespace QVM {
	}
}