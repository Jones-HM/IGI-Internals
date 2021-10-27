#pragma once 
#include "Common.hpp" 
#include "Logger.hpp" 
#include "CommonConst.hpp" 
#include "json.hpp"
using json = nlohmann::json;

typedef uint32_t NativeHash;

namespace IGI {
	enum class HASH
	{
		ANIM_TRIGGER_PARSE = 0x0004EC070,
		COMPILER_CLEANUP = 0x004B83D0,
		CONFIG_COMPILE = 0x004F0CF0,
		CONFIG_READ = 0x00405850,
		CONFIG_WRITE = 0x00405BD0,
		CUTSCENE_DELETE = 0x00415AB0,
		ERROR_SHOW = 0x004AF7B0,
		FRAMES_SET = 0x00402820,
		GAMEMATERIAL_LOAD = 0x00408350,
		GRAPHICS_RESET = 0x00403B70,
		GRAPH_OPEN = 0x004F9FF0,
		HUMANPLAYER_LOAD = 0x004137A0,
		INPUT_DISABLE = 0x004157D0,
		INPUT_ENABLE = 0x00415830,
		LOG_ADD = 0x004BBC30,
		LEVEL_LOAD = 0x004F0E10,
		LEVEL_QUIT = 0x00416550,
		LEVEL_RESTART = 0x00416FE0,
		LEVEL_START = 0x00415B30,
		MENU_MANAGER = 0x00418B00,
		MUSIC_DISABLE = 0x00415A60,
		MUSIC_ENABLE = 0x00415A10,
		MUSIC_SFX_VOLUME = 0x00495F30,
		MUSIC_VOLUME = 0x00495E70,
		MUSIC_UPDATE_VOLUME = 0x004158E0,
		PHYSICS_OBJ_TYPE_PARSE = 0x004EDFE0,
		QHASH_INIT = 0x004B0D60,
		QHASH_VAL_SET = 0x004B0D40,
		QHASH_VAL_GET = 0x004B0DB0,
		QHASH_RESET = 0x004B0D90,
		QSC_COMPILE = 0x004B8410,
		QTASK_UPDATE = 0x004F0E90,
		QTASK_UPDATE_LIST = 0x00401B20,
		QVM_ASSEMBLE = 0x004BB270,
		QVM_COMPILE = 0x004B85B0,
		QVM_LOAD = 0x004B80B0,
		RESOURCE_FLUSH = 0x004B63D0,
		RESOURCE_LOAD = 0x004B5F00,
		RESOURCE_UNLOAD = 0x004B6380,
		SCRIPT_INIT = 0x004F0E50,
		SCRIPT_SETSYMBOL_CXT = 0x004B8930,
		STATUS_MESSAGE_CLEAR = 0x00485AD0,
		STATUS_MESSAGE_SHOW = 0x00485970,
		SYMBOL_REMOVE = 0x004C0460,
		SYMBOL_CHECK = 0x004C0560,
		TASKTYPE_SET = 0x004B8810,
		WARNING_SHOW = 0x004AF810,
		WEAPON_CONFIG_READ = 0x004071E0,
		WEAPON_TYPE_OPEN = 0x00413B70,
		WEAPON_TOTAL = 0x00413BB0,
		WEAPON_GUN_PICKUP = 0x0045FFC0,
		WEAPON_AMMO_PICKUP = 0x0045FF80,
		HUMAN_HIT_DAMAGE = 0x00416D80,
		HUMAN_CAM_VIEW = 0x00463760,
		MEMORY_ALLOC = 0x004B0C60,
		MEMORY_DEALLOC = 0x004B0BE0,
		MISSION_OPEN = 0x00484E60,
		FILE_OPEN = 0x004A5350,
		PLAYER_PROFILE_ACTIVE = 0x00406220,
		SOLDIER_EXECUTE = 0x0045C440,
		HUMAN_TASK_VIEW_RESET = 0x004659E0,//Unimplemented Hash.
		GAME_DATA_SYMBOL_LOAD = 0x004A53B3,//Unimplemented Hash.
		GAME_DATA_SYMBOL_REGISTER_BOOL8 = 0x004B7EE0,//Unimplemented Hash.
		GAME_DATA_SYMBOL_REGISTER_INT16 = 0x004B7E80,//Unimplemented Hash.
		GAME_DATA_SYMBOL_REGISTER_INT32 = 0x004B7FA0,//Unimplemented Hash.
		GAME_DATA_SYMBOL_REGISTER_REAL32 = 0x004B7FC0,//Unimplemented Hash.
		GAME_DATA_SYMBOL_REMOVE = 0x004B80A0,//Unimplemented Hash.
		LOADING_SCREEN_SHOW = 0x0048A440,//Unimplemented Hash.
		QTASK_HASH_TABLE = 0x004BAAC0,//Unimplemented Hash.
	};

	class Natives {
	private:
		//Structure to hold Native info. 
		struct NativeSig {
			uint32_t address;//Address (Hash) of native. 
			string name; //Name (Symbol) of native. 
			string signature; //Signature of native. 
			string note; //Note (Comment) for method.
		};
		//Natives sig list.
		std::vector<NativeSig> native_sig;

		friend bool LoadNativesFile(string);
		friend void to_json(json&, const NativeSig&);
		friend void from_json(const json&, NativeSig&);
		void InitNativesSignatures();
		json ReadJsonFile(std::string);
		void WriteJSON(string, json);
		void GenerateNativesFile(string);

	public:
		Natives();
		Natives(const Natives&) = default;
		Natives(Natives&&) = default;
		~Natives();

		string FindNativeName(uint32_t);
		NativeHash FindNativeAddress(string);
		string FindNativeSignature(uint32_t);
		string FindNativeSignature(string);
		string FindNativeNote(string);
		string FindNativeNote(uint32_t);
		int GetNativesCount();
	};
	inline Natives* g_Natives{};
}