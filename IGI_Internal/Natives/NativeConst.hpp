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
//#define NATIVE_INVOKE g_ThreadInvoker.Invoke
#define GAME_LEVEL_MAX READ_PTR(0x5C89FC)

//Directory Constants.
#define NATIVE_LOCAL_DIR (LPCSTR)"LOCAL:"
#define NATIVE_MISSION_DIR (LPCSTR)"MISSION:"
#define NATIVE_WEAPONS_DIR (LPCSTR)"WEAPONS:"

//Common Native constants. 
#define NATIVE_CONST_CONFIG_FILE (LPCSTR)"LOCAL:config.qsc"
#define NATIVE_CONST_MAINMENU_FILE (LPCSTR) "LOCAL:menusystem\\mainmenu.qsc"
#define NATIVE_CONST_INGAMEMENU_FILE (LPCSTR) "LOCAL:menusystem\\ingame.qsc"
#define NATIVE_CONST_WEAPON_CONFIG_FILE (LPCSTR) "LOCAL:weapons/weaponconfig.qsc"
#define NATIVE_CONST_STATUSSCREEN_AMMO (LPCSTR)"STATUSSCREEN:ammo.spr"
#define NATIVE_CONST_STATUSSCREEN_GUN (LPCSTR)"STATUSSCREEN:a_gun.spr"
#define NATIVE_CONST_STATUSSCREEN_WEAPON (LPCSTR)"STATUSSCREEN:weapon.spr"
#define NATIVE_CONST_STATUSSCREEN_NOTE (LPCSTR)"STATUSSCREEN:note.spr"

#define GUN_PICKUP_PTR (int*)0x19F720
#define AMMO_PICKUP_PTR (int*)0x19F820
#define GUN_PICKUP_SET(gun_id) *GUN_PICKUP_PTR = gun_id
#define AMMO_PICKUP_SET(ammo_id) *AMMO_PICKUP_PTR = ammo_id
#define WEAPON_ID_COUNT 45
#define AI_BUF_SIZE 8000

	inline char local_buf[0x1E] = { NULL }; // Local buffer to store value from different methods. 
	inline float MUSIC_CONST = 10.0f;
	inline LPVOID status_byte = (LPVOID)0x567C74;
	inline int gun_pickup_ptr = READ_STATIC_PTR_OFF2(0x005BDC6C, 0x1B0, 0xCB4);
	inline int humanplayer_ptr = READ_STATIC_PTR_OFF2(0x0056E210, 0x40, 0x24);
	
	//Structure to hold Human soldier information.
	struct HumanSoldier {
		string model_id;//Model id of soldier - Like 001_01_1 -> Jones.
		int ai_id; //A.I Id of soldier - Like Id '503' and AiFile = 'AI/503.qvm'
		int graph_id; //Graph Id of sodlier.
		string weapon; //Weapon currently holding.
		bool is_dead; //Is Soldier dead ?
		int e_team; //Soldier team type , 0 = Friendly, 1 = Enemy.
		int32_t address; //Address pointer of soldier.
	};

	inline std::vector<HumanSoldier> soldiers;

	//Constants for Weapon/Ammo.
	enum NATIVE_AMMO {
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

	enum NATIVE_WEAPON {
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


	inline std::array<NATIVE_AMMO, WEAPON_ID_COUNT> weapons_ammo_list = {
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_919, //GLOCK
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_357, //Desert Eagle
	NATIVE_AMMO::AMMO_ID_556, //M16A2
	NATIVE_AMMO::AMMO_ID_762, //AK47
	NATIVE_AMMO::AMMO_ID_919, //UZI
	NATIVE_AMMO::AMMO_ID_919, //MP5SD 
	NATIVE_AMMO::AMMO_ID_12, //SPAS12
	NATIVE_AMMO::AMMO_ID_12, //JACKHAMMER 
	NATIVE_AMMO::AMMO_ID_556, //MINIMI
	NATIVE_AMMO::AMMO_ID_DRAGUNOV, //DRAGUNOV
	NATIVE_AMMO::AMMO_ID_1000, //RPG18
	NATIVE_AMMO::AMMO_ID_919, //UZIX2 
	NATIVE_AMMO::AMMO_ID_GRENADE, //GRENADE
	NATIVE_AMMO::AMMO_ID_FLASHBANG, //FLASHBANG
	NATIVE_AMMO::AMMO_ID_PROXIMITYMINE, //PROXIMITYMINE
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //BINOCULARS
	NATIVE_AMMO::AMMO_ID_MEDIPACK, //MEDIPACK
	NATIVE_AMMO::AMMO_ID_NONE, //KNIFE
	NATIVE_AMMO::AMMO_ID_357, //COLT
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_NONE, //Empty Slot
	NATIVE_AMMO::AMMO_ID_556, //APC
	NATIVE_AMMO::AMMO_ID_127, //MIL
	NATIVE_AMMO::AMMO_ID_127, //M2HB
	NATIVE_AMMO::AMMO_ID_556, //T80
	NATIVE_AMMO::AMMO_ID_127, //SENTRY
	};
}