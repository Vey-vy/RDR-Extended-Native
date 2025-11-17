#pragma once
#include "rdr/scrNativeCallContext.hpp"

namespace FunctionType {
	using RegisterNativeCommand = __int64 (*)(PVOID table, rage::scrNativeHash hash, rage::scrNativeHandler handler);
	using SetWeather = __int64(*)(int type);
}