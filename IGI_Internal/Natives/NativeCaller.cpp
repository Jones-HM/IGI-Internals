#include "NativeCaller.hpp" 
#include "Natives.hpp" 
using namespace IGI;

NativeCaller::NativeCaller()
{
	g_NativeCaller = this;
	InitHashMaps();
}

NativeCaller::~NativeCaller() {
	g_NativeCaller = nullptr;
}

//Init Hashes with 0 Arguments. 
void NativeCaller::InitHashMapArgs0() {
	//Map to store Hashes and Handlers. 
	std::map<int, std::function<int(void)>> nativeMap0;

	//Declaring Function for Native hashes. 
	std::function<int(void)> HumanPlayerLoad{ (int(__cdecl*)(void))HASH::HUMANPLAYER_LOAD };
	std::function<int(void)> GameMaterialLoad{ (int(__cdecl*)(void))HASH::GAMEMATERIAL_LOAD };
	std::function<int(void)> QHashReset{ (int(__cdecl*)(void))HASH::QHASH_RESET };
	std::function<int(void)> QTaskUpdate{ (int(__cdecl*)(void))HASH::QTASK_UPDATE };
	std::function<int(void)> LevelRestart{ (int(__cdecl*)(void))HASH::LEVEL_RESTART };
	std::function<int(void)> WeaponTypeOpen{ (int(__cdecl*)(void))HASH::WEAPON_TYPE_OPEN };
	std::function<int(void)> WeaponTotalCount{ (int(__cdecl*)(void))HASH::WEAPON_TOTAL_COUNT };
	std::function<int(void)> HumanHitDamageGet{ (int(__cdecl*)(void))HASH::HUMAN_HIT_DAMAGE_GET };

	//Adding Native methods with Args0 to HashMap. 
	NativeMapAdd((NativeHash)HASH::HUMANPLAYER_LOAD, HumanPlayerLoad, nativeMap0, native_map0_list0);
	NativeMapAdd((NativeHash)HASH::HUMANPLAYER_LOAD, GameMaterialLoad, nativeMap0, native_map0_list0);
	NativeMapAdd((NativeHash)HASH::QHASH_RESET, QHashReset, nativeMap0, native_map0_list0);
	NativeMapAdd((NativeHash)HASH::QTASK_UPDATE, QTaskUpdate, nativeMap0, native_map0_list0);
	NativeMapAdd((NativeHash)HASH::LEVEL_RESTART, LevelRestart, nativeMap0, native_map0_list0);
	NativeMapAdd((NativeHash)HASH::WEAPON_TYPE_OPEN, WeaponTypeOpen, nativeMap0, native_map0_list0);
	NativeMapAdd((NativeHash)HASH::WEAPON_TOTAL_COUNT, WeaponTotalCount, nativeMap0, native_map0_list0);
	NativeMapAdd((NativeHash)HASH::HUMAN_HIT_DAMAGE_GET, HumanHitDamageGet, nativeMap0, native_map0_list0);

}

//Init Hashes with 1 Arguments. 
void NativeCaller::InitHashMapArgs1() {

	//Map to store Hashes and Handlers. 
	std::map<int, std::function<int(const char*)>> native_map1;
	std::map<int, std::function<int(int)>> native_map2;
	std::map<int, std::function<int(float)>> native_map3;
	std::map<int, std::function<int(int*)>> native_map4;

	//Declaring Function for Native hashes. 
	std::function<int(const char*)> ConfigRead{ (int(__cdecl*)(const char*))HASH::CONFIG_READ };
	std::function<int(const char*)> ConfigWrite{ (int(__cdecl*)(const char*))HASH::CONFIG_WRITE };
	std::function<int(const char*)> InputEnable{ (int(__cdecl*)(const char*))HASH::INPUT_ENABLE };
	std::function<int(const char*)> InputDisable{ (int(__cdecl*)(const char*))HASH::INPUT_DISABLE };
	std::function<int(const char*)> EnableMusic{ (int(__cdecl*)(const char*))HASH::MUSIC_ENABLE };
	std::function<int(const char*)> DisableMusic{ (int(__cdecl*)(const char*))HASH::MUSIC_DISABLE };
	std::function<int(const char*)> CutsceneDelete{ (int(__cdecl*)(const char*))HASH::CUTSCENE_DELETE };
	std::function<int(const char*)> StatusMsgDelete{ (int(__cdecl*)(const char*))HASH::STATUS_MESSAGE_CLEAR };
	std::function<int(const char*)> VolumeUpdate{ (int(__cdecl*)(const char*))HASH::MUSIC_UPDATE_VOLUME };
	std::function<int(const char*)> GraphicsReset{ (int(__cdecl*)(const char*))HASH::GRAPHICS_RESET };
	std::function<int(const char*)> QscCompile{ (int(__cdecl*)(const char*))HASH::QSC_COMPILE };
	std::function<int(const char*)> LogAdd{ (int(__cdecl*)(const char*))HASH::LOG_ADD };
	std::function<int(int*)> QTaskUpdateList{ (int(__cdecl*)(int*))HASH::QTASK_UPDATE_LIST };

	std::function<int(int)> QHashInit{ (int(__cdecl*)(int))HASH::QHASH_INIT };
	std::function<int(int)> FramesSet{ (int(__cdecl*)(int))HASH::FRAMES_SET };
	std::function<int(float)> VolumeSFXSet{ (int(__cdecl*)(float))HASH::MUSIC_SET_SFX_VOLUME };

	//Adding Native methods with Args1 to HashMap. 
	NativeMapAdd((NativeHash)HASH::CONFIG_READ, ConfigRead, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::CONFIG_WRITE, ConfigWrite, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::INPUT_ENABLE, InputEnable, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::INPUT_DISABLE, InputDisable, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::MUSIC_ENABLE, EnableMusic, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::MUSIC_DISABLE, DisableMusic, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::CUTSCENE_DELETE, CutsceneDelete, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::STATUS_MESSAGE_CLEAR, StatusMsgDelete, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::MUSIC_UPDATE_VOLUME, VolumeUpdate, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::GRAPHICS_RESET, GraphicsReset, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::QSC_COMPILE, QscCompile, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::LOG_ADD, LogAdd, native_map1, native_map1_list1);
	NativeMapAdd((NativeHash)HASH::FRAMES_SET, FramesSet, native_map2, native_map1_list2);
	NativeMapAdd((NativeHash)HASH::QHASH_INIT, QHashInit, native_map2, native_map1_list2);
	NativeMapAdd((NativeHash)HASH::MUSIC_SET_SFX_VOLUME, VolumeSFXSet, native_map3, native_map1_list3);
	NativeMapAdd((NativeHash)HASH::QTASK_UPDATE_LIST, QTaskUpdateList, native_map4, native_map1_list4);


}

