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

	static HRESULT DirectXInit();
	static void CreateOverlay();
	static WPARAM Present(MSG Message);
	static void HandleMessage();
	static void EndOfFrame();
	static void CloseRender();
	static void render();
	static void CleanuoD3D();
	static void Menu();
};