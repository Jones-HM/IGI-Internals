#pragma once
#define SOLDIER_DATA_ALL
#include "Common.hpp"
#include "Logger.hpp"
#include "Utils/DbgHelper.hpp"
#include "GTLibc.hpp"
#include "DbgHelper.hpp"
#include "CommonConst.hpp"
#include "Natives/NativeHelper.hpp"
#include "HumanSoldier.hpp"
#include "Utils/Utility.hpp"

using namespace IGI;

auto ParseConfig = (int(__cdecl*)(char* q_file))0x405850;
auto ParseWeaponConfig = (int(__cdecl*)(int index, char* cfg_file))0x4071E0;
auto CompileConfig = (int(__cdecl*)(int a1, char* q_file, int a3))0x4F0CF0;
auto CreateConfig = (int(__cdecl*)(char* file_name))0x405BD0;
auto LevelLoad = (int(__cdecl*)(int level_addr, int level_len, int param3, int param4))0x4F0E10;
auto LoadLevelMenu = (void(__cdecl*)(void))0x416fe0;
auto LoadResourceFile = (int* (__cdecl*)(const char* res_file, char** buffer))0x4B5F00;
auto MenuManager = (int(__cdecl*)(int a1, char* a2, char a3, char a4, int a5))0x418B00;
auto QuitLevel = (int(__cdecl*)(int param))0x416550;
auto QuitLevelParam = (int(__cdecl*)(unsigned int a1, int a2, int a3))0x401530;
auto RemoveSymbol = (char* (__cdecl*)(int* a1, char symbol))0x4C0460;
auto ScriptInit = (char(__cdecl*)(int a1, char a2, int a3, char a4))0x4F0E50;
auto Script_SetSymbolContext = (int(__cdecl*)(char symbol_name, int symbol_buf))0x4B8930;
auto SetFramesVar = (void(__cdecl*)(int frame_count))0x402820;
auto StartLevel = (int(__cdecl*)(int, int, int, int))0x415B30;
auto StatusMsg = (int(__cdecl*)(int send_status, const char* buffer, const char* msg_sprite, const char* v6))0x485970;
auto TaskTypeSet = (int(__cdecl*)(int* task_name, int task_param))0x4B8810;
auto UnLoadResource = (int(__cdecl*)(char* res_file))0x4B6380;
auto UpdateQTaskList = (int(__cdecl*)(int a1))0x401B20;
auto ResourceLoad = (int* (__cdecl*)(const char*, char**))0x004B5F00;

decltype(ParseConfig) ParseConfigOut;
decltype(ParseWeaponConfig) ParseWeaponConfigOut;
decltype(CreateConfig) CreateConfigOut;
decltype(SetFramesVar) SetFramesVarOut;
decltype(StartLevel) StartLevelOut;
decltype(QuitLevel) QuitLevelOut;
decltype(LevelLoad) LevelLoadOut;
decltype(LoadResourceFile) LoadResourceOut;
decltype(StatusMsg) StatusMsgOut;

auto CompileQVM = (void(__cdecl*)(char*))0x4B85B0;
decltype(CompileQVM) CompileQVMOut;

auto GameOpenFile = (FILE * (__cdecl*)(char*, char*))0x4A5350;
decltype(GameOpenFile) GameOpenFileOut;

auto LoadQVM = (int* (__cdecl*)(LPCSTR))0x4B80B0;
decltype(LoadQVM) LoadQVMOut;

auto AssembleQAS = (int(__cdecl*)(char*, char*))0x4BB270;
decltype(AssembleQAS) AssembleQASOut;

auto AmmoTypeOpen = (void(__cdecl*)(int))0x47CA40;
decltype(AmmoTypeOpen) AmmoTypeOpenOut;

auto WeaponTypeOpen = (void(__cdecl*)(int))0x413A90;
decltype(WeaponTypeOpen) WeaponTypeOpenOut;

