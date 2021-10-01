#include "Utility.hpp" 
#include <GTLibc.hpp>

inline HMODULE g_Module;
inline HANDLE g_Handle;

const string Utility::GetRunningExecutableFolder() {

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
string Utility::GetLastErrorAsString()
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

const string Utility::GetModuleNameWithoutExtension(const HMODULE module) {

	const string file_nameWithExtension = GetModuleName(module);

	size_t lastIndex = file_nameWithExtension.find_last_of(".");
	if (lastIndex == -1) {
		return file_nameWithExtension;
	}

	return file_nameWithExtension.substr(0, lastIndex);
}

string Utility::ReadFile(string file_name) {
	try {
		std::ifstream ifs(file_name);
		std::string file_data(std::istreambuf_iterator<char>{ifs}, {});
		return file_data;
	}
	catch (std::exception& e) {
		std::cerr << "Exception : " << e.what() << std::endl;
	}
	return {};
}

void Utility::WriteFile(string file_name, string file_data) {
	std::ofstream fout(file_name);
	fout << file_data << std::endl;
	fout.close();
}

bool Utility::WriteMemory(LPVOID address, std::vector<byte>& v_bytes)
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

bool Utility::IsAsciiStr(const std::string& s)
{
    return std::all_of(s.begin(), s.end(), [](unsigned char c) { 
        return c >= 0x20 && c <= 0x7F; 
    });
}
