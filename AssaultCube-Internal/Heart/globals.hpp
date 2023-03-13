#pragma once
#include <iostream>
#include <Windows.h>

namespace Globals
{
	inline uintptr_t BaseAddress = NULL;
	inline uintptr_t LocalPlayer = NULL;
	void Initialize()
	{
		BaseAddress = reinterpret_cast<std::uintptr_t>(GetModuleHandle("ac_client.exe"));
		LocalPlayer = *reinterpret_cast<std::uintptr_t*>(BaseAddress + 0x17E0A8);
	}

}