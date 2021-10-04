#pragma once 
#ifndef __UTILS_H__ 
#define __UTILS_H__ 

#include "Common.hpp" 
using std::string;

namespace Utility {

	const string GetRunningExecutableFolder();

	const string GetModuleFolder();

	void    SetModuleHandle(const HMODULE);
	const HMODULE  GetModuleHandle();
	void    SetHandle(const HANDLE);
	const HANDLE  GetHandle();
	string   GetBaseFileName(string);
	string   GetLastErrorAsString();
	const string GetModuleName(const HMODULE);
	const string GetModuleNameWithoutExtension(const HMODULE);
	string ReadFile(const string&, bool = false);
	void WriteFile(const string&, string, bool=false);
	bool RemoveFile(const string&);
	bool WriteMemory(LPVOID, std::vector<byte>&);
	bool StrCaseCompare(const string&, const string&);
	bool IsAsciiStr(const std::string&);
}

#endif // __UTILITY_H__