#pragma once 
#include "Common.hpp" 

namespace IGI {
	/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__ 
#define LINE_NO __LINE__ 
#define FUNC_NAME __func__ 
#endif 

//Defining Helper for Pointers. 
//Defining Helper for Pointers. 
#define READ_PTR(addr) *(PINT)addr 
#define READ_PTR_OFF(addr,off) (READ_PTR((addr + off)))
#define READ_PTR_OFF2(addr,off1,off2) *(PINT)(READ_PTR_OFF(addr,off1) + off2)
#define READ_PTR_OFF3(addr,off1,off2,off3) *(PINT)(READ_PTR_OFF2(addr,off1,off2) + off3)
#define READ_STATIC_PTR_OFF2(addr,off1,off2) READ_PTR_OFF(READ_PTR(addr),off1) + (DWORD)off2 
#define READ_STATIC_PTR_OFF3(addr,off1,off2,off3) READ_PTR_OFF2(READ_PTR(addr),off1,off2) + (DWORD)off3 

#define TYPEID(x) typeid(x).name()
#define TYPE(x) string(typeid(x).name())
#define CHECK_TYPE(x,y) TYPE(x) == string(y)
#define LINE_DEBUG LOG_FILE("[%s] LINE : %d", FUNC_NAME, LINE_NO); 

//Player profile constants. 
#define PLAYER_ACTIVE_MISSION_OFF  0x20 
#define PLAYER_NEXT_OFF 0x564 
#define PLAYER_NAME_SIZE 0x14 
#define PLAYER_BASE_ADDR 0xBC28C8 
#define PLAYER_ACTIVE_INDEX READ_PTR(0xBC2384)
//Get player active or index address. 
#define PLAYER_INDEX_ADDR(index) PLAYER_BASE_ADDR + ((index) * PLAYER_NEXT_OFF)
#define PLAYER_ACTIVE_ADDR PLAYER_INDEX_ADDR(PLAYER_ACTIVE_INDEX)

//Native constants. 
#define NATIVE_INVOKE g_NativeCaller->NativeInvoker 
#define GAME_LEVEL_MAX READ_PTR(0x5C89FC)

//Common Native constants. 
#define  NATIVE_CONST_CONFIG_FILE (LPCSTR)"LOCAL:config.qsc"
#define  NATIVE_CONST_MAINMENU_FILE (LPCSTR) "LOCAL:menusystem\\mainmenu.qsc"
#define  NATIVE_CONST_INMENU_FILE (LPCSTR) "LOCAL:menusystem\\ingame.qsc"
#define  NATIVE_CONST_WEAPON_CONFIG_FILE (LPCSTR) "LOCAL:weapons/weaponconfig.qsc"
#define STATUSSCREEN_AMMO (LPCSTR)"STATUSSCREEN:ammo.spr"
#define STATUSSCREEN_GUN (LPCSTR)"STATUSSCREEN:a_gun.spr"
#define STATUSSCREEN_WEAPON (LPCSTR)"STATUSSCREEN:weapon.spr"
#define STATUSSCREEN_NOTE (LPCSTR)"STATUSSCREEN:note.spr"
#define GUN_AMMO_PICKUP_PTR READ_PTR(READ_STATIC_PTR_OFF2(0x005BDC6C, 0x1B0, 0xCB4))

	static char local_buf[0x1E] = { NULL }; // Local buffer to store value from different methods. 
	const float MUSIC_CONST = 10.0f;
	const LPVOID status_byte = (LPVOID)0x567C74;

}