auto LoadGameData = (int(__cdecl*)(char*, const char*, const char*, int))0x4A53B3;
decltype(LoadGameData) LoadGameDataOut;

auto SetGameDataSymbol = (int(__cdecl*)(char*))0x4B80A0;
decltype(SetGameDataSymbol) SetGameDataSymbolOut;

auto TextPrint = (void(__cdecl*)(int*, char*, int, int))0x004B77F0;
decltype(TextPrint) TextPrintOut;

auto GameTextPrint = (void(__cdecl*)(int**, char*))0x4B6E90;
decltype(GameTextPrint) GameTextPrintOut;

auto StatusMessageShow = (int(__cdecl*)(int, int**, int*, int, int, int))0x485DB0;
decltype(StatusMessageShow) StatusMessageShowOut;

auto GunPickup = (void(__cdecl*)(int, int*))0x45FFC0;
auto AmmoPickup = (void(__cdecl*)(int, int*))0x45FF80;
auto GenericPickup = (void(__cdecl*)(int*))0x46C3C0;
decltype(GunPickup) GunPickupOut;
decltype(AmmoPickup) AmmoPickupOut;
decltype(GenericPickup) GenericPickupOut;

auto ShowError = (void(__cdecl*)(LPCSTR))0x4AF7B0;
decltype(ShowError) ShowErrorOut;

auto ShowWarning = (void(__cdecl*)(LPCSTR))0x4AF810;
decltype(ShowWarning) ShowWarningOut;

auto GetHumanHit = (uint32_t(__cdecl*)(void))0x00416d80;
decltype(GetHumanHit) GetHumanHitOut;

auto SoldierViewCam = (uint32_t(__cdecl*)(int, int))0x00463760;
decltype(SoldierViewCam) SoldierViewCamOut;

auto SFXItems = (int* (__cdecl*)(int, char*))0x004E6B00;
decltype(SFXItems) SFXItemsOut;

auto HumanSoldierHit = (void(__cdecl*)(int, char*, int))0x004637C0;
decltype(HumanSoldierHit) HumanSoldierHitOut;

auto HumanSoldierDead = (void(__cdecl*)(int, char*, int))0x004638A0;
decltype(HumanSoldierDead) HumanSoldierDeadOut;

auto WeaponDrop = (void(__cdecl*)(int**))0x0046CEA0;
decltype(WeaponDrop) WeaponDropOut;

auto SoldierDead = (void(__cdecl*)(int, int))0x0045C440;
decltype(SoldierDead) SoldierDeadOut;

auto DebugPlayerData = (void(__cdecl*)(int, char*))0x004E7BD0;
decltype(DebugPlayerData) DebugPlayerDataOut;

auto DebugSoldierData = (void(__cdecl*)(int, char*))0x00460C80;
decltype(DebugSoldierData) DebugSoldierDataOut;

auto DbgPrint = (void(__cdecl*)(void))0x004E7840;
decltype(DbgPrint) DbgAllocOut;

void ReadWholeFile(LPCSTR file_name, LPCSTR file_mode = "rb") {
	LOG_WARNING("%s File : %s Mode : %s", FUNC_NAME, file_name, file_mode);
	try {
		if (string(file_mode) == "rb") {
			auto file_string = string(file_name);
			std::ifstream ifs(file_name, std::ios::binary);
			if (ifs.good()) {
				std::ofstream ofs(file_string.insert(file_string.length() - 4, "_open"), std::ios::binary);
				std::vector<byte> buffer(std::istreambuf_iterator<char>(ifs), {});
				std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<char>(ofs, ""));
			}
			else
			{
				LOG_WARNING("%s File : %s doesn't exist", FUNC_NAME, file_name);
			}
		}
	}
	catch (const std::exception& ex) {
		std::cout << "Exception: " << ex.what() << std::endl;
	}
}



void DbgAllocDetour(void) {
	LOG_INFO("%s called", "DbgAlloc");
}

