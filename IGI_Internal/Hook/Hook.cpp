#include "Hook.hpp"
#include <HookDetours.hpp>
using namespace IGI;
using namespace IGI;

Hook::Hook() {
	g_Hook = this;
	Hook::Initialize();
}

Hook::Hook(bool createOnInit) {
	g_Hook = this;
	Hook::Initialize();
	if (createOnInit) {
		if (hook_status == MH_OK) {
			if (CreateHooks() == MH_OK) {
				EnableHooks();
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
	MH_STATUS mh_status;

	mh_status = MH_OK;//CreateHook(CameraUpdate, &CameraUpdateDetour, &CameraUpdateOut);
	if (mh_status != MH_OK)LOG_ERROR("CameraUpdate Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;// CreateHook(ParseConfig, &ParseConfigDetour, &ParseConfigOut);
	if (mh_status != MH_OK)LOG_ERROR("ParseConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(CreateConfig, &CreateConfigDetour, &CreateConfigOut);
	if (mh_status != MH_OK)LOG_ERROR("CreateConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(StatusMsg, &StatusMsgDetour, &StatusMsgOut);
	if (mh_status != MH_OK)LOG_ERROR("StatusMsg Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(GenericPickup, &GenericPickupDetour, &GenericPickupOut);
	if (mh_status != MH_OK)LOG_ERROR("GenericPickup Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(WeaponDrop, &WeaponDropDetour, &WeaponDropOut);
	if (mh_status != MH_OK)LOG_ERROR("WeaponDrop Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(DebugPlayerData, &DebugPlayerDataDetour, &DebugPlayerDataOut);
	if (mh_status != MH_OK)LOG_ERROR("DebugPlayerData Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(DebugSoldierData, &DebugSoldierDataDetour, &DebugSoldierDataOut);
	if (mh_status != MH_OK)LOG_ERROR("DebugSoldierData Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(GunPickup, &GunPickupDetour, &GunPickupOut);
	if (mh_status != MH_OK)LOG_ERROR("GunPickup Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(AmmoPickup, &AmmoPickupDetour, &AmmoPickupOut);
	if (mh_status != MH_OK)LOG_ERROR("AmmoPickup Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(ShowError, &ShowErrorDetour, &ShowErrorOut);
	if (mh_status != MH_OK)LOG_ERROR("ShowError Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(ShowWarning, &ShowWarningDetour, &ShowWarningOut);
	if (mh_status != MH_OK) LOG_ERROR("ShowWarning Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(GetPlayerXPHit, &GetPlayerXPHitDetour, &GetPlayerXPHitOut);
	if (mh_status != MH_OK)LOG_ERROR("GetPlayerXPHit Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(SoldierViewCam, &SoldierViewCamDetour, &SoldierViewCamOut);
	if (mh_status != MH_OK)LOG_ERROR("HumanViewCam Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(SoldierDead, &SoldierDeadDetour, &SoldierDeadOut);
	if (mh_status != MH_OK)LOG_ERROR("SoldierDead Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(SoldierHit, &SoldierHitDetour, &SoldierHitOut);
	if (mh_status != MH_OK)LOG_ERROR("HumanSoldierHit Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(HumanSoldierDead, &HumanSoldierDeadDetour, &HumanSoldierDeadOut);
	if (mh_status != MH_OK)LOG_ERROR("HumanXPDead Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(StatusMessageShow, &StatusMessageShowDetour, &StatusMessageShowOut);
	if (mh_status != MH_OK)LOG_ERROR("StatusMessageShow Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(SetGameDataSymbol, &SetGameDataSymbolDetour, &SetGameDataSymbolOut);
	if (mh_status != MH_OK)LOG_ERROR("SetGameDataSymbol Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(TextPrint, &TextPrintDetour, &TextPrintOut);
	if (mh_status != MH_OK)LOG_ERROR("TextPrint Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(GameTextPrint, &GamePrintTextDetour, &GameTextPrintOut);
	if (mh_status != MH_OK)LOG_ERROR("GameTextPrint Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(ParseWeaponConfig, &ParseWeaponConfigDetour, &ParseWeaponConfigOut); 
	if (mh_status != MH_OK) LOG_ERROR("ParseWeaponConfig Hooking error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(StartLevel, &StartLevelDetour, &StartLevelOut);
	if (mh_status != MH_OK)LOG_ERROR("StartLevel Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(QuitLvl, &QuitLvlDetour, &QuitLvlOut); 
	if (mh_status != MH_OK)LOG_ERROR("QuitLvl Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(LoadResourceFile, &LoadResourceDetour, &LoadResourceOut);
	if (mh_status != MH_OK)LOG_ERROR("LoadResourceFile Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(ResourceUnpack, &ResourceUnpackDetour, &ResourceUnpackOut);
	if (mh_status != MH_OK)LOG_ERROR("ResourceUnpack Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(IsResourceLoaded, &IsResourceLoadedDetour, &IsResourceLoadedOut);
	if (mh_status != MH_OK)LOG_ERROR("IsResourcePacked Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(ResourceUnload, &ResourceUnloadDetour, &ResourceUnloadOut);
	if (mh_status != MH_OK)LOG_ERROR("ResourceUnload Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(ResourceFlush, &ResourceFlushDetour, &ResourceFlushOut);
	if (mh_status != MH_OK)LOG_ERROR("ResourceFlush Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(LoadGameData, &LoadGameDataDetour, &LoadGameDataOut);
	if (mh_status != MH_OK)LOG_ERROR("LoadGameData Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(LevelLoad, &LevelLoadDetour, &LevelLoadOut);
	if (mh_status != MH_OK)LOG_ERROR("LevelLoad Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(GameOpenFile, &GameOpenFileDetour, &GameOpenFileOut);
	if (mh_status != MH_OK)LOG_ERROR("GameOpenFile Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(GameOpenQFile, &GameOpenQFileDetour, &GameOpenQFileOut);
	if (mh_status != MH_OK)LOG_ERROR("GameOpenQFile Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(LoadQVM, &LoadQVMDetour, &LoadQVMOut);
	if (mh_status != MH_OK)LOG_ERROR("LoadQVM Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(CompileQVM, &CompileQVMDetour, &CompileQVMOut); 
	if (mh_status != MH_OK) LOG_ERROR("CompileQVM Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(AssembleQVM, &AssembleQVMDetour, &AssembleQVMOut);
	if (mh_status != MH_OK)LOG_ERROR("AssembleQAS Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(ParseQVM, &ParseQVMDetour, &ParseQVMOut);
	if (mh_status != MH_OK)LOG_ERROR("ParseQVM Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = CreateHook(CleanupQVM, &CleanupQVMDetour, &CleanupQVMOut);
	if (mh_status != MH_OK)LOG_ERROR("CleanupQVM Createhook error : %s", MH_StatusToString(mh_status));

	mh_status = MH_OK;//CreateHook(SFXItems, &SFXItemsDetour, &SFXItemsOut);
	if (mh_status != MH_OK)LOG_ERROR("SFXItems Createhook error : %s", MH_StatusToString(mh_status));

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
	auto mh_status = MH_DisableHook(p_target);
	if (mh_status != MH_OK)
		LOG_ERROR("Error disabling hook.");
	else
		LOG_INFO("Hook disabled.");
	return mh_status;
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
		LOG_ERROR("Error disabling hooks.");
	else
		LOG_WARNING("All hooks disabled.");
	return status;
}