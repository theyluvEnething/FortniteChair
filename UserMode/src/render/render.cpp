#include <D3D9.h>
#include "render.h"
#include <Windows.h>
#include <iostream>
#include <DirectXMath.h>
#include "../cheat/cheat.h"
#include "../cheat/sdk/sdk.h"
#include "../cheat/settings/settings.h"
#include "../driver/driver.h"
#include "../util/skStr.h"
#include <tlhelp32.h> 
#include <tchar.h>


IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

HWND Render::GameHwnd = NULL;
RECT Render::GameRect = { NULL };
HWND Render::MyHwnd = NULL;
MSG Render::Message = { NULL };
BOOL Render::bMenu = TRUE;
BOOL Render::HoverOverMenu = TRUE;
BOOL Render::InForeground = TRUE;

//uintptr_t procid = NULL;
bool ColorPicker(const char* label, ImColor& col);
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);



HHOOK mouseHook;
HRESULT Render::DirectXInit() {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = Render::MyHwnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, MyHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(MyHwnd);
	ImGui_ImplDX9_Init(p_Device);
	ImGui::StyleColorsClassic();
	//ImGuiStyle* style = &ImGui::GetStyle();

	//ImGui::StyleColorsClassic();
	//style->WindowPadding = ImVec2(8, 8);
	//style->WindowRounding = 5.0f;
	//style->FramePadding = ImVec2(4, 2);
	//style->FrameRounding = 0.0f;
	//style->ItemSpacing = ImVec2(8, 4);
	//style->ItemInnerSpacing = ImVec2(4, 4);
	//style->IndentSpacing = 21.0f;
	//style->ScrollbarSize = 14.0f;
	//style->ScrollbarRounding = 0.0f;
	//style->GrabMinSize = 10.0f;
	//style->GrabRounding = 0.0f;
	//style->TabRounding = 0.f;
	//style->ChildRounding = 0.0f;
	//style->WindowBorderSize = 1.f;
	//style->ChildBorderSize = 1.f;
	//style->PopupBorderSize = 0.f;
	//style->FrameBorderSize = 0.f;
	//style->TabBorderSize = 0.f;

	//style->Colors[ImGuiCol_Text] = ImVec4(0.000f, 0.678f, 0.929f, 1.0f);
	//style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.0263f, 0.0357f, 1.00f);
	//style->Colors[ImGuiCol_WindowBg] = ImVec4(0.059f, 0.051f, 0.071f, 1.00f);
	//style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.071f, 0.071f, 0.090f, 1.00f);
	//style->Colors[ImGuiCol_PopupBg] = ImVec4(0.0f, 0.0263f, 0.0357f, 1.00f);
	//style->Colors[ImGuiCol_Border] = ImColor(0.000f, 0.678f, 0.929f, 1.0f);
	//style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0263f, 0.0357f, 0.00f);
	//style->Colors[ImGuiCol_FrameBg] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_TitleBg] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.263f, 0.357f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_CheckMark] = ImColor(87, 119, 134, 255);
	//style->Colors[ImGuiCol_SliderGrab] = ImColor(119, 134, 169, 150);
	//style->Colors[ImGuiCol_SliderGrabActive] = ImColor(119, 134, 169, 150);
	//style->Colors[ImGuiCol_Button] = ImColor(26, 23, 31, 255);
	//style->Colors[ImGuiCol_ButtonHovered] = ImColor(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_ButtonActive] = ImColor(0.102f, 0.090f, 0.122f, 1.000f);
	//style->Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_Column] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	//style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	//style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	//style->Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	//style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	//style->WindowTitleAlign.x = 0.50f;
	//style->FrameRounding = 0.0f;

	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowBorderSize = 1;
	style->FrameBorderSize = 1;
	style->WindowTitleAlign = { 0.5f, 0.5f };
	style->WindowRounding = 0.0f;
	style->FrameRounding = 0.0f;
	style->Colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_Border] = Settings::Misc::MenuColor;

	style->Colors[ImGuiCol_TitleBg] = ImColor(8, 8, 8, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(8, 8, 8, 255);

	style->Colors[ImGuiCol_WindowBg] = ImColor(14, 14, 14, 255);
	style->Colors[ImGuiCol_FrameBg] = ImColor(8, 8, 8, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(8, 8, 8, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(8, 8, 8, 255);

	style->Colors[ImGuiCol_Button] = ImColor(12, 12, 12, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(24, 24, 38, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(36, 36, 70, 255);

	style->Colors[ImGuiCol_SliderGrab] = ImColor(68, 68, 68, 255);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(68, 68, 68, 255);

	style->Colors[ImGuiCol_CheckMark] = ImColor(36, 36, 36, 255);
	style->Colors[ImGuiCol_Header] = ImColor(68, 68, 68, 255);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(68, 68, 68, 255);
	style->Colors[ImGuiCol_PopupBg] = ImColor(38, 38, 38, 255);


	p_Object->Release();
	return S_OK;
}

namespace gui
{
	typedef HWND(WINAPI* CreateWindowInBand)(_In_ DWORD dwExStyle, _In_opt_ ATOM atom, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam, DWORD band);
	CreateWindowInBand create_window_in_band = 0;
	enum ZBID
	{
		ZBID_DEFAULT = 0,
		ZBID_DESKTOP = 1,
		ZBID_UIACCESS = 2,
		ZBID_IMMERSIVE_IHM = 3,
		ZBID_IMMERSIVE_NOTIFICATION = 4,
		ZBID_IMMERSIVE_APPCHROME = 5,
		ZBID_IMMERSIVE_MOGO = 6,
		ZBID_IMMERSIVE_EDGY = 7,
		ZBID_IMMERSIVE_INACTIVEMOBODY = 8,
		ZBID_IMMERSIVE_INACTIVEDOCK = 9,
		ZBID_IMMERSIVE_ACTIVEMOBODY = 10,
		ZBID_IMMERSIVE_ACTIVEDOCK = 11,
		ZBID_IMMERSIVE_BACKGROUND = 12,
		ZBID_IMMERSIVE_SEARCH = 13,
		ZBID_GENUINE_WINDOWS = 14,
		ZBID_IMMERSIVE_RESTRICTED = 15,
		ZBID_SYSTEM_TOOLS = 16,
		ZBID_LOCK = 17,
		ZBID_ABOVELOCK_UX = 18,
	};
	DWORD duplicate_winlogin_token(DWORD session_id, DWORD desired_access, PHANDLE token_phandle)
	{
		DWORD dwerr;
		PRIVILEGE_SET ps;
		ps.PrivilegeCount = 1;
		ps.Control = PRIVILEGE_SET_ALL_NECESSARY;
		if (LookupPrivilegeValue(NULL, SE_TCB_NAME, &ps.Privilege[0].Luid))
		{
			HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (INVALID_HANDLE_VALUE != snapshot_handle)
			{
				BOOL bcont, bfound = FALSE;
				PROCESSENTRY32 pe;
				pe.dwSize = sizeof(pe);
				dwerr = ERROR_NOT_FOUND;
				for (bcont = Process32First(snapshot_handle, &pe); bcont; bcont = Process32Next(snapshot_handle, &pe))
				{
					HANDLE process_handle;
					if (0 != _tcsicmp(pe.szExeFile, TEXT("winlogon.exe"))) continue;
					process_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
					if (process_handle)
					{
						HANDLE token_handle;
						DWORD retlen, sid;
						if (OpenProcessToken(process_handle, TOKEN_QUERY | TOKEN_DUPLICATE, &token_handle))
						{
							BOOL ftcb;
							if (PrivilegeCheck(token_handle, &ps, &ftcb) && ftcb)
							{
								if (GetTokenInformation(token_handle, TokenSessionId, &sid, sizeof(sid), &retlen) && sid == session_id)
								{
									bfound = TRUE;
									if (DuplicateTokenEx(token_handle, desired_access, 0, SecurityImpersonation, TokenImpersonation, token_phandle))
									{
										dwerr = ERROR_SUCCESS;
									}
									else
									{
										dwerr = GetLastError();
									}
								}
							}
							CloseHandle(token_phandle);
						}
						CloseHandle(process_handle);
					}
					if (bfound) break;
				}
				CloseHandle(snapshot_handle);
			}
			else
			{
				dwerr = GetLastError();
			}
		}
		else
		{
			dwerr = GetLastError();
		}
		return dwerr;
	}
	DWORD create_ui_access_token(PHANDLE token_phandle)
	{
		DWORD dwerr;
		HANDLE token_self_handle;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &token_self_handle))
		{
			DWORD session_id, retlen;
			if (GetTokenInformation(token_self_handle, TokenSessionId, &session_id, sizeof(session_id), &retlen))
			{
				HANDLE token_system_handle;
				dwerr = duplicate_winlogin_token(session_id, TOKEN_IMPERSONATE, &token_system_handle);
				if (ERROR_SUCCESS == dwerr)
				{
					if (SetThreadToken(NULL, token_system_handle))
					{
						if (DuplicateTokenEx(token_self_handle, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_DEFAULT, 0, SecurityAnonymous, TokenPrimary, token_phandle))
						{
							BOOL ui_access = TRUE;
							if (!SetTokenInformation(*token_phandle, TokenUIAccess, &ui_access, sizeof(ui_access)))
							{
								dwerr = GetLastError();
								CloseHandle(*token_phandle);
							}
						}
						else
						{
							dwerr = GetLastError();
						}
						RevertToSelf();
					}
					else
					{
						dwerr = GetLastError();
					}
					CloseHandle(token_system_handle);
				}
			}
			else
			{
				dwerr = GetLastError();
			}
			CloseHandle(token_self_handle);
		}
		else
		{
			dwerr = GetLastError();
		}
		return dwerr;
	}
	BOOL check_for_ui_acces(DWORD* pdwerr, DWORD* ui_access)
	{
		BOOL result = FALSE;
		HANDLE token_handle;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token_handle))
		{
			DWORD retlen;
			if (GetTokenInformation(token_handle, TokenUIAccess, ui_access, sizeof(*ui_access), &retlen))
			{
				result = TRUE;
			}
			else
			{
				*pdwerr = GetLastError();
			}
			CloseHandle(token_handle);
		}
		else
		{
			*pdwerr = GetLastError();
		}
		return result;
	}
	DWORD prepare_for_ui_access()
	{
		DWORD dwerr;
		HANDLE token_ui_access_handle;
		BOOL ui_access;
		if (check_for_ui_acces(&dwerr, (DWORD*)&ui_access))
		{
			if (ui_access)
			{
				dwerr = ERROR_SUCCESS;
			}
			else
			{
				dwerr = create_ui_access_token(&token_ui_access_handle);
				if (ERROR_SUCCESS == dwerr)
				{
					STARTUPINFO si;
					PROCESS_INFORMATION pi;
					GetStartupInfo(&si);

					std::wstring cmdLineStr = std::wstring(GetCommandLine()) + L" final";
					LPWSTR cmdLine = const_cast<LPWSTR>(cmdLineStr.c_str());


					if (CreateProcessAsUser(token_ui_access_handle, 0, cmdLine, 0, 0, FALSE, 0, 0, 0, &si, &pi))
					{
						CloseHandle(pi.hProcess), CloseHandle(pi.hThread);
						ExitProcess(0);
					}
					else
					{
						dwerr = GetLastError();
					}
					CloseHandle(token_ui_access_handle);
				}
			}
		}
		return dwerr;
	}
}