void DebugPlayerDataDetour(int soldier_addr, char* event_type) {

}

int new_addr = 0, ai_count = 0, new_level = 1;

void __cdecl DebugSoldierDataDetour2(int soldier_addr, char* event_type) {

	if (soldiers.size() > 0) {
		for (auto& soldier : soldiers) {
			//auto soldier = soldiers.at(0);
			if (soldier_addr == soldier.GetAddress()) {
				//soldier.PrintSoldierData();
				//LOG_CONSOLE("%s Addr: %p Expr: '%s' Event: '%s'", "", soldier_addr, (char*)(soldier_addr + 0x2EC + 0x107C), event_type);
				return DebugSoldierDataOut(soldier_addr, event_type);
			}
		}
	}

	//void* ai_addr = reinterpret_cast<void*>(soldier_addr + 0x100);
	//string ai_model_str(8, NULL);
	//std::memcpy(ai_model_str.data(), ai_addr, 8);

	//if (ai_model_str == "003_01_1") {
	//	soldier_t id = 2004;
	//	
	//	string ai_expr(20,'\0');
	//	void* addr = (void*)(soldier_addr + 0x100 + 0x1308);

	//	std::strcpy(ai_expr.data(),(char*)addr);
	//	//LOG_FILE("ai_expr %s\n", ai_expr.c_str());
	//	
	//	auto find_human_ai_id = [](string s)-> uint32_t {
	//		string id; for (const auto& c : s) if (::isdigit(c)) id += c;
	//		return std::stoi(id);
	//	};

	//	if (ai_expr.find("HumanSoldier_") != string::npos) {
	//		auto ai_id = find_human_ai_id(ai_expr);
	//		if (ai_id == id) {
	//			DebugSoldierDataOut(soldier_addr, event_type);
	//		}
	//	}
	//}
}

void DebugSoldierDataDetour(int soldier_addr, char* event_type) {

	//Vector to hold AI data.
	std::vector<uint8_t> ai_data(AI_BUF_SIZE_HALF, '\0');


#ifdef SOLDIER_DATA_ALL
	ai_data.resize(AI_BUF_SIZE_FULL);
#endif

	if (g_level_changed) {
		new_addr = ai_count = 0;
		soldiers.clear();
	}

	if (ai_count == AI_COUNT_MAX) {
		LOG_CONSOLE("Total A.I count: %d for Level: %d", soldiers.size(), new_level);
		ai_count++;
		return;
	}

	if (soldier_addr != new_addr && ai_count <= AI_COUNT_MAX) {
		//LOG_FILE("%s soldier_addr: %p Expr: '%s' AiEvent: '%s'", "DebugSoldierData", soldier_addr, (char*)(soldier_addr + 0x2EC + 0x107C), event_type);

		void* ai_addr = reinterpret_cast<void*>(soldier_addr + 0x100);

		HumanSoldier soldier;
		std::memcpy(ai_data.data(), ai_addr, ai_data.capacity());
		//Add soldier data information.
		soldier.AddSoldierData(soldier_addr, ai_data);
		new_addr = soldier_addr;
		ai_count++;
		DebugSoldierDataOut(soldier_addr, event_type);
	}
	new_level = g_curr_level;
}

void SoldierDeadDetour(int param_1, int param_2) {
	LOG_FILE("%s-->> param_1: %p param_2: %p", "SoldierDead", param_1, param_2);
	//g_DbgHelper->StackTrace(true);
	SoldierDeadOut(param_1, param_2);
}

void WeaponDropDetour(int** param_1) {

	int entity_dead = 0;
	//__asm {
	//	fld[esi + 0x100]
	//	fcomp[esi + 0x118]
	//	fnstsw ax
	//	test ah, 0x01
	//	je MY_LBL
	//	MY_LBL :
	//	mov entity_dead, 1
	//}

	if (*(double*)(param_1 + 0x40) < *(double*)(param_1 + 0x46)) entity_dead = 1;

	if (entity_dead)
	{
		LOG_CONSOLE("%s param_1: %p", "WeaponDrop", param_1);
		LOG_CONSOLE("%s p_100: %f p_118: %f p_120 : %f", "WeaponDrop", *(double*)(param_1 + 0x100), *(double*)(param_1 + 0x118), *(double*)(param_1 + 0x120));
	}
	WeaponDropOut(param_1);
}


