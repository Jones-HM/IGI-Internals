#pragma once
#include "IGINatives.h"
#include "IGINativeCaller.h"

namespace NATIVE_HELPER {
auto g_IGINativeCaller = IGINativeCaller::getInstance();
#define IGI_NATIVE_CONFIG_PARSE(cfg_file) g_IGINativeCaller->NativeCaller<char*>((int)NATIVE_HASH::HASH::CONFIG_PARSE_HASH,cfg_file);
#define IGI_NATIVE_CONFIG_CREATE(cfg_file) g_IGINativeCaller->NativeCaller<char*>((int)NATIVE_HASH::HASH::CONFIG_CREATE_HASH,cfg_file);
}