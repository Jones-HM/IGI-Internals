#include "HumanSoldier.hpp"
#include "Logger.hpp"
//#define SOLDIER_DATA_ALL

using namespace IGI;

HumanSoldier::HumanSoldier() : HumanSoldier("", 0, 0, "", true, 0, 0) {}
HumanSoldier::HumanSoldier(string model_id, int soldier_id, int graph_id, string weapon, bool is_dead, int e_team, uint32_t address)
{
	this->model_id = model_id; this->soldier_id = soldier_id; this->graph_id = graph_id; this->weapon = weapon; this->is_dead = is_dead; this->e_team = e_team; this->address = address;
}

void HumanSoldier::AddSoldier() {
	auto it = std::find_if(soldiers.begin(), soldiers.end(), [&soldier = *this](HumanSoldier& curr_soldier) -> bool { return soldier.soldier_id == curr_soldier.soldier_id; });
	if (it == soldiers.end())soldiers.push_back(*this);
}

bool HumanSoldier::RemoveSoldier() {
	auto it = std::find_if(soldiers.begin(), soldiers.end(), [&soldier = *this](HumanSoldier& curr_soldier) -> bool { return soldier.soldier_id == curr_soldier.soldier_id; });
	if (it != soldiers.end()) { soldiers.erase(it); return true; }
	return false;
}

void HumanSoldier::AddSoldierData(uint32_t address, std::vector<uint8_t> data) {

	auto soldier = this;
	std::string soldier_data(data.begin(), data.end());

	if (!soldier_data.empty()) {
		std::regex data_regex(R"(\w{0,}_\w{0,})");
		auto words_begin = std::sregex_iterator(soldier_data.begin(), soldier_data.end(), data_regex);
		auto words_end = std::sregex_iterator();

		string soldier_tag = "HumanSoldier_", weapon_id_tag = "WEAPON_ID";
		string ai_tag = "HumanAI_", ai_type_tag = "AITYPE_", ai_mission_tag = "MISSION:AI";

		auto find_human_ai_id = [](string s)-> uint32_t {
			string id; for (const auto& c : s) if (::isdigit(c)) id += c;
			return std::stoi(id);
		};

		for (auto index = words_begin; index != words_end; ++index) {

			auto match = *index;
			string match_str = match.str();

			if (match_str.find(soldier_tag) != std::string::npos) {
				soldier->soldier_id = find_human_ai_id(match_str);
			}

#ifdef SOLDIER_DATA_ALL
			else if (match_str.find(ai_tag) != std::string::npos) {
				soldier->ai_id = find_human_ai_id(match_str);
			}

			else if (match_str.find(ai_type_tag) != std::string::npos)
				soldier->ai_type = match_str;
#endif

			else if (match_str.find(weapon_id_tag) != std::string::npos)
				soldier->weapon += match_str + ", ";
		}

#ifdef SOLDIER_DATA_ALL
		if (auto pos = soldier_data.find(ai_mission_tag); pos != std::string::npos) {
			std::stringstream ss;
			for (int i = 6; i <= 8; i++)
				ss << std::hex << (int)data.at(pos - i);
			ss >> soldier->graph_id;
		}
#endif

		soldier->model_id = soldier_data.substr(0, 8);
		soldier->address = address;

		//Add soldier to Soldiers list.
		soldier->AddSoldier();
	}
	else LOG_ERROR("Trying to add empty data for HumanSoldier");
}

bool HumanSoldier::ValidateSoldier() {
	return (this->soldier_id > 0 && this->soldier_id <= 5000 && this->address > 0 && this->address <= UINT32_MAX);
}

HumanSoldier HumanSoldier::FindSoldier(soldier_t id) {
	auto it = std::find_if(soldiers.begin(), soldiers.end(), [&](HumanSoldier& soldier) -> bool { return id == soldier.GetSoldierId(); });
	if (it != soldiers.end()) return *it;
	return {};
}

HumanSoldier HumanSoldier::FindSoldier(address_t address) {
	auto it = std::find_if(soldiers.begin(), soldiers.end(), [&](HumanSoldier& soldier) -> bool { return address == soldier.GetAddress(); });
	if (it != soldiers.end()) return *it;
	return {};
}

HumanSoldier HumanSoldier::FindSoldier(string model_id) {
	auto it = std::find_if(soldiers.begin(), soldiers.end(), [&](HumanSoldier& soldier) -> bool { return model_id == soldier.GetModelId(); });
	if (it != soldiers.end()) return *it;
	return {};
}

HumanSoldier HumanSoldier::FindSoldier(ai_t ai_id) {
	auto it = std::find_if(soldiers.begin(), soldiers.end(), [&](HumanSoldier& soldier) -> bool { return ai_id == soldier.GetAIId(); });
	if (it != soldiers.end()) return *it;
	return {};
}

HumanSoldier HumanSoldier::FindSoldier(graph_t graph_id) {
	auto it = std::find_if(soldiers.begin(), soldiers.end(), [&](HumanSoldier& soldier) -> bool { return graph_id == soldier.GetGraphId(); });
	if (it != soldiers.end()) return *it;
	return {};
}

void HumanSoldier::PrintSoldierDataList() {
	std::stringstream ss;
	for (auto& soldier : soldiers) {
		if (soldier.ValidateSoldier()) {
			ss << "Model: " << soldier.model_id << "\tSoldierId: " << std::dec << soldier.soldier_id << "\tAIType: " << soldier.ai_type << "\tGraphId: " << std::dec << soldier.graph_id << "\tWeapon: " << soldier.weapon << "\tAddress: " << HEX_ADDR_FMT(soldier.address);

#ifdef SOLDIER_DATA_ALL
			ss << "\tAi_id: " << std::dec << soldier.ai_id << "\nAI_TYPE: " << soldier.ai_type << "\tGraphId: " << soldier.graph_id;
#endif
			LOG_FILE("SoldierData: %s", ss.str().c_str());
			ss.str(std::string());
		}
		else LOG_CONSOLE("Soldier data invalid for Id: %d", soldier.GetSoldierId());
	}
}

void HumanSoldier::PrintSoldierData() {
	std::stringstream ss;
	auto soldier = *this;
	if (!soldier.ValidateSoldier())
		LOG_CONSOLE("Soldier data invalid for Id: %d", soldier.GetSoldierId());

	ss << "Model: " << soldier.model_id << " Id: " << soldier.soldier_id << " " << soldier.weapon << " Address: " << HEX_ADDR_FMT(soldier.address);

#ifdef SOLDIER_DATA_ALL
	ss << " AIid: " << std::dec << soldier.ai_id << " " << soldier.ai_type << " GraphId: " << std::dec << soldier.graph_id;
#endif
	if (!soldier.ValidateSoldier())
		LOG_FILE("Soldier: %s", ss.str().c_str());
	else
		LOG_CONSOLE("Soldier: %s", ss.str().c_str());
}