bool Render::InitGui() {
	//mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
	//if (mouseHook == NULL) {
	//	std::cout << "[!] Failed to hook mouse event!" << std::endl;
	//	return false;
	//}

	HMODULE user32;
	HMODULE win32u;
	HMODULE user32_lib = LoadLibraryA(skCrypt("user32.dll"));
	if (!user32_lib) return false;
	HMODULE win32u_lib = LoadLibraryA(skCrypt("win32u.dll"));
	if (!win32u_lib) return false;
	user32 = GetModuleHandleA(skCrypt("user32.dll"));
	if (!user32) return false;
	win32u = GetModuleHandleA(skCrypt("win32u.dll"));
	if (!win32u) return false;
	const DWORD dwerr = gui::prepare_for_ui_access();
	if (ERROR_SUCCESS != dwerr) return false;
	gui::create_window_in_band = reinterpret_cast<gui::CreateWindowInBand>(GetProcAddress(user32, "CreateWindowInBand"));
	if (!gui::create_window_in_band)
	{
		gui::create_window_in_band = reinterpret_cast<gui::CreateWindowInBand>(GetProcAddress(win32u, "CreateWindowInBand"));
		if (!gui::create_window_in_band) return false;
	}
	return true;
}

void Render::CreateOverlay()
{
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		0,
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		0,
		0,
		"Frontend",
		LoadIcon(0, IDI_APPLICATION)
	};
	ATOM rce = RegisterClassExA(&wcex);
	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	MyHwnd = gui::create_window_in_band(0, rce, L"Frontend", WS_POPUP, rect.left, rect.top, rect.right, rect.bottom, 0, 0, wcex.hInstance, 0, gui::ZBID_UIACCESS);
	SetWindowLong(MyHwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(MyHwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyHwnd, &margin);
	ShowWindow(MyHwnd, SW_SHOW);
	UpdateWindow(MyHwnd);
}

void Render::CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}

	UnhookWindowsHookEx(mouseHook);
}



