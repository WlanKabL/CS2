#include "includes.h"
#include "Render.hpp"
#include "MenuConfig.hpp"
#include <fstream>
#include <sstream>
#include "CheatsRelated.h"
#include <algorithm>  
#include "xor.hpp"
#include <iostream>
#include <string>
#include <regex>
#include "Return_Spoofer.h"
#include "detour.h"
#include "CheatMenu.h"
#if CUSTOM_ICONS
#include "iconscpp.h"
#include "icons.h"
#endif

#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif


bool  g_WantUpdateHasGamepad = true;


IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::GetCurrentContext() == NULL)
		return 0;

	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
	{
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			::SetCapture(hwnd);
		io.MouseDown[button] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		int button = 0;
		if (msg == WM_LBUTTONUP) { button = 0; }
		if (msg == WM_RBUTTONUP) { button = 1; }
		if (msg == WM_MBUTTONUP) { button = 2; }
		if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
		io.MouseDown[button] = false;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hwnd)
			::ReleaseCapture();
		return 0;
	}
	case WM_MOUSEWHEEL:
		io.MouseWheel += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		return 0;
	case WM_MOUSEHWHEEL:
		io.MouseWheelH += (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return 0;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		io.AddInputCharacter((unsigned int)wParam);
		return 0;
	case WM_DEVICECHANGE:
		if ((UINT)wParam == DBT_DEVNODES_CHANGED)
			g_WantUpdateHasGamepad = true;
		return 0;
	}
	return 0;
}

Present oPresent;
HWND window = 0;
WNDPROC oWndProc;
ID3D11Device* pDevice = 0;
ID3D11DeviceContext* pContext = 0;
ID3D11RenderTargetView* mainRenderTargetView;

bool Init;
void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}



HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	
	

	if (!Loading)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			Loading = true;
		}

		else
		{
			return oPresent(pSwapChain, SyncInterval, Flags);
		}
	}


	

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (GetAsyncKeyState(VK_INSERT) & 1)
		MenuConfig::ShowMenu = !MenuConfig::ShowMenu;


	Renderer();
	EnableCheats();
	DrawFov();


	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}


VOID FindGadget()
{

	DWORD64 GameOverlayRenderer64 = reinterpret_cast<DWORD64>(GetModuleHandle(L"gameoverlayrenderer64.dll"));


	uintptr_t StartAddress = GameOverlayRenderer64;
	uintptr_t EndAddress = GameOverlayRenderer64 + 0x167C000;

	std::string ReturnGadget = "FF 27";

	uintptr_t ReturnAddress = PatternScan(StartAddress, EndAddress, ReturnGadget, false, false, 0);

	printf("Return Address : %llx\n", ReturnAddress);

	Offset::ReturnAddress = ReturnAddress;
}


VOID hkRender()
{

	//FindGadget();

	DWORD64 GameOverlayRenderer64 = reinterpret_cast<DWORD64>(GetModuleHandle(L"gameoverlayrenderer64.dll"));
	uintptr_t GameOverlayStart = GameOverlayRenderer64;
	uintptr_t GameOverlayEnd = GameOverlayStart + 0x2FB000;
	std::string PresentPattern = "48 8B ?? ?? ?? ?? ?? 48 89 ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 89 ?? ?? ?? 89 74";
	uintptr_t AOBPresent = PatternScan(GameOverlayStart, GameOverlayEnd, PresentPattern, true, false, 3);
	uintptr_t pSwapChain = AOBPresent + 0x20;
	DWORD64 FirstDeference;
	DWORD64 Deference2;
	DWORD64 OriginalPresent;
	ReadMemoryInternal<DWORD64>(pSwapChain, FirstDeference, sizeof(DWORD64));
	ReadMemoryInternal<DWORD64>(FirstDeference, Deference2, sizeof(DWORD64));
	ReadMemoryInternal<DWORD64>(Deference2 + 0x40, OriginalPresent, sizeof(DWORD64));
	oPresent = reinterpret_cast<Present>(OriginalPresent);
	printf("%llx\n", oPresent);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oPresent, hkPresent);
	DetourTransactionCommit();

}
