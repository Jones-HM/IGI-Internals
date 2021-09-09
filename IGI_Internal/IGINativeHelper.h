#pragma once
#include "IGINatives.h"
#include "IGINativeCaller.h"
using namespace NATIVE_HASH;

namespace NATIVE_HELPER {

//Native handler instance.
auto g_IGINativeCaller = IGINativeCaller::getInstance();

//Common Native constants.
#define IGI_NATIVE_CONFIG_FILE "LOCAL:config.qsc";
#define IGI_NATIVE_MAINMENU_FILE "LOCAL:menusystem\\mainmenu.qsc";
#define IGI_NATIVE_INGAME_MENU_FILE "LOCAL:menusystem\\ingame.qsc";
#define IGI_NATIVE_WEAPON_CONFIG_FILE "LOCAL:weapons/weaponconfig.qsc";


#define IGI_NATIVE_CONFIG_PARSE(configFile) g_IGINativeCaller->NativeCaller((int)HASH::CONFIG_PARSE_HASH,configFile);
#define IGI_NATIVE_CONFIG_CREATE(configFile) g_IGINativeCaller->NativeCaller((int)HASH::CONFIG_CREATE_HASH,configFile);
#define IGI_NATIVE_WEAPON_CONFIG_PARSE(configFile) g_IGINativeCaller->NativeCaller((int)HASH::WEAPON_PARSE_CONFIG_HASH,NULL,configFile);

#define IGI_NATIVE_GAME_QUIT *(PINT)0x005C8DE8 = 0;
#define IGI_NATIVE_GAME_LEVEL_SET(level) *(PINT)0x00539560 = level;
#define IGI_NATIVE_GAME_LEVEL_INC *(PINT)0x00539560 += 1;
#define IGI_NATIVE_GAME_LEVEL_DEC *(PINT)0x00539560 -= 1;
#define IGI_NATIVE_GAME_QUIT *(PINT)0x005C8DE8 = 0;
#define IGI_NATIVE_DISABLE_WARNINGS *(PINT)0x00936274 = 0;
#define IGI_NATIVE_DISABLE_ERRORS *(PINT)0x00936268 = 0;
}