#include "DllMain.hpp"

string InternalDataRead();
void DllMainLoop();
void StartLevelMain(int = 1, bool = true, bool = true, int = 1);
void QuitLevelMain();
bool g_DbgEnabled = true, g_PlayerEnabled = true;
LPVOID viewport_x = (LPVOID)0x00BCAB08, viewport_y = (LPVOID)0x00BCAB10, viewport_z = (LPVOID)0x00BCAB18;

void DllMainLoop() {

	g_menu_screen = READ_PTR(menu_screen_ptr);
	g_game_level = LEVEL::GET();
	if (g_curr_level != g_game_level) {
		g_curr_level = g_game_level;
		g_level_changed = true;
	}

	if (g_level_changed) {
		g_level_changed ^= 1;
	}

	if (g_menu_screen == MENU_SCREEN_MAINMENU) {
		//g_Console->Clear();

		game_resources.clear();
		g_level_graphs.clear();
	}

	else if (g_menu_screen == MENU_SCREEN_INGAME) {

		//Ctrl-Menu Controls.
		if (GT_HotKeysPressed(VK_CONTROL, VK_F1)) {
			DEBUG::INIT(GAME_FONT_BIG);
			DEBUG::ENABLE(g_DbgEnabled);
			string dbg_msg = "Debug mode " + std::string((g_DbgEnabled) ? "Enabled" : "Disabled");
			MISC::STATUS_MESSAGE_SHOW(dbg_msg.c_str());
			g_DbgEnabled = !g_DbgEnabled;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F2)) {
			QTASK::UPDATE();
			g_AutoMsgBox->Show("", 100);
			LEVEL::LOAD();
			g_AutoMsgBox->Show("", 100);
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F3)) {
			auto internal_data = InternalDataRead();
			int weapon_id = std::stoi(internal_data);
			WEAPON::WEAPON_PICKUP(weapon_id);
			MISC::STATUS_MESSAGE_SHOW(string("WeaponPickup Id:" + std::to_string(weapon_id) + " done!"));
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F4)) {
			auto internal_data = InternalDataRead();
			int frames = std::stoi(internal_data);
			MISC::FRAMES_SET(frames);
			MISC::STATUS_MESSAGE_SHOW(string("Game frames changed to " + std::to_string(frames) + "FPS"));
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F5)) {
			CONFIG::READ();
			MISC::STATUS_MESSAGE_SHOW("Game config read!");
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F6)) {
			CONFIG::WRITE();
			MISC::STATUS_MESSAGE_SHOW("Game config write!");
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F7)) {
			CONFIG::WEAPON_CONFIG_READ();
			MISC::STATUS_MESSAGE_SHOW("Weapon config read!");
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F8)) {
			HUMAN::HUMAN_PLAYER_LOAD();
			MISC::STATUS_MESSAGE_SHOW("Humanplayer load!");
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F9)) {
			auto internal_data = InternalDataRead();
			int cam_view = std::stoi(internal_data) > 5 ? 1 : std::stoi(internal_data);
			HUMAN::CAM_VIEW_SET(cam_view);
			MISC::STATUS_MESSAGE_SHOW("Humanplayer camera view = CAM_" + std::to_string(cam_view));
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F10)) {
			GAME::INPUT_ENABLE();
			g_PlayerEnabled = true;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F11)) {
			GAME::INPUT_DISABLE();
			g_PlayerEnabled = false;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F12)) {
			GAME::INPUT_DISABLE();
			g_PlayerEnabled = false;

			Camera::Controls controls;
			controls.UP(VK_SPACE);
			controls.DOWN(VK_MENU);
			controls.LEFT(VK_LEFT);
			controls.RIGHT(VK_RIGHT);
			controls.FORWARD(VK_UP);
			controls.BACKWARD(VK_DOWN);
			controls.CALIBRATE(VK_BACK);
			controls.QUIT(VK_HOME);
			controls.AXIS_OFF(0.5f);

			CAMERA::FREECAM(controls);
			GAME::INPUT_ENABLE();
			g_PlayerEnabled = true;
		}

		//Alt-Menu Controls.
		else if (GT_HotKeysPressed(VK_MENU, VK_F1)) {
			StartLevelMain(4);
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F2)) {
			QuitLevelMain();
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F3)) {
			string script_file = InternalDataRead();
			LOG_INFO("Script Parsing file '%s'", script_file.c_str());
			int status = SCRIPT::PARSE(script_file); //Status '0' success, 'Non-zero' error.
			if (status == 0)
				MISC::STATUS_MESSAGE_SHOW("Script Parsing file done!");
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F4)) {
			string script_file = InternalDataRead();
			LOG_INFO("Script Assembling file '%s'", script_file.c_str());
			int status = SCRIPT::ASSEMBLE(script_file); //Status '0' success, 'Non-zero' error.
			if (status == 0)
				MISC::STATUS_MESSAGE_SHOW("Script Assemble file done!");
		}
		else if (GT_HotKeysPressed(VK_MENU, VK_F5)) {
			string script_file = InternalDataRead();
			SCRIPT::COMPILE(script_file);
			LOG_INFO("Script Compile file '%s' done!", script_file.c_str());
			MISC::STATUS_MESSAGE_SHOW("Script Compile file done!");
		}
		else if (GT_HotKeysPressed(VK_MENU, VK_F6)) {
			string resource_file = InternalDataRead();
			LOG_INFO("Resource Load file '%s'", resource_file.c_str());
			auto resource_addr = RESOURCE::LOAD(resource_file);
			LOG_INFO("Resource '%s' loaded at %p", resource_file.c_str(), resource_addr);
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F7)) {
			string resource_file = InternalDataRead();
			LOG_INFO("Resource Unload file '%s'", resource_file.c_str());
			RESOURCE::UNLOAD(resource_file);
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F8)) {
			string resource_file = InternalDataRead();
			LOG_INFO("Resource Unpack file '%s'", resource_file.c_str());
			auto resource_addr = RESOURCE::UNPACK(resource_file);
			LOG_INFO("Resource '%s' unpacked at %p", resource_file.c_str(), resource_addr);
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F9)) {
			string resource_file = InternalDataRead();
			LOG_INFO("Resource Flush file '%s'", resource_file.c_str());
			auto resource_addr = RESOURCE::FIND(resource_file);
			RESOURCE::FLUSH(resource_addr);
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F10)) {
			string resource_file = InternalDataRead();
			LOG_INFO("Resource Unpack file '%s'", resource_file.c_str());
			bool is_loaded = RESOURCE::IS_LOADED(resource_file);
			if (is_loaded) LOG_INFO("Resource is loaded");
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F11)) {
			string resource_file = InternalDataRead();
			LOG_INFO("Resource Unpack find name '%s'", resource_file.c_str());
			auto resource_addr = RESOURCE::FIND(resource_file);
			LOG_INFO("Resource '%s' unpacked at %p", resource_file.c_str(), resource_addr);
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F12)) {
			RESOURCE::ANIMATION_INFO_SAVE("IGI_Animations.txt");
			RESOURCE::FONT_INFO_SAVE("IGI_Fonts.txt");
			RESOURCE::SOUND_INFO_SAVE("IGI_Sound.txt");
			RESOURCE::MATERIAL_INFO_SAVE("IGI_Material.txt");
			RESOURCE::LIGHTMAP_INFO_SAVE("IGI_Lightmap.txt");
			RESOURCE::OBJECT_INFO_SAVE("IGI_Object.txt");
			RESOURCE::RESOURCE_INFO_SAVE("IGI_Resource.txt");
			RESOURCE::TERRAIN_INFO_SAVE("IGI_Terrain.txt");
			RESOURCE::TEXTURE_INFO_SAVE("IGI_Texture.txt");
			RESOURCE::GRAPHICS_2D_INFO_SAVE("IGI_2D_Graphics.txt");
			RESOURCE::GRAPHICS_3D_INFO_SAVE("IGI_3D_Graphics.txt");
		}

		//Shift-Menu Controls.
		else if (GT_HotKeysPressed(VK_SHIFT, VK_F1)) {
			string model = InternalDataRead();
			LOG_INFO("MEF Model remove '%s'", model.c_str());
			RESOURCE::MEF_REMOVE_MODEL(model);
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F2)) {
			RESOURCE::MEF_RESTORE_MODELS();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F3)) {
			RESOURCE::MEF_EXTRACT();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F3)) {
			string qvm_file = InternalDataRead();
			LOG_INFO("QVM Load file '%s'", qvm_file.c_str());
			auto qvm_addr = QVM::LOAD(qvm_file);
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F4)) {
			string qvm_file = InternalDataRead();
			LOG_INFO("QVM Read file '%s'", qvm_file.c_str());
			int status = QVM::READ(qvm_file); //Status '0' success, 'Non-zero' error.
		}
		else if (GT_HotKeysPressed(VK_SHIFT, VK_F5)) {
			string qvm_file = InternalDataRead();
			LOG_INFO("QVM cleanup file '%s'", qvm_file.c_str());
			auto qvm_addr = QVM::LOAD(qvm_file);
			int status = QVM::READ((int)qvm_addr); //Overloaded method 'QVM::READ(int)'
			if (status == 0) QVM::CLEANUP(qvm_addr);
		}
		else if (GT_HotKeysPressed(VK_SHIFT, VK_F6)) {
			SFX::MUSIC_ENABLE();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F7)) {
			SFX::MUSIC_DISABLE();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F8)) {
			string volume = InternalDataRead();
			LOG_INFO("Volume set %f", volume.c_str());
			SFX::VOLUME_SET(std::stof(volume));
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F9)) {
			string volume = InternalDataRead();
			LOG_INFO("Volume SFX set %f", volume.c_str());
			SFX::VOLUME_SFX_SET(std::stof(volume));
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F10)) {
			SFX::VOLUME_UPDATE();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F11)) {
			GFX::RESET();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F12)) {
			MISC::CUTSCENE_DELETE();
		}

		//Ctrl-Alt-Menu Controls.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F1)) {
			string mission = InternalDataRead();
			LOG_INFO("Player Active mission set %d", mission.c_str());
			PLAYER::ACTIVE_MISSION_SET(std::stoi(mission));
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F2)) {
			string name = InternalDataRead();
			LOG_INFO("Player Active name set %d", name.c_str());
			PLAYER::ACTIVE_NAME_SET(name);
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F3)) {

			string data = InternalDataRead();
			std::vector<string> vec;
			g_Utility.Tokenize(data, '\n', vec);

			int index = 0;
			byte mission = 1;
			index = std::stoi(vec.at(0));
			mission = std::stoi(vec.at(1));

			LOG_INFO("Player index mission set mission %d\tIndex: %d", mission,index);
			PLAYER::INDEX_MISSION_SET(index, mission);
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F4)) {
		string data = InternalDataRead();
		std::vector<string> vec;
		g_Utility.Tokenize(data, '\n', vec);

		int index = 0;
		string name = "Jones";
		index = std::stoi(vec.at(0));
		name = std::stoi(vec.at(1));

		LOG_INFO("Player index name set name %s\tIndex: %d", name, index);
		PLAYER::INDEX_NAME_SET(index, name);
		}
		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F5)) {
			PLAYER::IS_PROFILE_ACTIVE();
		}
		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F6)) {
			MISC::GAMEMATERIAL_LOAD();
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F7)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F8)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F9)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F10)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F11)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_F12)) {
			;
		}

		//Ctrl-Shift-Menu Controls.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F1)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F2)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F3)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F4)) {
			;
		}
		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F5)) {
			;
		}
		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F6)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F7)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F8)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F9)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F10)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F11)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SHIFT, VK_F12)) {
			;
		}

		//Ctrl-Alt-Shift-Menu Controls.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F1)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F2)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F3)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F4)) {
			;
		}
		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F5)) {
			;
		}
		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F6)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F7)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F8)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F9)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F10)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F11)) {
			;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_MENU, VK_SHIFT, VK_F12)) {
			;
		}


		else if (GT_IsKeyToggled(VK_SNAPSHOT)) {
			g_Console->Clear();
		}
	}

	else if (g_menu_screen == MENU_SCREEN_RESTART) {
		soldiers.clear();
		if (!g_PlayerEnabled) GAME::INPUT_DISABLE();
	}

}


