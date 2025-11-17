#include "Extended.hpp"

#include "Native.hpp"
#include "logger/Logger.hpp"
#include "memory/PatternBatch.hpp"

#include <sstream>
#include <vector>

void ExtendedNative::Init() {
	Memory::PatternBatch Pattern;
	Pattern.add("scrEngine::RegisterNativeCommand", "48 89 5C 24 ? 57 48 83 EC 20 44 8B 0D ? ? ? ?", [ & ](Memory::Handle ptr) {
		m_RegisterNativeCommand = ptr.as<FunctionType::RegisterNativeCommand>();
	});
	Pattern.add("RegistrationTable", "48 8B 05 ? ? ? ? 0F B7 CA 48 03 C9 C1 EA 10 66 39 54 C8 ? 75 39", [ & ](Memory::Handle ptr) {
		m_RegistrationTable = ptr.add(3).rip().as<PVOID>();
	});
	Pattern.add("GameBuild", "41 BF ? ? ? ? 48 8D 4E 40 45 8B C7 48 8D 15 ? ? ? ? E8 ? ? ? ?", [ & ](Memory::Handle ptr) {
		m_GameBuild = ptr.add(16).rip().as<const char*>();
	});

	Pattern.run(Memory::Module("RDR.exe"));
}

void ExtendedNative::RegisterNative(rage::scrNativeHash hash, rage::scrNativeHandler handler) {
	m_RegisterNativeCommand(m_RegistrationTable, hash, handler);
}

void ExtendedNative::Registers() {
	struct NativeEntry {
		rage::scrNativeHash hash;
		rage::scrNativeHandler handler;
	};

	std::vector<NativeEntry> natives = {
	    {0x74997D4D86261033ULL, NativeCommandBeginScanner},
	    {0xD5E272FA192490E4ULL, NativeCommandAddPattern},
	    {0x30480E1B90848DA7ULL, NativeCommandRunScanner},
	    {0xD4542B7AB11D47A6ULL, NativeCommandGetResult},
	    {0x06A513C3BEB1FC67ULL, NativeCommandSetModule},
	    {0x44C5FCEF3BFF633EULL, NativeCommandClearResults},
	    {0xD44D6107FBEC04C6ULL, NativeCommandGetLastError},
	    {0x9D76FB1B3D7BD4CBULL, NativeCommandEndScanner},
	    {0x46AAECAA41FA4104ULL, NativeCommandCountResults},
	    {0x5B41FE069EDA2F42ULL, NativeCommandReadInt32},
	    {0x85C4EFB7475A8243ULL, NativeCommandReadUInt32},
	    {0x85CEFFB74762F1F8ULL, NativeCommandReadUInt64},
	    {0x978987E041571564ULL, NativeCommandReadFloat},
	    {0x8677D786F81760DFULL, NativeCommandReadString},
	    {0xF43B3796FADD87EBULL, NativeCommandWriteInt32},
	    {0x6D13F5F4B44F6EA8ULL, NativeCommandWriteUInt32},
	    {0xD46B8F39844BB441ULL, NativeCommandWriteFloat},
	    {0x5C72358DD41FCB30ULL, NativeCommandGetModuleBase},
	    {0x338B18FD88A8A414ULL, NativeCommandGetModuleSize},
	    {0x7223ED37CC9BB780ULL, NativeCommandVectorDistance},
	    {0x8EA0A40DA049F827ULL, NativeCommandVectorLength},
	    {0x51DFA790482861A2ULL, NativeCommandFormatString},
		{0x768934218542736DULL, NativeCommandGetGameVersion},
	};

	for (const auto& native : natives) {
		RegisterNative(native.hash, native.handler);
	}

	std::stringstream ss;
	ss << "Successfully registered " << natives.size() << " native functions";
	Logger::Success(ss.str());
}

const char* ExtendedNative::GetGameBuild() {
	return m_GameBuild;
}