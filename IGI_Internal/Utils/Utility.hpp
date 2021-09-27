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
	string ReadFile(string);
	void WriteFile(string, string);
	bool StrCaseCompare(const string&, const string&);
}

#endif // __UTILITY_H__