#pragma once
#include "Natives.hpp"
#include "NativeCaller.hpp"
#define NATIVE_DECL __declspec(noinline) inline

namespace IGI {
	//Common Native constants.
	const auto NATIVE_CONFIG_FILE = "LOCAL:config.qsc";
	const auto NATIVE_MAINMENU_FILE = "LOCAL:menusystem\\mainmenu.qsc";
	const auto NATIVE_INMENU_FILE = "LOCAL:menusystem\\ingame.qsc";
	const auto NATIVE_WEAPON_CONFIG_FILE = "LOCAL:weapons/weaponconfig.qsc";
	char localBuf[0x1E] = { NULL }; // Local buffer to store value from different methods.

#define NATIVE_INVOKE g_NativeCaller.NativeInvoker
#define NATIVE_QUIT *(PINT)0x005C8DE8 = 0;
#define NATIVE_LEVEL_SET(level) *(PINT)0x00539560 = level;
#define NATIVE_LEVEL_INC *(PINT)0x00539560 += 1;
#define NATIVE_LEVEL_DEC *(PINT)0x00539560 -= 1;
#define NATIVE_QUIT *(PINT)0x005C8DE8 = 0;
#define NATIVE_DISABLE_WARNINGS *(PINT)0x00936274 = 0;
#define NATIVE_DISABLE_ERRORS *(PINT)0x00936268 = 0;
#define NATIVE_UNLIMITED_AMMO *(PINT)0x0056E214 = 1;

	namespace CONFIG {
		NATIVE_DECL void CONFIG_PARSE(const char* configFile) { NATIVE_INVOKE((NativeHash)HASH::CONFIG_PARSE, configFile); }
		NATIVE_DECL void CONFIG_CREATE(const char* configFile) { NATIVE_INVOKE((NativeHash)HASH::CONFIG_CREATE, configFile); }
		NATIVE_DECL void WEAPON_CONFIG_PARSE(const char* configFile) { NATIVE_INVOKE((NativeHash)HASH::WEAPON_CONFIG_PARSE, NULL, configFile); }
	}

	namespace GAME {
		NATIVE_DECL void ENABLE_INPUT() { NATIVE_INVOKE((NativeHash)HASH::ENABLE_INPUT, (LPCSTR)localBuf); }
		NATIVE_DECL void DISABLE_INPUT() { NATIVE_INVOKE((NativeHash)HASH::DISABLE_INPUT, (LPCSTR)localBuf); }
	}

	namespace LEVEL {
	}

	namespace HUMAN {
	}

	namespace WEAPON {
	}

	namespace QTASK {
	}

	namespace GFX {
		NATIVE_DECL void GRAPHICS_RESET() { NATIVE_INVOKE((NativeHash)HASH::GRAPHICS_RESET, (LPCSTR)localBuf); }
	}

	namespace SFX {
		NATIVE_DECL void MUSIC_ENABLE() { NATIVE_INVOKE((NativeHash)HASH::MUSIC_ENABLE, (LPCSTR)localBuf); }
		NATIVE_DECL void MUSIC_DISABLE() { NATIVE_INVOKE((NativeHash)HASH::MUSIC_DISABLE, (LPCSTR)localBuf); }
		NATIVE_DECL void MUSIC_UPDATE_VOLUME() { NATIVE_INVOKE((NativeHash)HASH::MUSIC_UPDATE_VOLUME, (LPCSTR)localBuf); }
		NATIVE_DECL void MUSIC_SET_VOLUME (int volume) { NATIVE_INVOKE((NativeHash)HASH::MUSIC_SET_VOLUME,volume,volume); }
		NATIVE_DECL void MUSIC_SET_SFX_VOLUME (int volume) { NATIVE_INVOKE((NativeHash)HASH::MUSIC_SET_SFX_VOLUME,volume); }
	}

	namespace MISC {
		NATIVE_DECL void FRAMES_SET(int frames) { NATIVE_INVOKE((NativeHash)HASH::FRAMES_SET, frames); };
		NATIVE_DECL void CUTSCENE_DELETE() { NATIVE_INVOKE((NativeHash)HASH::CUTSCENE_DELETE, (LPCSTR)localBuf); }
		NATIVE_DECL void STATUSMSG_DELETE() { NATIVE_INVOKE((NativeHash)HASH::STATUSMSG_DELETE, (LPCSTR)localBuf); }
	}
}