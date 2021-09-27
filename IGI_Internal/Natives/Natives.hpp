#pragma once 
#include "Common.hpp" 
#include "Logger.hpp" 
#include "NativeConst.hpp" 
#include "nlohmann/json.hpp"
using json = nlohmann::json;

typedef uint32_t NativeHash;

namespace IGI {
	enum class HASH
	{
		ANIM_TRIGGER_PARSE = 0x04EC070,
		COMPILER_CLEANUP = 0x4B83D0,
		CONFIG_COMPILE = 0x4F0CF0,
		CONFIG_READ = 0x405850,
		CONFIG_WRITE = 0x405BD0,
		CUTSCENE_DELETE = 0x415AB0,
		ERROR_SHOW = 0x4AF7B0,
		FRAMES_SET = 0x402820,
		GAMEMATERIAL_LOAD = 0x408350,
		GRAPHICS_RESET = 0x403B70,
		GRAPH_OPEN = 0x4F9FF0,
		HUMANPLAYER_LOAD = 0x4137A0,
		INPUT_DISABLE = 0x4157D0,
		INPUT_ENABLE = 0x415830,
		LOG_ADD = 0x4BBC30,
		LEVEL_LOAD = 0x4F0E10,
		LEVEL_QUIT = 0x416550,
		LEVEL_RESTART = 0x416FE0,
		LEVEL_START = 0x415B30,
		MENU_MANAGER = 0x418B00,
		MUSIC_DISABLE = 0x415A60,
		MUSIC_ENABLE = 0x415A10,
		MUSIC_SET_SFX_VOLUME = 0x495F30,
		MUSIC_SET_VOLUME = 0x495E70,
		MUSIC_UPDATE_VOLUME = 0x4158E0,
		PHYSICS_OBJ_TYPE_PARSE = 0x4EDFE0,
		QHASH_INIT = 0x4B0D60,
		QHASH_RESET = 0x4B0D90,
		QSC_COMPILE = 0x4B8410,
		QTASK_UPDATE = 0x4F0E90,
		QTASK_UPDATE_LIST = 0x401B20,
		QVM_ASSEMBLE = 0x4BB270,
		QVM_COMPILE = 0x4B85B0,
		QVM_LOAD = 0x4B80B0,
		RESOURCE_FLUSH = 0x4B63D0,
		RESOURCE_LOAD = 0x4B5F00,
		RESOURCE_UNLOAD = 0x4B6380,
		SCRIPT_INIT = 0x4F0E50,
		SCRIPT_SETSYMBOL_CXT = 0x4B8930,
		STATUS_MESSAGE_DELETE = 0x485AD0,
		STATUS_MESSAGE_SHOW = 0x485970,
		SYMBOL_REMOVE = 0x4C0460,
		SYMBOL_CHECK = 0x4C0560,
		TASKTYPE_SET = 0x4B8810,
		WARNING_SHOW = 0x4AF810,
		WEAPON_CONFIG_READ = 0x4071E0,
		WEAPON_TYPE_OPEN = 0x413B70,
	};

	class Natives {
	private:
		//Structure to hold Native info. 
		struct NativeSig {
			uint32_t address;//Address of native. 
			string name; //Name (Symbol) of native. 
			string signature; //Signature of native. 
			string note; //Note comment for method.
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
	inline Natives *g_Natives{};
}