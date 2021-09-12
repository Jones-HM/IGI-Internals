#pragma once
#include "Common.h"

/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__
#define LINE_NO __LINE__
#define FUNC_NAME __func__
#endif

#define TYPEID(x) typeid(x).name()
#define TYPE(x) std::string(typeid(x).name())
#define CHECK_TYPE(x,y) TYPE(x) == std::string(y)
#define LINE_DEBUG LOG_DEBUG("[%s] LINE : %d", FUNC_NAME, LINE_NO);