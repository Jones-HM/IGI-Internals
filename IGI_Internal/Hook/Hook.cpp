#include "Hook.hpp"
#include <HookDetours.hpp>
using namespace IGI;
using namespace Utility;

Hook::Hook() {
	g_Hook = this;
	Hook::Initialize();
}

Hook::Hook(bool createOnInit) {
	g_Hook = this;
	Hook::Initialize();
	if (createOnInit) {
		if (hook_status == MH_OK) {
			if (g_Hook->CreateHooks() == MH_OK) {
				g_Hook->EnableHooks();
			}
			else
				LOG_ERROR("Error occurred while creating hooks.");
		}
	}
}

Hook::~Hook() {
	g_Hook = nullptr;
	DisableHooks();
}

bool Hook::Initialize() {
	MH_STATUS mh_status = hook_status = MH_Initialize();
	if (mh_status != MH_OK)
	{
		LOG_ERROR("Minhook Init Error : %s", MH_StatusToString(mh_status));
		return false;
	}
	else if (mh_status == MH_OK)
		LOG_INFO("Minhook init success");
	return true;
}

MH_STATUS Hook::CreateHooks() {

	if (hook_status != MH_OK) return MH_ERROR_NOT_INITIALIZED;

	auto mh_status = MH_OK;// CreateHook(ParseConfig, &ParseConfigDetour, &ParseConfigOut);
	if (mh_status != MH_OK)LOG_ERROR("ParseConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(CreateConfig, &CreateConfigDetour, &CreateConfigOut);
	if (mh_status != MH_OK)LOG_ERROR("CreateConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(StatusMsg, &StatusMsgDetour, &StatusMsgOut);
	if (mh_status != MH_OK)LOG_ERROR("StatusMsg Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(GenericPickup, &GenericPickupDetour, &GenericPickupOut);
	if (mh_status != MH_OK)LOG_ERROR("GenericPickup Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(GunPickup, &GunPickupDetour, &GunPickupOut);
	if (mh_status != MH_OK)LOG_ERROR("GunPickup Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(AmmoPickup, &AmmoPickupDetour, &AmmoPickupOut);
	if (mh_status != MH_OK)LOG_ERROR("AmmoPickup Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(ShowError, &ShowErrorDetour, &ShowErrorOut);
	if (mh_status != MH_OK)LOG_ERROR("ShowError Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(ShowWarning, &ShowWarningDetour, &ShowWarningOut);
	if (mh_status != MH_OK) LOG_ERROR("ShowWarning Hooking error : %s", MH_StatusToString(mh_status));


	mh_status = MH_OK;//CreateHook(StatusMessageShow, &StatusMessageShowDetour, &StatusMessageShowOut);
	if (mh_status != MH_OK)LOG_ERROR("StatusMessageShow Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(SetGameDataSymbol, &SetGameDataSymbolDetour, &SetGameDataSymbolOut);
	if (mh_status != MH_OK)LOG_ERROR("SetGameDataSymbol Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(TextPrint, &PrintTextDetour, &TextPrintOut);
	if (mh_status != MH_OK)LOG_ERROR("TypeWriteBox Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(GameTextPrint, &GamePrintTextDetour, &GameTextPrintOut);
	if (mh_status != MH_OK)LOG_ERROR("TypeWriteBoxEx Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(ParseWeaponConfig, &ParseWeaponConfigDetour, &ParseWeaponConfigOut); 
	if (mh_status != MH_OK) LOG_ERROR("ParseWeaponConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(StartLevel, &StartLevelDetour, &StartLevelOut); 
	if (mh_status != MH_OK)LOG_ERROR("StartLevel Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(QuitLvl, &QuitLvlDetour, &QuitLvlOut); 
	if (mh_status != MH_OK)LOG_ERROR("QuitLvl Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(LoadResourceFile, &LoadResourceDetour, &LoadResourceOut);
	if (mh_status != MH_OK)LOG_ERROR("LoadResourceFile Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(LevelLoad, &LevelLoadDetour, &LevelLoadOut);
	if (mh_status != MH_OK)LOG_ERROR("LevelLoad Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(GameOpenFile, &GameOpenFileDetour, &GameOpenFileOut);
	if (mh_status != MH_OK)LOG_ERROR("GameOpenFile Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(LoadQVM, &LoadQVMDetour, &LoadQVMOut);
	if (mh_status != MH_OK)LOG_ERROR("LoadQVM Createhook error : %s", MH_StatusToString(mh_status));

	//mh_status = CreateHook(CompileQVM, &CompileQVMDetour, &CompileQVMOut); 
	//if (mh_status != MH_OK)
	// LOG_ERROR("CompileQVM Createhook error : %s", MH_StatusToString(mh_status)); 

	mh_status = CreateHook(AssembleQAS, &AssembleQASDetour, &AssembleQASOut);
	if (mh_status != MH_OK)LOG_ERROR("AssembleQAS Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(AmmoTypeOpen, &AmmoTypeOpenDetour, &AmmoTypeOpenOut);
	if (mh_status != MH_OK)LOG_ERROR("AmmoTypeOpen Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(WeaponTypeOpen, &WeaponTypeOpenDetour, &WeaponTypeOpenOut);
	if (mh_status != MH_OK)LOG_ERROR("WeaponTypeOpen Createhook error : %s", MH_StatusToString(mh_status));

	return mh_status;
}

MH_STATUS Hook::EnableHook(LPVOID p_target) {
	if (hook_status != MH_OK) return MH_ERROR_NOT_INITIALIZED;

	MH_STATUS mh_status = MH_EnableHook(p_target);
	if (mh_status == MH_OK)
		LOG_INFO("Hook enabled.");
	else
		LOG_ERROR("Error enabling Hook Reason: %s", MH_StatusToString(mh_status));
	return mh_status;
}

MH_STATUS Hook::DisableHook(LPVOID p_target) {
	if (MH_DisableHook(p_target) != MH_OK)
		LOG_ERROR("Error disabling hook.");
	else
		LOG_INFO("Hook disabled.");
}

MH_STATUS Hook::EnableHooks() {
	if (hook_status != MH_OK) return MH_ERROR_NOT_INITIALIZED;

	MH_STATUS mh_status = MH_EnableHook(MH_ALL_HOOKS);
	if (mh_status == MH_OK)
		LOG_INFO("All hooks enabled.");
	else
		LOG_ERROR("Error enabling hooks reason: %s", MH_StatusToString(mh_status));
	return mh_status;
}

MH_STATUS Hook::DisableHooks() {
	MH_STATUS status = MH_DisableHook(MH_ALL_HOOKS);
	if (status != MH_OK)
		LOG_ERROR("Error disabling hook.");
	else
		LOG_INFO("All Hooks disabled.");
	return status;
}