void HumanSoldierHitDetour(int address, char* param_2, int param_3) {
	//LOG_CONSOLE("%s address: %p param_2: '%s' param_3 : %d", "SoldierHit", address, param_2, param_3);
	//soldiers.clear();

#define SOLDIER_DATA_ALL
	//Vector to hold AI data.
	std::vector<uint8_t> ai_data(AI_BUF_SIZE_HALF, '\0');

#ifdef SOLDIER_DATA_ALL
	ai_data.resize(AI_BUF_SIZE_FULL);
#endif

	void* ai_address = reinterpret_cast<void*>(address + 0x100);
	HumanSoldier soldier;
	string ai_data_info;

	std::thread th{ [&]() {
		std::memcpy(ai_data.data(), ai_address, ai_data.capacity());
		std::string soldier_data(ai_data.begin(), ai_data.end());
		//WriteFileType(std::string(g_Utility.GetModuleFolder() + "\\" + "ai_data.dat"), soldier_data, BINARY_FILE);
		//Add soldier data information.
	soldier.AddSoldierData(address,ai_data);
	soldier.PrintSoldierData();
		} };
	th.join();

	HumanSoldierHitOut(address, param_2, param_3);
}

void HumanSoldierDeadDetour(int param_1, char* param_2, int param_3) {
	LOG_CONSOLE("%s param_1: %p param_2: '%s' param_3 : %d", "SoldierDead", param_1, param_2, param_3);
	//g_DbgHelper->StackTrace(true);
	HumanSoldierDeadOut(param_1, param_2, param_3);
}

int* __cdecl SFXItemsDetour(int param_1, char* sfx_item) {
	auto ret_val = SFXItemsOut(param_1, sfx_item);
	LOG_FILE("%s param_1: %p ret_val : %p sfx_item: %s", "SFXItems", param_1, ret_val, sfx_item);
	//const string log_file = GetModuleFolder() + "\\" + LOG_FILE_NAME;
	//auto file_data =g_Utility.ReadFile(log_file);

	//if (file_data.find(sfx_item) == std::string::npos)
	//	LOG_FILE("\"%s\",", sfx_item);

	return ret_val;
}

void SoldierViewCamDetour(int param_1, int param_2) {
	LOG_INFO("%s param_1: %p param_2: %d", "SoldierViewCam", param_1, param_2);
	SoldierViewCamOut(param_1, param_2);
}

uint32_t GetHumanHitDetour(void) {
	uint32_t xp_hit = GetHumanHitOut();
	LOG_INFO("%s xp_hit: %p", "GetHumanHit", xp_hit);
	return xp_hit;
}

void ShowWarningDetour(LPCSTR warn_msg) {
	LOG_FILE("%s warn_msg: %s", "ShowWarning", warn_msg);
	ShowWarningOut(warn_msg);
}

void ShowErrorDetour(LPCSTR err_msg) {
	LOG_FILE("%s err_msg: %s", "ShowError", err_msg);
	ShowErrorOut(err_msg);
}

void GenericPickupDetour(int* param_1) {
	if ((*(char*)(param_1 + 0x119) != '\0') && (*(char*)(param_1 + 0x11a) != '\0'))
		LOG_FILE("%s param_1: %p *param_1: %p param_1_handler: '%s'", "GenericPickup", param_1, *param_1, *(param_1 + 0x1A0));
	GenericPickupOut(param_1);
}

void GunPickupDetour(int param_1, int* param_2) {
	LOG_FILE("%s param_1: %p param_2: %p Weapon_Id : %d", "GunPickup", param_1, param_2, *param_2);
	GunPickupOut(param_1, param_2);
}