string InternalDataRead() {
	string internal_data_file = g_Utility.GetModuleFolder() + "\\" + "IGI-Internals-data.txt";
	auto internal_data = ReadFileType(internal_data_file, ASCII_FILE);
	return internal_data;
}

void StartLevelMain(int level, bool disable_warn, bool disable_err, int hash_val) {

	LEVEL::SET(level);
	if (disable_warn) MISC::WARNINGS_DISABLE();
	if (disable_err) MISC::ERRORS_DISABLE();

	//QuitLevelMain();
	//std::this_thread::sleep_for(10s);

	QTASK::HASH_INIT(1);
	QTASK::UPDATE();

	//auto StartLevel = (int(__cdecl*)(int, int, int, int))0x415B30;
	//StartLevel(*(PINT)0x57BABC,00000000, * (PINT)0xC28C5C, * (PINT)(*(PINT)0x57BABC));

	auto StartLevelCaller = (int(__cdecl*)(int))0x416900;
	StartLevelCaller(*(PINT)0x00567C8C);

	QTASK::RESET();
}

void QuitLevelMain() {
	auto sub_416d40 = (int(__cdecl*)())0x416d40;
	auto ppiVar4 = (int**)sub_416d40();

	auto sub_004015f0 = (int(__cdecl*)(int**))0x004015f0;
	sub_004015f0(ppiVar4);
	*(int*)(0x00567c8c + 0x28) = 3;

	auto sub_402890 = (int(__cdecl*)())0x402890;
	int iVar2 = sub_402890();

	auto MenuManager = (int(__cdecl*)(int, const char*, char, char, int))0x418b00;
	MenuManager(iVar2, "LOCAL:menusystem\\mainmenu.qsc", '\x1', '\x1', 1);
}