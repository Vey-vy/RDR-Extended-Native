#include "extended/Extended.hpp"
#include "extended/Native.hpp"
#include "hooking/Hooking.hpp"
#include "logger/Logger.hpp"
#include "pch.hpp"
#include "pointers/Pointers.hpp"

#include <cstdio>
#include <iostream>
#include <windows.h>
#include <thread>

void CreateConsole() {
	AllocConsole();
	FILE* f = nullptr;
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);
	freopen_s(&f, "CONIN$", "r", stdin);
	std::cout.clear();
	std::cerr.clear();
	std::cin.clear();
	Logger::Info("Console created");
}

DWORD WINAPI InitThread(LPVOID lpParam) {
	CreateConsole();

	ExtendedNative::Init();
	Pointers::Init();
	Hooking::Init();
	Hooking::Enable();

	ExtendedNative::Registers();

	Logger::Success(std::format("Initialized ExtendedNative with rdr {}", ExtendedNative::GetGameBuild()));

	// while(true) {
	// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	// }

	return 0;
}

BOOL APIENTRY DllMain(HMODULE _Module, DWORD _ReasonForCall, LPVOID _Reserved) {
	switch (_ReasonForCall) {
	case DLL_PROCESS_ATTACH:
	{
		CreateThread(NULL, 0, InitThread, NULL, 0, NULL);
	} break;
	case DLL_PROCESS_DETACH:
	{
	} break;
	}
	return TRUE;
}