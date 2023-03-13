#pragma once
#include <Windows.h>
#include <cstdio>
#include <chrono>
#include <thread>

namespace Heart
{
	void __stdcall Init(HMODULE Instance);
	void __stdcall Destroy();
	FILE* AllocConsole();
}