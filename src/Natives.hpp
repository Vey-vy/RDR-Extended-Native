#pragma once
#include "Logger.hpp"

#include <iomanip>
#include <sstream>

template<typename T>
T invoke(uint64_t hash) {
	std::stringstream ss;
	ss << "Invoking native 0x" << std::hex << std::setfill('0') << std::setw(16) << hash;
	Logger::Info(ss.str());

	if constexpr (std::is_same_v<T, void>) {
		return;
	} else if constexpr (std::is_same_v<T, bool>) {
		return true;
	} else if constexpr (std::is_same_v<T, int32_t>) {
		return 0;
	} else if constexpr (std::is_same_v<T, uint32_t>) {
		return 0U;
	} else if constexpr (std::is_same_v<T, uint64_t>) {
		return 0ULL;
	} else if constexpr (std::is_same_v<T, float>) {
		return 0.0f;
	} else if constexpr (std::is_same_v<T, const char*>) {
		return "";
	}
}

namespace SCANNER {
	inline bool Begin(const char* moduleName = "RDR.exe") {
		return invoke<bool>(0x74997D4D86261033);
	}

	inline bool AddPattern(const char* name, const char* pattern, int32_t offset = 0, int32_t operationType = 0, int32_t operationValue = 0) {
		return invoke<bool>(0xD5E272FA192490E4);
	}

	inline bool Run(uint32_t flags = 0) {
		return invoke<bool>(0x30480E1B90848DA7);
	}

	inline uint64_t GetResult(const char* name) {
		return invoke<uint64_t>(0xD4542B7AB11D47A6);
	}

	inline bool SetModule(const char* moduleName) {
		return invoke<bool>(0x06A513C3BEB1FC67);
	}

	inline bool ClearResults() {
		return invoke<bool>(0x44C5FCEF3BFF633E);
	}

	inline const char* GetLastError() {
		return invoke<const char*>(0xD44D6107FBEC04C6);
	}

	inline bool End(bool clearResults = true) {
		return invoke<bool>(0x9D76FB1B3D7BD4CB);
	}

	inline int32_t CountResults() {
		return invoke<int32_t>(0x46AAECAA41FA4104);
	}
}

namespace MEMORY {
	namespace Read {
		inline int32_t Int32(uint64_t address) {
			return invoke<int32_t>(0x5B41FE069EDA2F42);
		}

		inline uint32_t UInt32(uint64_t address) {
			return invoke<uint32_t>(0x85C4EFB7475A8243);
		}

		inline uint64_t UInt64(uint64_t address) {
			return invoke<uint64_t>(0x85CEFFB74762F1F8);
		}

		inline float Float(uint64_t address) {
			return invoke<float>(0x978987E041571564);
		}

		inline const char* String(uint64_t address, int32_t maxLength = 256) {
			return invoke<const char*>(0x8677D786F81760DF);
		}
	}

	namespace Write {
		inline bool Int32(uint64_t address, int32_t value) {
			return invoke<bool>(0xF43B3796FADD87EB);
		}

		inline bool UInt32(uint64_t address, uint32_t value) {
			return invoke<bool>(0x6D13F5F4B44F6EA8);
		}

		inline bool Float(uint64_t address, float value) {
			return invoke<bool>(0xD46B8F39844BB441);
		}
	}
}

namespace MODULE {
	inline uint64_t GetBase(const char* moduleName) {
		return invoke<uint64_t>(0x5C72358DD41FCB30);
	}

	inline uint32_t GetSize(const char* moduleName) {
		return invoke<uint32_t>(0x338B18FD88A8A414);
	}
}

namespace VECTOR {
	inline float Distance(float x1, float y1, float z1, float x2, float y2, float z2) {
		return invoke<float>(0x7223ED37CC9BB780);
	}

	inline float Length(float x, float y, float z) {
		return invoke<float>(0x8EA0A40DA049F827);
	}
}

namespace UTIL {
	inline const char* FormatString(const char* format) {
		return invoke<const char*>(0x51DFA790482861A2);
	}
}
