#pragma once 
#include "Natives.hpp" 
#include "NativeCaller.hpp" 
#include "Camera.hpp"
#include "HumanSoldier.hpp"
#include "GameResource.hpp"

#define NATIVE_DECL __declspec(noinline) inline 

namespace IGI {

	namespace MISC {
		NATIVE_DECL void FRAMES_SET(int frames) { NATIVE_INVOKE<Void>((Void)HASH::FRAMES_SET, frames); };
		NATIVE_DECL void CUTSCENE_DELETE() { NATIVE_INVOKE<Void>((Void)HASH::CUTSCENE_DELETE, (const char*)local_buf); }
		NATIVE_DECL void GAMEMATERIAL_LOAD() { NATIVE_INVOKE<Void>((Void)HASH::GAMEMATERIAL_LOAD); }
		NATIVE_DECL void LOG_ADD(const char* log_msg) { NATIVE_INVOKE<Void>((Void)HASH::LOG_ADD, log_msg); }
		NATIVE_DECL void STATUS_MESSAGE_SHOW(const char* status_msg, const char* status_sprite) { NATIVE_INVOKE<Void>((Void)HASH::STATUS_MESSAGE_SHOW, *(PINT)0x00A758AC, status_msg, status_sprite, &status_byte); }
		NATIVE_DECL void STATUS_MESSAGE_SHOW(const char* status_msg) { STATUS_MESSAGE_SHOW(status_msg, GAME_STATUSSCREEN_NOTE); }
		NATIVE_DECL void STATUS_MESSAGE_CLEAR() { NATIVE_INVOKE<Void>((Void)HASH::STATUS_MESSAGE_CLEAR, (const char*)local_buf); }
		NATIVE_DECL void WARNINGS_DISABLE() { *(PINT)0x00936274 = 0; }
		NATIVE_DECL void ERRORS_DISABLE() { *(PINT)0x00936268 = 0; }
	}

	namespace PLAYER {
		NATIVE_DECL void INDEX_NAME_SET(int index, const char* name) { std::memcpy((char*)PLAYER_INDEX_ADDR(index + 1), name, PLAYER_NAME_SIZE); }
		NATIVE_DECL void INDEX_MISSION_SET(int index, byte mission) { *(byte*)(PLAYER_INDEX_ADDR(index + 1) + PLAYER_ACTIVE_MISSION_OFF) = (byte)mission; }
		NATIVE_DECL void ACTIVE_NAME_SET(const char* name) { std::memcpy((char*)PLAYER_ACTIVE_ADDR, name, PLAYER_NAME_SIZE); }
		NATIVE_DECL void ACTIVE_MISSION_SET(byte mission) { { *(byte*)(PLAYER_ACTIVE_ADDR + PLAYER_ACTIVE_MISSION_OFF) = (byte)mission; } }
		NATIVE_DECL char* IS_PROFILE_ACTIVE() { return NATIVE_INVOKE<char*>((Void)HASH::PLAYER_PROFILE_ACTIVE); }
	}

