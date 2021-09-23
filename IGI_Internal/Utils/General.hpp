#pragma once
#ifndef __GENERAL_H__
#define __GENERAL_H__

#include "Common.hpp"

namespace Utility {

	const std::string	GetRunningExecutableFolder();

	const std::string	GetModuleFolder();

	void				SetModuleHandle(const HMODULE module);
	const HMODULE		GetModuleHandle();
	const std::string	GetModuleName(const HMODULE module);
	const std::string	GetModuleNameWithoutExtension(const HMODULE module);
}

#endif // __GENERAL_H__