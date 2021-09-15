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
	std::map<int, std::function<int(float)>> nativeMap3;

	//Declaring Function for Native hashes.
	std::function<int(const char*)> ConfigParse{ (int(__cdecl*)(const char*))HASH::CONFIG_PARSE };
	std::function<int(const char*)> ConfigCreate{ (int(__cdecl*)(const char*))HASH::CONFIG_CREATE };
	std::function<int(const char*)> EnableInput{ (int(__cdecl*)(const char*))HASH::ENABLE_INPUT };
	std::function<int(const char*)> DisableInput{ (int(__cdecl*)(const char*))HASH::DISABLE_INPUT };
	std::function<int(const char*)> EnableMusic{ (int(__cdecl*)(const char*))HASH::MUSIC_ENABLE };
	std::function<int(const char*)> DisableMusic{ (int(__cdecl*)(const char*))HASH::MUSIC_DISABLE };
	std::function<int(const char*)> CutsceneDelete{ (int(__cdecl*)(const char*))HASH::CUTSCENE_DELETE };
	std::function<int(const char*)> StatusMsgDelete{ (int(__cdecl*)(const char*))HASH::STATUSMSG_DELETE };
	std::function<int(const char*)> VolumeUpdate{ (int(__cdecl*)(const char*))HASH::MUSIC_UPDATE_VOLUME };
	std::function<int(const char*)> GraphicsReset{ (int(__cdecl*)(const char*))HASH::GRAPHICS_RESET };

	std::function<int(int)> FramesSet{ (int(__cdecl*)(int))HASH::FRAMES_SET };
	std::function<int(float)> VolumeSFXSet{ (int(__cdecl*)(float))HASH::MUSIC_SET_SFX_VOLUME };

	//Adding Native methods with Args1 to HashMap.
	NativeMapAdd((NativeHash)HASH::CONFIG_PARSE, ConfigParse, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::CONFIG_CREATE, ConfigCreate, nativeMap1, nativeMap1List1);

	NativeMapAdd((NativeHash)HASH::ENABLE_INPUT, EnableInput, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::DISABLE_INPUT, DisableInput, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::MUSIC_ENABLE, EnableMusic, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::MUSIC_DISABLE, DisableMusic, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::CUTSCENE_DELETE, CutsceneDelete, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::STATUSMSG_DELETE, StatusMsgDelete, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::MUSIC_UPDATE_VOLUME, VolumeUpdate, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::GRAPHICS_RESET, GraphicsReset, nativeMap1, nativeMap1List1);
	NativeMapAdd((NativeHash)HASH::FRAMES_SET, FramesSet, nativeMap2, nativeMap1List2);
	NativeMapAdd((NativeHash)HASH::MUSIC_SET_SFX_VOLUME, VolumeSFXSet, nativeMap3, nativeMap1List3);


}

void NativeCaller::InitHashMapArgs2() {

	//Map to store Hashes and Handlers.
	std::map<int, std::function<int(int, const char*)>> nativeMap2;
	std::map<int, std::function<int(float,float)>> nativeMap3;

	//Declaring Function for Native hashes.
	std::function<int(int, const char*)> ParseWeaponConfig{ (int(__cdecl*)(int,const char*))HASH::WEAPON_CONFIG_PARSE };
	std::function<int(float, float)> MusicSetVolume{ (int(__cdecl*)(float,float))HASH::MUSIC_SET_VOLUME };

	//Adding Native methods with Args2 to HashMap.
	NativeMapAdd((NativeHash)HASH::WEAPON_CONFIG_PARSE, ParseWeaponConfig, nativeMap2, nativeMap2List2);
	NativeMapAdd((NativeHash)HASH::MUSIC_SET_VOLUME, MusicSetVolume, nativeMap3, nativeMap2List3);
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