void Render::render() {
	char fpsinfo[64];

	// IMPLEMENT LATER
	//sprintf_s(fpsinfo, ("FPS: %03d"), Util::get_fps());
	//sprintf_s(fpsinfo, ("FPS: %03d"), 1.f / ImGui::GetIO().DeltaTime);
	//printf("framerate: %03d\n", (int)ImGui::GetIO().Framerate);
	//printf("deltatime: %03d\n", (int)(1.f / ImGui::GetIO().DeltaTime));
	//printf("philipp fps calc: %03d\n", Util::get_fps());
	sprintf_s(fpsinfo, ("FPS: %03d"), (int)ImGui::GetIO().Framerate);

	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), 15, ImVec2(50, 50), ImColor(0, 255, 0), fpsinfo);

}

inline float actual_fov = Settings::Aimbot::Fov;
void Render::FovCircle() {
	if (!Settings::Aimbot::Enabled or !Settings::Aimbot::ShowFov)
		return;

	if (Settings::CloseRange::Enabled && Cheat::IsCloseRange && !cache::InLobby)
	{
		actual_fov = actual_fov + 0.1f * (Settings::CloseRange::CurrentFov - actual_fov);
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Width / 2, Height / 2), actual_fov, Settings::Aimbot::FovColor, 99, 2);
		if (Settings::Aimbot::FillFovCircle) {
			ImColor FovTransparent = ImColor((int)(Settings::Aimbot::FovColor.Value.x * 255),
				(int)(Settings::Aimbot::FovColor.Value.y * 255),
				(int)(Settings::Aimbot::FovColor.Value.z * 255),
				30);
			ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(Width / 2, Height / 2), Settings::CloseRange::CurrentFov, FovTransparent, 99);
		}
	}
	else {
		actual_fov = actual_fov + 0.1f * (Settings::Aimbot::Fov - actual_fov);
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Width / 2, Height / 2), actual_fov, Settings::Aimbot::FovColor, 99, 2);
		if (Settings::Aimbot::FillFovCircle) {
			ImColor FovTransparent = ImColor((int)(Settings::Aimbot::FovColor.Value.x * 255),
				(int)(Settings::Aimbot::FovColor.Value.y * 255),
				(int)(Settings::Aimbot::FovColor.Value.z * 255),
				30);
			ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(Width / 2, Height / 2), Settings::Aimbot::Fov, FovTransparent, 99);
		}
	}
}

