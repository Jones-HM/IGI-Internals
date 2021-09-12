#include "NativeCaller.hpp"
#include "Natives.hpp"
using namespace IGI;

NativeCaller::NativeCaller() 
{
	InitHashMaps();
}

//Init Hashes with only 0 Arguments.
void NativeCaller::InitHashMapArgs0() {
	//Map to store Hashes and Handlers.
	std::map<int, std::function<int(void)>> nativeMap0;

	//Declaring Function for Native hashes.
	std::function<int(void)> StatusTimer{ (int(__cdecl*)(void))HASH::STATUS_TIMER };
	std::function<int(void)> HumanPlayerLoad{ (int(__cdecl*)(void))HASH::HUMANPLAYER_LOAD };
	std::function<int(void)> QHashReset{ (int(__cdecl*)(void))HASH::QHASH_RESET };
	std::function<int(void)> QTaskUpdate{ (int(__cdecl*)(void))HASH::QTASK_UPDATE };
	std::function<int(void)> LevelRestart{ (int(__cdecl*)(void))HASH::LEVEL_RESTART };

	//Adding Native methods with Args0 to HashMap.
	NativeMapAdd((NativeHash)HASH::STATUS_TIMER, StatusTimer, nativeMap0,nativeMap0List0);
	NativeMapAdd((NativeHash)HASH::HUMANPLAYER_LOAD, HumanPlayerLoad, nativeMap0, nativeMap0List0);
	NativeMapAdd((NativeHash)HASH::QHASH_RESET, QHashReset, nativeMap0, nativeMap0List0);
	NativeMapAdd((NativeHash)HASH::QTASK_UPDATE, QTaskUpdate, nativeMap0, nativeMap0List0);
	NativeMapAdd((NativeHash)HASH::LEVEL_RESTART, LevelRestart, nativeMap0, nativeMap0List0);

}

//Init Hashes with only 1 Arguments.
void NativeCaller::InitHashMapArgs1() {

	//Map to store Hashes and Handlers.
	std::map<int, std::function<int(const char*)>> nativeMap1;
	std::map<int, std::function<int(int)>> nativeMap2;

	//Declaring Function for Native hashes.
	std::function<int(const char*)> ConfigParse{ (int(__cdecl*)(const char*))HASH::CONFIG_PARSE };
	std::function<int(const char*)> ConfigCreate{ (int(__cdecl*)(const char*))HASH::CONFIG_CREATE };
	std::function<int(int)> GameSetFrames{ (int(__cdecl*)(int))HASH::GAME_SET_FRAMES };

	//Adding Native methods with Args1 to HashMap.
	NativeMapAdd((NativeHash)HASH::CONFIG_PARSE, ConfigParse, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::CONFIG_CREATE, ConfigCreate, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::GAME_SET_FRAMES, GameSetFrames, nativeMap2, nativeMap1List2);
}

void NativeCaller::InitHashMapArgs2() {

	//Map to store Hashes and Handlers.
	std::map<int, std::function<int(int, const char*)>> nativeMap2;

	//Declaring Function for Native hashes.
	std::function<int(int, const char*)> ParseWeaponConfig{ (int(__cdecl*)(int index, const char* cfgFile))HASH::WEAPON_CONFIG_PARSE };

	//Adding Native methods with Args2 to HashMap.
	NativeMapAdd((NativeHash)HASH::WEAPON_CONFIG_PARSE, ParseWeaponConfig, nativeMap2, nativeMap2List2);
}

void NativeCaller::InitHashMapArgs3() {
}

void NativeCaller::InitHashMapArgs4() {
}

void NativeCaller::InitHashMaps() {
	InitHashMapArgs0();
	InitHashMapArgs1();
	InitHashMapArgs2();
}
