#pragma once 
#include "Common.hpp" 
#include "Logger.hpp" 
#include "CommonConst.hpp" 
#include "Natives.hpp"

using namespace IGI;
using namespace IGI;

namespace IGI {

	typedef uint32_t NativeHash;
	typedef void* Void;

	class NativeCaller {
		int native_argc = 0;
		bool m_hash_found = false;

		template <typename RT = Void>
		RT NativeInvoke(void* func_ptr) {
			return NativeInvokeT<RT>(func_ptr);
		}

		template <typename RT = Void, typename T>
		RT NativeInvoke(void* func_ptr, T param) {
			return NativeInvokeT<RT>(func_ptr, param);
		}

		template <typename RT = Void, typename T1, typename T2>
		RT NativeInvoke(void* func_ptr, T1 param1, T2 param2) {
			return NativeInvokeT<RT>(func_ptr, param1, param2);
		}

		template <typename RT = Void, typename T1, typename T2, typename T3 = Void>
		RT NativeInvoke(void* func_ptr, T1 param1, T2 param2, T3 param3) {
			return NativeInvokeT<RT>(func_ptr, param1, param2, param3);
		}

		template<typename RT = Void, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
		RT NativeInvoke(void* func_ptr, T1 param1, T2 param2, T3 param3, T4 param4) {
			return NativeInvokeT<RT>(func_ptr, param1, param2, param3, param4);
		}

		template<typename RT = Void, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
		RT NativeInvokeT(Void native_hash, T1 param1 = nullptr, T2 param2 = nullptr, T3 param3 = nullptr, T4 param4 = nullptr) {
			std::stringstream ss;
			ss << "Hash : " << native_hash << " Param1: " << HEX_ADDR_FMT(param1) << " type: " << TYPE(param1) << " Param2: " << HEX_ADDR_FMT(param2) << " type: " << TYPE(param2) <<
				" Param3: " << HEX_ADDR_FMT(param3) << " type: " << TYPE(param3) << " Param4: " << HEX_ADDR_FMT(param4) << " type: " << TYPE(param4) << " Argc: " << native_argc << std::endl;
			LOG_FILE("%s(): %s", FUNC_NAME, ss.str().data());

#define IS_SAME_T1(T) std::is_same_v<T1,T> 
			m_hash_found = false;
			if constexpr (!IS_SAME_T1(void*)) {
				if (native_argc >= 1) {
					LOG_FILE("Found handler for Hash %p\tSymbol : %s\n", native_hash, ::g_Natives->FindNativeName(reinterpret_cast<NativeHash>(native_hash)).c_str());
					m_hash_found = true;

					std::function<RT(T1, T2, T3, T4)> hash_invoker{ (RT(_cdecl*)(T1,T2,T3,T4))native_hash };
					auto ret_val = std::invoke(hash_invoker, param1, param2, param3, param4);
					if constexpr (!std::is_same_v<RT, Void>) return ret_val;
				}
			}
			else {
				LOG_FILE("Found handler for Hash %p\tSymbol : %s\n", native_hash, ::g_Natives->FindNativeName(reinterpret_cast<NativeHash>(native_hash)).c_str());
				m_hash_found = true;

				std::function<RT(void)> hash_invoker{ (RT(_cdecl*)(void))native_hash };
				auto ret_val = std::invoke(hash_invoker);
				if constexpr (!std::is_same_v<RT, Void>) return ret_val;
			}
			if (!m_hash_found)
				LOG_FILE("Error finding handler for Hash 0x%X", native_hash);
			return reinterpret_cast<RT>(nullptr);
		}

	public:
		NativeCaller() = default;
		~NativeCaller() = default;

		template<typename RT = Void, class... Args>
		RT Invoke(Args... args)
		{
			native_argc = sizeof...(Args) - 1;//Not counting native_hash as Arg. 
			auto ret_val = NativeInvoke<RT>(args...);

			if constexpr (!std::is_same_v<RT, void>)
				return ret_val;
		}

	};
	inline NativeCaller g_NativeCaller;
}