#pragma once
#include <Windows.h>

namespace keyauth
{
	void check();
	inline bool KeyAuthCheckPassed = false;
	inline LPCSTR KeyPath = "";
};