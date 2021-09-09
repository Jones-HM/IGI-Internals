#pragma once
#include "IGINatives.h"
#include "framework.h"
#define FORCEINLINE __forceinline
using namespace NATIVE_HASH;

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

class IGINativeCaller
{
private:
	int nativeArgc = 0;
	std::list<std::map<int, std::function<int(const char*)>>> nativeMap1List1;
	std::list<std::map<int, std::function<int(int)>>> nativeMap1List2;
	std::list<std::map<int, std::function<int(const char*, int)>>> nativeMap2List1;
	std::list<std::map<int, std::function<int(int, const char*)>>> nativeMap2List2;

	IGINativeCaller() {
		InitHashMaps<void>();
	}

	template<typename NF, typename NM>
	void NativeMapInsert(int nativeHash, NF nativeFunc, NM& nativeMap) {
		//nativeMap.insert(std::pair<decltype(nativeHash), decltype(nativeFunc)>>(nativeHash, nativeFunc));
		nativeMap[nativeHash] = nativeFunc;
		//assert(nativeMap.find(nativeHash)->second == nativeFunc); // post-condition
	}

	//Init Hashes with only 1 Arguments.
	void InitHashesArgs1() {
		std::map<int, std::function<int(const char*)>> nativeMap1;
		//Declaring Function pointers to Native hashes.
		std::function<int(const char*)> ConfigParse{ (int(__cdecl*)(const char*))HASH::CONFIG_PARSE_HASH };
		std::function<int(const char*)> ConfigCreate{ (int(__cdecl*)(const char*))HASH::CONFIG_CREATE_HASH };

		//Init ConfigParse method.
		//nativeMap1.insert(std::pair<int, std::function<int(char*)>>((int)HASH::CONFIG_PARSE_HASH, ConfigParse));
		//NativeMapInit<decltype(nativeMap1),nativeMap1List1>(nativeMap1);

		//Add ConfigParse method to map.
		NativeMapInsert((int)HASH::CONFIG_PARSE_HASH, ConfigParse, nativeMap1);
		nativeMap1List1.push_back(nativeMap1);

		//Add ConfigCreate method to map.
		NativeMapInsert((int)HASH::CONFIG_CREATE_HASH, ConfigCreate, nativeMap1);
		nativeMap1List1.push_back(nativeMap1);

		//nativeMap1.insert(std::pair <int, std::function<int(char*)>>((int)HASH::CONFIG_CREATE_HASH, ConfigCreate));
		//nativeMap1List1.push_back(nativeMap1);
		//NativeMapInit<decltype(nativeMap1),nativeMap1List1>(nativeMap1);
	}

	void InitHashesArgs2() {
		std::map<int, std::function<int(int, const char*)>> nativeMap2;
		std::function<int(int, const char*)> ParseWeaponConfig{ (int(__cdecl*)(int index, const char* cfgFile))HASH::WEAPON_PARSE_CONFIG_HASH };
		
		
		NativeMapInsert((int)HASH::WEAPON_PARSE_CONFIG_HASH, ParseWeaponConfig, nativeMap2);
		nativeMap2List2.push_back(nativeMap2);
	}

	void InitHashesArgs3() {
	}

	void InitHashesArgs4() {
	}

	template <typename T>
	void InitHashMaps() {
		InitHashesArgs1();
	}

public:
	IGINativeCaller(IGINativeCaller const&) = delete;
	IGINativeCaller& operator=(IGINativeCaller const&) = delete;

	static std::shared_ptr<IGINativeCaller> getInstance()
	{
		static std::shared_ptr<IGINativeCaller> s{ new IGINativeCaller };
		return s;
	}


	template <typename T>
	void NativeCall(int nativeHash, T param1) {
		NativeInvoke(nativeHash, param1);
	}

	template <typename T1, typename T2>
	void NativeCall(int nativeHash, T1 param1, T2 param2) {
		NativeInvoke(nativeHash, param1, param2);
	}

