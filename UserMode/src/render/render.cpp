#include <D3D9.h>
#include "render.h"
#include <Windows.h>
#include <iostream>
#include <DirectXMath.h>
#include "../cheat/cheat.h"
#include "../cheat/sdk/sdk.h"
#include "../cheat/settings/settings.h"
#include "../cheat/driver/driver.h"
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

//uintptr_t procid = NULL;
bool ColorPicker(const char* label, ImColor &col);

HRESULT Render::DirectXInit()
{
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
	bool init()
	{
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
		const DWORD dwerr = prepare_for_ui_access();
		if (ERROR_SUCCESS != dwerr) return false;
		create_window_in_band = reinterpret_cast<CreateWindowInBand>(GetProcAddress(user32, "CreateWindowInBand"));
		if (!create_window_in_band)
		{
			create_window_in_band = reinterpret_cast<CreateWindowInBand>(GetProcAddress(win32u, "CreateWindowInBand"));
			if (!create_window_in_band) return false;
		}
		return true;
	}
}

bool Render::InitGui()
{
	if (gui::init())
	{
		return true;
	}
	return false;
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

void Render::FovCircle() {
	if (!Settings::Aimbot::Enabled or !Settings::Aimbot::ShowFov)
		return;

	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Width / 2, Height / 2), Settings::Aimbot::Fov, Settings::Aimbot::FovColor, 99, 2);
	if (!Settings::Aimbot::FillFovCircle) return;

	ImColor FovTransparent = ImColor((int)(Settings::Aimbot::FovColor.Value.x*255),
									 (int)(Settings::Aimbot::FovColor.Value.y*255),
									 (int)(Settings::Aimbot::FovColor.Value.z*255), 
									 30);

	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(Width/2, Height/2), Settings::Aimbot::Fov, FovTransparent, 99);
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
	}
	else
	{
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


inline const char* VisualMode[] = { "Enemy", "Team", "Bot"};
inline int CurrentVisualMode = 0;
inline int lastSmoothX = 10;
inline bool SwitchedSmoothLock = false;
bool bMenu = true;
//secret menu down here xD
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


	int ColorButtonFlags = (ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoPicker  |
							   ImGuiColorEditFlags_NoTooltip  | ImGuiColorEditFlags_NoLabel        | ImGuiColorEditFlags_NoSidePreview);

	if (GetAsyncKeyState(VK_INSERT) & 1) bMenu = !bMenu;
	if (bMenu)
	{
		static POINT Mouse;
		GetCursorPos(&Mouse);
		//ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(Mouse.x, Mouse.y), float(4), ImColor(255, 0, 0), 50);

		ImGui::SetNextWindowSize({ 620, 350 });
		ImGui::Begin(skCrypt("Fortnite"), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

		ImGui::SetCursorPos({ 25.4f,31.f });
		ImGui::Text(skCrypt("renewable"));
		style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
		style->Colors[ImGuiCol_SliderGrab] = ImColor(68, 68, 68, 255);
		style->Colors[ImGuiCol_SliderGrabActive] = ImColor(68, 68, 68, 255);
		style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);

		//style->Colors[ImGuiCol_SliderGrab] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 100);
		//style->Colors[ImGuiCol_SliderGrabActive] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 100);

		//style->Colors[ImGuiCol_CheckMark] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 255);

		ImGui::SetCursorPos({ 22.f,56.f });
		if (ImGui::Button(skCrypt("Aimbot"), { 89.f, 32.f }))
		{
			MenuTab = 0;
		}

		ImGui::SetCursorPos({ 22.f,93.f });
		if (ImGui::Button(skCrypt("Visuals"), { 89.f, 32.f }))
		{
			MenuTab = 1;
		}

		ImGui::SetCursorPos({ 22.f,130.f });
		if (ImGui::Button(skCrypt("Misc"), { 89.f, 32.f }))
		{
			MenuTab = 2;
		}

		ImGui::SetCursorPos({ 22.f,253.f });
		if (ImGui::Button(skCrypt("Discord"), { 89.f, 32.f }))
		{
			system(skCrypt("start https://discord.gg/DPeKRDTTKY"));

		}

		ImGui::SetCursorPos({ 22.f,290.f });
		if (ImGui::Button(skCrypt("Unload"), { 68.f, 25.f }))
		{
			Settings::SaveConfig();
			exit(0);
		}

		style->Colors[ImGuiCol_Button] = ImColor(12, 12, 12, 0);
		style->Colors[ImGuiCol_ButtonActive] = ImColor(24, 24, 38, 0);
		style->Colors[ImGuiCol_ButtonHovered] = ImColor(36, 36, 70, 0);

		//style->ItemInnerSpacing = ImVec2(8, 2);
		style->WindowPadding = ImVec2(16, 16);
		//style->FramePadding = ImVec2(4, 2);
		style->ItemSpacing = ImVec2(8, 8);

		if (MenuTab == 0)
		{
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 160);
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild(skCrypt("##Aimbot"), { 450.f,279.f }, true);
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 80);


			ImGui::Checkbox(skCrypt("Enabled"), &Settings::Aimbot::Enabled);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22);
			ImGui::Checkbox(skCrypt("Show Fov"), &Settings::Aimbot::ShowFov);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 22);
			ImGui::Checkbox(skCrypt("Fill Fov"), &Settings::Aimbot::FillFovCircle);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12);
			if (ImGui::ColorButton(skCrypt("##FovColor"), Settings::Aimbot::FovColor, ColorButtonFlags))
				ImGui::OpenPopup(skCrypt("##FovColorPickerPopUp"));

			if (ImGui::BeginPopup(skCrypt("##FovColorPickerPopUp"))) {
				ColorPicker(skCrypt("##FovColorColorPicker"), Settings::Aimbot::FovColor);
				ImGui::EndPopup();
			}

			ImGui::SliderFloat(skCrypt("##Fov"), &Settings::Aimbot::Fov, 50, 300, skCrypt("Fov: %.2f"));
			
			ImGui::SliderFloat(skCrypt("##Smoothness X"), &Settings::Aimbot::SmoothX, 1, 40, skCrypt("Smoothness X: %.2f"));
			ImGui::SameLine();
			ImGui::Checkbox(skCrypt("Lock"), &Settings::Aimbot::LockSmooth);
			if (Settings::Aimbot::LockSmooth) {
				SwitchedSmoothLock = true;
				ImGui::SliderFloat(skCrypt("##Smoothness Y"), &Settings::Aimbot::SmoothX, 1, 40, skCrypt("Smoothness Y: %.2f"));
			}
			else {
				Settings::Aimbot::SmoothY = SwitchedSmoothLock ? Settings::Aimbot::SmoothX : Settings::Aimbot::SmoothY;
				SwitchedSmoothLock = false;
				ImGui::SliderFloat(skCrypt("##Smoothness Y"), &Settings::Aimbot::SmoothY, 1, 40, skCrypt("Smoothness Y: %.2f"));
			}


			ImGui::Combo(skCrypt("##Aimkey"), &Settings::Aimbot::CurrentAimkey, Settings::Aimbot::Aimkey, sizeof(Settings::Aimbot::Aimkey) / sizeof(*Settings::Aimbot::Aimkey));

			switch (Settings::Aimbot::CurrentAimkey) {
			case 0: {
				Settings::Aimbot::CurrentKey = VK_LBUTTON;
			} break;
			case 1: {
				Settings::Aimbot::CurrentKey = VK_RBUTTON;
			} break;
			}

			ImGui::Combo(skCrypt("##TargetPart"), &Settings::Aimbot::CurrentTargetPart, Settings::Aimbot::TargetPart, sizeof(Settings::Aimbot::TargetPart) / sizeof(*Settings::Aimbot::TargetPart));
			lastSmoothX = Settings::Aimbot::SmoothX;

			ImGui::Checkbox(skCrypt("Bullet Prediction"), &Settings::Aimbot::Predict);
		}
		if (MenuTab == 1)
		{
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 160);
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild(skCrypt("##Visuals"), { 450.f,279.f }, true);
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 80);

			ImGui::Checkbox(skCrypt("Enabled"), &Settings::Visuals::Enabled);
			ImGui::SameLine();
			ImGui::SetCursorPosX(90);
			ImGui::Combo(skCrypt("##VisualMode"), &CurrentVisualMode, VisualMode, sizeof(VisualMode) / sizeof(*VisualMode));

			switch (CurrentVisualMode) {
				case 0: {
					ImGui::Checkbox(skCrypt("Box"), &Settings::Visuals::Box);
					ImGui::SameLine();

					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##BoxColor"), Settings::Visuals::BoxColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##BoxColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##BoxColorPickerPopUp"))) {
						ColorPicker(skCrypt("##BoxColorPicker"), Settings::Visuals::BoxColor);
						ImGui::EndPopup();
					}

					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##BoxThickness"), &Settings::Visuals::BoxLineThickness, 1, 10, skCrypt("Thickness: %.2f"));



					ImGui::Checkbox(skCrypt("Fill"), &Settings::Visuals::FillBox);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##FillBoxColor"), Settings::Visuals::BoxFillColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##FillBoxColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##FillBoxColorPickerPopUp"))) {
						ColorPicker(skCrypt("##FillBoxColorPicker"), Settings::Visuals::BoxFillColor);
						ImGui::EndPopup();
					}



					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);
					ImGui::Checkbox(skCrypt("Bone"), &Settings::Visuals::Bone);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##BoneColor"), Settings::Visuals::BoneColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##BoneColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##BoneColorPickerPopUp"))) {
						ColorPicker(skCrypt("##BoneColorPicker"), Settings::Visuals::BoneColor);
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##BoneThickness"), &Settings::Visuals::BoneLineThickness, 1, 10, skCrypt("Thickness: %.2f"));

					ImGui::Checkbox(skCrypt("On Team"), &Settings::Visuals::BoneOnTeam);
					ImGui::SameLine();
					ImGui::SliderFloat(skCrypt("##BoneDisplayRange"), &Settings::Visuals::BoneDisplayRange, 0, 350, skCrypt("Display Range: %.5f"));



					ImGui::Checkbox(skCrypt("Traces"), &Settings::Visuals::Traces);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##TracesColor"), Settings::Visuals::TracesColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##TracesColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##TracesColorPickerPopUp"))) {
						ColorPicker(skCrypt("##TracesColorPicker"), Settings::Visuals::TracesColor);
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##TracesThickness"), &Settings::Visuals::TraceLineThickness, 1, 10, skCrypt("Thickness: %.2f"));

					ImGui::Combo(skCrypt("##TracesOptions"), &Settings::Visuals::CurrentTracesOption, Settings::Visuals::TracesOptions, sizeof(Settings::Visuals::TracesOptions) / sizeof(*Settings::Visuals::TracesOptions));

					ImGui::Checkbox(skCrypt("Distance"), &Settings::Visuals::Distance);
				} break;


				case 1: {
					ImGui::Checkbox(skCrypt("Box"), &Settings::Visuals::Box);
					ImGui::SameLine();

					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##TeamBoxColor"), Settings::Visuals::TeamBoxColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##TeamBoxColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##TeamBoxColorPickerPopUp"))) {
						ColorPicker(skCrypt("##TeamBoxColorPicker"), Settings::Visuals::TeamBoxColor);
						ImGui::EndPopup();
					}

					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##BoxThickness"), &Settings::Visuals::BoxLineThickness, 1, 10, skCrypt("Thickness: %.2f"));



					ImGui::Checkbox(skCrypt("Fill"), &Settings::Visuals::FillBox);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##TeamFillBoxColor"), Settings::Visuals::TeamBoxFillColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##TeamFillBoxColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##TeamFillBoxColorPickerPopUp"))) {
						ColorPicker(skCrypt("##TeamFillBoxColorPicker"), Settings::Visuals::TeamBoxFillColor);
						ImGui::EndPopup();
					}



					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);
					ImGui::Checkbox(skCrypt("Bone"), &Settings::Visuals::Bone);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##TeamBoneColor"), Settings::Visuals::TeamBoneColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##TeamBoneColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##TeamBoneColorPickerPopUp"))) {
						ColorPicker(skCrypt("##TeamBoneColorPicker"), Settings::Visuals::TeamBoneColor);
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##BoneThickness"), &Settings::Visuals::BoneLineThickness, 1, 10, skCrypt("Thickness: %.2f"));

					ImGui::Checkbox(skCrypt("On Team"), &Settings::Visuals::BoneOnTeam);
					ImGui::SameLine();
					ImGui::SliderFloat(skCrypt("##BoneDisplayRange"), &Settings::Visuals::BoneDisplayRange, 0, 350, skCrypt("Display Range: %.5f"));





					ImGui::Checkbox(skCrypt("Traces"), &Settings::Visuals::Traces);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##TeamTracesColor"), Settings::Visuals::TeamTracesColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##TeamTracesColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##TeamTracesColorPickerPopUp"))) {
						ColorPicker(skCrypt("##TeamTracesColorPicker"), Settings::Visuals::TeamTracesColor);
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##TracesThickness"), &Settings::Visuals::TraceLineThickness, 1, 10, skCrypt("Thickness: %.2f"));

					ImGui::Combo(skCrypt("##TracesOptions"), &Settings::Visuals::CurrentTracesOption, Settings::Visuals::TracesOptions, sizeof(Settings::Visuals::TracesOptions) / sizeof(*Settings::Visuals::TracesOptions));

					ImGui::Checkbox(skCrypt("Distance"), &Settings::Visuals::Distance);

				} break;

				case 2: {
					ImGui::Checkbox(skCrypt("Box"), &Settings::Visuals::Box);
					ImGui::SameLine();

					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##BotBoxColor"), Settings::Visuals::BotBoxColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##BotBoxColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##BotBoxColorPickerPopUp"))) {
						ColorPicker(skCrypt("##BotBoxColorPicker"), Settings::Visuals::BotBoxColor);
						ImGui::EndPopup();
					}

					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##BoxThickness"), &Settings::Visuals::BoxLineThickness, 1, 10, skCrypt("Thickness: %.2f"));



					ImGui::Checkbox(skCrypt("Fill"), &Settings::Visuals::FillBox);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##BotFillBoxColor"), Settings::Visuals::BotBoxFillColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##BotFillBoxColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##BotFillBoxColorPickerPopUp"))) {
						ColorPicker(skCrypt("##BotFillBoxColorPicker"), Settings::Visuals::BotBoxFillColor);
						ImGui::EndPopup();
					}



					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);
					ImGui::Checkbox(skCrypt("Bone"), &Settings::Visuals::Bone);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##BotBoneColor"), Settings::Visuals::BotBoneColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##BotBoneColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##BotBoneColorPickerPopUp"))) {
						ColorPicker(skCrypt("##BotBoneColorPicker"), Settings::Visuals::BotBoneColor);
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##BoneThickness"), &Settings::Visuals::BoneLineThickness, 1, 10, skCrypt("Thickness: %.2f"));

					ImGui::Checkbox(skCrypt("On Team"), &Settings::Visuals::BoneOnTeam);
					ImGui::SameLine();
					ImGui::SliderFloat(skCrypt("##BoneDisplayRange"), &Settings::Visuals::BoneDisplayRange, 0, 350, skCrypt("Display Range: %.5f"));





					ImGui::Checkbox(skCrypt("Traces"), &Settings::Visuals::Traces);
					ImGui::SameLine();
					ImGui::SetCursorPosX(80);
					if (ImGui::ColorButton(skCrypt("##BotTracesColor"), Settings::Visuals::BotTracesColor, ColorButtonFlags))
						ImGui::OpenPopup(skCrypt("##BotTracesColorPickerPopUp"));

					if (ImGui::BeginPopup(skCrypt("##BotTracesColorPickerPopUp"))) {
						ColorPicker(skCrypt("##BotTracesColorPicker"), Settings::Visuals::BotTracesColor);
						ImGui::EndPopup();
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(110);
					ImGui::SliderFloat(skCrypt("##TracesThickness"), &Settings::Visuals::TraceLineThickness, 1, 10, skCrypt("Thickness: %.2f"));

					ImGui::Combo(skCrypt("##TracesOptions"), &Settings::Visuals::CurrentTracesOption, Settings::Visuals::TracesOptions, sizeof(Settings::Visuals::TracesOptions) / sizeof(*Settings::Visuals::TracesOptions));

					ImGui::Checkbox(skCrypt("Distance"), &Settings::Visuals::Distance);

				} break;
			}
		}
		if (MenuTab == 2)
		{
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 160);
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild(skCrypt("##Misc"), { 450.f,279.f }, true);
			style->Colors[ImGuiCol_Border] = ImColor(int(Settings::Misc::MenuColor.Value.x * 255), int(Settings::Misc::MenuColor.Value.y * 255), int(Settings::Misc::MenuColor.Value.z * 255), 80);



			ImGui::PushItemWidth(200);
			ImGui::Combo(skCrypt("##SelectTabs"), &Settings::Misc::selectedTabIndex, Settings::Misc::tabs, sizeof(Settings::Misc::tabs) / sizeof(*Settings::Misc::tabs));


			if (Settings::Misc::selectedTabIndex == 0) {
				ImGui::Checkbox(skCrypt("Triggerbot"), &Settings::Misc::TriggerBot);
				ImGui::SameLine();
				ImGui::Checkbox(skCrypt("Only when Aimbot"), &Settings::Misc::OnlyWhenAimbot);
				ImGui::Text(skCrypt("GUI Color"));
				ImGui::SameLine();
				ImGui::SetCursorPosX(80);
				if (ImGui::ColorButton(skCrypt("##GUIColor"), Settings::Misc::MenuColor, ColorButtonFlags))
					ImGui::OpenPopup(skCrypt("##GUIColorPopUp"));

				if (ImGui::BeginPopup(skCrypt("##GUIColorPopUp"))) {
					ColorPicker(skCrypt("##GUIColorPicker"), Settings::Misc::MenuColor);
					ImGui::EndPopup();
				}

				ImGui::SetCursorPosY(230);
				ImGui::SetCursorPosX(75);
				if (ImGui::Button(skCrypt("Save Config"), { 100.f, 25.f }))
					Settings::SaveConfig();

				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Load Config"), { 100.f, 25.f }))
					Settings::LoadConfig();

				ImGui::SameLine();
				if (ImGui::Button(skCrypt("Reset Config"), { 100.f, 25.f }))
					Settings::DefaultConfig();
			}
		}
		ImGui::EndChild();


		style->Colors[ImGuiCol_Border] = Settings::Misc::MenuColor;;

		//ImGui::SetCursorPos({ 17.f,56.f });
		//ImGui::BeginChild("##Child000001", { 20.f,30.f }, true);
		//ImGui::EndChild();




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





bool ColorPicker(const char* label, ImColor &col)
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