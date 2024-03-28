#pragma once
#include <string>
#include <Windows.h>

inline uintptr_t handleFromString(const char* c)
{
	ptrdiff_t h;
	sscanf_s(c, "%tx", &h); // only hex supported, %td for decimal.
	return (uintptr_t)h;
}


namespace keyauth
{
	void check();
	std::string get_var(const char* var_name);
	inline bool KeyAuthCheckPassed = false;
	inline LPCSTR KeyPath = "";
};