	template <typename T1, typename T2, typename T3>
	void NativeCall(int nativeHash, T1 param1, T2 param2, T3 param3) {
		NativeInvoke(nativeHash, param1, param2, param3);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void NativeCall(int nativeHash, T1 param1, T2 param2 = NULL, T3 param3 = NULL, T4 param4 = NULL) {
		NativeInvoke(nativeHash, param1, param2, param3, param4);
	}

	template<class... Args>
	void NativeCaller(Args... args)
	{
		nativeArgc = sizeof...(Args) - 1;
		NativeCall(args...);
	}


	template <typename T1, typename T2 = int, typename T3 = int, typename T4 = int>
	void NativeInvoke(int nativeHash, T1 param1 = NULL, T2 param2 = NULL, T3 param3 = NULL, T4 param4 = NULL) {
		LOG_CONSOLE("%s() Hash : 0x%X param1: %p param2: %p param3: %p param4: %p nativeArgc : %d", FUNC_NAME, nativeHash, param1, param2, param3, param4,nativeArgc);

		if (nativeArgc == 1) {
			
			if (CHECK_TYPE_CONST_CHAR_PTR(param1)) {
				LOG_CONSOLE("%s() nativeArgc1 param1: %s type : %s", FUNC_NAME, (const char*)param1, TYPEID(param1));
				NativeInvokeHash1(nativeHash, (const char*)param1, nativeMap1List1);
			}
			else if (CHECK_TYPE_INT(param1)) {
				LOG_CONSOLE("%s() nativeArgc1 param1: %p type : %s", FUNC_NAME, (int)param1, TYPEID(param1));
				NativeInvokeHash1(nativeHash, (int)param1, nativeMap1List2);
			}
			
		}
		else if (nativeArgc == 2) {
			
			if (CHECK_TYPE_CONST_CHAR_PTR(param1) && CHECK_TYPE_INT(param2)) {
				LOG_CONSOLE("%s() nativeArgc2 param1: %s type : %s param2: %p type : %s", FUNC_NAME, (const char*)param1, TYPEID(param1), (int)param2, TYPEID(param2));
				NativeInvokeHash2(nativeHash, (const char*)param1, (int)param2, nativeMap2List1);
			}
			else if (CHECK_TYPE_INT(param1) && CHECK_TYPE_CONST_CHAR_PTR(param2)) {
				LOG_CONSOLE("%s() nativeArgc2 param1: %p type : %d param2: %s type : %s", FUNC_NAME, (int)param1, TYPEID(param1), (const char*)param2, TYPEID(param2));
				NativeInvokeHash2(nativeHash, (int)param1, (const char*)param2, nativeMap2List2);
			}
		}
	}

	//Hashes call with 1 Args.
	template <typename T1,typename L>
	FORCEINLINE void NativeInvokeHash1(int nativeHash, T1 param, L nativeMapListArg) {
		LOG_CONSOLE("%s() Hash : 0x%X param: %p type : %s nativeArgc: %d", FUNC_NAME, nativeHash, param, TYPEID(param), nativeArgc);

			for (auto const& nativeMapList : nativeMapListArg) {
				for (auto const& nativeMap : nativeMapList) {
					if (nativeMap.first == nativeHash) {
						auto NativeHandler = nativeMap.second;
						auto nativeArg = param;
						NativeHandler(nativeArg);
						LOG_CONSOLE("%s calling with Hash : 0x%X param : %p\n", FUNC_NAME, nativeHash, param);
						return;
					}
				}
			}
			LOG_ERROR("Error finding handler for Hash 0x%X", nativeHash);
	}

	//Hashes call with 2 Args.
	template <typename T1, typename T2, typename L>
	FORCEINLINE void NativeInvokeHash2(int nativeHash, T1 param1,T2 param2, L nativeMapListArg) {
		LOG_CONSOLE("%s() Hash : 0x%X param1: %p type : %s param2: %p type : %s  nativeArgc: %d", FUNC_NAME, nativeHash, param1, TYPEID(param1), param2, TYPEID(param2), nativeArgc);

		for (auto const& nativeMapList : nativeMapListArg) {
			for (auto const& nativeMap : nativeMapList) {
				if (nativeMap.first == nativeHash) {
					auto NativeHandler = nativeMap.second;
					NativeHandler(param1,param2);
					LOG_CONSOLE("%s calling with Hash %p param1 : %p param2 : %p\n", FUNC_NAME,nativeHash, param1,param2);
					return;
				}
			}
		}
		LOG_ERROR("Error finding handler for Hash 0x%X", nativeHash);
	}

};