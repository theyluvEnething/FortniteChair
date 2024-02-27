#include <iostream>
#include <Windows.h>
#include <memory>
#include <cstdint>
#include <vector>
#include <string_view>
#include "cheat/cheat.h"
#include "render/render.h"
#include "cheat/data/offsets.h"
#include "cheat/driver/driver.h"
#include "cheat/settings/settings.h"
#include "util/skStr.h"
#include "cheat/driver/mapper/include/mappermain.hpp"
//#include "cheat/data/input.h"

auto main() -> void
{
	std::cout << skCrypt("[+] UserMode started!") << std::endl;
	//std::cout << "[+] UserMode started!" << std::endl;

	driver::setup();

	(USE_FUNCTION_HOOK_DRIVER); (USE_SIGNATURE_SCAN_DRIVER);
	driver::WhichDriver = USE_FUNCTION_HOOK_DRIVER;

	std::cout << skCrypt("[>] checking driver") << std::endl;
	if (driver::check()) {
		std::cout << skCrypt("[>] Driver is running.") << std::endl;	
	}
	else {
		std::cout << skCrypt("[!] Driver not running.") << std::endl;
		if (IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe"))) {
			std::cout << skCrypt("[!] Close Fortnite\nPress any key to exit...") << std::endl;
			std::getchar(); return;
		}
		else {
			std::cout << skCrypt("[>] Mapping driver...") << std::endl;
			mappermain();
			std::cout << skCrypt("[>] Successfully mapped driver") << std::endl;
		}
	}
	
	
	if (!IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe"))) {
		std::cout << skCrypt("[!] Please open fortnite to continue!") << std::endl;
	}

	while (!IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe"))) Sleep(1000);
	std::cout << skCrypt("[!] Detected FortniteClient-Win64-Shipping.exe!") << std::endl;
	// FortniteClient-Win64-Shipping //ITS FUCKING .exe
	ProcId = driver::find_process(skCrypt("FortniteClient-Win64-Shipping.exe"));
	BaseId = driver::find_image();
	std::cout << skCrypt("[>] ProcessId: ") << ProcId << skCrypt(" | 0x") << std::hex << ProcId << std::dec << std::endl;
	std::cout << skCrypt("[>] BaseAddress: ") << BaseId  << skCrypt(" | 0x") << std::hex << BaseId << std::dec << std::endl;


	while (!ProcId || !BaseId && IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe"))) 
	{
		std::cout << skCrypt("[!] Failed getting base addy or procid...") << std::endl;
		Sleep(1000);
		ProcId = driver::find_process(skCrypt("FortniteClient-Win64-Shipping.exe"));
		BaseId = driver::find_image();
	}

	
	std::cout << skCrypt("[+] Succesfully found Fortnite: ") << ProcId << skCrypt(" | ") << std::hex << BaseId << std::dec << std::endl;

	Render::GameHwnd = Util::get_process_wnd(ProcId);

	Settings::DefaultConfig(); 
	Settings::ConfigPath = StringAdd(GetAppDataPath(), "\\config");
	Settings::LoadConfig();
	Settings::Initialized = TRUE;

	if (!Render::InitGui())
	{
		printf(skCrypt("failed with overlay! :( (start as admin maybe)"));
		Sleep(3000);
		exit(1);
	}

	Render::CreateOverlay();
	Render::DirectXInit();

	Cheat::Init();
	Cheat::Present();	
}
