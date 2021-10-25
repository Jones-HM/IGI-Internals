#pragma once 
#ifndef __UTILS_H__ 
#define __UTILS_H__ 

#include "Common.hpp" 
#include "CommonConst.hpp"
using std::string;
enum { BINARY_FILE, ASCII_FILE, HEX_FILE };
#define ReadFileType(file,FILE_TYPE) std::get<FILE_TYPE>(g_Utility.ReadFile(file, FILE_TYPE));
#define WriteFileType(file,data,FILE_TYPE) g_Utility.WriteFile(file,data,FILE_TYPE);

namespace IGI {
	class Utility {
	public:
		Utility() = default;
		~Utility() = default;

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
		std::tuple<std::vector<uint8_t>, string, string>  ReadFile(string, int);
		bool WriteFile(string, binary_t,int);
		bool RemoveFile(const string&);
		bool WriteMemory(LPVOID, binary_t&);
		bool StrCaseCompare(const string&, const string&);
		bool IsAsciiStr(const std::string&);
		void Tokenize(string&,char,vector<string>&);
	};
	inline Utility g_Utility;
}
#endif // __UTILITY_H__