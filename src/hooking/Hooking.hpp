#pragma once
#include "pch.hpp"

class Hooking {
public:
	static void Init();
	static void Enable();
	static void Shutdown();

    static unsigned __int64 __fastcall vfx_register(__int64 _this, const char* a2, __int64 a3);

private:
    static inline void* m_OirginalTest;
};