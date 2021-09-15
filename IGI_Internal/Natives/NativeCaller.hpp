#pragma once
#define FORCEINLINE __forceinline
#include "Common.h"
#include "Logger.hpp"
#include "NativeConst.hpp"
using namespace Utility;

namespace IGI {
	typedef uint32_t NativeHash;
	typedef void* Void;

	class NativeCaller
	{
	private:
		int nativeArgc = 0;
		std::list<std::map<int, std::function<int(void)>>> nativeMap0List0;
		std::list<std::map<int, std::function<int(const char*)>>> nativeMap1List1;
		std::list<std::map<int, std::function<int(int)>>> nativeMap1List2;
		std::list<std::map<int, std::function<int(float)>>> nativeMap1List3;
		std::list<std::map<int, std::function<int(const char*, int)>>> nativeMap2List1;
		std::list<std::map<int, std::function<int(int, const char*)>>> nativeMap2List2;
		std::list<std::map<int, std::function<int(float, float)>>> nativeMap2List3;

		template<typename NF, typename NM>
		void NativeMapInsert(NativeHash nativeHash, NF nativeFunc, NM& nativeMap) {
			nativeMap[nativeHash] = nativeFunc;
		}

		template<typename NF, typename NM, typename NML>
		void NativeMapAdd(NativeHash nativeHash, NF nativeFunc, NM& nativeMap, NML& nativeMapList) {
			NativeMapInsert(nativeHash, nativeFunc, nativeMap);
			nativeMapList.push_back(nativeMap);
		}


		//Init Hashes with only 0 Arguments.
		void InitHashMapArgs0();
		//Init Hashes with only 1 Arguments.
		void InitHashMapArgs1();
		void InitHashMapArgs2();
		void InitHashMapArgs3();
		void InitHashMapArgs4();
		void InitHashMaps();

	public:

		NativeCaller();
		~NativeCaller() = default;

		template<class... Args>
		void NativeInvoker(Args... args)
		{
			nativeArgc = sizeof...(Args) - 1;//Not counting NativeHash as Arg.
			NativeCall(args...);
		}

	private:
		template <typename T>
		void NativeCall(T nativeHash) {
			LOG_INFO("nativeHash type: %s", TYPEID(nativeHash));
			NativeInvoke(nativeHash);
		}

		template <typename T>
		void NativeCall(NativeHash nativeHash, T param) {
			NativeInvoke(nativeHash, param);
		}

		template <typename T1, typename T2>
		void NativeCall(NativeHash nativeHash, T1 param1, T2 param2) {
			NativeInvoke(nativeHash, param1, param2);
		}

		template <typename T1, typename T2, typename T3>
		void NativeCall(NativeHash nativeHash, T1 param1, T2 param2, T3 param3) {
			NativeInvoke(nativeHash, param1, param2, param3);
		}

		template <typename T1, typename T2, typename T3, typename T4>
		void NativeCall(NativeHash nativeHash, T1 param1, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			NativeInvoke(nativeHash, param1, param2, param3, param4);
		}

		template <typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
		void NativeInvoke(NativeHash nativeHash, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			LOG_INFO("%s() Hash : 0x%X param1: %p param2: %p param3: %p param4: %p nativeArgc : %d", FUNC_NAME, nativeHash, param1, param2, param3, param4, nativeArgc);
			static_assert(sizeof(nativeHash) <= sizeof(uint32_t));

#define IS_SAME_T1(T) std::is_same_v<T1,T>
#define IS_SAME_T2(T1X,T2X) IS_SAME_T1(T1X) && std::is_same_v<T2,T2X>

			if (nativeArgc == 0) {
				LOG_DEBUG("%s() NativeArg count 0", FUNC_NAME);
				bool hashFound = NativeInvokeT(nativeHash, nativeMap0List0);
			}

			else if (nativeArgc == 1)
			{
#ifdef _DEBUG
				if (IS_SAME_T1(LPCSTR))
					LOG_DEBUG("%s() NativeArg count 1 param1: %s type : %s", FUNC_NAME, (LPCSTR)param1, TYPEID(param1));
				else if (IS_SAME_T1(INT))
					LOG_DEBUG("%s() NativeArg count 1 param1 : %d type : %s", FUNC_NAME, (INT)param1, TYPEID(param1));
#endif
				bool hashFound = NativeInvokeT(nativeHash, nativeMap1List1, param1);
				if (!hashFound)
					hashFound = NativeInvokeT(nativeHash, nativeMap1List2, param1);
				if (!hashFound)
					hashFound = NativeInvokeT(nativeHash, nativeMap1List3, param1);
			}
			else if (nativeArgc == 2)
			{
#ifdef _DEBUG
				if (IS_SAME_T2(INT, LPCSTR))
					LOG_INFO("%s() nativeArgc2 param1: %p type : %s param2: %s type : %s", FUNC_NAME, (int)param1, TYPEID(param1), (LPCSTR)param2, TYPEID(param2));
				else if (IS_SAME_T2(LPCSTR, INT))
					LOG_INFO("%s() nativeArgc2 param1: %p type : %s param2: %s type : %s", FUNC_NAME, (LPCSTR)param1, TYPEID(param1), (int)param2, TYPEID(param2));
				else if (IS_SAME_T2(INT, INT))
					LOG_INFO("%s() nativeArgc2 param1: %d type : %s param2: %d type : %s", FUNC_NAME, (INT)param1, TYPEID(param1), (INT)param2, TYPEID(param2));
#endif
				bool hashFound = NativeInvokeT(nativeHash, nativeMap2List1, param1, param2);
				if (!hashFound)
					hashFound = NativeInvokeT(nativeHash, nativeMap2List2, param1, param2);
				if (!hashFound)
					hashFound = NativeInvokeT(nativeHash, nativeMap2List3, param1, param2);
			}
		}