	namespace CONFIG {
		NATIVE_DECL void READ() { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_READ, GAME_CONFIG_FILE); }
		NATIVE_DECL void READ(const char* config_file) { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_READ, config_file); }
		NATIVE_DECL void WRITE() { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_WRITE, GAME_CONFIG_FILE); }
		NATIVE_DECL void WRITE(const char* config_file) { NATIVE_INVOKE<Void>((Void)HASH::CONFIG_WRITE, config_file); }
		NATIVE_DECL void WEAPON_CONFIG_READ() { NATIVE_INVOKE<Void>((Void)HASH::WEAPON_CONFIG_READ, 0, GAME_WEAPON_CONFIG_FILE); }
		NATIVE_DECL void WEAPON_CONFIG_READ(const char* config_file) { NATIVE_INVOKE<Void>((Void)HASH::WEAPON_CONFIG_READ, 0, config_file); }
	}

	namespace DEBUG {
		NATIVE_DECL void INIT() { *(int*)0x0056DF94 = 1; *(int*)0x00A5EA75 = (int)0x005C8BF4; }
		NATIVE_DECL void INIT(const char* font_type) { *(int*)0x00A5EA75 = (int)0x005C8BF4; strcpy((char*)0x0054D958, font_type); }
		NATIVE_DECL void ENABLE(bool state) { *(uint8_t*)0x005BDC1C = state; }
	}

	namespace GAME {
		NATIVE_DECL void INPUT_ENABLE() { NATIVE_INVOKE<Void>((Void)HASH::INPUT_ENABLE, (const char*)local_buf); }
		NATIVE_DECL void INPUT_DISABLE() { NATIVE_INVOKE<Void>((Void)HASH::INPUT_DISABLE, (const char*)local_buf); }
		NATIVE_DECL void QUIT() { *(PINT)0x5C8DE8 = 0; }
	}

	namespace LEVEL {
		NATIVE_DECL void RESTART() { NATIVE_INVOKE<Void>((Void)HASH::LEVEL_RESTART); }
		NATIVE_DECL int GET() { return READ_PTR(0x00539560); }
		NATIVE_DECL void SET(int level) { *(PINT)0x00539560 = (level < 1 || level > GAME_LEVEL_MAX) ? 1 : level; }
	}

	namespace HUMAN {
		NATIVE_DECL void HUMAN_PLAYER_LOAD() { NATIVE_INVOKE<Void>((Void)HASH::HUMANPLAYER_LOAD); }
		NATIVE_DECL uint32_t HIT_DAMAGE_GET() { return NATIVE_INVOKE<uint32_t>((Void)HASH::HUMAN_HIT_DAMAGE); }
		NATIVE_DECL void CAM_VIEW_SET(int cam_type) { NATIVE_INVOKE<Void>((Void)HASH::HUMAN_CAM_VIEW, (int)READ_PTR(humanplayer_ptr), cam_type); }
	}

	namespace SOLDIER {
		NATIVE_DECL void INIT() { g_Soldier.Init(true); }
		NATIVE_DECL void ADD(HumanSoldier& soldier) { soldier.AddSoldier(); }
		NATIVE_DECL bool REMOVE(HumanSoldier& soldier) { return soldier.RemoveSoldier(); }
		NATIVE_DECL bool VALIDATE(HumanSoldier& soldier) { return soldier.ValidateSoldier(); }
		NATIVE_DECL HumanSoldier FIND(soldier_t soldier_id) { return g_Soldier.FindSoldier(soldier_id); }
		NATIVE_DECL HumanSoldier FIND(ai_t ai_id) { return g_Soldier.FindSoldier(ai_id); }
		NATIVE_DECL HumanSoldier FIND(graph_t graph_id) { return g_Soldier.FindSoldier(graph_id); }
		NATIVE_DECL HumanSoldier FIND(address_t address) { return g_Soldier.FindSoldier(address); }
		NATIVE_DECL HumanSoldier FIND(string model_id) { return g_Soldier.FindSoldier(model_id); }
		NATIVE_DECL void DEBUG_DATA(HumanSoldier& soldier) { MISC::STATUS_MESSAGE_SHOW(soldier.DebugSoldierData().c_str()); std::this_thread::sleep_for(7s); MISC::STATUS_MESSAGE_CLEAR(); }
		NATIVE_DECL void DEBUG_DATA_LIST(HumanSoldier& soldier) { soldier.DebugSoldierDataList(); }
		NATIVE_DECL void EXECUTE(int soldier_ptr, int soldier_addr) { NATIVE_INVOKE<Void>((Void)HASH::SOLDIER_EXECUTE, soldier_ptr, soldier_addr); }
		NATIVE_DECL void EXECUTE(HumanSoldier& soldier) { g_Soldier.ExecuteSoldier(soldier.GetSoldierId()); }
		NATIVE_DECL void EXECUTE(soldier_t soldier_id) { g_Soldier.ExecuteSoldier(soldier_id); }
		NATIVE_DECL void EXECUTE() { if (soldiers.size() > 0) { auto soldier = soldiers.at(0); SOLDIER::EXECUTE(soldier); } }
		NATIVE_DECL void EXECUTE_ALL() { g_Soldier.ExecuteSoldiers(); }
		NATIVE_DECL void CAM_VIEW_SET(HumanSoldier& soldier, int cam_type) { NATIVE_INVOKE<Void>((Void)HASH::HUMAN_CAM_VIEW, (int)READ_PTR(soldier.GetAddress()), cam_type); }
	}

	namespace WEAPON {
		NATIVE_DECL void UNLIMITED_AMMO_SET(bool enable) { *(PINT)0x56E214 = enable; }
		NATIVE_DECL void TYPE_OPEN() { NATIVE_INVOKE<Void>((Void)HASH::WEAPON_TYPE_OPEN); }
		NATIVE_DECL int TOTAL_COUNT() { return NATIVE_INVOKE<int>((Void)HASH::WEAPON_TOTAL); }
		NATIVE_DECL void GUN_PICKUP(int weapon_id) { GUN_PICKUP_SET(weapon_id); NATIVE_INVOKE<Void>((Void)HASH::WEAPON_GUN_PICKUP, READ_PTR(gun_pickup_ptr), GUN_PICKUP_PTR); }
		NATIVE_DECL void AMMO_PICKUP(int ammo_id) { AMMO_PICKUP_SET(ammo_id); NATIVE_INVOKE<Void>((Void)HASH::WEAPON_AMMO_PICKUP, READ_PTR(gun_pickup_ptr), AMMO_PICKUP_PTR); }
		NATIVE_DECL void WEAPON_PICKUP(int weapon_id) { int ammo_id = weapons_ammo_list.at(weapon_id); GUN_PICKUP(weapon_id); AMMO_PICKUP(ammo_id); }
	}

	namespace CAMERA {
		NATIVE_DECL void ATTACH() { g_Camera.Attach(); }
		NATIVE_DECL void DEATTACH() { g_Camera.Deattach(); }
		NATIVE_DECL void CALIBRATE() { g_Camera.CalibrateView(); }
		NATIVE_DECL void FREECAM(Camera::Controls& controls) { g_Camera.FreeCam(controls); }
		NATIVE_DECL void X_POS_UPDATE(double x) { g_Camera.WritePosition(x); }
		NATIVE_DECL void Y_POS_UPDATE(double y) { g_Camera.WritePosition(NULLF, y); }
		NATIVE_DECL void Z_POS_UPDATE(double z) { g_Camera.WritePosition(NULLF, NULLF, z); }
		NATIVE_DECL void POS_UPDATE(Camera::Position& pos) { g_Camera.WritePosition(pos); }
		NATIVE_DECL void ANGLE_UPDATE(Camera::Angle& angle) { g_Camera.WriteAngle(angle); }
		NATIVE_DECL void PITCH_UPDATE(float pitch) { g_Camera.WriteAngle(pitch); }
		NATIVE_DECL void ROLL_UPDATE(float roll) { g_Camera.WriteAngle(NULLF, roll); }
		NATIVE_DECL void YAW_UPDATE(float yaw) { g_Camera.WriteAngle(NULLF, NULLF, yaw); }
		NATIVE_DECL void FOV_UPDATE(float fov) { g_Camera.WriteAngle(NULLF, NULLF, NULLF, fov); }
		NATIVE_DECL Camera::Position POS_READ() { return g_Camera.ReadPosition(); }
		NATIVE_DECL Camera::Angle ANGLE_READ() { return g_Camera.ReadAngle(); }
	}

	namespace RESOURCE {
		NATIVE_DECL void RESOURCE_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_RES); }
		NATIVE_DECL void TEXTURE_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_TEXTURE); }
		NATIVE_DECL void FONT_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_FONT); }
		NATIVE_DECL void ANIMATION_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_ANIMATION); }
		NATIVE_DECL void LIGHTMAP_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_OLM); g_Resource->SaveGameResource(file, GAME_RESOURCE_LMP); }
		NATIVE_DECL void GRAPHICS_2D_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_SPRITE); g_Resource->SaveGameResource(file, GAME_RESOURCE_PIC); g_Resource->SaveGameResource(file, GAME_RESOURCE_TEXTURE); }
		NATIVE_DECL void GRAPHICS_3D_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_MEF); g_Resource->SaveGameResource(file, GAME_RESOURCE_TEXTURE); }
		NATIVE_DECL void TERRAIN_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_CMD); g_Resource->SaveGameResource(file, GAME_RESOURCE_CTR); g_Resource->SaveGameResource(file, GAME_RESOURCE_HMP); g_Resource->SaveGameResource(file, GAME_RESOURCE_LMP); }
		NATIVE_DECL void SOUND_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_WAV); }
		NATIVE_DECL void MATERIAL_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_MTP); }
		NATIVE_DECL void OBJECT_INFO_SAVE(const char* file) { g_Resource->SaveGameResource(file, GAME_RESOURCE_QVM); }
		NATIVE_DECL void MEF_EXTRACT() { g_Resource->ExtractResourceFile(GAME_RESOURCE_MEF); }
		NATIVE_DECL bool IS_LOADED(const char* resource_file) { return NATIVE_INVOKE<bool>((Void)HASH::RESOURCE_IS_LOADED, resource_file, (int*)&resource_file); }
		NATIVE_DECL bool IS_LOADED(Resource& resource) { return IS_LOADED(resource.name.c_str()); }
		NATIVE_DECL int* LOAD(const char* resource_file, char** buffer) { return NATIVE_INVOKE<int*>((Void)HASH::RESOURCE_LOAD, resource_file, buffer); }
		NATIVE_DECL int* LOAD(const char* resource_file) { return LOAD(resource_file, NULL); }
		template <typename T>NATIVE_DECL void LOAD(T resource_files) { for (const auto& resource : resource_files) LOAD(resource.c_str()); }
		NATIVE_DECL void UNLOAD(const char* resource_file) { NATIVE_INVOKE<Void>((Void)HASH::RESOURCE_UNLOAD, resource_file); }
		template <typename T>NATIVE_DECL void UNLOAD(T resource_files) { for (const auto& resource : resource_files) if (IS_LOADED(resource.c_str())) UNLOAD(resource.c_str()); else LOG_ERROR("Resource '%s' cannot be loaded", resource.c_str()); }
		NATIVE_DECL void FLUSH(int resource_file) { NATIVE_INVOKE<Void>((Void)HASH::RESOURCE_FLUSH, resource_file); }
		NATIVE_DECL int PACK_UNPACK(const char* resource_file, char** buffer) { return NATIVE_INVOKE<int>((Void)HASH::RESOURCE_PACK_UNPACK, resource_file, buffer); }
		NATIVE_DECL int PACK_UNPACK(const char* resource_file) { return PACK_UNPACK(resource_file, NULL); }
		NATIVE_DECL void UNPACK(int* res_ptr, int res_addr, int res_size) { NATIVE_INVOKE<Void>((Void)HASH::RESOURCE_UNPACK, res_ptr, res_addr, res_size); }
		NATIVE_DECL address_t FIND(const char* resource_name) { return g_Resource->FindGameResource(resource_name); }
		NATIVE_DECL Resource FIND(string& resource_name) { return Resource(resource_name, g_Resource->FindGameResource(resource_name), 0); }
		NATIVE_DECL string MEF_FIND_MODEL_NAME(string& model_id) { return g_Resource->MEF_FindModelName(model_id); }
		NATIVE_DECL void MEF_REMOVE_MODEL(string model_id) { g_Resource->MEF_RemoveModel(model_id); }
		NATIVE_DECL void MEF_RESTORE_MODEL(string model_id) { g_Resource->MEF_RestoreModel(model_id); }
	}

	namespace QTASK {
		NATIVE_DECL void UPDATE() { NATIVE_INVOKE<Void>((Void)HASH::QTASK_UPDATE); }
		NATIVE_DECL void UPDATE_LIST(int* qtask_list) { NATIVE_INVOKE<Void>((Void)HASH::QTASK_UPDATE_LIST, qtask_list); }
		NATIVE_DECL void RESET() { NATIVE_INVOKE<Void>((Void)HASH::QHASH_RESET); }
		NATIVE_DECL void HASH_INIT(int hash_val) { NATIVE_INVOKE<Void>((Void)HASH::QHASH_INIT, hash_val); };
		NATIVE_DECL void HASH_VAL_SET() { NATIVE_INVOKE<Void>((Void)HASH::QHASH_VAL_SET); };
		NATIVE_DECL int	 HASH_VAL_GET() { return NATIVE_INVOKE<int>((Void)HASH::QHASH_VAL_GET); };
	}

	namespace QFILE {
		NATIVE_DECL void COMPILE(const char* qsc_file) { NATIVE_INVOKE<Void>((Void)HASH::QSC_COMPILE, qsc_file); }
		NATIVE_DECL void OPEN(const char* file, char* mode) { NATIVE_INVOKE<Void>((Void)HASH::FILE_OPEN, file, mode); }
	}

	namespace GFX {
		NATIVE_DECL void GRAPHICS_RESET() { NATIVE_INVOKE<Void>((Void)HASH::GRAPHICS_RESET, (const char*)local_buf); }
	}

	namespace SFX {
		NATIVE_DECL void MUSIC_ENABLE() { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_ENABLE, (const char*)local_buf); }
		NATIVE_DECL void MUSIC_DISABLE() { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_DISABLE, (const char*)local_buf); }
		NATIVE_DECL void VOLUME_UPDATE() { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_UPDATE_VOLUME, (const char*)local_buf); }
		NATIVE_DECL void VOLUME_SET(float volume) { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_VOLUME, volume, volume); }
		NATIVE_DECL void VOLUME_SFX_SET(float volume) { NATIVE_INVOKE<Void>((Void)HASH::MUSIC_SFX_VOLUME, volume); }
	}

	namespace MEMORY {
		NATIVE_DECL uint32_t ALLOC(int num, int size) { return NATIVE_INVOKE<uint32_t>((Void)HASH::MEMORY_ALLOC, num, size); }
		NATIVE_DECL void DEALLOC() { NATIVE_INVOKE<Void>((Void)HASH::MEMORY_DEALLOC); }
	}

	namespace MISSION {
		NATIVE_DECL void OPEN(char** ptr_mission) { NATIVE_INVOKE<Void>((Void)HASH::MISSION_OPEN, ptr_mission); }
	}
}