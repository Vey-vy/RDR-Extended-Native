#pragma once
#include "../src/Logger.hpp"

#include <sstream>
#include <iomanip>

template<typename T>
T invoke(uint64_t hash) {
	std::stringstream ss;
	ss << "Invoking native 0x" << std::hex << std::setfill('0') << std::setw(16) << hash;
	Logger::Info(ss.str());
	
	if constexpr (std::is_same_v<T, void>) {
		return;
	} else if constexpr (std::is_same_v<T, bool>) {
		return true;
	} else if constexpr (std::is_same_v<T, int>) {
		return 0;
	} else if constexpr (std::is_same_v<T, uint64_t>) {
		return 0ULL;
	} else if constexpr (std::is_same_v<T, float>) {
		return 0.0f;
	} else if constexpr (std::is_same_v<T, const char*>) {
		return "";
	}
}

// namespace SCANNER {
// 	static void SCANNER_BEGIN() { return invoke<void>(0x74997D4D86261033); }
// }