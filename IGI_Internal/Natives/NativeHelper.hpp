#pragma once 
#include "Natives.hpp" 
#include "NativeCaller.hpp" 
#include "Utils/AutoMsgBox.hpp" 
#define NATIVE_DECL __declspec(noinline) inline 

namespace IGI {
	namespace PLAYER {
		//Set Player profile properties. 
		NATIVE_DECL void INDEX_NAME_SET(int index, const char* name) { std::memcpy((char*)PLAYER_INDEX_ADDR(index + 1), name, PLAYER_NAME_SIZE); }
		NATIVE_DECL void INDEX_MISSION_SET(int index, byte mission) { *(byte*)(PLAYER_INDEX_ADDR(index + 1) + PLAYER_ACTIVE_MISSION_OFF) = (byte)mission; }
		NATIVE_DECL void ACTIVE_NAME_SET(const char* name) { std::memcpy((char*)PLAYER_ACTIVE_ADDR, name, PLAYER_NAME_SIZE); }
		NATIVE_DECL void ACTIVE_MISSION_SET(byte mission) { { *(byte*)(PLAYER_ACTIVE_ADDR + PLAYER_ACTIVE_MISSION_OFF) = (byte)mission; } }
	}

	namespace CONFIG {
		NATIVE_DECL void READ() { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_READ, NATIVE_CONST_CONFIG_FILE); }
		NATIVE_DECL void READ(const char* config_file) { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_READ, config_file); }
		NATIVE_DECL void WRITE() { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_WRITE, NATIVE_CONST_CONFIG_FILE); }
		NATIVE_DECL void WRITE(const char* config_file) { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_WRITE, config_file); }
		NATIVE_DECL void WEAPON_CONFIG_READ() { NATIVE_INVOKE<Void>((Void)HASH::WEAPON_CONFIG_READ, 0, NATIVE_CONST_WEAPON_CONFIG_FILE); }
		NATIVE_DECL void WEAPON_CONFIG_READ(const char* config_file) { NATIVE_INVOKE<Void>((Void)HASH::WEAPON_CONFIG_READ, 0, config_file); }
	}

	namespace GAME {
		NATIVE_DECL void INPUT_ENABLE() { NATIVE_INVOKE<Void>((Void)HASH::INPUT_ENABLE, (const char*)local_buf); }
		NATIVE_DECL void INPUT_DISABLE() { NATIVE_INVOKE<Void>((Void)HASH::INPUT_DISABLE, (const char*)local_buf); }
		NATIVE_DECL void QUIT() { *(PINT)0x5C8DE8 = 0; }
	}

	namespace LEVEL {
		NATIVE_DECL void RESTART() { NATIVE_INVOKE<Void>((Void)HASH::LEVEL_RESTART); }
		NATIVE_DECL void SET(int level) { *(PINT)0x539560 = (level < 1 || level > GAME_LEVEL_MAX) ? 1 : level; }
	}

	namespace HUMAN {
		NATIVE_DECL void HUMAN_PLAYER_LOAD() { NATIVE_INVOKE<Void>((Void)HASH::HUMANPLAYER_LOAD); }
		NATIVE_DECL uint32_t HIT_DAMAGE_GET() { NATIVE_INVOKE<uint32_t>((Void)HASH::HUMAN_HIT_DAMAGE_GET); }
		NATIVE_DECL void VIEW_CAM(int cam_type) { NATIVE_INVOKE<Void>((Void)HASH::HUMAN_VIEW_CAM, (int)READ_PTR(humanplayer_ptr), cam_type); }
	}

	namespace WEAPON {
		NATIVE_DECL void UNLIMITED_AMMO_SET (bool enable) { *(PINT)0x56E214 = enable; }
		NATIVE_DECL void TYPE_OPEN() { NATIVE_INVOKE<Void>((Void)HASH::WEAPON_TYPE_OPEN); }
		NATIVE_DECL int TOTAL_COUNT() { return NATIVE_INVOKE<int>((Void)HASH::WEAPON_TOTAL_COUNT); }
		NATIVE_DECL void GUN_PICKUP(int weapon_id) {  GUN_PICKUP_SET(weapon_id); NATIVE_INVOKE<Void>((Void)HASH::WEAPON_GUN_PICKUP, READ_PTR(gun_pickup_ptr),GUN_PICKUP_PTR); }
		NATIVE_DECL void AMMO_PICKUP(int ammo_id) { AMMO_PICKUP_SET(ammo_id); NATIVE_INVOKE<Void>((Void)HASH::WEAPON_AMMO_PICKUP, READ_PTR(gun_pickup_ptr), AMMO_PICKUP_PTR); }
		NATIVE_DECL void WEAPON_PICKUP(int weapon_id) {int ammo_id = weapons_ammo_list.at(weapon_id); GUN_PICKUP(weapon_id); AMMO_PICKUP(ammo_id); }
	}

	namespace QTASK {
		NATIVE_DECL void UPDATE() { NATIVE_INVOKE<Void>((Void)HASH::QTASK_UPDATE); }
		NATIVE_DECL void UPDATE_LIST(int* qtask_list) { NATIVE_INVOKE<Void>((Void)HASH::QTASK_UPDATE_LIST, qtask_list); }
		NATIVE_DECL void RESET() { NATIVE_INVOKE<Void>((Void)HASH::QHASH_RESET); }
		NATIVE_DECL void HASH_INIT(int hash_val) { NATIVE_INVOKE<Void>((Void)HASH::QHASH_INIT, hash_val); };
	}

	namespace QFILE {
		NATIVE_DECL void QSC_COMPILE(const char* qsc_file) { NATIVE_INVOKE<Void>((Void)HASH::QSC_COMPILE, qsc_file); }
	}

	namespace GFX {
		NATIVE_DECL void GRAPHICS_RESET() { NATIVE_INVOKE<Void>((Void)HASH::GRAPHICS_RESET, (const char*)local_buf); }
	}

	namespace SFX {
		NATIVE_DECL void MUSIC_ENABLE() { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_ENABLE, (const char*)local_buf); }
		NATIVE_DECL void MUSIC_DISABLE() { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_DISABLE, (const char*)local_buf); }
		NATIVE_DECL void VOLUME_UPDATE() { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_UPDATE_VOLUME, (const char*)local_buf); }
		NATIVE_DECL void VOLUME_SET(float volume) { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_VOLUME_SET, volume, volume); }
		NATIVE_DECL void VOLUME_SFX_SET(float volume) { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_SFX_VOLUME_SET, volume); }
	}

	namespace MISC {
		NATIVE_DECL void FRAMES_SET(int frames) { NATIVE_INVOKE<Void>((Void)HASH::FRAMES_SET, frames); };
		NATIVE_DECL void CUTSCENE_DELETE() { NATIVE_INVOKE<Void>((Void)HASH::CUTSCENE_DELETE, (const char*)local_buf); }
		NATIVE_DECL void STATUS_MESSAGE_CLEAR() { NATIVE_INVOKE<Void>((Void)HASH::STATUS_MESSAGE_CLEAR, (const char*)local_buf); }
		NATIVE_DECL void GAMEMATERIAL_LOAD() { NATIVE_INVOKE<Void>((Void)HASH::GAMEMATERIAL_LOAD); }
		NATIVE_DECL void LOG_ADD(const char* log_msg) { NATIVE_INVOKE<Void>((Void)HASH::LOG_ADD, log_msg); }
		NATIVE_DECL void STATUS_MESSAGE_SHOW(const char* status_msg, const char* status_sprite) { NATIVE_INVOKE<Void>((Void)HASH::STATUS_MESSAGE_SHOW, *(PINT)0xA758AC, status_msg, status_sprite, (const char*)&status_byte); std::this_thread::sleep_for(5s); MISC::STATUS_MESSAGE_CLEAR(); }
		NATIVE_DECL void STATUS_MESSAGE_SHOW(const char* status_msg) { STATUS_MESSAGE_SHOW(status_msg, NATIVE_CONST_STATUSSCREEN_NOTE); }
		NATIVE_DECL void WARNINGS_DISABLE() { *(PINT)0x936274 = 0; }
		NATIVE_DECL void ERRORS_DISABLE() { *(PINT)0x936268 = 0; }
	}
}