void Render::EndOfFrame() {
	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&p_Params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void Render::HandleMessage()
{
	static RECT old_rc;
	if (PeekMessage(&Render::Message, MyHwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	if (GetAsyncKeyState(0x23) & 1)
		exit(8);
	HWND hwnd_active = GetForegroundWindow();

	if (hwnd_active == GameHwnd) {
		HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
		SetWindowPos(MyHwnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		Render::InForeground = TRUE;
	}
	else
	{
		Render::InForeground = FALSE;
		GameHwnd = Util::get_process_wnd(ProcId);
		Sleep(250);
	}
	RECT rc;
	POINT xy;

	ZeroMemory(&rc, sizeof(RECT));
	ZeroMemory(&xy, sizeof(POINT));
	GetClientRect(GameHwnd, &rc);
	ClientToScreen(GameHwnd, &xy);
	rc.left = xy.x;
	rc.top = xy.y;

	ImGuiIO& io = ImGui::GetIO();
	io.ImeWindowHandle = GameHwnd;
	io.DeltaTime = 1.0f / 60.0f;

	POINT p;
	GetCursorPos(&p);
	io.MousePos.x = p.x - xy.x;
	io.MousePos.y = p.y - xy.y;

	if (GetAsyncKeyState(0x1)) {
		io.MouseDown[0] = true;
		io.MouseClicked[0] = true;
		io.MouseClickedPos[0].x = io.MousePos.x;
		io.MouseClickedPos[0].x = io.MousePos.y;
	}
	else
		io.MouseDown[0] = false;
	if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
	{

		old_rc = rc;

		Width = rc.right;
		Height = rc.bottom;

		p_Params.BackBufferWidth = Width;
		p_Params.BackBufferHeight = Height;
		SetWindowPos(MyHwnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
		p_Device->Reset(&p_Params);
	}
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Render::CloseRender() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Render::CleanuoD3D();
	DestroyWindow(MyHwnd);
}


inline const char* VisualMode[] = { "enemy", "team", "bot" };
inline int CurrentVisualMode = 0;
inline int lastSmoothX = 10;
inline bool SwitchedSmoothLock = false;
inline bool SwitchedCloseRangeSmoothLock = false;

void Render::Menu() {
	static int MenuTab = 0;
	float
		TextSpaceLine = 90.f,
		SpaceLineOne = 120.f,
		SpaceLineTwo = 280.f,
		SpaceLineThr = 420.f;
	static const char* HitboxList[]{ "Head","Neck","Chest","Pelvis" };
	static int SelectedHitbox = 0;

	static const char* MouseKeys[]{ "RMouse","LMouse","Control","Shift","Alt" };
	static int KeySelected = 0;
	ImGuiStyle* style = &ImGui::GetStyle();


	int ColorButtonFlags = (ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker |
		ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview);

	if (GetAsyncKeyState(VK_INSERT) & 1) bMenu = !bMenu;
	if (bMenu)
	{
		static POINT Mouse;
		GetCursorPos(&Mouse);

		auto& io = ImGui::GetIO();
		Render::HoverOverMenu = io.WantCaptureMouse;


		//ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(Mouse.x, Mouse.y), float(4), ImColor(255, 0, 0), 50);

		ImGui::SetNextWindowSize({ 620, 350 });
		ImGui::Begin(skCrypt("Fortnite"), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

		ImGui::SetCursorPos({ 260.f,15.f });
		ImGui::Text(skCrypt("renewable fortnite"));
		//style->Colors[ImGuiCol_Border] = ImColor(255, 255, 255, 255);
		style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
		style->Colors[ImGuiCol_SliderGrab] = ImColor(68, 68, 68, 255);
		style->Colors[ImGuiCol_SliderGrabActive] = ImColor(68, 68, 68, 255);
		style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);

		//style->Colors[ImGuiCol_SliderGrab] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 100);
		//style->Colors[ImGuiCol_SliderGrabActive] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 100);

		//style->Colors[ImGuiCol_CheckMark] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 255);

		ImGui::SetCursorPos({ 33.f,90.f });

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f)); // Text links ausrichten
		if (ImGui::Button(skCrypt("aimbot"), { 89.f, 32.f }))
		{
			MenuTab = 0;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f)); // Text links ausrichten
		ImGui::SetCursorPos({ 33.f,90.f + 37.f });
		if (ImGui::Button(skCrypt("visuals"), { 89.f, 32.f }))
		{
			MenuTab = 1;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5f)); // Text links ausrichten
		ImGui::SetCursorPos({ 33.f,90.f + 37.f * 2 });
		if (ImGui::Button(skCrypt("misc"), { 89.f, 32.f }))
		{
			MenuTab = 2;
		}

		ImGui::SetCursorPos({22.f, 310.f });
		if (ImGui::Button(skCrypt("discord"), { 70.f, 32.f }))
		{
			system(skCrypt("start https://discord.gg/renewable"));

		}

		/*ImGui::SetCursorPos({22.f,290.f});
		if (ImGui::Button(skCrypt("Unload"), { 68.f, 25.f }))
		{
			Settings::SaveConfig();
			exit(0);
		}*/
		

		style->Colors[ImGuiCol_Button] = ImColor(12, 12, 12, 0);
		style->Colors[ImGuiCol_ButtonActive] = ImColor(24, 24, 38, 0);
		style->Colors[ImGuiCol_ButtonHovered] = ImColor(36, 36, 70, 0);

		//style->ItemInnerSpacing = ImVec2(8, 2);
		style->WindowPadding = ImVec2(16, 16);
		//style->FramePadding = ImVec2(4, 2);
		style->ItemSpacing = ImVec2(8, 8);




		// =========================== //
		//			  AIMBOT		   //
		// =========================== //
		if (MenuTab == 0)
		{
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255 * 0.75), int(Settings::Misc::MenuColor.Value.y * 255 * 0.75), int(Settings::Misc::MenuColor.Value.z * 255 * 0.75), 255);
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild(skCrypt("##Aimbot"), { 450.f,279.f }, true);
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255 * 0.5), int(Settings::Misc::MenuColor.Value.y * 255 * 0.5), int(Settings::Misc::MenuColor.Value.z * 255 * 0.5), 255);


			ImGui::Checkbox(skCrypt("enabled"), &Settings::Aimbot::Enabled);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22);
			ImGui::Checkbox(skCrypt("show Fov"), &Settings::Aimbot::ShowFov);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22);
			ImGui::Checkbox(skCrypt("fill Fov"), &Settings::Aimbot::FillFovCircle);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);
			if (ImGui::ColorButton(skCrypt("##FovColor"), Settings::Aimbot::FovColor, ColorButtonFlags))
				ImGui::OpenPopup(skCrypt("##FovColorPickerPopUp"));

			if (ImGui::BeginPopup(skCrypt("##FovColorPickerPopUp"))) {
				ColorPicker(skCrypt("##FovColorColorPicker"), Settings::Aimbot::FovColor);
				ImGui::EndPopup();
			}

			ImGui::SliderFloat(skCrypt("##Fov"), &Settings::Aimbot::Fov, 50, 300, skCrypt("fov: %.2f"));

			ImGui::SliderFloat(skCrypt("##Smoothness X"), &Settings::Aimbot::SmoothX, 1, 100, skCrypt("smoothness x: %.2f"));
			ImGui::SameLine();
			ImGui::Checkbox(skCrypt(""), &Settings::Aimbot::LockSmooth);
			if (Settings::Aimbot::LockSmooth) {
				SwitchedSmoothLock = true;
				ImGui::SliderFloat(skCrypt("##Smoothness Y"), &Settings::Aimbot::SmoothX, 1, 100, skCrypt("smoothness y: %.2f"));
			}
			else {
				Settings::Aimbot::SmoothY = SwitchedSmoothLock ? Settings::Aimbot::SmoothX : Settings::Aimbot::SmoothY;
				SwitchedSmoothLock = false;
				ImGui::SliderFloat(skCrypt("##Smoothness Y"), &Settings::Aimbot::SmoothY, 1, 100, skCrypt("smoothness y: %.2f"));
			}


			ImGui::Combo(skCrypt("##Aimkey"), &Settings::Aimbot::CurrentAimkey, Settings::Aimbot::Aimkey, sizeof(Settings::Aimbot::Aimkey) / sizeof(*Settings::Aimbot::Aimkey));

			switch (Settings::Aimbot::CurrentAimkey) {
				case 0: {
					Settings::Aimbot::ControllerMode = false;
					Settings::Aimbot::CurrentKey = VK_LBUTTON;
				} break;
				case 1: {
					Settings::Aimbot::ControllerMode = false;
					Settings::Aimbot::CurrentKey = VK_RBUTTON;
				} break;
				case 2: {
					Settings::Aimbot::ControllerMode = false;
					Settings::Aimbot::CurrentKey = VK_XBUTTON1;
				} break;
				case 3: {
					Settings::Aimbot::ControllerMode = false;
					Settings::Aimbot::CurrentKey = VK_XBUTTON2; 
				} break;
				case 4: {
					Settings::Aimbot::ControllerMode = false;
					Settings::Aimbot::CurrentKey = VK_LCONTROL;
				} break;
				case 5: {
					Settings::Aimbot::ControllerMode = false;
					Settings::Aimbot::CurrentKey = VK_LSHIFT;
				} break;
				case 6: {
					Settings::Aimbot::ControllerMode = false;
					Settings::Aimbot::CurrentKey = VK_LMENU;
				} break;
				case 7: {
					Settings::Aimbot::ControllerMode = true;
				} break;
			}

			ImGui::Combo(skCrypt("##TargetPart"), &Settings::Aimbot::CurrentTargetPart, Settings::Aimbot::TargetPart, sizeof(Settings::Aimbot::TargetPart) / sizeof(*Settings::Aimbot::TargetPart));
			lastSmoothX = Settings::Aimbot::SmoothX;

			ImGui::Checkbox(skCrypt("bullet prediction"), &Settings::Aimbot::Predict);
		}




		// =========================== //
		//			 VISUALS		   //
		// =========================== //
		if (MenuTab == 1)
		{
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255 * 0.75), int(Settings::Misc::MenuColor.Value.y * 255 * 0.75), int(Settings::Misc::MenuColor.Value.z * 255 * 0.75), 255);
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild(skCrypt("##Visuals"), { 450.f,279.f }, true, ImGuiWindowFlags_NoScrollbar);
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255 * 0.5), int(Settings::Misc::MenuColor.Value.y * 255 * 0.5), int(Settings::Misc::MenuColor.Value.z * 255 * 0.5), 255);

			ImGui::Checkbox(skCrypt("enabled"), &Settings::Visuals::Enabled);
			ImGui::SameLine();
			ImGui::SetCursorPosX(142);
			ImGui::Combo(skCrypt("##VisualMode"), &CurrentVisualMode, VisualMode, sizeof(VisualMode) / sizeof(*VisualMode));


			switch (CurrentVisualMode) {
			case 0: {
				ImGui::Checkbox(skCrypt("box"), &Settings::Visuals::Box);
				ImGui::SameLine();

				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##BoxColor"), Settings::Visuals::BoxColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##BoxColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##BoxColorPickerPopUp"))) {
					ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BoxColor);
					ImGui::EndPopup();
				}

				ImGui::SameLine();

				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##BoxThickness"), &Settings::Visuals::BoxLineThickness, 1, 10, skCrypt("thickness: %.2f"));



				ImGui::Checkbox(skCrypt("fill"), &Settings::Visuals::FillBox);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##FillBoxColor"), Settings::Visuals::BoxFillColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##FillBoxColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##FillBoxColorPickerPopUp"))) {
					ColorPicker(skCrypt("##FillBoxColorPicker"), Settings::Visuals::BoxFillColor);
					ImGui::EndPopup();
				}



				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);
				ImGui::Checkbox(skCrypt("bone"), &Settings::Visuals::Bone);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##BoneColor"), Settings::Visuals::BoneColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##BoneColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##BoneColorPickerPopUp"))) {
					if (Settings::Visuals::LockColors)
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BoxColor);
					else
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BoneColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##BoneThickness"), &Settings::Visuals::BoneLineThickness, 1, 10, skCrypt("thickness: %.2f"));



				ImGui::Checkbox(skCrypt("traces"), &Settings::Visuals::Traces);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##TracesColor"), Settings::Visuals::TracesColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##TracesColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##TracesColorPickerPopUp"))) {
					if (Settings::Visuals::LockColors)
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::TracesColor);
					else
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::TracesColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##TracesThickness"), &Settings::Visuals::TraceLineThickness, 1, 10, skCrypt("thickness: %.2f"));

				ImGui::Combo(skCrypt("##TracesOptions"), &Settings::Visuals::CurrentTracesOption, Settings::Visuals::TracesOptions, sizeof(Settings::Visuals::TracesOptions) / sizeof(*Settings::Visuals::TracesOptions));


				ImGui::Checkbox(skCrypt("distance"), &Settings::Visuals::Distance);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);

				if (ImGui::ColorButton(skCrypt("##DistanceColor"), Settings::Visuals::DistanceColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##DistanceColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##DistanceColorPickerPopUp"))) {
					if (Settings::Visuals::LockColors)
						ColorPicker(skCrypt("##DistanceColorPicker"), Settings::Visuals::DistanceColor);
					else
						ColorPicker(skCrypt("##DistanceColorPicker"), Settings::Visuals::DistanceColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##TextSize"), &Settings::Visuals::TextSize, 0.1, 5, skCrypt("textsize: %.1f"));


				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 28);
				ImGui::Checkbox(skCrypt("sync colors"), &Settings::Visuals::LockColors);

				if (Settings::Visuals::LockColors) {
					Settings::Visuals::BoneColor = Settings::Visuals::BoxColor;
					Settings::Visuals::TracesColor = Settings::Visuals::BoxColor;
				}
			} break;



			// TEAM
			case 1: {
				ImGui::Checkbox(skCrypt("box"), &Settings::Visuals::BoxOnTeam);
				ImGui::SameLine();

				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##TeamBoxColor"), Settings::Visuals::TeamBoxColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##TeamBoxColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##TeamBoxColorPickerPopUp"))) {
					ColorPicker(skCrypt("##TeamBoxColorPicker"), Settings::Visuals::TeamBoxColor);
					ImGui::EndPopup();
				}

				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##BoxThickness"), &Settings::Visuals::BoxLineThickness, 1, 10, skCrypt("thickness: %.2f"));



				ImGui::Checkbox(skCrypt("fill"), &Settings::Visuals::FillBoxOnTeam);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##TeamFillBoxColor"), Settings::Visuals::TeamBoxFillColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##TeamFillBoxColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##TeamFillBoxColorPickerPopUp"))) {
					ColorPicker(skCrypt("##TeamFillBoxColorPicker"), Settings::Visuals::TeamBoxFillColor);
					ImGui::EndPopup();
				}



				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);
				ImGui::Checkbox(skCrypt("bone"), &Settings::Visuals::BoneOnTeam);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##TeamBoneColor"), Settings::Visuals::TeamBoneColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##TeamBoneColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##TeamBoneColorPickerPopUp"))) {
					if (Settings::Visuals::LockColorsTeam)
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::TeamBoxColor);
					else
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::TeamBoneColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##BoneThickness"), &Settings::Visuals::BoneLineThickness, 1, 10, skCrypt("thickness: %.2f"));

	




				ImGui::Checkbox(skCrypt("traces"), &Settings::Visuals::TracesOnTeam);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##TeamTracesColor"), Settings::Visuals::TeamTracesColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##TeamTracesColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##TeamTracesColorPickerPopUp"))) {
					if (Settings::Visuals::LockColorsTeam)
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::TeamBoxColor);
					else
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::TeamTracesColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##TracesThickness"), &Settings::Visuals::TraceLineThickness, 1, 10, skCrypt("thickness: %.2f"));

				ImGui::Combo(skCrypt("##TracesOptions"), &Settings::Visuals::CurrentTracesOption, Settings::Visuals::TracesOptions, sizeof(Settings::Visuals::TracesOptions) / sizeof(*Settings::Visuals::TracesOptions));

				ImGui::Checkbox(skCrypt("distance"), &Settings::Visuals::DistanceOnTeam);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				
				if (ImGui::ColorButton(skCrypt("##DistanceColor"), Settings::Visuals::DistanceColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##DistanceColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##DistanceColorPickerPopUp"))) {
					if (Settings::Visuals::LockColors)
						ColorPicker(skCrypt("##DistanceColorPicker"), Settings::Visuals::TeamDistanceColor);
					else
						ColorPicker(skCrypt("##DistanceColorPicker"), Settings::Visuals::TeamDistanceColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##TextSize"), &Settings::Visuals::TextSize, 0.1, 5, skCrypt("textsize: %.1f"));


				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 28);
				ImGui::Checkbox(skCrypt("sync colors"), &Settings::Visuals::LockColorsTeam);
				if (Settings::Visuals::LockColorsTeam) {
					Settings::Visuals::TeamBoneColor = Settings::Visuals::TeamBoxColor;
					Settings::Visuals::TeamTracesColor = Settings::Visuals::TeamBoxColor;
				}
			} break;



				// BOT
			case 2: {
				ImGui::Checkbox(skCrypt("box"), &Settings::Visuals::BoxOnBot);
				ImGui::SameLine();

				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##BotBoxColor"), Settings::Visuals::BotBoxColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##BotBoxColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##BotBoxColorPickerPopUp"))) {
					ColorPicker(skCrypt("##BotBoxColorPicker"), Settings::Visuals::BotBoxColor);
					ImGui::EndPopup();
				}

				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##BoxThickness"), &Settings::Visuals::BoxLineThickness, 1, 10, skCrypt("thickness: %.2f"));



				ImGui::Checkbox(skCrypt("fill"), &Settings::Visuals::FillBoxOnBot);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##BotFillBoxColor"), Settings::Visuals::BotBoxFillColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##BotFillBoxColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##BotFillBoxColorPickerPopUp"))) {
					ColorPicker(skCrypt("##BotFillBoxColorPicker"), Settings::Visuals::BotBoxFillColor);
					ImGui::EndPopup();
				}



				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);
				ImGui::Checkbox(skCrypt("bone"), &Settings::Visuals::BoneOnBot);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##BotBoneColor"), Settings::Visuals::BotBoneColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##BotBoneColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##BotBoneColorPickerPopUp"))) {
					if (Settings::Visuals::LockColorsBot)
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BotBoxColor);
					else
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BotBoneColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##BoneThickness"), &Settings::Visuals::BoneLineThickness, 1, 10, skCrypt("thickness: %.2f"));



				ImGui::Checkbox(skCrypt("traces"), &Settings::Visuals::TracesOnBot);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);
				if (ImGui::ColorButton(skCrypt("##BotTracesColor"), Settings::Visuals::BotTracesColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##BotTracesColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##BotTracesColorPickerPopUp"))) {
					if (Settings::Visuals::LockColorsBot)
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BotBoxColor);
					else
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BotTracesColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##TracesThickness"), &Settings::Visuals::TraceLineThickness, 1, 10, skCrypt("thickness: %.2f"));

				ImGui::Combo(skCrypt("##TracesOptions"), &Settings::Visuals::CurrentTracesOption, Settings::Visuals::TracesOptions, sizeof(Settings::Visuals::TracesOptions) / sizeof(*Settings::Visuals::TracesOptions));

				ImGui::Checkbox(skCrypt("distance"), &Settings::Visuals::DistanceOnBot);
				ImGui::SameLine();
				ImGui::SetCursorPosX(117);

				if (ImGui::ColorButton(skCrypt("##DistanceColor"), Settings::Visuals::DistanceColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##DistanceColorPickerPopUp"));

				if (ImGui::BeginPopup(skCrypt("##DistanceColorPickerPopUp"))) {
					if (Settings::Visuals::LockColors)
						ColorPicker(skCrypt("##DistanceColorPicker"), Settings::Visuals::BotDistanceColor);
					else
						ColorPicker(skCrypt("##DistanceColorPicker"), Settings::Visuals::BotDistanceColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(142);
				ImGui::SliderFloat(skCrypt("##TextSize"), &Settings::Visuals::TextSize, 0.1, 5, skCrypt("textsize: %.1f"));


				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 28);
				ImGui::Checkbox(skCrypt("sync colors"), &Settings::Visuals::LockColorsBot);
				if (Settings::Visuals::LockColorsBot) {
					Settings::Visuals::BotBoneColor = Settings::Visuals::BotBoxColor;
					Settings::Visuals::BotTracesColor = Settings::Visuals::BotBoxColor;
				}

				if (!Settings::CloseRange::Enabled) {
					Settings::CloseRange::DynamicFov = FALSE;
					Settings::CloseRange::SmartSmooth = FALSE;
					Settings::CloseRange::TriggerBot = FALSE;
				}

			} break;
			}
		}



		// =========================== //
		//		MISC / CLOSE RANGE	   //
		// =========================== //
		if (MenuTab == 2)
		{
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255 * 0.75), int(Settings::Misc::MenuColor.Value.y * 255 * 0.75), int(Settings::Misc::MenuColor.Value.z * 255 * 0.75), 255);
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild(skCrypt("##Misc"), { 450.f,279.f }, true);
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255 * 0.5), int(Settings::Misc::MenuColor.Value.y * 255 * 0.5), int(Settings::Misc::MenuColor.Value.z * 255 * 0.5), 255);



			ImGui::PushItemWidth(200);
			ImGui::Combo(skCrypt("##SelectTabs"), &Settings::Misc::selectedTabIndex, Settings::Misc::tabs, sizeof(Settings::Misc::tabs) / sizeof(*Settings::Misc::tabs));


			if (Settings::Misc::selectedTabIndex == 0) {
				ImGui::Checkbox(skCrypt("triggerbot"), &Settings::Misc::TriggerBot);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("only when aimbot"), &Settings::Misc::OnlyWhenAimbot);
				ImGui::Text(skCrypt("gui color"));
				ImGui::SameLine();


				//ImGui::Checkbox(skCrypt("skeleton on Self"), &Settings::Visuals::BoneOnSelf);

				ImGui::Checkbox(skCrypt("show fps"), &Settings::Misc::showFPS);
				//ImGui::SliderInt(skCrypt("##FPSLimit"), &Settings::Misc::FPSLimit, 100, 3000, skCrypt("fps limit: %i"));

				ImGui::Checkbox(skCrypt("show watermark"), &Settings::Visuals::showWatermark);
				ImGui::Checkbox(skCrypt("loot esp"), &Settings::Visuals::lootEsp);

				ImGui::SetCursorPosY(233);
				ImGui::SetCursorPosX(57);

				if (ImGui::ColorButton(skCrypt("##GUIColor"), Settings::Misc::MenuColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##GUIColorPopUp"));

				if (ImGui::BeginPopup(skCrypt("##GUIColorPopUp"))) {
					ColorPicker(skCrypt("##GUIColorPicker"), Settings::Misc::MenuColor);
					ImGui::EndPopup();
				}

				ImGui::SameLine();
				ImGui::SetCursorPosY(230);

				if (ImGui::Button(skCrypt("save config"), { 100.f, 25.f }))
					Settings::SaveConfig();

				ImGui::SameLine();
				if (ImGui::Button(skCrypt("load config"), { 100.f, 25.f }))
					Settings::LoadConfig();

				ImGui::SameLine();
				if (ImGui::Button(skCrypt("reset config"), { 100.f, 25.f }))
					Settings::DefaultConfig();
			}
			if (Settings::Misc::selectedTabIndex == 1)
			{

				ImGui::Checkbox(skCrypt("enable closerange"), &Settings::CloseRange::Enabled);
				ImGui::SliderFloat(skCrypt("##CloseRangeDistance"), &Settings::CloseRange::distance, 5, 100, skCrypt("distance: %.1f"));


				ImGui::SliderFloat(skCrypt("##NormalFov"), &Settings::CloseRange::CurrentFov, 50, 450, skCrypt("closerange fov: %.1f"));


				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				ImGui::SliderFloat(skCrypt("##CloseRange Smoothness X"), &Settings::CloseRange::SmoothX, 1, 40, skCrypt("smoothness x: %.2f"));
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("lock"), &Settings::CloseRange::LockSmooth);
				if (Settings::CloseRange::LockSmooth) {
					SwitchedCloseRangeSmoothLock = true;
					ImGui::SliderFloat(skCrypt("##Smoothness Y"), &Settings::CloseRange::SmoothX, 1, 40, skCrypt("smoothness y: %.2f"));
				}
				else {
					Settings::CloseRange::SmoothY = SwitchedCloseRangeSmoothLock ? Settings::CloseRange::SmoothX : Settings::CloseRange::SmoothY;
					SwitchedCloseRangeSmoothLock = false;
					ImGui::SliderFloat(skCrypt("##Smoothness Y"), &Settings::CloseRange::SmoothY, 1, 40, skCrypt("smoothness y: %.2f"));
				}
				//ImGui::SameLine();
				//ImGui::Checkbox(skCrypt("Dynamic Fov"), &Settings::CloseRange::DynamicFov);
				//ImGui::SliderFloat(skCrypt("##CloseRangeMaxFov"), &Settings::CloseRange::MaxFov, 50, 420, skCrypt("Max Fov: %.1f"));
				//ImGui::SameLine();
				//ImGui::Checkbox(skCrypt("Instant Interpolation"), &Settings::CloseRange::InstantInterpolation);


				ImGui::Checkbox(skCrypt("triggerbot only on close"), &Settings::CloseRange::TriggerBot);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("use smart smoothness"), &Settings::CloseRange::SmartSmooth);



				if (ImGui::ColorButton(skCrypt("##CloseRangeBoxColor"), Settings::CloseRange::BoxColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##CloseRangeBoxColorPickerPopUp"));
				if (ImGui::BeginPopup(skCrypt("##CloseRangeBoxColorPickerPopUp"))) {
					ColorPicker(skCrypt("##CloseRangeBotBoxColorPicker"), Settings::CloseRange::BoxColor);
					ImGui::EndPopup();
				}


				ImGui::SameLine();
				ImGui::Text(skCrypt("box"));
				ImGui::SameLine();


				if (ImGui::ColorButton(skCrypt("##CloseRangeBoneColor"), Settings::CloseRange::BoneColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##CloseRangeBoneColorPickerPopUp"));
				if (ImGui::BeginPopup(skCrypt("##CloseRangeBoneColorPickerPopUp"))) {
					if (Settings::CloseRange::LockColors)
						ColorPicker(skCrypt("##CloseRangeBotBoxColorPicker"), Settings::CloseRange::BoxColor);
					else
						ColorPicker(skCrypt("##CloseRangeBotBoxColorPicker"), Settings::CloseRange::BoneColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Text(skCrypt("bone"));
				ImGui::SameLine();


				if (ImGui::ColorButton(skCrypt("##CloseRangeTracesColor"), Settings::CloseRange::TracesColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##CloseRangeTracesColorPickerPopUp"));
				if (ImGui::BeginPopup(skCrypt("##CloseRangeTracesColorPickerPopUp"))) {
					if (Settings::CloseRange::LockColors)
						ColorPicker(skCrypt("##CloseRangeBotBoxColorPicker"), Settings::CloseRange::BoxColor);
					else
						ColorPicker(skCrypt("##CloseRangeBotBoxColorPicker"), Settings::CloseRange::TracesColor);
					ImGui::EndPopup();
				}
				ImGui::SameLine();
				ImGui::Text(skCrypt("traces"));


				//ImGui::SameLine();
				//ImGui::Checkbox(skCrypt("Interpolate"), &Settings::CloseRange::InterpolateColor);

				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("sync colors"), &Settings::CloseRange::LockColors);

				ImGui::Checkbox(skCrypt("interpolate colors with Distance"), &Settings::CloseRange::InterpolateColor);


				if (Settings::CloseRange::LockColors) {
					Settings::CloseRange::BoneColor = Settings::CloseRange::BoxColor;
					Settings::CloseRange::TracesColor = Settings::CloseRange::BoxColor;
				}

				//ImGui::SameLine();
				//ImGui::Checkbox("Use Smart Smoothness", &Settings::CloseRange::SmartSmooth);
			}
		}
		ImGui::EndChild();


		ImColor Grey = ImColor(0.5f, 0.5f, 0.5f, 1.f);

		if (MenuTab == 0)
		{

			style->Colors[ImGuiCol_Border] = Settings::Misc::MenuColor;
			ImGui::SetCursorPos({ 28.f, 90.f + 6.f });
			ImGui::BeginChild("##Kind000001", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();
			ImGui::SetCursorPos({ 28.f, 90.f + 6.f + 37.f });

			style->Colors[ImGuiCol_Border] = Grey;
			ImGui::BeginChild("##Kind000002", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();

			ImGui::SetCursorPos({ 28.f, 90.f + 6.f + 37.f * 2 });

			style->Colors[ImGuiCol_Border] = Grey;
			ImGui::BeginChild("##Kind000003", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();
		}
		else if (MenuTab == 1)
		{

			style->Colors[ImGuiCol_Border] = Grey;
			ImGui::SetCursorPos({ 28.f, 90.f + 6.f });
			ImGui::BeginChild("##Kind000001", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();
			ImGui::SetCursorPos({ 28.f, 90.f + 6.f + 37.f });

			style->Colors[ImGuiCol_Border] = Settings::Misc::MenuColor;
			ImGui::BeginChild("##Kind000002", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();

			ImGui::SetCursorPos({ 28.f, 90.f + 6.f + 37.f * 2 });

			style->Colors[ImGuiCol_Border] = Grey;
			ImGui::BeginChild("##Kind000003", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();
		}

		else if (MenuTab == 2)
		{

			style->Colors[ImGuiCol_Border] = Grey;
			ImGui::SetCursorPos({ 28.f, 90.f + 6.f });
			ImGui::BeginChild("##Kind000001", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();
			ImGui::SetCursorPos({ 28.f, 90.f + 6.f + 37.f });

			style->Colors[ImGuiCol_Border] = Grey;
			ImGui::BeginChild("##Kind000002", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();

			ImGui::SetCursorPos({ 28.f, 90.f + 6.f + 37.f * 2 });

			style->Colors[ImGuiCol_Border] = Settings::Misc::MenuColor;
			ImGui::BeginChild("##Kind000003", { 2.f,20.f }, true, ImGuiWindowFlags_NoScrollbar);
			ImGui::EndChild();
		}
		style->Colors[ImGuiCol_Border] = Settings::Misc::MenuColor;




		ImGui::End();
	}
}





void Render::DrawFilledBox(int x, int y, int w, int h, const ImColor color) {
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

void Render::DrawCornerBox(int X, int Y, int W, int H, const ImColor color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
}

void Render::DrawOutlinedCornerBox(int X, int Y, int W, int H, const ImColor color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImColor(0, 0, 0, 255), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImColor(0, 0, 0, 255), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImColor(0, 0, 0, 255), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImColor(0, 0, 0, 255), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImColor(0, 0, 0, 255), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImColor(0, 0, 0, 255), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImColor(0, 0, 0, 255), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImColor(0, 0, 0, 255), thickness + 2);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
}

void Render::DrawLine(int x0, int y0, int x1, int y1, const ImColor color, int thickness) {
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x0, y0), ImVec2(x1, y1), color, thickness);
}

void Render::DrawOutlinedLine(int x0, int y0, int x1, int y1, const ImColor color, int thickness, int outline) {
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x0, y0), ImVec2(x1, y1), ImColor(0, 0, 0, 255), thickness + outline);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x0, y0), ImVec2(x1, y1), color, thickness);
}

void Render::DrawNormalText(int x, int y, float size, const ImColor color, const char* text) {
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), size, ImVec2(x, y), color, text);
}

void Render::DrawOutlinedText(int x, int y, float size, const ImColor color, const char* text)
{
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), size, ImVec2(x + 1, y + 1), ImColor(0, 0, 0, 255), text);
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), size, ImVec2(x + 1, y - 1), ImColor(0, 0, 0, 255), text);
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), size, ImVec2(x - 1, y + 1), ImColor(0, 0, 0, 255), text);
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), size, ImVec2(x - 1, y - 1), ImColor(0, 0, 0, 255), text);

	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), size, ImVec2(x, y), color, text);
}


