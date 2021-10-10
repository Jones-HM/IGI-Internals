#pragma once 
#include "Common.hpp"
#include "Logger.hpp"
using namespace IGI;

namespace IGI {
	class Memory {
	private:
		const char* game_loaded_sig = "25 73 2F 6F 62 6A 65 63 74 73 2E"; //Level Load Path 
		const char* player_loaded_sig = "1E 00 00 00 24 BA ?? ??"; //Game FPS. 
		const char* game_running_sig = "02 00 00 00 00 00 00 00 58 53 68"; //Menu Manager 

		std::uint8_t* PatternScan(void*, const char*);
	public:
		Memory();
		Memory(bool);
		~Memory();
		Memory(const Memory&) = default;
		Memory(Memory&&) = default;

		bool WriteMemory(LPVOID,std::vector<byte>&);
		bool SignatureScan(std::string&);
	};
	inline Memory* g_Memory{};
}