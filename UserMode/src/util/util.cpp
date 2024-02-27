#include "util.h"
#include <string>
#include <Shlobj.h>
#include <iostream>
#include <TlHelp32.h>
#include <string.h>
#include "../cheat/driver/driver.h"

#pragma warning (disable : 4996)

typedef int(__stdcall* f_funci)();

int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);
int CenterX = Width / 2;
int CenterY = Height / 2;

int Util::get_fps()
{
	using namespace std::chrono;
	static int count = 0;
	static auto last = high_resolution_clock::now();
	auto now = high_resolution_clock::now();
	static int fps = 0;

	count++;

	if (duration_cast<milliseconds>(now - last).count() > 1000) {
		fps = count;
		count = 0;
		last = now;
	}

	return fps;
}

void Util::PrintFirst512Bytes() {
	char buffer[512];
	driver::read(BaseId, buffer, sizeof(buffer));
	std::cout << "Reads: " << std::endl;
	for (int i = 0; i < sizeof(buffer); i++) {
		if (i % 8 == 0) std::cout << " | ";
		if (i % 24 == 0) std::cout << std::endl;
		if ((int)((uint8_t)buffer[i]) < 16) std::cout << "0";
		std::cout << std::hex << (int)((uint8_t)buffer[i]) << " ";
	}
	std::cout << std::dec << std::endl;
	std::getchar();
}


void Util::PrintPtr(std::string text, uintptr_t ptr) {
	std::cout << text << ptr << std::endl;
}
void Util::Print2D(std::string text, Vector2 pos) {
	std::cout << text << pos.x << ", " << pos.y << std::endl;
}
void Util::Print3D(std::string text, Vector3 pos) {
	std::cout << text << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}
double Util::GetCrossDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}
D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}
D3DMATRIX Matrix(Vector3 rot, Vector3 origin) {
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

LPCSTR GetAppDataPath() {
	wchar_t path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path))) {
		static char narrowPath[MAX_PATH];
		WideCharToMultiByte(CP_UTF8, 0, path, -1, narrowPath, MAX_PATH, NULL, NULL);
		return narrowPath;
	}
	else {
		return "";
	}
}

LPCSTR StringAdd(LPCSTR lpStr, const char* str) {
	int lpStrLength = strlen(lpStr);
	int strLength = strlen(str);
	char* combinedString = new char[lpStrLength + strLength + 1]; // +1 for null terminator
	strcpy(combinedString, lpStr);
	strcat(combinedString, str);
	return combinedString;
}

HWND Util::get_process_wnd(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bresult = EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL
		{
			auto pparams = (std::pair<HWND, uint32_t>*)(lparam);
			uint32_t processid = 0;
			if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processid)) && processid == pparams->second)
			{
				SetLastError((uint32_t)-1);
				pparams->first = hwnd;
				return FALSE;
			}
			return TRUE;
		}, (LPARAM)&params);
	if (!bresult && GetLastError() == -1 && params.first) return params.first;
	return 0;
}


bool IsProcessRunning(const char* processName) {
	// Get a snapshot of all processes
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	// Iterate over processes
	if (Process32First(hSnapshot, &pe)) {
		do {
			std::string narrowString;
			int bufferSize = WideCharToMultiByte(CP_UTF8, 0, pe.szExeFile, -1, nullptr, 0, nullptr, nullptr);
			if (bufferSize > 0) {
				narrowString.resize(bufferSize);
				WideCharToMultiByte(CP_UTF8, 0, pe.szExeFile, -1, narrowString.data(), bufferSize, nullptr, nullptr);
			}

			//std::cout << "Current process: " << narrowString << std::endl;

			if (_stricmp(narrowString.c_str(), processName) == 0) {
				CloseHandle(hSnapshot);
				return true; // Found the process
			}
		} while (Process32Next(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
	return false; // Process not found
}
