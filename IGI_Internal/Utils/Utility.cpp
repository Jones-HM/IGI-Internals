#include "Utility.hpp" 
#include "GTLibc.hpp"
#include "Logger.hpp"

inline HMODULE g_Module;
inline HANDLE g_Handle;
using namespace IGI;


const string Utility::GetExecutableFolder() {

	char file_name[MAX_PATH];
	GetModuleFileNameA(NULL, file_name, MAX_PATH);

	string current_path = file_name;
	return current_path.substr(0, current_path.find_last_of("\\"));
}

const string Utility::GetModuleFolder() {

	char file_name[MAX_PATH];
	GetModuleFileNameA(g_Module, file_name, MAX_PATH);

	string current_path = file_name;
	return current_path.substr(0, current_path.find_last_of("\\"));
}

void Utility::SetModuleHandle(const HMODULE module) {

	g_Module = module;
}

const HMODULE Utility::GetModuleHandle() {

	return g_Module;
}

void Utility::SetHandle(const HANDLE handle) {

	g_Handle = handle;
}

const HANDLE Utility::GetHandle() {

	return g_Handle;
}


//Returns the last Win32 error, in string format. Returns an empty string if there is no error. 
string Utility::GetLastErrorString()
{
	//Get the error message ID, if any. 
	DWORD error_message_i_d = ::GetLastError();
	if (error_message_i_d == 0) {
		return string(); //No error message has been recorded 
	}

	LPSTR message_buffer = nullptr;

	//Ask Win32 to give us the string version of that message ID. 
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be). 
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, error_message_i_d, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message_buffer, 0, NULL);

	//Copy the error message into a string. 
	string message(message_buffer, size);

	//Free the Win32's string's buffer. 
	LocalFree(message_buffer);

	return message;
}

string Utility::GetBaseFileName(string file_name)
{
	size_t pos = file_name.rfind(("."));
	if (pos == string::npos)  //No extension. 
		return file_name;

	if (pos == 0)    //. is at the front. Not an extension. 
		return file_name;

	return file_name.substr(0, pos);
}

const string Utility::GetModuleName(const HMODULE module) {

	char file_name[MAX_PATH];
	GetModuleFileNameA(module, file_name, MAX_PATH);

	string full_path = file_name;

	size_t lastIndex = full_path.find_last_of("\\") + 1;
	return full_path.substr(lastIndex, full_path.length() - lastIndex);
}

const string Utility::GetModuleNameExtension(const HMODULE module) {

	const string file_name = GetModuleName(module);

	size_t last_index = file_name.find_last_of(".");
	if (last_index == -1) {
		return file_name;
	}

	return file_name.substr(0, last_index);
}

std::tuple<std::vector<uint8_t>, string, string> Utility::ReadFile(string file_name, int file_type)
{
	std::ifstream in_stream(file_name, (file_type == ASCII_FILE) ? std::ios::in : (file_type == BINARY_FILE || file_type == HEX_FILE) ? std::ios::binary : std::ios::in);

	/*Buffers to store output data from file.*/
	string str_buf, hex_buf;
	std::vector<uint8_t> vec_buf;

	try {
		if (file_type == BINARY_FILE) {

			/*Open the stream in binary mode.*/

			if (in_stream.good()) {
				/*Read Binary data using streambuffer iterators.*/
				std::vector<uint8_t> v_buf((std::istreambuf_iterator<char>(in_stream)), (std::istreambuf_iterator<char>()));
				vec_buf = v_buf;
				in_stream.close();
			}

			else {
				throw std::exception();
			}

		}

		else if (file_type == ASCII_FILE) {

			/*Open the stream in default mode.*/
			string ascii_data;

			if (in_stream.good()) {
				/*Read ASCII data using getline*/
				while (getline(in_stream, ascii_data))
					str_buf += ascii_data + "\n";

				in_stream.close();
			}
			else {
				throw std::exception();
			}
		}

		else if (file_type == HEX_FILE) {

			/*Open the stream in default mode.*/

			if (in_stream.good()) {
				/*Read Hex data using streambuffer iterators.*/
				std::vector<char> h_buf((std::istreambuf_iterator<char>(in_stream)), (std::istreambuf_iterator<char>()));
				string hex_str_buf(h_buf.begin(), h_buf.end());
				hex_buf = hex_str_buf;

				in_stream.close();
			}
			else {
				throw std::exception();
			}
		}

	}

	catch (...) {
		string ex_str = "Error: " + file_name + ": No such file or directory";
		throw std::exception(ex_str.c_str());
	}

	auto tuple_data = make_tuple(vec_buf, str_buf, hex_buf);
	return tuple_data;
}

