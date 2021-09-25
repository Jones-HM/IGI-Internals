#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include "Common.hpp"

namespace Utility {

	const std::string	GetRunningExecutableFolder();

	const std::string	GetModuleFolder();

	void				SetModuleHandle(const HMODULE);
	const HMODULE		GetModuleHandle();
	void				SetHandle(const HANDLE);
	const HANDLE		GetHandle();
	std::string			GetBaseFileName(std::string);
	std::string			GetLastErrorAsString();
	const std::string	GetModuleName(const HMODULE);
	const std::string	GetModuleNameWithoutExtension(const HMODULE);
}

#endif // __UTILITY_H__