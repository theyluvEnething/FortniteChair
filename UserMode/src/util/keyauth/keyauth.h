#pragma once
#include <string>
#include <Windows.h>

__forceinline std::string getSecondPart(const std::string input) {
    size_t pos = input.find("-");
    if (pos != std::string::npos) {
        // If the "-" character is found, return the substring after it
        return input.substr(pos + 1);
    }
    else {
        // If the "-" character is not found, return an empty string or the original string
        return "";
    }
}

__forceinline uintptr_t handleFromString(std::string c)
{
	ptrdiff_t h;
	sscanf_s(c.c_str(), "%tx", &h); // only hex supported, %td for decimal.
	return (uintptr_t)h;
}


namespace keyauth
{
	void check();
	std::string get_var(std::string var_name);
	inline bool KeyAuthCheckPassed = false;
	inline LPCSTR KeyPath = "";
};