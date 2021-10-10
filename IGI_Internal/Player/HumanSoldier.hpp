#pragma once
#include "Common.hpp"
#include "CommonConst.hpp"

namespace IGI {
	//Class to hold Human Player/Soldier information.
	class HumanSoldier {
	private:
		struct {
			string model_id;//Model id of soldier - Like 001_01_1 -> Jones.
			uint16_t soldier_id; //HumanSoldier id - Like Id '2206' For Detection,Alive status,team etc, Expression: 'HumanAI_2206.isDetection'.
			int16_t ai_id; //A.I Id of soldier - Like Id '503' and File = 'AI/503.qvm'
			string ai_type; //AI Type Sniper,Patrol,Guard or Security Expression: 'AITYPE_PATROL_AK'.
			int32_t graph_id; //Graph Id of sodlier - Like 4019 and File: 'graphs/4019.dat'
			string weapon; //Weapon currently holding. - Not primary Weapon.
			bool is_dead; //Is Soldier dead ? Expression: 'HumanSoldier_XXXX.isDead'
			int8_t e_team; //Soldier team type , 0 = Friendly, 1 = Enemy. Expression: 'HumanSoldier_XXXX.eTeam'
			uint32_t address; //Address pointer of soldier.
		};
	public:
		HumanSoldier();
		HumanSoldier(string model_id, int ai_id, int graph_id, string weapon, bool is_dead, int e_team, uint32_t address);
		~HumanSoldier() = default;

		void AddSoldier();
		bool RemoveSoldier();
		void AddSoldierData(uint32_t address, std::vector<uint8_t> data);
		bool ValidateSoldier();
		static HumanSoldier FindSoldier(soldier_t soldier_id);
		static HumanSoldier FindSoldier(ai_t ai_id);
		static HumanSoldier FindSoldier(graph_t graph_id);
		static HumanSoldier FindSoldier(address_t address);
		static HumanSoldier FindSoldier(string model_id);
		void PrintSoldierData();
		static void PrintSoldierDataList();

		//Setters/Getters.
		string GetModelId() { return (!model_id.empty()) ? model_id : string(""); }
		uint16_t GetSoldierId() { return (soldier_id > 0 && soldier_id <= AI_ID_MAX) ? soldier_id : (uint16_t)AI_ID_INVALID; }
		int16_t GetAIId() { return (ai_id > 0 && ai_id <= AI_ID_MAX) ? ai_id : (int16_t)AI_ID_INVALID; }
		uint16_t GetGraphlId() { return (graph_id > 0 && graph_id <= AI_ID_MAX) ? graph_id : (uint16_t)AI_ID_INVALID; }
		string GetWeapon() { return (!weapon.empty()) ? weapon : string(""); }
		bool IsDead() { return is_dead; }
		void IsDead(bool value) { is_dead = value; }
		int8_t GetTeam() { return (e_team > 0 && e_team < 2) ? e_team : (int8_t)AI_ID_INVALID; }
		uint32_t GetAddress() { return (address > 0 && address < UINT32_MAX) ? address : (uint32_t)AI_ID_INVALID; }
	};
	inline std::vector<HumanSoldier> soldiers;
}