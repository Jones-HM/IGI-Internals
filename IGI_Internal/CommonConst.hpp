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
#define READ_PTR(addr) *(PINT)addr 
#define READ_PTR_OFF(addr,off) (READ_PTR((addr + off)))
#define READ_PTR_OFF2(addr,off1,off2) *(PINT)(READ_PTR_OFF(addr,off1) + off2)
#define READ_PTR_OFF3(addr,off1,off2,off3) *(PINT)(READ_PTR_OFF2(addr,off1,off2) + off3)
#define READ_STATIC_PTR_OFF(addr,off) (READ_PTR(addr) + (DWORD)off)
#define READ_STATIC_PTR_OFF2(addr,off1,off2) READ_PTR_OFF(READ_PTR(addr),off1) + (DWORD)off2 
#define READ_STATIC_PTR_OFF3(addr,off1,off2,off3) READ_PTR_OFF2(READ_PTR(addr),off1,off2) + (DWORD)off3 

#define TYPE(x) string(typeid(x).name())
#define TYPEID(x) typeid(x).name()
#define TYPEID_S(x) (TYPE(x).length() < 2 || TYPE(x).empty()) ? "UnknownType" : TYPEID(x)

#define CHECK_TYPE(x,y) TYPE(x) == string(y)
#define LINE_DEBUG LOG_FILE("[%s] LINE : %d", FUNC_NAME, LINE_NO); 

//Player profile constants. 
#define PLAYER_ACTIVE_MISSION_OFF  0x0020 
#define PLAYER_NEXT_OFF 0x00564 
#define PLAYER_NAME_SIZE 0x0014 
#define PLAYER_BASE_ADDR 0x00BC28C8 
#define PLAYER_ACTIVE_INDEX READ_PTR(0x00BC2384)
//Get player active or index address. 
#define PLAYER_INDEX_ADDR(index) PLAYER_BASE_ADDR + ((index) * PLAYER_NEXT_OFF)
#define PLAYER_ACTIVE_ADDR PLAYER_INDEX_ADDR(PLAYER_ACTIVE_INDEX)

//Native constants. 
#define NATIVE_INVOKE g_NativeCaller.Invoke
#define GAME_LEVEL_MAX READ_PTR(0x005C89FC)

//Directory Constants.
#define GAME_LOCAL_DIR (const char*)R"(LOCAL:)"
#define GAME_MISSION_DIR (const char*)R"(MISSION:)"
#define GAME_WEAPONS_DIR (const char*)R"(WEAPONS:)"

//Common constants. 
#define GAME_CONST_CONFIG_FILE (const char*)R"(LOCAL:config.qsc)"
#define GAME_CONST_MAINMENU_FILE (const char*)R"(LOCAL:menusystem\mainmenu.qsc)"
#define GAME_CONST_INGAMEMENU_FILE (const char*)R"(LOCAL:menusystem\ingame.qsc)"
#define GAME_CONST_WEAPON_CONFIG_FILE (const char*)R"(LOCAL:weapons/weaponconfig.qsc)"
#define GAME_CONST_STATUSSCREEN_AMMO (const char*)R"(STATUSSCREEN:ammo.spr)"
#define GAME_CONST_STATUSSCREEN_GUN (const char*)R"(STATUSSCREEN:a_gun.spr)"
#define GAME_CONST_STATUSSCREEN_WEAPON (const char*)R"(STATUSSCREEN:weapon.spr)"
#define GAME_CONST_STATUSSCREEN_NOTE (const char*)R"(STATUSSCREEN:note.spr)"
#define GAME_CONST_FONT_BIG (const char*)R"(LOCAL:computer/font1.fnt)"
#define GAME_CONST_FONT_MEDIUM (const char*)R"(LOCAL:computer/font3.fnt)"
#define GAME_CONST_FONT_SMALL (const char*)R"(LOCAL:computer/font4.fnt)"
#define GAME_CONST_FONT_DEBUG (const char*)R"(LOCAL:debug.fnt)"