void AmmoPickupDetour(int param_1, int* param_2) {
	LOG_FILE("%s param_1: %p param_2: %p Ammo_Id : %d", "AmmoPickup", param_1, param_2, *param_2);
	AmmoPickupOut(param_1, param_2);
}

void StatusMessageShowDetour(int param_1, int** param_2, int* param_3, int param_4, int param_5, int param_6) {
	LOG_FILE("%s param_1 : %p param_2 : %p param_3 : %p param_4 : %p param_5 : %p param_6 : %p", "StatusMessageShow", param_1, param_2, param_3, param_4, param_5, param_6);
	Sleep(100);
	char* text = nullptr;

	__asm {
		lea ebx, [esi + 0x10]
		shl edx, 0x0A
		sub eax, ecx
		lea edi, [esp + edx * 0x1 + 0x28]
		mov text, edi
	}
	LOG_FILE("%s text: %s", "StatusMessageShow", text);
	g_DbgHelper->StackTrace(true);
	StatusMessageShowOut(param_1, param_2, param_3, param_4, param_5, param_6);
}

void __cdecl GamePrintTextDetour(int** param_1, char* param_2) {
	LOG_CONSOLE("%s p1 : %p *p1 : %p **p1 : '%s' p2: '%s'", "Print", param_1, *param_1, READ_PTR(param_1 + 0x180), param_2);
	//g_DbgHelper->StackTrace(true);
	GameTextPrintOut(param_1, param_2);
}

void __cdecl TextPrintDetour(int* param_1, char* param_2, int param_3, int param_4) {
	//LOG_FILE("%s called...", __FUNCTION__);
	LOG_CONSOLE("%s param_1 : %p param_2 : '%s' param_3 : %p param_4 : %p", "TextPrint", param_1, param_2 == NULL ? "" : param_2, param_3, param_4);
	//g_DbgHelper->StackTrace(true);
	TextPrintOut(param_1, param_2, param_3, param_4);
}

int __cdecl SetGameDataSymbolDetour(char* symbol_file) {
	LOG_FILE("%s : symbol_file : %s", "SetGameDataSymbol", symbol_file);
	Sleep(100);
	int ret_val = SetGameDataSymbolOut(symbol_file);
	g_DbgHelper->StackTrace(true);
	return ret_val;
}

int __cdecl LoadGameDataDetour(char* res_buf, const char* res_path, const char* res_name, int res_ptr) {
	//LOG_FILE("%s res_buf: '%s' res_path: '%s' res_name: '%s' res_ptr: %p", "LoadGameData", res_buf, res_path, res_name, res_ptr);
	//ReadWholeFile(res_buf);
	//if (std::string(res_buf).find(".mef") != std::string::npos)
	//	mef_files.push_back(res_name);
	return LoadGameDataOut(res_buf, res_path, res_name, res_ptr);
}

int* __cdecl LoadResourceDetour(char* file_name, char** param2) {
	int* res_addr = LoadResourceOut(file_name, param2);
	//LOG_FILE("%s File '%s'\t Address %p", "LoadResource", file_name, (int)res_addr);
	//ReadWholeFile(file_name);
	if (std::string(file_name).find(".mef") != std::string::npos)
		mef_files.push_back(file_name);

	return res_addr;
}

int __cdecl  StatusMsgDetour(int send_status, const char* buffer, const char* msg_sprite, const char* status_byte) {
	LOG_FILE("%s send_status : %p buffer : '%s' msg_sprite : '%s' status_byte : %p", FUNC_NAME, send_status, buffer, msg_sprite, status_byte);
	Sleep(100);
	return StatusMsgOut(send_status, buffer, msg_sprite, status_byte);
}

