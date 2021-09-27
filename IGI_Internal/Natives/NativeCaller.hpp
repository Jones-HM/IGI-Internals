#pragma once 
#define FORCEINLINE __forceinline 
#include "Common.hpp" 
#include "Logger.hpp" 
#include "NativeConst.hpp" 
#include "Natives/Natives.hpp"
using namespace Utility;

namespace IGI {
	typedef uint32_t NativeHash;
	typedef void* Void;

	class NativeCaller
	{
	private:
		int native_argc = 0;
		std::list<std::map<int, std::function<int(void)>>> nativeMap0List0;
		std::list<std::map<int, std::function<int(const char*)>>> native_map1_list1;
		std::list<std::map<int, std::function<int(int)>>> native_map1_list2;
		std::list<std::map<int, std::function<int(float)>>> native_map1_list3;
		std::list<std::map<int, std::function<int(int*)>>> native_map1_list4;
		std::list<std::map<int, std::function<int(const char*, int)>>> native_map2_list1;
		std::list<std::map<int, std::function<int(int, const char*)>>> native_map2_list2;
		std::list<std::map<int, std::function<int(float, float)>>> native_map2_list3;

		template<typename NF, typename NM>
		void NativeMapInsert(NativeHash native_hash, NF native_func, NM& native_map) {
			native_map[native_hash] = native_func;
		}

