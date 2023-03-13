#pragma once
#include <iostream>
#include <Windows.h>
#include "offsets.hpp"

namespace Globals
{
	inline uintptr_t BaseAddress = NULL;
	inline uintptr_t LocalPlayer = NULL;
	inline uint32_t ScreenWidth = NULL;
	inline uint32_t ScreenHeight = NULL;

	void Initialize()
	{
		BaseAddress = reinterpret_cast<std::uintptr_t>(GetModuleHandle("ac_client.exe"));
		LocalPlayer = *reinterpret_cast<std::uintptr_t*>(BaseAddress + 0x17E0A8);
		ScreenWidth = *reinterpret_cast<std::uint32_t*>(BaseAddress + Offsets::ScreenWidth);
		ScreenHeight = *reinterpret_cast<std::uint32_t*>(BaseAddress + Offsets::ScreenHeight);
	}
}