int __cdecl ParseConfigDetour(char* q_file) {
	LOG_FILE("%s : q_file : %s", FUNC_NAME, q_file);
	Sleep(100);
	int ret_val = ParseConfigOut(q_file);
	//g_DbgHelper->StackTrace(true, false, true);
	return ret_val;
}

int __cdecl CreateConfigDetour(char* q_file) {
	LOG_FILE("%s : q_file : %s", FUNC_NAME, q_file);
	Sleep(100);
	int ret_val = CreateConfigOut(q_file);

	//g_DbgHelper->StackTrace(true, false, false);
	return ret_val;
}

int __cdecl ParseWeaponConfigDetour(int index, char* cfg_file) {
	LOG_FILE("%s index : %d cfg_file : %s", FUNC_NAME, index, cfg_file);
	Sleep(100);
	return ParseWeaponConfigOut(index, cfg_file);
}

void __cdecl SetFramesDetour(int frames) {
	LOG_FILE("%s : Frames : %d", FUNC_NAME, frames);
	Sleep(100);
	return SetFramesVarOut(frames);
}

int __cdecl  StartLevelDetour(int param1, int param2, int param3, int param4) {
	LOG_FILE("%s param1 : %p param2 : %p param3 : %p param4 : %p", FUNC_NAME, param1, param2, param3, param4);
	Sleep(100);
	g_DbgHelper->StackTrace(true, false, true);
	return StartLevelOut(param1, param2, param3, param4);
}

int __cdecl  QuitLvlDetour(int param1, int param2, int param3, int param4) {
	LOG_FILE("%s param1 : %p param2 : %p param3 : %p param4 : %p", FUNC_NAME, param1, param2, param3, param4);
	Sleep(100);
	return QuitLevelOut(param1);
}

void __cdecl AmmoTypeOpenDetour(int param1) {
	LOG_INFO("%s param1 : %p", FUNC_NAME, param1);
	AmmoTypeOpenOut(param1);
}

void __cdecl WeaponTypeOpenDetour(int param1) {
	LOG_INFO("%s param1 : %p", FUNC_NAME, param1);
	WeaponTypeOpenOut(param1);
}

int __cdecl AssembleQASDetour(char* file_out, char* file_in) {
	int qas_ret = AssembleQASOut(file_out, file_in);
	LOG_INFO("%s file_in : %s file_out : %s  Status : %d", "AssembleQAS", file_in, file_out, qas_ret);
	return qas_ret;
}

void __cdecl CompileQVMDetour(char* file_name) {
	LOG_INFO("%s file_name : %s", "CompileQVM", file_name);

	*(PDWORD64)(0x0201B1) += 1;
	strcpy((char*)0x943606, file_name);
	Sleep(100);
	CompileQVMOut(file_name);
}

int* __cdecl LoadQVMDetour(LPCSTR file_name) {
	LOG_CONSOLE("%s file_name : %s", "LoadQVM", file_name);

	//auto CompileQVM = (int(__cdecl*)(int))0x4B85B0;
	//auto CompileCleanUp = (int(__cdecl*)(int*))0x4B83D0;
	auto qvm_file = LoadQVMOut(file_name);

	//ReadWholeFile(file_name); 
	return qvm_file;
}

FILE* __cdecl GameOpenFileDetour(char* file_name, char* file_mode) {
	//if (string(file_mode) == "wb")
	LOG_CONSOLE("%s File : '%s' Mode : '%s'", "OpenFile", file_name, file_mode);

	//ReadWholeFile(file_name, file_mode); 
	//g_DbgHelper->StackTrace(true, false, true);

	Sleep(100);
	return GameOpenFileOut(file_name, file_mode);
}

int __cdecl LevelLoadDetour(int param1, int param2, int param3, int param4) {
	LOG_INFO("%s param1 : %s param2 : %p param3 : %p  param4 : %p", "LevelLoad", param1, param2, param3, param4);
	Sleep(100);
	g_DbgHelper->StackTrace(true, false, true);
	return LevelLoadOut(param1, param2, param3, param4);
}
