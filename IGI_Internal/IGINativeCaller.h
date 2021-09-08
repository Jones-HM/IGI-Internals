#pragma once
#include "IGINatives.h"
#include <functional>
#include <map>
#include <list>
#include <memory>

class IGINativeCaller
{
private:
	std::map<int, std::function<int(char*)>> nativeMap1;
	std::list<std::map<int, std::function<int(char*)>>> nativeMap1List;

	IGINativeCaller() {
		InitHashMaps<void>();
	}

	template<typename T>
	void NativeMapInit(T nativeMap) {
		nativeMap1List.push_back(nativeMap);
		nativeMap.clear();
	}

	template <typename T>
	void InitHashMaps() {
		std::function<int(char*)> ConfigParse{ (int(__cdecl*)(char*))NATIVE_HASH::HASH::CONFIG_PARSE_HASH };
		std::function<int(char*)> ConfigCreate{ (int(__cdecl*)(char*))NATIVE_HASH::HASH::CONFIG_CREATE_HASH };

		LOG_CONSOLE("%s init hash CONFIG_PARSE_HASH\n", FUNC_NAME);
		nativeMap1.insert(std::pair <int, std::function<int(char*)>>((int)NATIVE_HASH::HASH::CONFIG_PARSE_HASH, ConfigParse));
		NativeMapInit<decltype(nativeMap1)>(nativeMap1);
		LOG_CONSOLE("%s init hash CONFIG_PARSE_HASH DONE\n", FUNC_NAME);

		LOG_CONSOLE("%s init hash CONFIG_CREATE_HASH\n", FUNC_NAME);
		nativeMap1.insert(std::pair <int, std::function<int(char*)>>((int)NATIVE_HASH::HASH::CONFIG_CREATE_HASH, ConfigCreate));
		NativeMapInit<decltype(nativeMap1)>(nativeMap1);
		LOG_CONSOLE("%s init hash CONFIG_CREATE_HASH DONE\n", FUNC_NAME);
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
	void NativeCaller(int nativeHash, T param1) {
		NativeCaller(nativeHash, param1, (T)nullptr, (T)nullptr, (T)nullptr);
	}

	template <typename T1, typename T2>
	void NativeCaller(int nativeHash, T1 param1, T2 param2) {
		NativeCaller(nativeHash, param1, param2, (T1)nullptr, (T1)nullptr);
	}

	template <typename T1, typename T2, typename T3>
	void NativeCaller(int nativeHash, T1 param1, T2 param2, T3 param3) {
		NativeCaller(nativeHash, param1, param2, param3, (T1)nullptr);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void NativeCaller(int nativeHash, T1 param1, T2 param2, T3 param3, T4 param4) {
		LOG_CONSOLE("%s Hash : 0x%X param1: %p param2: %p param3: %p param4: %p", FUNC_NAME,nativeHash, param1, param2, param3, param4);

		for (auto const& nativeMapList : nativeMap1List)
		{
			for (auto const& nativeMap : nativeMapList) {
				if (nativeMap.first == (int)NATIVE_HASH::HASH::CONFIG_PARSE_HASH
					|| nativeMap.first == (int)NATIVE_HASH::HASH::CONFIG_CREATE_HASH
					) {
					LOG_CONSOLE("%s calling wit param : %s\n", FUNC_NAME, param1);
					nativeMap.second(param1);
					break;
				}
			}
		}
	}
};