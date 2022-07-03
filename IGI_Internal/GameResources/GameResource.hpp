#pragma once
#include "Common.hpp"
#include "CommonConst.hpp"
#include "Utility.hpp"

namespace IGI {
	//Struct to hold internal resource properties.
	struct Resource {
		string name;
		address_t address;
		size_t size;
	public:
		//Ctors/Dtors
		Resource() = default;
		~Resource() = default;
		Resource(string name, address_t address, size_t size) { this->name = name; this->address = address; this->size = size; }
	};

	struct ResourceMEF {
		string name;
		string id;
		binary_t data;
		address_t address;
		size_t size;
	public:
		//Ctors/Dtors
		ResourceMEF() = default;
		~ResourceMEF() = default;
		ResourceMEF(string name, string id, binary_t data, address_t address, size_t size) { this->name = name; this->id = id; this->data = data; this->address = address; this->size = size; }
	};

	//Class to hold Game resource information.
	class GameResource {
	public:
		std::regex model_re = std::regex(R"(\d{3})");//Model Id regex.
		std::regex model_re_full = std::regex(R"(\d{3}_\d{2}_\d{1})");//Model Id full regex.
		std::list<string> mef_files;//Store MEF files.
		string models_data;//Stores MEF Models info.
		void ExtractResource(Resource resource, string resource_dir);
		void InitGameResource();
	public:
		//Ctors/Dtors
		GameResource();
		~GameResource();

		//Resource Methods.
		void SaveGameResource(string file = LOGGER_FILE, string resource_type = GAME_RESOURCE_MEF);
		address_t FindGameResource(string resource_name);
		void ExtractResourceFile(string resource_type);
		void SaveTexureFile();

		//MEF Resource Methods.
		string MEF_FindModelName(string model_id);
		string MEF_FindModelId(string model_id, bool full_id=true);
		void MEF_RemoveModel(string model_id);
		void MEF_RestoreModel(string model_id);
		void MEF_RestoreModels();
		//Setters/Getters.
	};

	inline GameResource* g_Resource{};
	inline std::vector<Resource> game_resources;
	inline std::vector<ResourceMEF> mef_resources;
	inline std::set<string> game_resources_set;
}