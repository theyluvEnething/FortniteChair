#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

__forceinline std::string getSecondPart(const std::string input, const std::string search) {
    size_t pos = input.find(search);
    if (pos != std::string::npos) {
        // If the "-" character is found, return the substring after it
        return input.substr(pos + 1);
    }
    else {
        // If the "-" character is not found, return an empty string or the original string
        return "";
    }
}

__forceinline std::string getFirstPart(const std::string& input, const std::string& search) {
    size_t pos = input.find(search);
    if (pos != std::string::npos) {
        // If the search string is found, return the substring before it
        return input.substr(0, pos);
    }
    else {
        // If the search string is not found, return an empty string or the original string
        // Depending on the desired behavior, you can return the original string here
        return "";
    }
}

__forceinline std::string getOffset(const std::string string, const std::string variable) {
    std::string ret;
    ret = getSecondPart(getFirstPart(getSecondPart(string, std::string() + variable + "-"), ","), "-");
    std::cout << ret << std::endl;
    return ret;
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