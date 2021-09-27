#include "Natives.hpp" 
#include "Utility.hpp"
#include "GTLibc.hpp"
#include <DbgHelper.hpp>

using namespace IGI;
using namespace Utility;


//Friend methods - Outside class.
namespace IGI {
	// conversion functions for NativeSig
	void to_json(json& j, const Natives::NativeSig& native_sig) {
		j = json{ {"address", native_sig.address},
				 {"name", native_sig.name},
				 {"signature", native_sig.signature},
				 {"note", native_sig.note} };
	}

	void from_json(const json& j, Natives::NativeSig& native_sig) {
		j.at("address").get_to(native_sig.address);
		j.at("name").get_to(native_sig.name);
		j.at("signature").get_to(native_sig.signature);
		j.at("note").get_to(native_sig.note);
	}

	bool LoadNativesFile(string file_name) {
		bool status = false;
		json json_data = g_Natives->ReadJsonFile(file_name);

		if (!json_data.empty()) {
			json& json_array = json_data["Natives"];//Get the Natives array.

			// convert json array of "NativeSig" to native_sig list.
			for (const auto& j : json_array) {
				if (auto it = j.find("Native"); it != j.end()) {
					g_Natives->native_sig.push_back(it->get<Natives::NativeSig>());
				}
			}
			status = true;
		}
		return status;
	}
}


Natives::Natives() {
	g_Natives = this;
	Natives::InitNativesSignatures();
}

Natives::~Natives() {
	g_Natives = nullptr;
}

void Natives::InitNativesSignatures() {
	const string natives_file = GetModuleFolder() + "\\" + NATIVES_FILE_NAME;

	bool nativesLoaded = LoadNativesFile(natives_file);

	if (!nativesLoaded) {

		string err_str = ("Method: " + std::string(FUNC_NAME) + "()\nReason: " + std::string("Natives definition couldn't be loaded from ") + std::string(NATIVES_FILE_NAME));
		LOG_ERROR("%s",err_str.c_str());
		GT_ShowError(err_str.c_str());
		throw std::exception(err_str.c_str());
	}
	else
		LOG_DEBUG("%s(): Natives definition loaded from '%s'",FUNC_NAME, NATIVES_FILE_NAME);
}

string Natives::FindNativeName(uint32_t native_address) {
	for (const auto& native : native_sig) {
		if (native.address == native_address)
			return native.name;
	}
	LOG_ERROR("%s() : Native name couldn't be found for 0x%X", FUNC_NAME, native_address);
	return string("");
}

uint32_t Natives::FindNativeAddress(string native_name) {

	for (const auto& native : native_sig) {

		if (Utility::StrCaseCompare(native.name, native_name)) {
			return native.address;
		}
	}
	LOG_ERROR("%s() : Native address couldn't be found for '%s'", FUNC_NAME, native_name);
	return 0;
}

string Natives::FindNativeSignature(uint32_t native_address) {
	for (const auto& native : native_sig) {
		if (native.address == native_address)
			return native.signature;
	}
	LOG_ERROR("%s() : Native signature couldn't be found for 0x%X", FUNC_NAME, native_address);
	return string("");
}

string Natives::FindNativeSignature(string native_name) {

	for (const auto& native : native_sig) {

		if (Utility::StrCaseCompare(native.name, native_name)) {
			return native.signature;
		}
	}
	LOG_ERROR("%s() : Native signature couldn't be found for '%s'", FUNC_NAME, native_name);
	return string("");
}

string Natives::FindNativeNote(string native_name) {

	for (const auto& native : native_sig) {

		if (Utility::StrCaseCompare(native.name, native_name)) {
			return native.note;
		}
	}
	LOG_ERROR("%s() : Native note couldn't be found for '%s'", FUNC_NAME, native_name);
	return string("");
}

string Natives::FindNativeNote(uint32_t native_address) {
	for (const auto& native : native_sig) {
		if (native.address == native_address)
			return native.note;
	}
	LOG_ERROR("%s() : Native note couldn't be found for 0x%X", FUNC_NAME, native_address);
	return string("");
}


int Natives::GetNativesCount() { return native_sig.size(); }

//JSON - Methods.

json Natives::ReadJsonFile(std::string file_name) {
	json json_data;
	try {
		std::ifstream ifs(file_name);
		if (ifs.is_open())
			json_data = json::parse(ifs);
		else {
			string err_str = ("Method: " + std::string(FUNC_NAME) + "()\nReason: " + std::string("Error parsing JSON from ") + file_name);
			GT_ShowError(err_str.c_str());
		}
	}
	catch (std::exception& ex) {
		GT_ShowError(ex.what());
	}
	return json_data;
}

void Natives::WriteJSON(string json_file, json json_obj) {
	// write prettified JSON to another file
	std::ofstream fout(json_file);
	fout << std::setw(4) << json_obj << std::endl;
	fout.close();
}

void Natives::GenerateNativesFile(string natives_file) {
	string natives_json = "{\n\t\"Natives\": [\n";

	for (const auto& native_sig : native_sig) {
		natives_json += "{\n\t\t\"Native\":{\n\t\t\t\"address\": " + std::to_string(native_sig.address) + ",";
		natives_json += "\n\t\t\t\"name\": \"" + native_sig.name + "\",";
		natives_json += "\n\t\t\t\"signature\": \"" + native_sig.signature + "\",";
		natives_json += "\n\t\t\t\"note\": \"" + native_sig.note + "\"}\n\t\t\t\t},";
	}
	natives_json += "]\n}";
	Utility::WriteFile(natives_file, natives_json);
}