		template<typename NF, typename NM, typename NML>
		void NativeMapAdd(NativeHash native_hash, NF native_func, NM& native_map, NML& native_map_list) {
			NativeMapInsert(native_hash, native_func, native_map);
			native_map_list .push_back(native_map);
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
		~NativeCaller();
		NativeCaller(const NativeCaller&) = default;
		NativeCaller(NativeCaller&&) = default;

		template<class... Args>
		void NativeInvoker(Args... args)
		{
			native_argc = sizeof...(Args) - 1;//Not counting NativeHash as Arg. 
			NativeCall(args...);
		}

	private:
		template <typename T>
		void NativeCall(T native_hash) {
			NativeInvoke(native_hash);
		}

		template <typename T>
		void NativeCall(NativeHash native_hash, T param) {
			NativeInvoke(native_hash, param);
		}

		template <typename T1, typename T2>
		void NativeCall(NativeHash native_hash, T1 param1, T2 param2) {
			NativeInvoke(native_hash, param1, param2);
		}

		template <typename T1, typename T2, typename T3>
		void NativeCall(NativeHash native_hash, T1 param1, T2 param2, T3 param3) {
			NativeInvoke(native_hash, param1, param2, param3);
		}

		template <typename T1, typename T2, typename T3, typename T4>
		void NativeCall(NativeHash native_hash, T1 param1, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			NativeInvoke(native_hash, param1, param2, param3, param4);
		}

		template <typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
		void NativeInvoke(NativeHash native_hash, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			LOG_INFO("%s() Hash : 0x%X param1: %p param2: %p param3: %p param4: %p native_argc : %d", FUNC_NAME, native_hash, param1, param2, param3, param4, native_argc);
			static_assert(sizeof(native_hash) <= sizeof(uint32_t));

#define IS_SAME_T1(T) std::is_same_v<T1,T> 
#define IS_SAME_T2(T1X,T2X) IS_SAME_T1(T1X) && std::is_same_v<T2,T2X> 

			if (native_argc == 0) {
				LOG_DEBUG("%s() NativeArg count 0", FUNC_NAME);
				bool hash_found = NativeInvokeT(native_hash, nativeMap0List0);
			}

			else if (native_argc == 1)
			{
#ifdef _DEBUG 
				if (IS_SAME_T1(LPCSTR))
					LOG_DEBUG("%s() NativeArg count 1 param1: %s type : %s", FUNC_NAME, (LPCSTR)param1, TYPEID(param1));
				else if (IS_SAME_T1(INT))
					LOG_DEBUG("%s() NativeArg count 1 param1 : %d type : %s", FUNC_NAME, (INT)param1, TYPEID(param1));
#endif 
				bool hash_found = NativeInvokeT(native_hash, native_map1_list1, param1);
				if (!hash_found)
					hash_found = NativeInvokeT(native_hash, native_map1_list2, param1);
				if (!hash_found)
					hash_found = NativeInvokeT(native_hash, native_map1_list3, param1);
				if (!hash_found)
					hash_found = NativeInvokeT(native_hash, native_map1_list4, param1);
			}
			else if (native_argc == 2)
			{
#ifdef _DEBUG 
				if (IS_SAME_T2(INT, LPCSTR))
					LOG_INFO("%s() native_argc2 param1: %p type : %s param2: %s type : %s", FUNC_NAME, (int)param1, TYPEID(param1), (LPCSTR)param2, TYPEID(param2));
				else if (IS_SAME_T2(LPCSTR, INT))
					LOG_INFO("%s() native_argc2 param1: %p type : %s param2: %s type : %s", FUNC_NAME, (LPCSTR)param1, TYPEID(param1), (int)param2, TYPEID(param2));
				else if (IS_SAME_T2(INT, INT))
					LOG_INFO("%s() native_argc2 param1: %d type : %s param2: %d type : %s", FUNC_NAME, (INT)param1, TYPEID(param1), (INT)param2, TYPEID(param2));
#endif 
				bool hash_found = NativeInvokeT(native_hash, native_map2_list1, param1, param2);
				if (!hash_found)
					hash_found = NativeInvokeT(native_hash, native_map2_list2, param1, param2);
				if (!hash_found)
					hash_found = NativeInvokeT(native_hash, native_map2_list3, param1, param2);
			}
		}


		//Gneral Invoke hashes method with N Args. 
		template <typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void, typename ML>
		bool NativeInvokeT(NativeHash native_hash, ML native_map_list_arg = nullptr, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			LOG_INFO("%s() Hash : 0x%X param1: %p type : %s param2: %p type : %s native_argc: %d", FUNC_NAME, native_hash, param1, TYPEID(param1), param2, TYPEID(param2), native_argc);

#define IS_SAME_MAP(MAP) std::is_same_v<decltype(native_map_list_arg), decltype(MAP)> 
#define IS_SAME_T1(T) std::is_same_v<T1,T> 
#define IS_SAME_T2(T1X,T2X) IS_SAME_T1(T1X) && std::is_same_v<T2,T2X> 
#define IS_SAME_T3(T1X,T2X,T3X) IS_SAME_T2(T1X,T2X) && std::is_same_v<T3,T3X> 
#define IS_SAME_T4(T1X,T2X,T3X,T4X) IS_SAME_T3(T1X,T2X,T3X) && std::is_same_v<T4,T4X> 

			for (auto const& native_map_list : native_map_list_arg) {
				for (auto const& native_map : native_map_list) {
					//LOG_INFO("%s TYPE native_map type : '%s'", FUNC_NAME, TYPEID(native_map)); 

					if (native_map.first == native_hash) {

						if constexpr (IS_SAME_MAP(nativeMap0List0)) {
							LOG_DEBUG("Found handler for Hash 0x%X\tSymbol : %s\n", native_hash,g_Natives->FindNativeName(native_hash).c_str());
							std::invoke(native_map.second);
							return true;
						}

						if constexpr ((IS_SAME_MAP(native_map1_list1) && IS_SAME_T1(LPCSTR))
							|| (IS_SAME_MAP(native_map1_list2) && IS_SAME_T1(INT))
							|| (IS_SAME_MAP(native_map1_list3) && IS_SAME_T1(INT))
							) {
							LOG_DEBUG("Found handler for Hash 0x%X\tSymbol : %s\n", native_hash, g_Natives->FindNativeName(native_hash).c_str());
							if constexpr (IS_SAME_MAP(native_map1_list3) && IS_SAME_T1(INT))
								std::invoke(native_map.second, (float)((float)param1 / MUSIC_CONST));
							else
								std::invoke(native_map.second, param1);
							return true;
						}

						if constexpr ((IS_SAME_MAP(native_map2_list1) && IS_SAME_T2(LPCSTR, INT))
							|| (IS_SAME_MAP(native_map2_list2) && IS_SAME_T2(INT, LPCSTR))
							|| (IS_SAME_MAP(native_map2_list3) && IS_SAME_T2(INT, INT))
							) {
							LOG_DEBUG("Found handler for Hash 0x%X\tSymbol : %s\n", native_hash, g_Natives->FindNativeName(native_hash).c_str());
							if constexpr (IS_SAME_MAP(native_map2_list3) && IS_SAME_T2(INT, INT))
								std::invoke(native_map.second, (float)((float)param1 / MUSIC_CONST), (float)((float)param2 / MUSIC_CONST));
							else
								std::invoke(native_map.second, param1, param2);
							return true;
						}
					}
				}
			}
			LOG_ERROR("Error finding handler for Hash 0x%X", native_hash);
			return false;
		}
	};
	inline NativeCaller* g_NativeCaller{};
}