ImColor Render::FadeColor(const ImColor StartColor, const ImColor EndColor, double Percent) {
	Percent = Clamp(Percent, 0, 1);
	return ImColor((StartColor.Value.x + Percent * (EndColor.Value.x - StartColor.Value.x)),
		(StartColor.Value.y + Percent * (EndColor.Value.y - StartColor.Value.y)),
		(StartColor.Value.z + Percent * (EndColor.Value.z - StartColor.Value.z)),
		StartColor.Value.w);
}

ImColor Render::FadeColorAlpha(const ImColor StartColor, const ImColor EndColor, double Percent) {
	return ImColor(StartColor.Value.x + Percent * (EndColor.Value.x - StartColor.Value.x),
		StartColor.Value.y + Percent * (EndColor.Value.y - StartColor.Value.y),
		StartColor.Value.z + Percent * (EndColor.Value.z - StartColor.Value.z),
		(float)(StartColor.Value.w + Percent * (EndColor.Value.w - StartColor.Value.w)));
}


bool ColorPicker(const char* label, ImColor& col)
{
	static const float HUE_PICKER_WIDTH = 20.0f;
	static const float CROSSHAIR_SIZE = 7.0f;
	static const ImVec2 SV_PICKER_SIZE = ImVec2(200, 200);

	ImColor color(col.Value.x, col.Value.y, col.Value.z);
	bool value_changed = false;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 picker_pos = ImGui::GetCursorScreenPos();

	ImColor colors[] = { ImColor(255, 0, 0),
		ImColor(255, 255, 0),
		ImColor(0, 255, 0),
		ImColor(0, 255, 255),
		ImColor(0, 0, 255),
		ImColor(255, 0, 255),
		ImColor(255, 0, 0) };

	for (int i = 0; i < 6; ++i)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 10, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 10 + HUE_PICKER_WIDTH,
				picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
			colors[i],
			colors[i],
			colors[i + 1],
			colors[i + 1]);
	}

	float hue, saturation, value;
	ImGui::ColorConvertRGBtoHSV(
		color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

	draw_list->AddLine(
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 8, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 12 + HUE_PICKER_WIDTH, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImColor(255, 255, 255));

	{
		const int step = 5;
		ImVec2 pos = ImVec2(0, 0);

		ImVec4 c00(1, 1, 1, 1);
		ImVec4 c10(1, 1, 1, 1);
		ImVec4 c01(1, 1, 1, 1);
		ImVec4 c11(1, 1, 1, 1);
		for (int y = 0; y < step; y++) {
			for (int x = 0; x < step; x++) {
				float s0 = (float)x / (float)step;
				float s1 = (float)(x + 1) / (float)step;
				float v0 = 1.0 - (float)(y) / (float)step;
				float v1 = 1.0 - (float)(y + 1) / (float)step;

				ImGui::ColorConvertHSVtoRGB(hue, s0, v0, c00.x, c00.y, c00.z);
				ImGui::ColorConvertHSVtoRGB(hue, s1, v0, c10.x, c10.y, c10.z);
				ImGui::ColorConvertHSVtoRGB(hue, s0, v1, c01.x, c01.y, c01.z);
				ImGui::ColorConvertHSVtoRGB(hue, s1, v1, c11.x, c11.y, c11.z);

				draw_list->AddRectFilledMultiColor(
					ImVec2(picker_pos.x + pos.x, picker_pos.y + pos.y),
					ImVec2(picker_pos.x + pos.x + SV_PICKER_SIZE.x / step, picker_pos.y + pos.y + SV_PICKER_SIZE.y / step),
					ImGui::ColorConvertFloat4ToU32(c00),
					ImGui::ColorConvertFloat4ToU32(c10),
					ImGui::ColorConvertFloat4ToU32(c11),
					ImGui::ColorConvertFloat4ToU32(c01));

				pos.x += SV_PICKER_SIZE.x / step;
			}
			pos.x = 0;
			pos.y += SV_PICKER_SIZE.y / step;
		}
	}

	float x = saturation * SV_PICKER_SIZE.x;
	float y = (1 - value) * SV_PICKER_SIZE.y;
	ImVec2 p(picker_pos.x + x, picker_pos.y + y);
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

	ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);

	if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0])
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(
			ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		/**/ if (mouse_pos_in_canvas.x < 0) mouse_pos_in_canvas.x = 0;
		else if (mouse_pos_in_canvas.x >= SV_PICKER_SIZE.x - 1) mouse_pos_in_canvas.x = SV_PICKER_SIZE.x - 1;

		/**/ if (mouse_pos_in_canvas.y < 0) mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1) mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		value = 1 - (mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1));
		saturation = mouse_pos_in_canvas.x / (SV_PICKER_SIZE.x - 1);
		value_changed = true;
	}

	ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 10, picker_pos.y));
	ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

	if ((ImGui::IsItemHovered() || ImGui::IsItemActive()) && ImGui::GetIO().MouseDown[0])
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(
			ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		/* Previous horizontal bar will represent hue=1 (bottom) as hue=0 (top). Since both colors are red, we clamp at (-2, above edge) to avoid visual continuities */
		/**/ if (mouse_pos_in_canvas.y < 0) mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 2) mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 2;

		hue = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
		value_changed = true;
	}

	color = ImColor::HSV(hue > 0 ? hue : 1e-6, saturation > 0 ? saturation : 1e-6, value > 0 ? value : 1e-6);
	col.Value.x = color.Value.x;
	col.Value.y = color.Value.y;
	col.Value.z = color.Value.z;
	float check[3] = { col.Value.x, col.Value.y, col.Value.z };
	return value_changed || ImGui::ColorEdit3(label, check);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (!Render::HoverOverMenu && Render::InForeground) {
		if (nCode == HC_ACTION) {
			if (wParam == WM_LBUTTONDOWN) {
				return 1;
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}