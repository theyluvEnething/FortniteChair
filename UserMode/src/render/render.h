#pragma once
#include "../util/util.h"
extern IDirect3D9Ex* p_Object;
extern IDirect3DDevice9Ex* p_Device;
extern D3DPRESENT_PARAMETERS p_Params;


class Render {
public:
	static HWND GameHwnd;
	static RECT GameRect;
	static HWND MyHwnd;
	static MSG Message;
	static BOOL bMenu;
	static BOOL HoverOverMenu;
	static BOOL InForeground;

	static bool InitGui();

	static HRESULT DirectXInit();
	static void CreateOverlay();
	static WPARAM Present(MSG Message);
	static void HandleMessage();
	static void EndOfFrame();
	static void FovCircle();
	static void CloseRender();
	static void render();
	static void CleanuoD3D();
	static void Menu();

	static void DrawCornerBox(int x, int y, int w, int h, const ImColor color, int thickness);
	static void DrawOutlinedCornerBox(int x, int y, int w, int h, const ImColor color, int thickness);
	static void DrawFilledBox(int x, int y, int w, int h, const ImColor color);
	static void DrawLine(int x0, int y0, int x1, int y1, const ImColor color, int thickness);
	static void DrawOutlinedLine(int x0, int y0, int x1, int y1, const ImColor color, int thickness, int outline = 2.5);
	static void DrawNormalText(int x, int y, float size, const ImColor color, const char* text);
	static void DrawOutlinedText(int x, int y, float size, const ImColor color, const char* text);

	static ImColor FadeColor(const ImColor StartColor, const ImColor EndColor, double Percent);
	static ImColor FadeColorAlpha(const ImColor StartColor, const ImColor EndColor, double Percent);
};

