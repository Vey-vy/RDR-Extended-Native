#pragma once
#include "Function.hpp"
#include "pch.hpp"
#include "rdr/scrNativeCallContext.hpp"

class ExtendedNative {
public:
	static void Init();
	static void RegisterNative(rage::scrNativeHash hash, rage::scrNativeHandler handler);
    static void Registers();

	static const char* GetGameBuild();

private:
	static inline FunctionType::RegisterNativeCommand m_RegisterNativeCommand;
	static inline PVOID m_RegistrationTable;
	static inline const char* m_GameBuild;
};