		//Gneral Invoke hashes method with N Args.
		template <typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void, typename ML>
		bool NativeInvokeT(NativeHash nativeHash, ML nativeMapListArg = nullptr, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			LOG_INFO("%s() Hash : 0x%X param1: %p type : %s param2: %p type : %s nativeArgc: %d", FUNC_NAME, nativeHash, param1, TYPEID(param1), param2, TYPEID(param2), nativeArgc);

#define IS_SAME_MAP(MAP) std::is_same_v<decltype(nativeMapListArg), decltype(MAP)>
#define IS_SAME_T1(T) std::is_same_v<T1,T>
#define IS_SAME_T2(T1X,T2X) IS_SAME_T1(T1X) && std::is_same_v<T2,T2X>
#define IS_SAME_T3(T1X,T2X,T3X) IS_SAME_T2(T1X,T2X) && std::is_same_v<T3,T3X>
#define IS_SAME_T4(T1X,T2X,T3X,T4X) IS_SAME_T3(T1X,T2X,T3X) && std::is_same_v<T4,T4X>

			for (auto const& nativeMapList : nativeMapListArg) {
				for (auto const& nativeMap : nativeMapList) {
					//LOG_INFO("%s TYPE nativeMap type : '%s'", FUNC_NAME, TYPEID(nativeMap));

					if (nativeMap.first == nativeHash) {

						if constexpr (IS_SAME_MAP(nativeMap0List0)) {
							std::invoke(nativeMap.second);
							LOG_DEBUG("Found handler for Hash 0x%X", nativeHash);
							return true;
						}

						if constexpr ((IS_SAME_MAP(nativeMap1List1) && IS_SAME_T1(LPCSTR))
							|| (IS_SAME_MAP(nativeMap1List2) && IS_SAME_T1(INT))
							|| (IS_SAME_MAP(nativeMap1List3) && IS_SAME_T1(INT))
							) {

							if constexpr (IS_SAME_MAP(nativeMap1List3) && IS_SAME_T1(INT))
								std::invoke(nativeMap.second, (float)((float)param1 / MUSIC_CONST));
							else
								std::invoke(nativeMap.second, param1);
							LOG_DEBUG("Found handler for Hash 0x%X", nativeHash);
							return true;
						}

						if constexpr ((IS_SAME_MAP(nativeMap2List1) && IS_SAME_T2(LPCSTR, INT))
							|| (IS_SAME_MAP(nativeMap2List2) && IS_SAME_T2(INT, LPCSTR))
							|| (IS_SAME_MAP(nativeMap2List3) && IS_SAME_T2(INT, INT))
							) {
							if constexpr (IS_SAME_MAP(nativeMap2List3) && IS_SAME_T2(INT, INT))
								std::invoke(nativeMap.second, (float)((float)param1 / MUSIC_CONST), (float)((float)param2 / MUSIC_CONST));
							else
								std::invoke(nativeMap.second, param1, param2);
							LOG_DEBUG("Found handler for Hash 0x%X", nativeHash);
							return true;
						}
					}
				}
			}
			LOG_ERROR("Error finding handler for Hash 0x%X", nativeHash);
			return false;
		}
	};
	inline NativeCaller g_NativeCaller;
}
