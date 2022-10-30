#include "Features.hpp"

void DllMainLoop() {
	static bool model_bool = false;
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

		//Ctrl-Menu Controls.
		if (GT_HotKeysPressed(VK_CONTROL, VK_F1)) {
			DEBUG::INIT(GAME_FONT_BIG);
			DEBUG::ENABLE(g_DbgEnabled);
			string dbg_msg = "Debug mode " + std::string((g_DbgEnabled) ? "Enabled" : "Disabled");
			LOG_INFO("%s", dbg_msg.c_str());
			MISC::STATUS_MESSAGE_SHOW(dbg_msg.c_str());
			g_DbgEnabled = !g_DbgEnabled;
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F2)) {
			QTASK::UPDATE();
			g_AutoMsgBox->Show("", 70);
			LEVEL::LOAD();
			g_AutoMsgBox->Show("", 70);
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F3)) {
			try {
				auto internal_data = InternalDataRead();
				int weapon_id = std::stoi(internal_data);
				WEAPON::WEAPON_PICKUP(weapon_id);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
			//MISC::STATUS_MESSAGE_SHOW(string("WeaponPickup Id:" + std::to_string(weapon_id) + " done!"));
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F4)) {
			try {
				auto internal_data = InternalDataRead();
				int frames = std::stoi(internal_data);
				MISC::FRAMES_SET(frames);
				MISC::STATUS_MESSAGE_SHOW(string("Game frames changed to " + std::to_string(frames) + "FPS"));
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
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
			MISC::STATUS_MESSAGE_SHOW("Humanplayer load success!");
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_F9)) {
			auto internal_data = InternalDataRead();
			int view = std::stoi(internal_data);
			int cam_view = (view <= 0 || view > 5) ? 1 : view;
			HUMAN::CAM_VIEW_SET(cam_view);
			MISC::STATUS_MESSAGE_SHOW("Humanplayer Camera #" + std::to_string(cam_view));
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

			g_Camera.RunFreeCamThread(controls);
			GAME::INPUT_ENABLE();
			g_PlayerEnabled = true;
		}

		//Alt-Menu Controls.
		else if (GT_HotKeysPressed(VK_MENU, VK_F1)) {
			try {
				string level = InternalDataRead();
				LOG_INFO("StarLevel level '%s'", level.c_str());
				StartLevelMain(std::stoi(level));
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}

		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F2)) {
			QuitLevelMain();
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F3)) {
			try {
				string script_file = InternalDataRead();
				LOG_INFO("Script Parsing file '%s'", script_file.c_str());
				int status = SCRIPT::PARSE(script_file); //Status '0' success, 'Non-zero' error.
				if (status == 0) MISC::STATUS_MESSAGE_SHOW("Script Parsing file done!");
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}

		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F4)) {
			try {
				string script_file = InternalDataRead();
				LOG_INFO("Script Assembling file '%s'", script_file.c_str());
				int status = SCRIPT::ASSEMBLE(script_file); //Status '0' success, 'Non-zero' error.
				if (status == 0) MISC::STATUS_MESSAGE_SHOW("Script Assemble file done!");
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F5)) {
			ScriptCompile();
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F6)) {
			try {
				string resource_file = InternalDataRead();
				LOG_INFO("Resource Load file '%s'", resource_file.c_str());
				auto resource_addr = RESOURCE::LOAD(resource_file);
				string data = "Resource '" + resource_file + "' loaded at address " + HEX_ADDR_STR(resource_addr);
				InternalDataWrite(data);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F7)) {
			try {

				string resource_file = InternalDataRead();
				LOG_INFO("Resource Unload file '%s'", resource_file.c_str());
				std::vector<string> res_list;
				g_Utility.Tokenize(resource_file, '\n', res_list);
				RESOURCE::UNLOAD(res_list);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F8)) {
			try {
				string resource_file = InternalDataRead();
				LOG_INFO("Resource Unpack file '%s'", resource_file.c_str());
				auto resource_addr = RESOURCE::UNPACK(resource_file);
				string data = "Resource '" + resource_file + "' unpacked at address " + HEX_ADDR_STR(resource_addr);
				InternalDataWrite(data);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F9)) {
			try {
				string resource_file = InternalDataRead();
				LOG_INFO("Resource Flush file '%s'", resource_file.c_str());
				auto resource_addr = RESOURCE::FIND(resource_file);
				RESOURCE::FLUSH(resource_addr);
				string data = "Resource '" + resource_file + "' found at address " + HEX_ADDR_STR(resource_addr);
				InternalDataWrite(data);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F10)) {
			try {
				string resource_file = InternalDataRead(), data;
				LOG_INFO("Resource is loaded file '%s'", resource_file.c_str());
				bool is_loaded = RESOURCE::IS_LOADED(resource_file);
				if (is_loaded) data = "TRUE"; else data = "FALSE";
				InternalDataWrite(data);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_MENU, VK_F11)) {
			try {
				string resource_file = InternalDataRead();
				LOG_INFO("Resource Unpack find name '%s'", resource_file.c_str());
				auto resource_addr = RESOURCE::FIND(resource_file);
				string data = "Resource '" + resource_file + "' found at address " + HEX_ADDR_STR(resource_addr);
				LOG_INFO("'%s'", data.c_str());
				InternalDataWrite(HEX_ADDR_STR(resource_addr));
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
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
			try {
				string model = InternalDataRead();
				LOG_INFO("MEF Model remove '%s'", model.c_str());
				RESOURCE::MEF_REMOVE_MODEL(model);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F2)) {
			RESOURCE::MEF_RESTORE_MODELS();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F3)) {
			RESOURCE::MEF_EXTRACT();
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F4)) {
			try {
				string qvm_file = InternalDataRead();
				LOG_INFO("QVM Load file '%s'", qvm_file.c_str());
				auto qvm_addr = QVM::LOAD(qvm_file);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F5)) {
			try {

				string qvm_addr = InternalDataRead();
				LOG_INFO("QVM Read Address '%s'", qvm_addr.c_str());
				int status = QVM::READ(std::stoi(qvm_addr)); //Status '0' success, 'Non-zero' error.
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}
		else if (GT_HotKeysPressed(VK_SHIFT, VK_F6)) {

			try {
				string qvm_file = InternalDataRead();
				LOG_INFO("QVM cleanup file '%s'", qvm_file.c_str());
				auto qvm_addr = QVM::LOAD(qvm_file);
				int status = QVM::READ((int)qvm_addr); //Overloaded method 'QVM::READ(int)'
				if (status == 0) QVM::CLEANUP(qvm_addr);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}
		else if (GT_HotKeysPressed(VK_SHIFT, VK_F7)) {
			SFX::MUSIC_ENABLE();
			LOG_INFO("Music Enabled");
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F8)) {
			SFX::MUSIC_DISABLE();
			LOG_INFO("Music Disabled");
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F9)) {
			string volume = InternalDataRead();
			LOG_INFO("Volume set %s", volume.c_str());
			SFX::VOLUME_SET(std::stof(volume));
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F10)) {
			try {
				string volume = InternalDataRead();
				LOG_INFO("Volume SFX set %s", volume.c_str());
				SFX::VOLUME_SFX_SET(std::stof(volume));
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F11)) {
			try {
				SFX::VOLUME_UPDATE();
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_SHIFT, VK_F12)) {
			try {
				GFX::RESET();
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		//Ctrl-Numpad Controls.

		else if (GT_HotKeysPressed(VK_CONTROL, VK_ADD)) {
			try
			{
				string active_name = PLAYER::ACTIVE_NAME_GET();
				LOG_INFO("Player active name %s", active_name.c_str());

				string name(active_name.data());
				InternalDataWrite(name);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_SUBTRACT)) {
			try
			{
				int active_mission = PLAYER::ACTIVE_MISSION_GET();
				LOG_INFO("Player active mission %d", active_mission);

				if (active_mission > 0 && active_mission <= 14) {
					InternalDataWrite(std::to_string(active_mission));
				}
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD0)) {
			string mission = InternalDataRead();
			LOG_INFO("Player Active mission set %d", mission.c_str());
			PLAYER::ACTIVE_MISSION_SET(std::stoi(mission));
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD1)) {
			string name = InternalDataRead();
			LOG_INFO("Player Active name set %d", name.c_str());
			PLAYER::ACTIVE_NAME_SET(name);
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD2)) {
			string data = InternalDataRead();
			std::vector<string> vec;
			g_Utility.Tokenize(data, '\n', vec);

			int index = 0;
			byte mission = 1;
			index = std::stoi(vec.at(0));
			mission = std::stoi(vec.at(1));

			LOG_INFO("Player index mission set mission %d\tIndex: %d", mission, index);
			PLAYER::INDEX_MISSION_SET(index, mission);
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD3)) {
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
		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD4)) {
			MISC::GAMEMATERIAL_LOAD();
		}
		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD5)) {
			MISC::MAGIC_OBJECT_LOAD();
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD6)) {
			MISC::PHYSICS_OBJECT_LOAD();
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD7)) {
			MISC::ANIM_TRIGGER_LOAD();
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_NUMPAD8)) {
			MISC::CUTSCENE_DELETE();
			MISC::STATUS_MESSAGE_SHOW("Cutscene removed");
		}

		else if (GT_HotKeysPressed(VK_CONTROL, VK_INSERT)) {
			StatusMsgShow();
		}

		//Alt-Numpad Controls.
		else if (GT_HotKeysPressed(VK_CONTROL, VK_HOME)) {
			try
			{
				auto data = InternalDataRead();
				int graph_id = std::stoi(data);
				string node_links = GRAPH::GET_NODE_LINKS(graph_id);
				InternalDataWrite(node_links);
			}
			catch (const std::exception& ex)
			{
				LOG_INFO("Exception: %s", ex.what());
			}
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

void StatusMsgShow() {
	try {
		string data = InternalDataRead();
		MISC::STATUS_MESSAGE_SHOW(data);
	}
	catch (const std::exception& ex)
	{
		LOG_INFO("Exception: %s", ex.what());
	}
}

void ScriptCompile() {
	try {
		string script_file = InternalDataRead();
		SCRIPT::COMPILE(script_file);
		LOG_INFO("Script Compile file '%s' done!", script_file.c_str());
	}
	catch (const std::exception& ex)
	{
		LOG_INFO("Exception: %s", ex.what());
	}
	//MISC::STATUS_MESSAGE_SHOW("Script Compile file done!");
}

string InternalDataRead() {
	string data;
	try {
		string internal_data_file = g_Utility.GetModuleFolder() + "\\" + PROJECT_NAME + "-data.txt";
		std::ifstream in_stream(internal_data_file);

		if (in_stream.good()) {
			std::getline(in_stream, data);
		}
		else {
			throw std::runtime_error("Internal data file doesn't exist in current directory");
		}
	}
	catch (const std::exception& ex)
	{
		LOG_INFO("Exception: %s", ex.what());
	}
	return data;
}

bool InternalDataWrite(string data) {
	string internal_data_file = g_Utility.GetModuleFolder() + "\\" + PROJECT_NAME + "-data.txt";
	auto status = WriteFileType(internal_data_file, binary_t(data.begin(), data.end()), BINARY_FILE);
	return status;
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
	auto sub_416d40 = (int(__cdecl*)())0x416d40;
	auto data = (int**)sub_416d40();

	auto sub_004015f0 = (int(__cdecl*)(int**))0x004015f0;
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