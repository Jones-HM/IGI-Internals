#include "Features.hpp"

void DllMainLoop() {
	g_menu_screen = READ_PTR(menu_screen_ptr);
	g_game_level = LEVEL::GET();
	if (g_curr_level != g_game_level) {
		g_curr_level = g_game_level;
		g_level_changed = true;
	}

	if (g_level_changed) {

		if (READ_PTR(humanplayer_ptr) == NULL)
			GT_ShowError("Humanplayer structure isn't loaded or invalid\nSome of features related to humanplayer wont work.\n");
		else LOG_INFO("Humanplayer structure is loaded properly at address 0x%x", READ_PTR(humanplayer_ptr));

		if (READ_PTR(gun_pickup_ptr) == NULL)
			GT_ShowError("Weapon structure isn't loaded or invalid\nSome of features related to Weapon wont work.\n");
		else LOG_INFO("Weapon structure is loaded properly at address 0x%x", READ_PTR(gun_pickup_ptr));

		g_level_changed ^= 1;
	}

	if (g_menu_screen == MENU_SCREEN_MAINMENU) {

		game_resources.clear();
		g_level_graphs.clear();
	}

	else if (g_menu_screen == MENU_SCREEN_INGAME) {

		// Enable Debug mode.
		if (GT_HotKeysPressed(VK_CONTROL, VK_F1)) {
			DEBUG::INIT(GAME_FONT_BIG);
			DEBUG::ENABLE(g_DbgEnabled);
			string dbg_msg = "Debug mode " + std::string((g_DbgEnabled) ? "Enabled" : "Disabled");
			LOG_INFO("%s", dbg_msg.c_str());
			MISC::STATUS_MESSAGE_SHOW(dbg_msg.c_str());
			g_DbgEnabled = !g_DbgEnabled;
		}

		// Restart game.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_F2)) {
			RestartLevel();
		}

		// Weapon pickup - AK47.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_F3)) {
			try {
				int weapon_id = GAME_WEAPON::WEAPON_ID_AK47;
				WEAPON::WEAPON_PICKUP(weapon_id);
				MISC::STATUS_MESSAGE_SHOW(string("WeaponPickup Id:" + std::to_string(weapon_id) + " done!"));
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		// Frames setting - 60 FPS.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_F4)) {
			try {
				int frames = 60;
				MISC::FRAMES_SET(frames);
				MISC::STATUS_MESSAGE_SHOW(string("Game frames changed to " + std::to_string(frames) + "FPS"));
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		// Humanplayer load.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_F5)) {
			HUMAN::HUMAN_PLAYER_LOAD();
			MISC::STATUS_MESSAGE_SHOW("Humanplayer load success!");
		}
	}

	else if (g_menu_screen == MENU_SCREEN_RESTART) {
		soldiers.clear();
		if (!g_PlayerEnabled) GAME::INPUT_DISABLE();
	}

}


#pragma region Native Helper Methods

void RestartLevel() {
	QTASK::UPDATE();
	g_AutoMsgBox->Show("", 70);
	LEVEL::LOAD();
	g_AutoMsgBox->Show("", 70);
}

void StartLevelMain(int level, bool disable_warn, bool disable_err, int hash_val) {

	LEVEL::SET(level);
	if (disable_warn) MISC::WARNINGS_DISABLE();
	if (disable_err) MISC::ERRORS_DISABLE();

	QTASK::HASH_INIT(1);
	QTASK::UPDATE();

	auto StartLevelCaller = (int(__cdecl*)(int))0x00416900;
	StartLevelCaller(*(PINT)0x00567C8C);

	QTASK::RESET();
}

void QuitLevelMain() {
	auto sub_416d40 = (int(__cdecl*)())0x00416D40;
	auto data = (int**)sub_416d40();

	auto sub_004015f0 = (int(__cdecl*)(int**))0x004015F0;
	sub_004015f0(data);
	*(int*)(0x00567c8c + 0x28) = 3; // Id of Main menu screen.

	auto sub_402890 = (int(__cdecl*)())0x402890;
	int menu_data = sub_402890();

	auto menuManager = (int(__cdecl*)(int, const char*, char, char, int))0x00418B00;
	menuManager(menu_data, "LOCAL:menusystem\\mainmenu.qsc", '\x1', '\x1', 1);
}

void ThreadCallerDelay(int delay) {
	std::this_thread::sleep_for(std::chrono::microseconds(delay));
}

template<typename RT, typename T1, typename T2, typename T3, typename T4>
RT ThreadCallerExec(void* func_ptr, T1 param1, T2 param2, T3 param3, T4 param4) {
	std::thread th(ThreadCallerDelay, delay_ms);

	auto ret_val = NATIVE_INVOKE<RT>(func_ptr, param1, param2, param3, param4);
	th.join();

	if constexpr (!std::is_same_v<RT, void>)
		return ret_val;
}
#pragma endregion