#define GUN_PICKUP_PTR (int*)0x0019F720
#define AMMO_PICKUP_PTR (int*)0x0019F820
#define GUN_PICKUP_SET(gun_id) *GUN_PICKUP_PTR = gun_id
#define AMMO_PICKUP_SET(ammo_id) *AMMO_PICKUP_PTR = ammo_id
#define WEAPON_ID_COUNT 45
#define AI_BUF_SIZE_HALF 0x001F40
#define AI_BUF_SIZE_FULL 0x007000
#define AI_COUNT_MAX 50
#define AI_ID_MAX 5000
#define AI_ID_INVALID -1

	//Base Address of Camera angle and position.
	inline const int32_t angle_base = 0x00BCAAE0,pos_base = 0x00BCAB08;

	//Datatypes for Human Soldier/Players.
	typedef uint16_t soldier_t;
	typedef int16_t ai_t;
	typedef int32_t graph_t;
	typedef uint32_t address_t;
	typedef std::vector<uint8_t> binary_t;

	enum MENU_SCREEN {
		MENU_SCREEN_MAINMENU = 3,
		MENU_SCREEN_INGAME = 4,
		MENU_SCREEN_RESTART = 5,
	};

	//Constants for Weapon/Ammo.
	enum GAME_AMMO {
		AMMO_ID_12,//Ammo type 12-Gauge, Type: Shotguns, Weapons: SPAS12/JACKHAMMER
		AMMO_ID_357,//Ammo type .357 Magnum, Type: Revolver , Weapons: Desert Eagle/Colt Anaconda. 
		AMMO_ID_556,//Ammo type 5.56x45mm, Type: AR/Heavy/LMG, Weapons: M16A2/MINIMI/APC/T80
		AMMO_ID_762,//Ammo type 7.62x39 mm, Type: Rifles, Weapons: AK47
		AMMO_ID_919,//Ammo type 9x19mm, Type: SMG, Weapons: GLOCK/UZI/UZIX2/MP5SD
		AMMO_ID_127,//12.7×99 mm NATO Type: HMG , Weapons: M2HB/MIL/SENTRY
		AMMO_ID_GRENADE,//Ammo type Custom, Type: Grenade , Weapons: Grenade.
		AMMO_ID_PROXIMITYMINE,//Ammo type Custom, Type: Grenade , Weapons: Proximitymine.
		AMMO_ID_M203,//Ammo type Custom, Type: Grenade Launcher , Weapons: M16A2.
		AMMO_ID_FLASHBANG,//Ammo type Custom, Type: Grenade , Weapons: Flashbang.
		AMMO_ID_44, //Ammo type .44 Magnum, Type: Revolver , Weapons: Colt Anaconda. {AMMO SLOT NOT USED}
		AMMO_ID_DRAGUNOV,//Ammo type 7.62×54mmR, Type: Sniper, Weapons: Dragunov
		AMMO_ID_MEDIPACK,//Ammo type Custom, Type: Syringe , Weapons: Medipack.
		AMMO_ID_1000,//Ammo type Custom, Type: Laucher , Weapons: LAW 80/RPG.
		AMMO_ID_NONE = -1 //Internally used. For Knife,Binoculars etc
	};

	enum GAME_WEAPON {
		WEAPON_ID_GLOCK = 1, //Weapon Type: Pistol.
		WEAPON_ID_DESERTEAGLE = 3,//Weapon Type: Pistol.
		WEAPON_ID_M16A2 = 4,//Weapon Type: Rifle.
		WEAPON_ID_AK47 = 5,//Weapon Type: Rifle.
		WEAPON_ID_UZI = 6,//Weapon Type: SMG.
		WEAPON_ID_MP5SD = 7,//Weapon Type: SMG.
		WEAPON_ID_SPAS12 = 8,//Weapon Type: Shotgun.
		WEAPON_ID_JACKHAMMER = 9,//Weapon Type: Shotgun.
		WEAPON_ID_MINIMI = 10,//Weapon Type: HMG (Machine Gun).
		WEAPON_ID_DRAGUNOV = 11,//Weapon Type: Sniper.
		WEAPON_ID_RPG18 = 12,//Weapon Type: Launcher.
		WEAPON_ID_UZIX2 = 13,//Weapon Type: SMG.
		WEAPON_ID_GRENADE = 14,//Weapon Type: Grenade.
		WEAPON_ID_FLASHBANG = 15,//Weapon Type: Grenade.
		WEAPON_ID_PROXIMITYMINE = 16,//Weapon Type: Grenade.
		WEAPON_ID_BINOCULARS = 18,//Weapon Type: Binoculars.
		WEAPON_ID_MEDIPACK = 19,//Weapon Type: MediPack.
		WEAPON_ID_KNIFE = 20,//Weapon Type: Knife.
		WEAPON_ID_COLT = 21,//Weapon Type: Revolver.
		WEAPON_ID_APC = 40,//Weapon Type: Launcher.
		WEAPON_ID_MIL = 41,//Weapon Type: HMG (Machine Gun).
		WEAPON_ID_M2HB = 42,//Weapon Type: HMG (Machine Gun).
		WEAPON_ID_T80 = 43,//Weapon Type: Launcher.
		WEAPON_ID_SENTRY = 44//Weapon Type: HMG (Machine Gun).
	};

	inline std::array<GAME_AMMO, WEAPON_ID_COUNT> weapons_ammo_list = {
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_919, //GLOCK
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_357, //Desert Eagle
	GAME_AMMO::AMMO_ID_556, //M16A2
	GAME_AMMO::AMMO_ID_762, //AK47
	GAME_AMMO::AMMO_ID_919, //UZI
	GAME_AMMO::AMMO_ID_919, //MP5SD 
	GAME_AMMO::AMMO_ID_12, //SPAS12
	GAME_AMMO::AMMO_ID_12, //JACKHAMMER 
	GAME_AMMO::AMMO_ID_556, //MINIMI
	GAME_AMMO::AMMO_ID_DRAGUNOV, //DRAGUNOV
	GAME_AMMO::AMMO_ID_1000, //RPG18
	GAME_AMMO::AMMO_ID_919, //UZIX2 
	GAME_AMMO::AMMO_ID_GRENADE, //GRENADE
	GAME_AMMO::AMMO_ID_FLASHBANG, //FLASHBANG
	GAME_AMMO::AMMO_ID_PROXIMITYMINE, //PROXIMITYMINE
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //BINOCULARS
	GAME_AMMO::AMMO_ID_MEDIPACK, //MEDIPACK
	GAME_AMMO::AMMO_ID_NONE, //KNIFE
	GAME_AMMO::AMMO_ID_357, //COLT
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_NONE, //Empty Slot
	GAME_AMMO::AMMO_ID_556, //APC
	GAME_AMMO::AMMO_ID_127, //MIL
	GAME_AMMO::AMMO_ID_127, //M2HB
	GAME_AMMO::AMMO_ID_556, //T80
	GAME_AMMO::AMMO_ID_127, //SENTRY
	};

}