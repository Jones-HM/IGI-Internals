#include "Natives.hpp"
using namespace IGI;
using namespace Utility;

FORCEINLINE bool std_strcmp(const string& a, const string& b) { return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {return tolower(a) == tolower(b); }); }

Natives::Natives() {
	Natives::InitNativesHashMap();
}

void Natives::InitNativesHashMap() {
	nativesHashMap.insert({ (NativeHash)HASH::ANIM_TRIGGER_PARSE ,"AnimTriggerParse" });
	nativesHashMap.insert({ (NativeHash)HASH::COMPILER_CLEANUP ,"CompilerCleanup" });
	nativesHashMap.insert({ (NativeHash)HASH::CONFIG_COMPILE ,"ConfigCompile" });
	nativesHashMap.insert({ (NativeHash)HASH::CONFIG_READ ,"ConfigRead" });
	nativesHashMap.insert({ (NativeHash)HASH::CONFIG_WRITE ,"ConfigWrite" });
	nativesHashMap.insert({ (NativeHash)HASH::CUTSCENE_DELETE ,"CutsceneDelete" });
	nativesHashMap.insert({ (NativeHash)HASH::ERROR_SHOW ,"ErrorShow" });
	nativesHashMap.insert({ (NativeHash)HASH::FRAMES_SET ,"FramesSet" });
	nativesHashMap.insert({ (NativeHash)HASH::GAMEMATERIAL_LOAD ,"GamematerialLoad" });
	nativesHashMap.insert({ (NativeHash)HASH::GRAPHICS_RESET ,"GraphicsReset" });
	nativesHashMap.insert({ (NativeHash)HASH::GRAPH_OPEN ,"GraphOpen" });
	nativesHashMap.insert({ (NativeHash)HASH::HUMANPLAYER_LOAD ,"HumanplayerLoad" });
	nativesHashMap.insert({ (NativeHash)HASH::INPUT_DISABLE ,"InputDisable" });
	nativesHashMap.insert({ (NativeHash)HASH::INPUT_ENABLE ,"InputEnable" });
	nativesHashMap.insert({ (NativeHash)HASH::LOG_ADD ,"LogAdd" });
	nativesHashMap.insert({ (NativeHash)HASH::LEVEL_LOAD ,"LevelLoad" });
	nativesHashMap.insert({ (NativeHash)HASH::LEVEL_QUIT ,"LevelQuit" });
	nativesHashMap.insert({ (NativeHash)HASH::LEVEL_RESTART ,"LevelRestart" });
	nativesHashMap.insert({ (NativeHash)HASH::LEVEL_START ,"LevelStart" });
	nativesHashMap.insert({ (NativeHash)HASH::MENU_MANAGER ,"MenuManager" });
	nativesHashMap.insert({ (NativeHash)HASH::MUSIC_DISABLE ,"MusicDisable" });
	nativesHashMap.insert({ (NativeHash)HASH::MUSIC_ENABLE ,"MusicEnable" });
	nativesHashMap.insert({ (NativeHash)HASH::MUSIC_SET_SFX_VOLUME ,"MusicSetSfxVolume" });
	nativesHashMap.insert({ (NativeHash)HASH::MUSIC_SET_VOLUME ,"MusicSetVolume" });
	nativesHashMap.insert({ (NativeHash)HASH::MUSIC_UPDATE_VOLUME ,"MusicUpdateVolume" });
	nativesHashMap.insert({ (NativeHash)HASH::PHYSICS_OBJ_TYPE_PARSE ,"PhysicsObjTypeParse" });
	nativesHashMap.insert({ (NativeHash)HASH::QHASH_INIT ,"QhashInit" });
	nativesHashMap.insert({ (NativeHash)HASH::QHASH_RESET ,"QhashReset" });
	nativesHashMap.insert({ (NativeHash)HASH::QSC_COMPILE ,"QscCompile" });
	nativesHashMap.insert({ (NativeHash)HASH::QTASK_UPDATE ,"QtaskUpdate" });
	nativesHashMap.insert({ (NativeHash)HASH::QTASK_UPDATE_LIST ,"QtaskUpdateList" });
	nativesHashMap.insert({ (NativeHash)HASH::QVM_ASSEMBLE ,"QvmAssemble" });
	nativesHashMap.insert({ (NativeHash)HASH::QVM_COMPILE ,"QvmCompile" });
	nativesHashMap.insert({ (NativeHash)HASH::QVM_LOAD ,"QvmLoad" });
	nativesHashMap.insert({ (NativeHash)HASH::RESOURCE_COMPILE ,"ResourceCompile" });
	nativesHashMap.insert({ (NativeHash)HASH::RESOURCE_FLUSH ,"ResourceFlush" });
	nativesHashMap.insert({ (NativeHash)HASH::RESOURCE_LOAD ,"ResourceLoad" });
	nativesHashMap.insert({ (NativeHash)HASH::RESOURCE_UNLOAD ,"ResourceUnload" });
	nativesHashMap.insert({ (NativeHash)HASH::SCRIPT_INIT ,"ScriptInit" });
	nativesHashMap.insert({ (NativeHash)HASH::SCRIPT_SETSYMBOL_CXT ,"ScriptSetsymbolCxt" });
	nativesHashMap.insert({ (NativeHash)HASH::STATUS_MESSAGE_DELETE ,"StatusMessageDelete" });
	nativesHashMap.insert({ (NativeHash)HASH::STATUS_MESSAGE_SHOW ,"StatusMessageShow" });
	nativesHashMap.insert({ (NativeHash)HASH::STATUS_MESSAGE_TIMER ,"StatusMessageTimer" });
	nativesHashMap.insert({ (NativeHash)HASH::SYMBOL_REMOVE ,"SymbolRemove" });
	nativesHashMap.insert({ (NativeHash)HASH::TASKTYPE_SET ,"TasktypeSet" });
	nativesHashMap.insert({ (NativeHash)HASH::WARNING_SHOW ,"WarningShow" });
	nativesHashMap.insert({ (NativeHash)HASH::WEAPON_CONFIG_READ ,"WeaponConfigRead" });
	nativesHashMap.insert({ (NativeHash)HASH::WEAPON_TYPE_OPEN ,"WeaponTypeOpen" });
}

string Natives::FindNativeName(NativeHash nativeAddr) {
	string nativeName;
	for (const auto& nHashMap : nativesHashMap) {
		if (nHashMap.first == nativeAddr)
			return nHashMap.second;
	}
	nativeName = string("Native name couldn't be found Address 0x") + HEX_ADDR_STR(nativeAddr);
	return nativeName;
}

NativeHash Natives::FindNativeAddr(string nativeName) {

	for (const auto& nHashMap : nativesHashMap) {

		if (std_strcmp(nHashMap.second.c_str(), nativeName.c_str())) {
			return nHashMap.first;
		}
	}
	string addrNotFoundErr = string();
	LOG_ERROR("Native address couldn't be found for '%s'", nativeName);
	return 0;
}
