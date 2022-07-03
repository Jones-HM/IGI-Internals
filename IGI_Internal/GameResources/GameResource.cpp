#include "GameResource.hpp"
#include "Logger.hpp"
#include "NativeHelper.hpp"

using namespace IGI;

GameResource::GameResource() {
	g_Resource = this;
	InitGameResource();
}

GameResource::~GameResource() {
	g_Resource = nullptr;
}

void GameResource::InitGameResource() {
	string mef_models = g_Utility.GetAppdataPath() + "\\QEditor\\" + MEF_MODELS_FILE;
	models_data = ReadFileType(mef_models, ASCII_FILE);
}

void GameResource::SaveGameResource(string file, string resource_type) {
	string res_dir = g_Utility.GetModuleFolder() + "\\" + "GameResources";
	fs::create_directory(res_dir);
	string res_file = res_dir + "\\" + file;

	std::ofstream fout(res_file, std::ios_base::app);
	if (fout.good()) {
		for (auto& resource : game_resources) {
			if (resource.name.find(resource_type) != std::string::npos) {
				fout << "Resource: '" << resource.name << "'\tAddress: " << HEX_ADDR_STR(resource.address) << std::endl;
			}
		}
	}
	LOG_CONSOLE("Resource file 'GameResources/%s' was saved successfully.", file.c_str());
	fout.close();
}

address_t GameResource::FindGameResource(string resource_name) {
	for (const auto& resource : game_resources) {
		if (resource.name.find(resource_name) != std::string::npos) {
			return resource.address;
		}
	}
	return NULL;
}

void GameResource::ExtractResourceFile(string resource_type) {
	fs::current_path(g_Utility.GetModuleFolder() + "\\");
	string res_dir = "level" + std::to_string(g_curr_level) + "_" + resource_type;
	fs::create_directory(res_dir);

	int res_count = std::count_if(game_resources.begin(), game_resources.end(), [&](Resource x) {return (int)x.name.find(resource_type) != std::string::npos; });
	//mef_count /= 2;
	LOG_INFO("res_count: %d", res_count);

	for (const auto& resource : game_resources) {
		if (resource.name.find(resource_type) != std::string::npos) {
			//int res_count = std::count_if(game_resources.begin(), game_resources.end(), [&](Resource x) {return resource.address == x.address; });
			//if (res_count > 1)
			//	game_resources_set.insert(resource.name);
			//else
			ExtractResource(resource, res_dir);
		}
	}
}

void GameResource::ExtractResource(Resource resource, string res_dir) {

	try {
		const bool resource_exist = game_resources_set.find(resource.name) != game_resources_set.end();
		if (resource_exist) return;

		binary_t mef_buf(resource.size, '\0');
		std::memcpy(mef_buf.data(), (void*)resource.address, mef_buf.capacity());

		resource.name = resource.name.substr(resource.name.find_last_of("/") + 1);
		string mesh_file = res_dir + "\\" + resource.name;
		WriteFileType(mesh_file, mef_buf, BINARY_FILE);
		LOG_INFO("Resource File %s saved successfully!", mesh_file.c_str());
	}
	catch (const std::exception& ex) {
		LOG_ERROR("Exception: %s", ex.what());
	}
}

void GameResource::SaveTexureFile() {

}

string GameResource::MEF_FindModelName(string model_id)
{
	if (models_data.empty()) return {};

	vector<string> tokens;
	g_Utility.Tokenize(models_data, '\n', tokens);

	for (const auto& token : tokens)
	{
		if (token.find(model_id) != string::npos)
		{
			string model_name = token.substr(0, token.find_first_of("=") - 1);
			return model_name;
		}
	}
	return {};
}

string GameResource::MEF_FindModelId(string model_id, bool full_id)
{
	if (models_data.empty()) return {};

	vector<string> tokens;
	g_Utility.Tokenize(models_data, '\n', tokens);

	for (const auto& token : tokens)
	{
		if (token.find(model_id) != string::npos)
		{
			size_t mef_id_size = (full_id) ? MEF_ID_SIZE : 3;
			string model_id = token.substr(token.find_first_of("=") + 2, mef_id_size);
			return model_id;
		}
	}
	return {};
}

void GameResource::MEF_RemoveModel(string model) {

	try {
		for (const auto& resource : game_resources) {
			if (resource.name.find(GAME_RESOURCE_MEF) != string::npos) {

				address_t model_addr;
				string model_id, model_name;
				bool model_match = false;

				//Model Id Half/Full.
				if (std::regex_match(model, model_re) || std::regex_match(model, model_re_full)) {
					LOG_INFO("%s Model Id Regex", FUNC_NAME);
					size_t mef_id_size = (std::regex_match(model, model_re)) ? 3 : MEF_ID_SIZE;
					model_id = resource.name.substr(resource.name.find_last_of("/") + 1, mef_id_size);

					if (model_id.find(model) != string::npos) {//If 'XXX_XX_X' = 'YYY_YY_Y' OR 'XXX' = 'YYY'
						model_addr = resource.address;
						model_name = MEF_FindModelName(model_id);
						model_match = true;
					}
				}
				//Model Name.
				else {
					LOG_INFO("%s Model Name Regex",FUNC_NAME);
					model_id = resource.name.substr(resource.name.find_last_of("/") + 1, 3);
					model_name = MEF_FindModelName(model_id);
					//Case insensitive Find.
					if (g_Utility.Find(model_name, model) != string::npos) {
						model_addr = resource.address;
						model_match = true;
					}
				}

				//If model Id/Name matches.
				if (model_match) {
					binary_t model_data(500000, '\0');//500kb MEF
					std::memcpy(model_data.data(), (void*)model_addr, model_data.capacity());

					//Find end point of MEF and resize buffer.
					binary_t mef_end{ 'N', 'A', 'M', 'E' };
					auto it = std::search(model_data.begin(), model_data.end(), mef_end.begin(), mef_end.end());
					size_t model_size = (it - model_data.begin());
					model_data.resize(model_size);

					//Add MEF Resource to list.
					ResourceMEF mef(model_name, model_id, model_data, model_addr, model_size);
					mef_resources.push_back(mef);
					LOG_INFO("MEF: Name: '%s' Id: %s Addr: %p Size: %d", model_name.c_str(), model_id.c_str(), model_addr, 0);

					//Remove current MEF from Memory.
					std::memset((void*)model_addr, 0, model_size);
				}
			}
		}
	}
	catch (const std::exception& ex) {
		GT_ShowError(ex.what());
		LOG_ERROR("Exception: %s", ex.what());
	}
}

void GameResource::MEF_RestoreModel(string model) {

	for (auto& mef : mef_resources) {
		if (std::regex_match(model, model_re) || std::regex_match(model, model_re_full)) {
			if (mef.id.find(model) != string::npos)
				std::memcpy((void*)mef.address, (void*)mef.data.data(), mef.size);
		}
		else {
			if (g_Utility.Find(mef.name, model) != string::npos) {
				std::memcpy((void*)mef.address, (void*)mef.data.data(), mef.size);
			}
		}
	}
}

void GameResource::MEF_RestoreModels() {
	for (auto& mef : mef_resources) {
		std::memcpy((void*)mef.address, (void*)mef.data.data(), mef.size);
	}
}