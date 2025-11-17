#pragma once
#include "memory/PatternBatch.hpp"
#include "pch.hpp"
#include "rdr/scrNativeCallContext.hpp"

#include <map>
#include <string>

enum class PatternOperationType : int32_t {
	None   = 0,
	Rip    = 1,
	Sub    = 2,
	Add    = 3,
	RipAdd = 4,
	RipSub = 5
};

static Memory::PatternBatch g_CurrentScanner;
static Memory::Range* g_CurrentRange = nullptr;
static std::string g_CurrentModule   = "RDR.exe";
static std::map<std::string, uint64_t> g_ScanResults;
static std::string g_LastError;

static inline void NativeCommandBeginScanner(rage::scrNativeCallContext* ctx) {
	const char* moduleName = ctx->GetArgument<const char*>(0);

	g_CurrentScanner = Memory::PatternBatch();
	g_ScanResults.clear();
	g_LastError.clear();

	if (moduleName && moduleName [ 0 ] != '\0') {
		g_CurrentModule = moduleName;
	} else {
		g_CurrentModule = "RDR.exe";
	}

	if (g_CurrentRange) {
		delete g_CurrentRange;
		g_CurrentRange = nullptr;
	}

	ctx->SetResult<bool>(true);
}

static inline void NativeCommandAddPattern(rage::scrNativeCallContext* ctx) {
	const char* name       = ctx->GetArgument<const char*>(0);
	const char* pattern    = ctx->GetArgument<const char*>(1);
	int32_t offset         = ctx->GetArgument<int32_t>(2);
	int32_t operationType  = ctx->GetArgument<int32_t>(3);
	int32_t operationValue = ctx->GetArgument<int32_t>(4);

	if (!name || !pattern) {
		g_LastError = "Invalid name or pattern";
		ctx->SetResult<bool>(false);
		return;
	}

	g_CurrentScanner.add(name, Memory::Pattern(pattern), [ name, offset, operationType, operationValue ](Memory::Handle ptr) {
		if (offset != 0) {
			ptr = ptr.add(offset);
		}

		switch (static_cast<PatternOperationType>(operationType)) {
		case PatternOperationType::Rip: ptr = ptr.rip(); break;
		case PatternOperationType::Sub:
			if (operationValue != 0) {
				ptr = ptr.sub(operationValue);
			}
			break;
		case PatternOperationType::Add:
			if (operationValue != 0) {
				ptr = ptr.add(operationValue);
			}
			break;
		case PatternOperationType::RipAdd:
			ptr = ptr.rip();
			if (operationValue != 0) {
				ptr = ptr.add(operationValue);
			}
			break;
		case PatternOperationType::RipSub:
			ptr = ptr.rip();
			if (operationValue != 0) {
				ptr = ptr.sub(operationValue);
			}
			break;
		case PatternOperationType::None:
		default: break;
		}

		g_ScanResults [ name ] = ptr.as<uint64_t>();
	});

	ctx->SetResult<bool>(true);
}

static inline void NativeCommandRunScanner(rage::scrNativeCallContext* ctx) {
	uint32_t flags = ctx->GetArgument<uint32_t>(0);

	try {
		if (!g_CurrentRange) {
			try {
				g_CurrentRange =
				    new Memory::Range(Memory::Module(g_CurrentModule.c_str()));
			} catch (const std::exception& e) {
				g_LastError = "Failed to load module: " + g_CurrentModule;
				ctx->SetResult<bool>(false);
				return;
			}
		}

		g_CurrentScanner.run(*g_CurrentRange);
		ctx->SetResult<bool>(true);
	} catch (const std::exception& e) {
		g_LastError = e.what();
		ctx->SetResult<bool>(false);
	}
}

static inline void NativeCommandGetResult(rage::scrNativeCallContext* ctx) {
	const char* name = ctx->GetArgument<const char*>(0);

	if (!name) {
		ctx->SetResult<uint64_t>(0);
		return;
	}

	auto it = g_ScanResults.find(name);
	if (it != g_ScanResults.end()) {
		ctx->SetResult<uint64_t>(it->second);
	} else {
		ctx->SetResult<uint64_t>(0);
	}
}

