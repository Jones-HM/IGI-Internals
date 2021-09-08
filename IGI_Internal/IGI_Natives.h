#pragma once
#include <functional>
#include <map>


class IGINative {

public:

private:
	std::map<int, std::function<int(char*)>> nativeMapArg1Char;
	template <typename T>
	void InitHashes() {

		//auto ConfigParse = (int(__cdecl*)(char*))IGI_CONFIG_PARSE_HASH;
		std::function<int(char*)> ConfigParseStd{ (int(__cdecl*)(char*))CONFIG_PARSE_HASH };

		nativeMapArg1Char.insert(std::pair <int, std::function<int(char*)>>(CONFIG_PARSE_HASH, ConfigParseStd));
	}

public:

	IGINative() {
		InitHashes<void>();
	}

	template <typename T>
	void NativeCaller(int nativeHash, T param1) {
		NativeCaller(nativeHash, param1, (T)nullptr, (T)nullptr, (T)nullptr);
	}

	template <typename T1, typename T2>
	void NativeCaller(int nativeHash, T1 param1,T2 param2) {
		NativeCaller(nativeHash, param1,param2, (T1)nullptr, (T1)nullptr);
	}

	template <typename T1, typename T2, typename T3>
	void NativeCaller(int nativeHash, T1 param1, T2 param2, T3 param3) {
		NativeCaller(nativeHash, param1, param2, param3, (T1)nullptr);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void NativeCaller(int nativeHash, T1 param1, T2 param2, T3 param3, T4 param4) {

		for (auto const& x : nativeMapArg1Char)
		{
			if (x.first == CONFIG_PARSE_HASH) {
				x.second(param1);
				break;
			}
		}
	}

};

namespace NATIVE_HASHES {
	enum HASHES
	{
		CONFIG_PARSE_HASH = 0x405850,
	};
}
