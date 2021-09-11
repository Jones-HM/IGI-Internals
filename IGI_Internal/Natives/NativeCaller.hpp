#pragma once
#define FORCEINLINE __forceinline
#include "Common.h"
#include "Logger.hpp"
using namespace Utility;

/*Re-Defining standard constants*/
#if !defined(FILE_NAME) && !defined(LINE_NO) && !defined(FUNC_NAME)
#define FILE_NAME __FILE__
#define LINE_NO __LINE__
#define FUNC_NAME __func__
#endif

#define TYPEID(x) typeid(x).name()
#define TYPE(x) std::string(typeid(x).name())
#define CHECK_TYPE(x,y) TYPE(x) == std::string(y)
#define CHECK_TYPE_INT(x) CHECK_TYPE(x, "int")
#define CHECK_TYPE_CHAR(x) CHECK_TYPE(x, "char")
#define CHECK_TYPE_CHAR_PTR(x) CHECK_TYPE(x, "char*")
#define CHECK_TYPE_CONST_CHAR_PTR(x) CHECK_TYPE(x, "char const *")
#define CHECK_TYPE_INT_PTR(x) CHECK_TYPE(x, "int*")
#define CHECK_TYPE_CONST_INT_PTR(x) CHECK_TYPE(x, "int const *")
#define LINE_DEBUG LOG_CONSOLE("[%s] LINE : %d", FUNC_NAME, LINE_NO);

namespace IGI {
	class NativeCaller
	{
	private:
		int nativeArgc = 0;
		std::list<std::map<int, std::function<int(void)>>> nativeMap0List0;
		std::list<std::map<int, std::function<int(const char*)>>> nativeMap1List1;
		std::list<std::map<int, std::function<int(int)>>> nativeMap1List2;
		std::list<std::map<int, std::function<int(const char*, int)>>> nativeMap2List1;
		std::list<std::map<int, std::function<int(int, const char*)>>> nativeMap2List2;

		template<typename NF, typename NM>
		void NativeMapInsert(int nativeHash, NF nativeFunc, NM& nativeMap) {
			nativeMap[nativeHash] = nativeFunc;
		}

		//Init Hashes with only 0 Arguments.
		void InitHashesArgs0();

		//Init Hashes with only 1 Arguments.
		void InitHashesArgs1();
		void InitHashesArgs2();
		void InitHashesArgs3();
		void InitHashesArgs4();

		void InitHashMaps();

	public:

		NativeCaller();
		~NativeCaller();

		template<class... Args>
		void NativeInvoke(Args... args)
		{
			nativeArgc = sizeof...(Args) - 1;
			NativeCall(args...);
		}

		template <typename T>
		void NativeCall(int nativeHash) {
			NativeInvokeHash(nativeHash);
		}

		template <typename T>
		void NativeCall(int nativeHash, T param) {
			NativeInvokeHash(nativeHash, param);
		}

		template <typename T1, typename T2>
		void NativeCall(int nativeHash, T1 param1, T2 param2) {
			NativeInvokeHash(nativeHash, param1, param2);
		}

		template <typename T1, typename T2, typename T3>
		void NativeCall(int nativeHash, T1 param1, T2 param2, T3 param3) {
			NativeInvokeHash(nativeHash, param1, param2, param3);
		}

		template <typename T1, typename T2, typename T3, typename T4>
		void NativeCall(int nativeHash, T1 param1, T2 param2 = NULL, T3 param3 = NULL, T4 param4 = NULL) {
			NativeInvokeHash(nativeHash, param1, param2, param3, param4);
		}

		template <typename T1, typename T2 = int, typename T3 = int, typename T4 = int>
		void NativeInvokeHash(int nativeHash, T1 param1 = NULL, T2 param2 = NULL, T3 param3 = NULL, T4 param4 = NULL) {
			static_assert(sizeof(nativeHash) <= sizeof(uint32_t));

			LOG_INFO("%s() Hash : 0x%X param1: %p param2: %p param3: %p param4: %p nativeArgc : %d", FUNC_NAME, nativeHash, param1, param2, param3, param4, nativeArgc);

			if (nativeArgc == 0) {
			}

			if (nativeArgc == 1) {

				if (CHECK_TYPE_CONST_CHAR_PTR(param1)) {
					LOG_INFO("%s() nativeArgc1 param1: %s type : %s", FUNC_NAME, (const char*)param1, TYPEID(param1));
					NativeInvokeHash1(nativeHash, (const char*)param1, nativeMap1List1);
				}
				else if (CHECK_TYPE_INT(param1)) {
					LOG_INFO("%s() nativeArgc1 param1: %p type : %s", FUNC_NAME, (int)param1, TYPEID(param1));
					NativeInvokeHash1(nativeHash, (int)param1, nativeMap1List2);
				}

			}
			else if (nativeArgc == 2) {

				if (CHECK_TYPE_CONST_CHAR_PTR(param1) && CHECK_TYPE_INT(param2)) {
					LOG_INFO("%s() nativeArgc2 param1: %s type : %s param2: %p type : %s", FUNC_NAME, (const char*)param1, TYPEID(param1), (int)param2, TYPEID(param2));
					NativeInvokeHash2(nativeHash, (const char*)param1, (int)param2, nativeMap2List1);
				}
				else if (CHECK_TYPE_INT(param1) && CHECK_TYPE_CONST_CHAR_PTR(param2)) {
					LOG_INFO("%s() nativeArgc2 param1: %p type : %s param2: %s type : %s", FUNC_NAME, (int)param1, TYPEID(param1), (const char*)param2, TYPEID(param2));
					NativeInvokeHash2(nativeHash, (int)param1, (const char*)param2, nativeMap2List2);
				}
			}
		}

		//Hashes call with 1 Args.
		template <typename T1, typename L>
		FORCEINLINE void NativeInvokeHash1(int nativeHash, T1 param, L nativeMapListArg) {
			LOG_INFO("%s() Hash : 0x%X param: %p type : %s nativeArgc: %d", FUNC_NAME, nativeHash, param, TYPEID(param), nativeArgc);

			for (auto const& nativeMapList : nativeMapListArg) {
				for (auto const& nativeMap : nativeMapList) {
					if (nativeMap.first == nativeHash) {
						auto NativeHandler = nativeMap.second;
						auto nativeArg = param;
						NativeHandler(nativeArg);
						LOG_INFO("%s calling with Hash : 0x%X param : %p\n", FUNC_NAME, nativeHash, param);
						return;
					}
				}
			}
			LOG_ERROR("Error finding handler for Hash 0x%X", nativeHash);
		}

		//Hashes call with 2 Args.
		template <typename T1, typename T2, typename L>
		FORCEINLINE void NativeInvokeHash2(int nativeHash, T1 param1, T2 param2, L nativeMapListArg) {
			LOG_INFO("%s() Hash : 0x%X param1: %p type : %s param2: %p type : %s  nativeArgc: %d", FUNC_NAME, nativeHash, param1, TYPEID(param1), param2, TYPEID(param2), nativeArgc);

			for (auto const& nativeMapList : nativeMapListArg) {
				for (auto const& nativeMap : nativeMapList) {
					if (nativeMap.first == nativeHash) {
						auto NativeHandler = nativeMap.second;
						NativeHandler(param1, param2);
						LOG_INFO("%s calling with Hash %p param1 : %p param2 : %p\n", FUNC_NAME, nativeHash, param1, param2);
						return;
					}
				}
			}
			LOG_ERROR("Error finding handler for Hash 0x%X", nativeHash);
		}
	};
	inline NativeCaller g_NativeCaller;
}
