#include "heart.hpp"
#include "hooks.hpp"
#include <iostream>
#include "globals.hpp"
#include "offsets.hpp"

void __stdcall Heart::Init(HMODULE Instance)
{
	FILE* ConsoleBuffer = nullptr;
	if(var::debugConsole) ConsoleBuffer = Heart::AllocConsole();

	Globals::Initialize();

	Hooks::Initialize();

	while (!GetAsyncKeyState(VK_END) & 1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (var::debugConsole)
		if (ConsoleBuffer)
			fclose(ConsoleBuffer);
	FreeLibraryAndExitThread(Instance, 0);
}

FILE* Heart::AllocConsole()
{
	::AllocConsole() && ::AttachConsole(GetCurrentProcessId());
	FILE* ConsoleBuffer = nullptr;
	freopen_s(&ConsoleBuffer, "CONOUT$", "w", stdout);
	SetConsoleTitle("egirl.dev Console DEBUG");
	return ConsoleBuffer;
}

void __stdcall Heart::Destroy()
{
	Hooks::Destroy();
	if(var::debugConsole) FreeConsole();
}