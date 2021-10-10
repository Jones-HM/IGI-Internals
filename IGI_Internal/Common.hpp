#pragma once 

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers 
/*Defining WIN32 Constants*/
#undef WIN32_LEAN_AND_MEAN 
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

using std::string;
using std::vector;
using namespace std::chrono_literals;
inline HMODULE g_Hmodule{};
inline HANDLE g_Main_Thread{};
inline DWORD g_Main_Thread_Id{};

inline std::vector<std::thread> threads;
inline std::list<string> mef_files;
#define LOG_FILE_NAME std::string(PROJECT_NAME) + ".log"
#define NATIVES_FILE_NAME "IGI-Natives.json"

#define HEX_ADDR_FMT(addr) std::setw(8) << std::setfill('0') << std::uppercase << std::hex << addr 
#define HEX_ADDR_STR(addr) (static_cast<std::stringstream const&>(std::stringstream() << HEX_ADDR_FMT(addr))).str()