static inline void NativeCommandSetModule(rage::scrNativeCallContext* ctx) {
	const char* moduleName = ctx->GetArgument<const char*>(0);

	if (!moduleName || moduleName [ 0 ] == '\0') {
		g_LastError = "Invalid module name";
		ctx->SetResult<bool>(false);
		return;
	}

	g_CurrentModule = moduleName;

	if (g_CurrentRange) {
		delete g_CurrentRange;
		g_CurrentRange = nullptr;
	}

	ctx->SetResult<bool>(true);
}

static inline void NativeCommandClearResults(rage::scrNativeCallContext* ctx) {
	g_ScanResults.clear();
	g_LastError.clear();
	ctx->SetResult<bool>(true);
}

static inline void NativeCommandGetLastError(rage::scrNativeCallContext* ctx) {
	ctx->SetResult<const char*>(g_LastError.c_str());
}

static inline void NativeCommandEndScanner(rage::scrNativeCallContext* ctx) {
	bool clearResults = ctx->GetArgument<bool>(0);

	if (g_CurrentRange) {
		delete g_CurrentRange;
		g_CurrentRange = nullptr;
	}

	if (clearResults) {
		g_ScanResults.clear();
		g_LastError.clear();
	}

	ctx->SetResult<bool>(true);
}

static inline void NativeCommandCountResults(rage::scrNativeCallContext* ctx) {
	ctx->SetResult<int32_t>(static_cast<int32_t>(g_ScanResults.size()));
}

static inline void NativeCommandReadInt32(rage::scrNativeCallContext* ctx) {
	uint64_t address = ctx->GetArgument<uint64_t>(0);

	if (address == 0) {
		ctx->SetResult<int32_t>(0);
		return;
	}

	try {
		int32_t value = *reinterpret_cast<int32_t*>(address);
		ctx->SetResult<int32_t>(value);
	} catch (...) {
		ctx->SetResult<int32_t>(0);
	}
}

static inline void NativeCommandReadUInt32(rage::scrNativeCallContext* ctx) {
	uint64_t address = ctx->GetArgument<uint64_t>(0);

	if (address == 0) {
		ctx->SetResult<uint32_t>(0);
		return;
	}

	try {
		uint32_t value = *reinterpret_cast<uint32_t*>(address);
		ctx->SetResult<uint32_t>(value);
	} catch (...) {
		ctx->SetResult<uint32_t>(0);
	}
}

static inline void NativeCommandReadUInt64(rage::scrNativeCallContext* ctx) {
	uint64_t address = ctx->GetArgument<uint64_t>(0);

	if (address == 0) {
		ctx->SetResult<uint64_t>(0);
		return;
	}

	try {
		uint64_t value = *reinterpret_cast<uint64_t*>(address);
		ctx->SetResult<uint64_t>(value);
	} catch (...) {
		ctx->SetResult<uint64_t>(0);
	}
}

static inline void NativeCommandReadFloat(rage::scrNativeCallContext* ctx) {
	uint64_t address = ctx->GetArgument<uint64_t>(0);

	if (address == 0) {
		ctx->SetResult<float>(0.0f);
		return;
	}

	try {
		float value = *reinterpret_cast<float*>(address);
		ctx->SetResult<float>(value);
	} catch (...) {
		ctx->SetResult<float>(0.0f);
	}
}

static inline void NativeCommandReadString(rage::scrNativeCallContext* ctx) {
	uint64_t address  = ctx->GetArgument<uint64_t>(0);
	int32_t maxLength = ctx->GetArgument<int32_t>(1);

	if (address == 0 || maxLength <= 0) {
		ctx->SetResult<const char*>("");
		return;
	}

	try {
		const char* str = reinterpret_cast<const char*>(address);
		static std::string buffer;
		buffer = std::string(str, maxLength);
		ctx->SetResult<const char*>(buffer.c_str());
	} catch (...) {
		ctx->SetResult<const char*>("");
	}
}

static inline void NativeCommandWriteInt32(rage::scrNativeCallContext* ctx) {
	uint64_t address = ctx->GetArgument<uint64_t>(0);
	int32_t value    = ctx->GetArgument<int32_t>(1);

	if (address == 0) {
		ctx->SetResult<bool>(false);
		return;
	}

	try {
		*reinterpret_cast<int32_t*>(address) = value;
		ctx->SetResult<bool>(true);
	} catch (...) {
		ctx->SetResult<bool>(false);
	}
}

