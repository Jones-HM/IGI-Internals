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
#define PLAYER_ACTIVE_MISSION_OFF  0x20 
#define PLAYER_NEXT_OFF 0x564 
#define PLAYER_NAME_SIZE 0x14 
#define PLAYER_BASE_ADDR 0xBC28C8 
#define PLAYER_ACTIVE_INDEX READ_PTR(0xBC2384)
//Get player active or index address. 
#define PLAYER_INDEX_ADDR(index) PLAYER_BASE_ADDR + ((index) * PLAYER_NEXT_OFF)
#define PLAYER_ACTIVE_ADDR PLAYER_INDEX_ADDR(PLAYER_ACTIVE_INDEX)

//Native constants. 
#define NATIVE_INVOKE g_NativeCaller.Invoke
//#define IGI_INVOKE g_ThreadInvoker.Invoke
#define GAME_LEVEL_MAX READ_PTR(0x5C89FC)

//Directory Constants.
#define IGI_LOCAL_DIR (const char*)R"(LOCAL:)"
#define IGI_MISSION_DIR (const char*)R"(MISSION:)"
#define IGI_WEAPONS_DIR (const char*)R"(WEAPONS:)"

//Common constants. 
#define IGI_CONST_CONFIG_FILE (const char*)R"(LOCAL:config.qsc)"
#define IGI_CONST_MAINMENU_FILE (const char*)R"(LOCAL:menusystem\mainmenu.qsc)"
#define IGI_CONST_INGAMEMENU_FILE (const char*)R"(LOCAL:menusystem\ingame.qsc)"
#define IGI_CONST_WEAPON_CONFIG_FILE (const char*)R"(LOCAL:weapons/weaponconfig.qsc)"
#define IGI_CONST_STATUSSCREEN_AMMO (const char*)R"(STATUSSCREEN:ammo.spr)"
#define IGI_CONST_STATUSSCREEN_GUN (const char*)R"(STATUSSCREEN:a_gun.spr)"
#define IGI_CONST_STATUSSCREEN_WEAPON (const char*)R"(STATUSSCREEN:weapon.spr)"
#define IGI_CONST_STATUSSCREEN_NOTE (const char*)R"(STATUSSCREEN:note.spr)"
#define IGI_CONST_FONT_BIG (const char*)R"(LOCAL:computer/font1.fnt)"
#define IGI_CONST_FONT_MEDIUM (const char*)R"(LOCAL:computer/font3.fnt)"
#define IGI_CONST_FONT_SMALL (const char*)R"(LOCAL:computer/font4.fnt)"
#define IGI_CONST_FONT_DEBUG (const char*)R"(LOCAL:debug.fnt)"

#define GUN_PICKUP_PTR (int*)0x19F720
#define AMMO_PICKUP_PTR (int*)0x19F820
#define GUN_PICKUP_SET(gun_id) *GUN_PICKUP_PTR = gun_id
#define AMMO_PICKUP_SET(ammo_id) *AMMO_PICKUP_PTR = ammo_id
#define WEAPON_ID_COUNT 45
#define AI_BUF_SIZE_HALF 0x1F40
#define AI_BUF_SIZE_FULL 0x7000
#define AI_COUNT_MAX 50
#define AI_ID_MAX 5000
#define AI_ID_INVALID -1

//Datatypes for Human Soldier/Players.
typedef uint16_t soldier_t;
typedef int16_t ai_t;
typedef int32_t graph_t;
typedef uint32_t address_t;
typedef std::vector<uint8_t> binary_t;

	inline char local_buf[0x1E] = { NULL }; // Local buffer to store value from different methods. 
	inline LPVOID status_byte = (LPVOID)0x567C74;
	inline int gun_pickup_ptr = READ_STATIC_PTR_OFF2(0x005BDC6C, 0x1B0, 0xCB4);
	inline int humanplayer_ptr = READ_STATIC_PTR_OFF2(0x0056E210, 0x40, 0x24);
	inline int menu_screen_ptr = READ_STATIC_PTR_OFF(0x00567C8C, 0x28);
	inline int g_game_level = 1,g_curr_level = 1,g_menu_screen = 0;
	inline bool	g_level_changed = false;

	enum MENU_SCREEN {
		MENU_SCREEN_MAINMENU = 3,
		MENU_SCREEN_INGAME = 4,
		MENU_SCREEN_RESTART = 5,
	};

	//Constants for Weapon/Ammo.
	enum IGI_AMMO {
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

	enum IGI_WEAPON {
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

	inline std::array<IGI_AMMO, WEAPON_ID_COUNT> weapons_ammo_list = {
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_919, //GLOCK
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_357, //Desert Eagle
	IGI_AMMO::AMMO_ID_556, //M16A2
	IGI_AMMO::AMMO_ID_762, //AK47
	IGI_AMMO::AMMO_ID_919, //UZI
	IGI_AMMO::AMMO_ID_919, //MP5SD 
	IGI_AMMO::AMMO_ID_12, //SPAS12
	IGI_AMMO::AMMO_ID_12, //JACKHAMMER 
	IGI_AMMO::AMMO_ID_556, //MINIMI
	IGI_AMMO::AMMO_ID_DRAGUNOV, //DRAGUNOV
	IGI_AMMO::AMMO_ID_1000, //RPG18
	IGI_AMMO::AMMO_ID_919, //UZIX2 
	IGI_AMMO::AMMO_ID_GRENADE, //GRENADE
	IGI_AMMO::AMMO_ID_FLASHBANG, //FLASHBANG
	IGI_AMMO::AMMO_ID_PROXIMITYMINE, //PROXIMITYMINE
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //BINOCULARS
	IGI_AMMO::AMMO_ID_MEDIPACK, //MEDIPACK
	IGI_AMMO::AMMO_ID_NONE, //KNIFE
	IGI_AMMO::AMMO_ID_357, //COLT
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_NONE, //Empty Slot
	IGI_AMMO::AMMO_ID_556, //APC
	IGI_AMMO::AMMO_ID_127, //MIL
	IGI_AMMO::AMMO_ID_127, //M2HB
	IGI_AMMO::AMMO_ID_556, //T80
	IGI_AMMO::AMMO_ID_127, //SENTRY
	};
}