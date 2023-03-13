#include "Heart/heart.hpp"

bool __stdcall DllMain(HMODULE Instance, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE Thread = CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Heart::Init), Instance, NULL, NULL);
		if (Thread)
		{
			CloseHandle(Thread);
		}
		break;
	}
	case DLL_PROCESS_DETACH:
		Heart::Destroy();
		break;
	}

	return true;
}