static inline void NativeCommandWriteUInt32(rage::scrNativeCallContext* ctx) {
	uint64_t address = ctx->GetArgument<uint64_t>(0);
	uint32_t value   = ctx->GetArgument<uint32_t>(1);

	if (address == 0) {
		ctx->SetResult<bool>(false);
		return;
	}

	try {
		*reinterpret_cast<uint32_t*>(address) = value;
		ctx->SetResult<bool>(true);
	} catch (...) {
		ctx->SetResult<bool>(false);
	}
}

static inline void NativeCommandWriteFloat(rage::scrNativeCallContext* ctx) {
	uint64_t address = ctx->GetArgument<uint64_t>(0);
	float value      = ctx->GetArgument<float>(1);

	if (address == 0) {
		ctx->SetResult<bool>(false);
		return;
	}

	try {
		*reinterpret_cast<float*>(address) = value;
		ctx->SetResult<bool>(true);
	} catch (...) {
		ctx->SetResult<bool>(false);
	}
}

static inline void NativeCommandGetModuleBase(rage::scrNativeCallContext* ctx) {
	const char* moduleName = ctx->GetArgument<const char*>(0);

	if (!moduleName) {
		ctx->SetResult<uint64_t>(0);
		return;
	}

	try {
		Memory::Module module(moduleName);
		ctx->SetResult<uint64_t>(static_cast<uint64_t>(module.begin().as<std::uintptr_t>()));
	} catch (...) {
		ctx->SetResult<uint64_t>(0);
	}
}

static inline void NativeCommandGetModuleSize(rage::scrNativeCallContext* ctx) {
	const char* moduleName = ctx->GetArgument<const char*>(0);

	if (!moduleName) {
		ctx->SetResult<uint32_t>(0);
		return;
	}

	try {
		Memory::Module module(moduleName);
		ctx->SetResult<uint32_t>(static_cast<uint32_t>(module.size()));
	} catch (...) {
		ctx->SetResult<uint32_t>(0);
	}
}

static inline void NativeCommandVectorDistance(rage::scrNativeCallContext* ctx) {
	float x1 = ctx->GetArgument<float>(0);
	float y1 = ctx->GetArgument<float>(1);
	float z1 = ctx->GetArgument<float>(2);
	float x2 = ctx->GetArgument<float>(3);
	float y2 = ctx->GetArgument<float>(4);
	float z2 = ctx->GetArgument<float>(5);

	float dx = x2 - x1;
	float dy = y2 - y1;
	float dz = z2 - z1;

	float distance = sqrtf(dx * dx + dy * dy + dz * dz);
	ctx->SetResult<float>(distance);
}

static inline void NativeCommandVectorLength(rage::scrNativeCallContext* ctx) {
	float x = ctx->GetArgument<float>(0);
	float y = ctx->GetArgument<float>(1);
	float z = ctx->GetArgument<float>(2);

	float length = sqrtf(x * x + y * y + z * z);
	ctx->SetResult<float>(length);
}

static inline void NativeCommandLog(rage::scrNativeCallContext* ctx) {
	// const char* message = ctx->GetArgument<const char*>(0);

	// if (!message) {
	// 	ctx->SetResult<bool>(false);
	// 	return;
	// }

	// try {
	// 	printf("[ExtendedNative] %s\n", message);
	// 	fflush(stdout);
	// 	ctx->SetResult<bool>(true);
	// } catch (...) {
	// 	ctx->SetResult<bool>(false);
	// }
}

static inline void NativeCommandFormatString(rage::scrNativeCallContext* ctx) {
	const char* format = ctx->GetArgument<const char*>(0);

	if (!format) {
		ctx->SetResult<const char*>("");
		return;
	}

	static std::string buffer;
	buffer = format;
	ctx->SetResult<const char*>(buffer.c_str());
}


static inline void NativeCommandIsKeyPressed(rage::scrNativeCallContext* ctx) {
	int key = ctx->GetArgument<int>(0);
	ctx->SetResult<bool>(key & GetAsyncKeyState(key));
}

static inline void NativeCommandGetGameVersion(rage::scrNativeCallContext* ctx) {
	auto version = ExtendedNative::GetGameBuild();
	ctx->SetResult<const char*>(version);
}