#include "Utility.hpp"

static HMODULE g_Module;
static HANDLE g_Handle;

const std::string Utility::GetRunningExecutableFolder() {

	char fileName[MAX_PATH];
	GetModuleFileNameA(NULL, fileName, MAX_PATH);

	std::string currentPath = fileName;
	return currentPath.substr(0, currentPath.find_last_of("\\"));
}

const std::string Utility::GetModuleFolder() {

	char fileName[MAX_PATH];
	GetModuleFileNameA(g_Module, fileName, MAX_PATH);

	std::string currentPath = fileName;
	return currentPath.substr(0, currentPath.find_last_of("\\"));
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
std::string Utility::GetLastErrorAsString()
{
	//Get the error message ID, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}

	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	return message;
}

std::string Utility::GetBaseFileName(std::string fileName)
{
	size_t pos = fileName.rfind(("."));
	if (pos == std::string::npos)  //No extension.
		return fileName;

	if (pos == 0)    //. is at the front. Not an extension.
		return fileName;

	return fileName.substr(0, pos);
}

const std::string Utility::GetModuleName(const HMODULE module) {

	char fileName[MAX_PATH];
	GetModuleFileNameA(module, fileName, MAX_PATH);

	std::string fullPath = fileName;

	size_t lastIndex = fullPath.find_last_of("\\") + 1;
	return fullPath.substr(lastIndex, fullPath.length() - lastIndex);
}

const std::string Utility::GetModuleNameWithoutExtension(const HMODULE module) {

	const std::string fileNameWithExtension = GetModuleName(module);

	size_t lastIndex = fileNameWithExtension.find_last_of(".");
	if (lastIndex == -1) {
		return fileNameWithExtension;
	}

	return fileNameWithExtension.substr(0, lastIndex);
}