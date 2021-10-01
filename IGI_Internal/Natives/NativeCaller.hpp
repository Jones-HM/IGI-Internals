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
		bool m_hash_found = false;
		std::list<std::map<int, std::function<int(void)>>> nativeMap0List0;
		std::list<std::map<int, std::function<int(const char*)>>> native_map1_list1;
		std::list<std::map<int, std::function<int(int)>>> native_map1_list2;
		std::list<std::map<int, std::function<int(float)>>> native_map1_list3;
		std::list<std::map<int, std::function<int(int*)>>> native_map1_list4;
		std::list<std::map<int, std::function<int(const char*, int)>>> native_map2_list1;
		std::list<std::map<int, std::function<int(int, const char*)>>> native_map2_list2;
		std::list<std::map<int, std::function<int(float, float)>>> native_map2_list3;
		std::list<std::map<int, std::function<int(int, LPCSTR, LPCSTR, LPCSTR)>>> native_map4_list1;

		template<typename NF, typename NM>
		void NativeMapInsert(NativeHash native_hash, NF native_func, NM& native_map) {
			native_map[native_hash] = native_func;
		}

		template<typename NF, typename NM, typename NML>
		void NativeMapAdd(NativeHash native_hash, NF native_func, NM& native_map, NML& native_map_list) {
			NativeMapInsert(native_hash, native_func, native_map);
			native_map_list.push_back(native_map);
		}


		//Init Hashes with N Arguments. 
		void InitHashMapArgs0();
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

		template<typename RT = Void, class... Args>
		RT NativeInvoker(Args... args)
		{
			native_argc = sizeof...(Args) - 1;//Not counting NativeHash as Arg. 
			auto ret_val = NativeCall<RT>(args...);

			if constexpr (!std::is_same_v<RT, void>)
				return ret_val;
		}

	private:
		template <typename RT = Void, typename T>
		RT NativeCall(T native_hash) {
			return NativeInvoke<RT>(native_hash);
		}

		template <typename RT = Void, typename T>
		RT NativeCall(NativeHash native_hash, T param) {
			return NativeInvoke<RT>(native_hash, param);
		}

		template <typename RT = Void, typename T1, typename T2>
		RT NativeCall(NativeHash native_hash, T1 param1, T2 param2) {
			return NativeInvoke<RT>(native_hash, param1, param2);
		}

		template <typename RT = Void, typename T1, typename T2, typename T3>
		RT NativeCall(NativeHash native_hash, T1 param1, T2 param2, T3 param3) {
			return NativeInvoke<RT>(native_hash, param1, param2, param3);
		}

		template <typename RT = Void, typename T1, typename T2, typename T3, typename T4>
		RT NativeCall(NativeHash native_hash, T1 param1, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			return NativeInvoke<RT>(native_hash, param1, param2, param3, param4);
		}

		template <typename RT = Void, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
		RT NativeInvoke(NativeHash native_hash, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			LOG_FILE("%s() Hash : 0x%X param1: %p param2: %p param3: %p param4: %p native_argc : %d", FUNC_NAME, native_hash, param1, param2, param3, param4, native_argc);
			static_assert(sizeof(native_hash) <= sizeof(uint32_t));

#define IS_SAME_T1(T) std::is_same_v<T1,T> 
#define IS_SAME_T2(T1X,T2X) IS_SAME_T1(T1X) && std::is_same_v<T2,T2X> 
#define IS_SAME_T3(T1X,T2X,T3X) IS_SAME_T2(T1X,T2X) && std::is_same_v<T3,T3X> 
#define IS_SAME_T4(T1X,T2X,T3X,T4X) IS_SAME_T3(T1X,T2X,T3X) && std::is_same_v<T4,T4X> 

			if (native_argc == 0) {
				LOG_FILE("%s() NativeArg count 0", FUNC_NAME);
				auto ret_val = NativeInvokeT<RT>(native_hash, nativeMap0List0);
				if (m_hash_found)
					return ret_val;
			}

			else if (native_argc == 1)
			{
#ifdef _DEBUG 
				if (IS_SAME_T1(LPCSTR))
					LOG_FILE("%s() NativeArg count 1 param1: %s type : %s", FUNC_NAME, (LPCSTR)param1, TYPEID(param1));
				else if (IS_SAME_T1(INT))
					LOG_FILE("%s() NativeArg count 1 param1 : %d type : %s", FUNC_NAME, (INT)param1, TYPEID(param1));
#endif 
				auto ret_val = NativeInvokeT<RT>(native_hash, native_map1_list1, param1);
				if (!m_hash_found)
					ret_val = NativeInvokeT<RT>(native_hash, native_map1_list2, param1);
				if (!m_hash_found)
					ret_val = NativeInvokeT<RT>(native_hash, native_map1_list3, param1);
				if (!m_hash_found)
					ret_val = NativeInvokeT<RT>(native_hash, native_map1_list4, param1);
				return ret_val;
			}
			else if (native_argc == 2)
			{
#ifdef _DEBUG 
				if (IS_SAME_T2(INT, LPCSTR))
					LOG_FILE("%s() native_argc2 param1: %p type : %s param2: %s type : %s", FUNC_NAME, (int)param1, TYPEID(param1), (LPCSTR)param2, TYPEID(param2));
				else if (IS_SAME_T2(LPCSTR, INT))
					LOG_FILE("%s() native_argc2 param1: %p type : %s param2: %s type : %s", FUNC_NAME, (LPCSTR)param1, TYPEID(param1), (int)param2, TYPEID(param2));
				else if (IS_SAME_T2(INT, INT))
					LOG_FILE("%s() native_argc2 param1: %d type : %s param2: %d type : %s", FUNC_NAME, (INT)param1, TYPEID(param1), (INT)param2, TYPEID(param2));
#endif 
				auto ret_val = NativeInvokeT<RT>(native_hash, native_map2_list1, param1, param2);
				if (!m_hash_found)
					ret_val = NativeInvokeT<RT>(native_hash, native_map2_list2, param1, param2);
				if (!m_hash_found)
					ret_val = NativeInvokeT<RT>(native_hash, native_map2_list3, param1, param2);
				return ret_val;
			}


			else if (native_argc == 3)
			{
#ifdef _DEBUG 
#endif
			}

			else if (native_argc == 4)
			{
#ifdef _DEBUG 
				if (IS_SAME_T4(INT, LPCSTR, LPCSTR, LPCSTR))
					LOG_FILE("%s() native_argc2 param1: %p type : %s param2: %s type : %s param3: %s type : %s param4: %s type : %s", FUNC_NAME, (int)param1, TYPEID(param1), (LPCSTR)param2, TYPEID(param2), (LPCSTR)param3, TYPEID(param3), (LPCSTR)param4, TYPEID(param4));
#endif
				auto ret_val = NativeInvokeT<RT>(native_hash, native_map4_list1, param1, param2, param3, param4);
				if (m_hash_found)
					return ret_val;
			}
		}


		//Gneral Invoke hashes method with N Args. 
		template <typename RT = Void, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void, typename ML>
		RT NativeInvokeT(NativeHash native_hash, ML native_map_list_arg = nullptr, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			LOG_FILE("%s() Hash : 0x%X param1: %p type : %s param2: %p type : %s param3: %p type : %s param4: %p type : %s native_argc: %d", FUNC_NAME, native_hash, param1, TYPEID(param1), param2, TYPEID(param2), param3, TYPEID(param3), param4, TYPEID(param4), native_argc);
			m_hash_found = false;
			bool b_map_once = true;

#define IS_SAME_MAP(MAP) std::is_same_v<decltype(native_map_list_arg), decltype(MAP)> 
#define IS_SAME_T1(T) std::is_same_v<T1,T> 
#define IS_SAME_T2(T1X,T2X) IS_SAME_T1(T1X) && std::is_same_v<T2,T2X> 
#define IS_SAME_T3(T1X,T2X,T3X) IS_SAME_T2(T1X,T2X) && std::is_same_v<T3,T3X> 
#define IS_SAME_T4(T1X,T2X,T3X,T4X) IS_SAME_T3(T1X,T2X,T3X) && std::is_same_v<T4,T4X> 

			for (const auto& native_map_list : native_map_list_arg) {
				for (const auto& native_map : native_map_list) {
					if (b_map_once && !m_hash_found) {
						LOG_FILE("%s Map type: '%s'", FUNC_NAME, TYPEID(native_map));
						b_map_once = !b_map_once;
					}

					if (native_map.first == native_hash) {

						//NativeInvoker0
						if constexpr (IS_SAME_MAP(nativeMap0List0)) {
							m_hash_found = true;
							LOG_FILE("Found handler for Hash 0x%X\tSymbol : %s\n", native_hash, g_Natives->FindNativeName(native_hash).c_str());
							auto ret_val = std::invoke(native_map.second);
							if constexpr (!std::is_same_v<RT, Void>) return ret_val; else return reinterpret_cast<RT>(nullptr);
						}

						//NativeInvoker1
						if constexpr ((IS_SAME_MAP(native_map1_list1) && IS_SAME_T1(LPCSTR))
							|| (IS_SAME_MAP(native_map1_list2) && IS_SAME_T1(INT))
							|| (IS_SAME_MAP(native_map1_list3) && IS_SAME_T1(INT))
							) {
							m_hash_found = true;
							LOG_FILE("Found handler for Hash 0x%X\tSymbol : %s\n", native_hash, g_Natives->FindNativeName(native_hash).c_str());
							if constexpr (IS_SAME_MAP(native_map1_list3) && IS_SAME_T1(INT)) {
								auto ret_val = std::invoke(native_map.second, (float)((float)param1 / MUSIC_CONST));
								if constexpr (!std::is_same_v<RT, Void>) return ret_val; else return reinterpret_cast<RT>(nullptr);
							}
							else {
								auto ret_val = std::invoke(native_map.second, param1);
								if constexpr (!std::is_same_v<RT, Void>) return ret_val; else return reinterpret_cast<RT>(nullptr);
							}
						}

						//NativeInvoker2
						if constexpr ((IS_SAME_MAP(native_map2_list1) && IS_SAME_T2(LPCSTR, INT))
							|| (IS_SAME_MAP(native_map2_list2) && IS_SAME_T2(INT, LPCSTR))
							|| (IS_SAME_MAP(native_map2_list3) && IS_SAME_T2(INT, INT))
							) {
							m_hash_found = true;
							LOG_FILE("Found handler for Hash 0x%X\tSymbol : %s\n", native_hash, g_Natives->FindNativeName(native_hash).c_str());
							if constexpr (IS_SAME_MAP(native_map2_list3) && IS_SAME_T2(INT, INT)) {
								auto ret_val = std::invoke(native_map.second, (float)((float)param1 / MUSIC_CONST), (float)((float)param2 / MUSIC_CONST));
								if constexpr (!std::is_same_v<RT, Void>) return ret_val; else return reinterpret_cast<RT>(nullptr);
							}
							else {
								auto ret_val = std::invoke(native_map.second, param1, param2);
								if constexpr (!std::is_same_v<RT, Void>) return ret_val; else return reinterpret_cast<RT>(nullptr);
							}
						}

						//NativeInvoker4
						if constexpr ((IS_SAME_MAP(native_map4_list1) && IS_SAME_T4(int, LPCSTR, LPCSTR, LPCSTR))
							) {
							m_hash_found = true;
							LOG_FILE("Found handler for Hash 0x%X\tSymbol : %s\n", native_hash, g_Natives->FindNativeName(native_hash).c_str());
							auto ret_val = std::invoke(native_map.second, param1, param2, param3, param4);
							if constexpr (!std::is_same_v<RT, Void>) return ret_val; else return reinterpret_cast<RT>(nullptr);
						}
					}
				}
			}
			if (!m_hash_found)
				LOG_FILE("Error finding handler for Hash 0x%X", native_hash);
			return reinterpret_cast<RT>(nullptr);
		}
	};
	inline NativeCaller* g_NativeCaller{};
}