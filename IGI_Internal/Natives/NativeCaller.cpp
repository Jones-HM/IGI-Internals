#include "NativeCaller.hpp"
#include "Natives.hpp"
using namespace IGI;

NativeCaller::NativeCaller() 
{
	InitHashMaps();
}

NativeCaller::~NativeCaller() {

}

//Init Hashes with only 0 Arguments.
void NativeCaller::InitHashesArgs0() {
	//Map to store Hashes and Handlers.
	std::map<int, std::function<int(void)>> nativeMap0;

	//Declaring Function for Native hashes.
	std::function<int(void)> StatusTimer{ (int(__cdecl*)(void))HASH::STATUS_TIMER };
	std::function<int(void)> HumanPlayerLoad{ (int(__cdecl*)(void))HASH::HUMANPLAYER_LOAD };
	std::function<int(void)> QHashReset{ (int(__cdecl*)(void))HASH::QHASH_RESET };
	std::function<int(void)> QTaskUpdate{ (int(__cdecl*)(void))HASH::QTASK_UPDATE };
	std::function<int(void)> LevelRestart{ (int(__cdecl*)(void))HASH::LEVEL_RESTART };

}

//Init Hashes with only 1 Arguments.
void NativeCaller::InitHashesArgs1() {

	//Map to store Hashes and Handlers.
	std::map<int, std::function<int(const char*)>> nativeMap1;

	//Declaring Function for Native hashes.
	std::function<int(const char*)> ConfigParse{ (int(__cdecl*)(const char*))HASH::CONFIG_PARSE };
	std::function<int(const char*)> ConfigCreate{ (int(__cdecl*)(const char*))HASH::CONFIG_CREATE };

	//Add ConfigParse method to map.
	NativeMapInsert((int)HASH::CONFIG_PARSE, ConfigParse, nativeMap1);
	nativeMap1List1.push_back(nativeMap1);

	//Add ConfigCreate method to map.
	NativeMapInsert((int)HASH::CONFIG_CREATE, ConfigCreate, nativeMap1);
	nativeMap1List1.push_back(nativeMap1);
}

void NativeCaller::InitHashesArgs2() {

	//Map to store Hashes and Handlers.
	std::map<int, std::function<int(int, const char*)>> nativeMap2;

	//Declaring Function for Native hashes.
	std::function<int(int, const char*)> ParseWeaponConfig{ (int(__cdecl*)(int index, const char* cfgFile))HASH::WEAPON_CONFIG_PARSE };

	//Add ParseWeaponConfig method to map.
	NativeMapInsert((int)HASH::WEAPON_CONFIG_PARSE, ParseWeaponConfig, nativeMap2);
	nativeMap2List2.push_back(nativeMap2);
}

void NativeCaller::InitHashesArgs3() {
}

void NativeCaller::InitHashesArgs4() {
}

void NativeCaller::InitHashMaps() {
	InitHashesArgs1();
	InitHashesArgs2();
}
