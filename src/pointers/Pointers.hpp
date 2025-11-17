#pragma once
#include "extended/Function.hpp"
#include "pch.hpp"
#include "rdr/scrNativeCallContext.hpp"
#include "rdr/sagActor.hpp"

class Pointers {
public:
	static void Init();
	static void SetWeather(int type);

	static inline PVOID m_VFXRegisters;

	static const char* GetErrorMessage();

private:
	static inline FunctionType::SetWeather m_SetWeather;
	static inline sagActor* m_GetPlayerActor;
	static inline const char* m_ErrorMessage;
};