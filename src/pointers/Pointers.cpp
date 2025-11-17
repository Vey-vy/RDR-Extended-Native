#include "Pointers.hpp"

#include "logger/Logger.hpp"
#include "memory/PatternBatch.hpp"

#include <sstream>
#include <vector>

void Pointers::Init() {
	Memory::PatternBatch Pattern;
	Pattern.add("SetWeather", "F3 0F 59 05 ? ? ? ? F3 0F 58 D0 E9 ? ? ? ?", [ & ](Memory::Handle ptr) {
		m_SetWeather = ptr.sub(82).as<FunctionType::SetWeather>();
	});
    Pattern.add("VFXRegisters", "48 8B D9 44 3B C8 72 0D 33 C0 48 8B 5C 24 ?", [ & ](Memory::Handle ptr) {
		m_VFXRegisters = ptr.sub(27).as<PVOID>();
	});
    Pattern.add("GetPlayerActor", "E8 ? ? ? ? 48 85 C0 74 DF", [ & ](Memory::Handle ptr) {
		m_GetPlayerActor = ptr.add(1).rip().as<sagActor*>();
	});
    Pattern.add("ErrorMessage", "48 8D 15 ? ? ? ? 4C 89 64 24 ? 48 8D 0D ? ? ? ?", [ & ](Memory::Handle ptr) {
		m_ErrorMessage = ptr.add(3).rip().as<const char*>();
	});

	Pattern.run(Memory::Module("RDR.exe"));
}

void Pointers::SetWeather(int type) {
	m_SetWeather(type);
}

const char* Pointers::GetErrorMessage() {
	return m_ErrorMessage;
}