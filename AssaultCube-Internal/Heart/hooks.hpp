#pragma once
#include <Windows.h>
#include <MinHook.h>
#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include "../Menu/menu.hpp"
#include "cheat.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
	typedef BOOL(__stdcall* wglSwapBuffersFn)(HDC hDc);
	typedef void(__cdecl* moveMouse)(int idx, int idy);
	inline wglSwapBuffersFn ogSwapBuffers = NULL;
	inline moveMouse ogMoveMouse = NULL;

	inline HWND Window = NULL;
	inline WNDPROC ogWndProc = NULL;

	inline void __cdecl hookMoveMouse(int idx, int idy) {
		if (Menu::showMenu) return;
		ogMoveMouse(idx, idy);
	}

	LRESULT CALLBACK hookWndProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (Menu::showMenu) {
			if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
				return true;
			}
			ImGuiIO& io = ImGui::GetIO();

			switch (uMsg) {
			case WM_LBUTTONDOWN:
				io.MouseDown[1] = !io.MouseDown[0];
				return 0;
			case WM_RBUTTONDOWN:
				io.MouseDown[1] = !io.MouseDown[1];
				return 0;
			case WM_MBUTTONDOWN:
				io.MouseDown[2] = !io.MouseDown[2];
				return 0;
			case WM_MOUSEWHEEL:
				return 0;
			case WM_MOUSEMOVE:
				io.MousePos.x = (signed short)(lParam);
				io.MousePos.y = (signed short)(lParam >> 16);
				return 0;
			}
		}
		return CallWindowProc(ogWndProc, hWnd, uMsg, wParam, lParam);
	}

	inline BOOL __stdcall hookSwapBuffers(HDC hDc)
	{
		static bool Initialized = false;
		if (!Initialized)
		{
			Window = WindowFromDC(hDc);
			ImGui::CreateContext();
			ImGui_ImplWin32_Init(Window);
			ImGui_ImplOpenGL2_Init();
			Menu::MenuIO();
			ogWndProc = (WNDPROC)SetWindowLongPtr(Window, GWL_WNDPROC, (LONG)(LONG_PTR)hookWndProcess);
			Initialized = true;
		}

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (GetAsyncKeyState(0x46) & 1)
			Menu::showMenu = !Menu::showMenu;
			
		if (Menu::showMenu)
			Menu::Render();

		egirl::ESP();
		egirl::Aimbot();
		egirl::Misc();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		return ogSwapBuffers(hDc);
	}

	inline void Initialize()
	{
		MH_Initialize();
		static uintptr_t base = reinterpret_cast<std::uintptr_t>(GetModuleHandle("ac_client.exe"));
		HMODULE OpenGL = GetModuleHandle("opengl32.dll");
		if (OpenGL == NULL)
			return;

		MH_CreateHook(GetProcAddress(OpenGL, "wglSwapBuffers"), reinterpret_cast<LPVOID>(hookSwapBuffers), reinterpret_cast<LPVOID*>(&ogSwapBuffers));
		MH_CreateHook((LPVOID)(base + 0x1908B0), reinterpret_cast<LPVOID>(hookMoveMouse), reinterpret_cast<LPVOID*>(&ogMoveMouse));
		MH_EnableHook(MH_ALL_HOOKS);
	}

	inline void Destroy()
	{
		SetWindowLongPtr(Window, GWL_WNDPROC, (LONG_PTR)ogWndProc);
		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
	}
}