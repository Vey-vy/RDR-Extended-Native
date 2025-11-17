#pragma once
#include "scrValue.hpp"
#include "scrVector.hpp"

namespace rage {
	class scrNativeCallContext {
	public:
		scrNativeCallContext(scrValue* retVal, std::int32_t argCount, scrValue* args) :
		    m_ReturnValue(retVal), m_ArgCount(argCount), m_Args(args), m_NumVectorRefs(0) {
		}

		template <typename T>
		T GetArgument(std::uint32_t index) {
			if (index >= m_ArgCount) {
				return T();
			}
			return m_Args[index].Any;
		}

		template <>
		const char* GetArgument<const char*>(std::uint32_t index) {
			if (index >= m_ArgCount) {
				return "";
			}
			return m_Args[index].String;
		}

		template <>
		scrVector* GetArgument<scrVector*>(std::uint32_t index) {
			if (index >= m_ArgCount) {
				return nullptr;
			}
			return reinterpret_cast<scrVector*>(m_Args[index].Reference);
		}

		template <typename T>
		void SetResult(T value) {
			if (m_ReturnValue) {
				m_ReturnValue->Any = static_cast<std::uint64_t>(value);
			}
		}

		template <>
		void SetResult<const char*>(const char* value) {
			if (m_ReturnValue) {
				m_ReturnValue->String = value;
			}
		}

		template <>
		void SetResult<float>(float value) {
			if (m_ReturnValue) {
				m_ReturnValue->Float = value;
			}
		}

		template <>
		void SetResult<bool>(bool value) {
			if (m_ReturnValue) {
				m_ReturnValue->Int = value ? 1 : 0;
			}
		}

		scrValue* m_ReturnValue;
		std::uint32_t m_ArgCount;
		scrValue* m_Args;
		std::int32_t m_NumVectorRefs;
		scrVector* m_VectorRefTargets [ 4 ];
		Vector3 m_VectorRefSources [ 4 ];
	};
	static_assert(sizeof(scrNativeCallContext) == 0x80);

	using scrNativeHash    = std::uint64_t;
	using scrNativeHandler = void (*)(scrNativeCallContext*);
}