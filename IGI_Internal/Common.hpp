#pragma once 
#include <windows.h> 
#include <iostream> 
#include <thread> 
#include <chrono> 
#include <vector> 
#include <functional> 
#include <string> 
#include <map> 
#include <list> 
#include <memory> 
#include <type_traits> 
#include <filesystem> 
#include <ostream> 
#include <fstream> 
#include <sstream> 
#include <iterator> 
#include <algorithm> 
#include <iomanip> 
#include <array> 
#include <regex>
#include <tuple>
#include <set>
#include "CommonConst.hpp"

using std::string;
using std::vector;
namespace fs = std::filesystem;
using namespace std::chrono_literals;
inline HMODULE g_Hmodule{};
inline HANDLE g_Main_Thread{};
inline DWORD g_Main_Thread_Id{};

inline char local_buf[0x1E] = { NULL }; // Local buffer to store value from different methods. 
inline void* status_byte_addr = (void*)0x00567C74;
inline byte status_byte = *(byte*)status_byte_addr;
inline int gun_pickup_ptr = READ_STATIC_PTR_OFF2(0x005BDC6C, 0x1B0, 0xCB4);
inline int humanplayer_ptr = READ_STATIC_PTR_OFF2(0x0056E210, 0x40, 0x24);
inline int menu_screen_ptr = READ_STATIC_PTR_OFF(0x00567C8C, 0x28);
inline int g_game_level = 1, g_curr_level = 1, g_menu_screen = 0;
inline bool	g_level_changed = false;

#define LOGGER_FILE string(PROJECT_NAME) + ".log"
#define NATIVES_FILE "IGI-Natives.json"
#define MEF_MODELS_FILE "IGI-Models.txt";

#define HEX_ADDR_FMT(addr) std::setw(8) << std::setfill('0') << std::uppercase << std::hex << addr 
#define HEX_ADDR_STR(addr) (static_cast<std::stringstream const&>(std::stringstream() << HEX_ADDR_FMT(addr))).str()

