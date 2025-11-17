#include "Hooking.hpp"

#include "logger/Logger.hpp"
#include "pointers/Pointers.hpp"

#include <MinHook.h>

void Hooking::Init() {
	MH_Initialize();

	//MH_CreateHook(Pointers::m_VFXRegisters, &vfx_register, &m_OirginalTest);
}

void Hooking::Enable() {
	MH_EnableHook(MH_ALL_HOOKS);
}

void Hooking::Shutdown() {
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
}

unsigned __int64 __fastcall Hooking::vfx_register(__int64 _this, const char* a2, __int64 a3) {
	Logger::Info("VFX Register");
	Logger::Info(a2);
	return reinterpret_cast<unsigned __int64 (*)(__int64, const char*, __int64)>(m_OirginalTest)(_this, a2, a3);
}