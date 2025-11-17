#pragma once
#include "Logger.hpp"

#include <iomanip>
#include <sstream>

template<typename T>
T invoke(uint64_t hash) { // Fake Function
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
	inline bool SCANNER_BEGIN(const char* moduleName = "RDR.exe") {
		return invoke<bool>(0x74997D4D86261033);
	}

	inline bool SCANNER_ADD_PATTERN(const char* name, const char* pattern, int32_t offset = 0, int32_t operationType = 0, int32_t operationValue = 0) {
		return invoke<bool>(0xD5E272FA192490E4);
	}

	inline bool SCANNER_RUN(uint32_t flags = 0) {
		return invoke<bool>(0x30480E1B90848DA7);
	}

	inline uint64_t SCANNER_GET_RESULT(const char* name) {
		return invoke<uint64_t>(0xD4542B7AB11D47A6);
	}

	inline bool SCANNER_SET_MODULE(const char* moduleName) {
		return invoke<bool>(0x06A513C3BEB1FC67);
	}

	inline bool SCANNER_CLEAR_RESULTS() {
		return invoke<bool>(0x44C5FCEF3BFF633E);
	}

	inline const char* SCANNER_GET_LAST_ERROR() {
		return invoke<const char*>(0xD44D6107FBEC04C6);
	}

	inline bool SCANNER_END(bool clearResults = true) {
		return invoke<bool>(0x9D76FB1B3D7BD4CB);
	}

	inline int32_t SCANNER_COUNT_RESULTS() {
		return invoke<int32_t>(0x46AAECAA41FA4104);
	}
}

namespace MEMORY {
	namespace Read {
		inline int32_t READ_INT32(uint64_t address) {
			return invoke<int32_t>(0x5B41FE069EDA2F42);
		}

		inline uint32_t READ_UINT32(uint64_t address) {
			return invoke<uint32_t>(0x85C4EFB7475A8243);
		}

		inline uint64_t READ_UINT64(uint64_t address) {
			return invoke<uint64_t>(0x85CEFFB74762F1F8);
		}

		inline float READ_FLOAT(uint64_t address) {
			return invoke<float>(0x978987E041571564);
		}

		inline const char* READ_STRING(uint64_t address, int32_t maxLength = 256) {
			return invoke<const char*>(0x8677D786F81760DF);
		}
	}

	namespace Write {
		inline bool WRITE_INT32(uint64_t address, int32_t value) {
			return invoke<bool>(0xF43B3796FADD87EB);
		}

		inline bool WRITE_UINT32(uint64_t address, uint32_t value) {
			return invoke<bool>(0x6D13F5F4B44F6EA8);
		}

		inline bool WRITE_FLOAT(uint64_t address, float value) {
			return invoke<bool>(0xD46B8F39844BB441);
		}
	}
}

namespace MODULE {
	inline uint64_t GET_MODULE_BASE(const char* moduleName) {
		return invoke<uint64_t>(0x5C72358DD41FCB30);
	}

	inline uint32_t GET_MODULE_SIZE(const char* moduleName) {
		return invoke<uint32_t>(0x338B18FD88A8A414);
	}
}

namespace VECTOR {
	inline float VECTOR_DISTANCE(float x1, float y1, float z1, float x2, float y2, float z2) {
		return invoke<float>(0x7223ED37CC9BB780);
	}

	inline float VECTOR_LENGTH(float x, float y, float z) {
		return invoke<float>(0x8EA0A40DA049F827);
	}
}

namespace UTIL {
	inline const char* FORMAT_STRING(const char* format) {
		return invoke<const char*>(0x51DFA790482861A2);
	}
}

namespace GAME_DATA {
	inline const char* GET_GAME_VERSION() {
		return invoke<const char*>(0x768934218542736DULL);
	}
}