//Init Hashes with 2 Arguments. 
void NativeCaller::InitHashMapArgs2() {

	//Map to store Hashes and Handlers. 
	std::map<int, std::function<int(int, const char*)>> native_map1;
	std::map<int, std::function<int(float, float)>> native_map2;
	std::map<int, std::function<int(int, int*)>> native_map3;
	std::map<int, std::function<int(int, int)>> native_map4;

	//Declaring Function for Native hashes. 
	std::function<int(int, const char*)> WeaponConfigRead{ (int(__cdecl*)(int,const char*))HASH::WEAPON_CONFIG_READ };
	std::function<int(float, float)> MusicSetVolume{ (int(__cdecl*)(float,float))HASH::MUSIC_SET_VOLUME };
	std::function<int(int, int*)> WeaponGunPickup{ (int(__cdecl*)(int, int*))HASH::WEAPON_GUN_PICKUP };
	std::function<int(int, int*)> WeaponAmmoPickup{ (int(__cdecl*)(int, int*))HASH::WEAPON_AMMO_PICKUP };
	std::function<int(int, int)> HumanViewCam{ (int(__cdecl*)(int, int))HASH::HUMAN_VIEW_CAM };

	//Adding Native methods with Args2 to HashMap. 
	NativeMapAdd((NativeHash)HASH::WEAPON_CONFIG_READ, WeaponConfigRead, native_map1, native_map2_list2);
	NativeMapAdd((NativeHash)HASH::MUSIC_SET_VOLUME, MusicSetVolume, native_map2, native_map2_list3);
	NativeMapAdd((NativeHash)HASH::WEAPON_GUN_PICKUP, WeaponGunPickup, native_map3, native_map2_list4);
	NativeMapAdd((NativeHash)HASH::WEAPON_AMMO_PICKUP, WeaponAmmoPickup, native_map3, native_map2_list4);
	NativeMapAdd((NativeHash)HASH::HUMAN_VIEW_CAM, HumanViewCam, native_map4, native_map2_list5);
}

//Init Hashes with 3 Arguments. 
void NativeCaller::InitHashMapArgs3() {
	//TL:DR
}

//Init Hashes with 4 Arguments. 
void NativeCaller::InitHashMapArgs4() {
	std::map<int, std::function<int(int,LPCSTR, LPCSTR, LPCSTR)>> native_map1;

	//Declaring Function for Native hashes. 
	std::function<int(int, LPCSTR, LPCSTR, LPCSTR)> StatusMessageShow{ (int(__cdecl*)(int,LPCSTR, LPCSTR, LPCSTR))HASH::STATUS_MESSAGE_SHOW };

	//Adding Native methods with Args4 to HashMap. 
	NativeMapAdd((NativeHash)HASH::STATUS_MESSAGE_SHOW, StatusMessageShow, native_map1, native_map4_list1);
}

void NativeCaller::InitHashMaps() {
	InitHashMapArgs0();
	InitHashMapArgs1();
	InitHashMapArgs2();
	InitHashMapArgs3();
	InitHashMapArgs4();
}