bool Utility::WriteFile(string file_name, binary_t file_data, int file_type)
{
	bool write_status = false;
	try {
		std::fstream out_stream(file_name, (file_type == ASCII_FILE) ? std::ios::out : (file_type == BINARY_FILE || file_type == HEX_FILE) ? (std::ios::out | std::ios::binary) : std::ios::out);

		if (out_stream.is_open()) {
			if (file_type == ASCII_FILE) {
				out_stream << file_data.data();
				write_status = true;
			}

			else if (file_type == BINARY_FILE) {
				out_stream.write((char*)file_data.data(), file_data.capacity());
				write_status = true;
			}

			else if (file_type == HEX_FILE) {
				int bin_data;
				std::stringstream ss;
				ss << std::hex << file_data.data();

				while (ss >> bin_data)
					out_stream.write(reinterpret_cast<const char*>(&bin_data), sizeof(char));

				write_status = true;
			}
			out_stream.close();
		}
		else {
			string ex_str = "Error: couldn't open " + file_name + " for output";
			throw std::exception(ex_str.c_str());
		}
	}
	catch (const std::exception& ex) {
		string ex_str = "Error: " + file_name + ": No such file or directory";
		LOG_ERROR("%s Exception: %s", FUNC_NAME, ex.what());
		GT_ShowError(ex.what());
	}
	return write_status;
}

bool Utility::RemoveFile(const string& file_name) {
	bool status = false;
	try {
		if (std::filesystem::remove(file_name))
			status = true;
	}
	catch (const std::filesystem::filesystem_error& err) {
		string err_msg = "Delete file exception: " + std::string(err.what());
		GT_ShowError(err_msg.c_str());
	}
	return status;
}

bool Utility::WriteMemory(LPVOID address,binary_t& v_bytes)
{
	if (address == NULL || v_bytes.size() == 0) {
		GT_ShowError("Error occurred while writing data to memory.");
		return false;
	}

	DWORD old_protection = NULL;
	const SIZE_T write_len = v_bytes.size() * sizeof(byte);

	VirtualProtect(address, write_len, PAGE_EXECUTE_READWRITE, &old_protection);
	std::memcpy(address, &v_bytes[0], write_len);
	VirtualProtect(address, write_len, old_protection, &old_protection);

	return true;
};

bool Utility::StrCaseCompare(const string& a, const string& b) {
	return std::equal(a.begin(), a.end(), b.begin(), b.end(), 
		[](char a, char b) {return tolower(a) == tolower(b); 
	});
}

size_t Utility::Find(string& data, string& search, size_t pos)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	std::transform(search.begin(), search.end(), search.begin(), ::tolower);
	return data.find(search, pos);
}

bool Utility::IsAsciiStr(const string& s)
{
    return std::all_of(s.begin(), s.end(), [](unsigned char c) { 
        return c >= 0x20 && c <= 0x7F; 
    });
}

void Utility::Tokenize(string& str_in, char delim, vector<string>& vec_out) {
	size_t start;
	size_t end = 0;

	while ((start = str_in.find_first_not_of(delim, end)) != string::npos)
	{
		end = str_in.find(delim, start);
		vec_out.push_back(str_in.substr(start, end - start));
	}
}

void Utility::Replace(string& str, const string& from, const string& to) {
	if (from.empty()) return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

int Utility::ShellExec(string shell_cmd)
{
	if (shell_cmd.empty()) return EXIT_FAILURE;
	return system(shell_cmd.c_str());
}

std::string Utility::Trim(const std::string& str, const std::string& whitespace)
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

string Utility::GetAppdataPath() {
	char* appdata = getenv("APPDATA");
	return string(appdata);
}