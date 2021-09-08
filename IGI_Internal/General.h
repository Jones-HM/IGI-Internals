#pragma once
#ifndef __GENERAL_H__
#define __GENERAL_H__

#include "framework.h"

namespace Utility {

	const std::string	GetRunningExecutableFolder();

	const std::string	GetOurModuleFolder();

	void				SetOurModuleHandle(const HMODULE module);
	const HMODULE		GetOurModuleHandle();
	const std::string	GetModuleName(const HMODULE module);
	const std::string	GetModuleNameWithoutExtension(const HMODULE module);
}

#endif // __GENERAL_H__