#include "Memory.hpp"
using namespace IGI;

Memory::Memory() {
	g_Memory = this;
}

Memory::~Memory() {
	g_Memory = nullptr;
}

std::uint8_t* Memory::PatternScan(void* module, const char* signature)
{
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_header = (PIMAGE_DOS_HEADER)module;
	auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dos_header->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scan_bytes[i];
		}
	}
	return nullptr;
}


bool Memory::SignatureScan(std::string& sig_error_reason) {

	bool status = true;

	auto module_handle = GetModuleHandle(NULL);
	auto game_loaded_sig_addr = PatternScan(module_handle, game_loaded_sig);
	auto player_loaded_sig_addr = PatternScan(module_handle, player_loaded_sig);
	auto game_running_sig_addr = PatternScan(module_handle, game_running_sig);

#if _DEBUG 
	LOG_DEBUG("GameLoaded Signature found at : %p", game_loaded_sig_addr);
	LOG_DEBUG("PlayerLoaded Signature found at : %p", player_loaded_sig_addr);
	LOG_DEBUG("GameRunning Signature found at : %p", game_running_sig_addr);
#endif 

	if (game_loaded_sig_addr == nullptr) {
		sig_error_reason = "\nGame load could not be verified!";
		status = false;
	}

	if (player_loaded_sig_addr == nullptr) {
		sig_error_reason = "\nPlayer load could not be verified!";
		status = false;
	}

	if (game_running_sig_addr != nullptr) {
		sig_error_reason = "\nGame Running could not be verified!";
		status = false;
	}
	return status;
}