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
//#include "cheat/data/input.h"

auto main() -> void
{
	std::cout << skCrypt("[+] UserMode started!") << std::endl;
	//std::cout << "[+] UserMode started!" << std::endl;

	(USE_FUNCTION_HOOK_DRIVER); (USE_SIGNATURE_SCAN_DRIVER);
	driver::WhichDriver = USE_FUNCTION_HOOK_DRIVER;
	driver::setup();


	if (driver::check()) {
		std::cout << skCrypt("[>] Driver is running.") << std::endl;
	} else {
		std::cout << skCrypt("[!] Driver not running.") << std::endl;
		std::getchar(); return;
	}

	// FortniteClient-Win64-Shipping
	ProcId = driver::find_process(skCrypt("ac_client.exe"));
	BaseId = driver::find_image();
	std::cout << skCrypt("[>] ProcessId: ") << ProcId << skCrypt(" | 0x") << std::hex << ProcId << std::dec << std::endl;
	std::cout << skCrypt("[>] BaseAddress: ") << BaseId  << skCrypt(" | 0x") << std::hex << BaseId << std::dec << std::endl;


	if (!ProcId || !BaseId || ProcId == 976) {
		std::cout << skCrypt("[!] Please open Fortnite.") << std::endl;
		std::getchar(); return;
	}

	
	std::cout << skCrypt("[+] Succesfully found Fortnite: ") << ProcId << skCrypt(" | ") << std::hex << BaseId << std::dec << std::endl;

	Render::GameHwnd = Util::get_process_wnd(ProcId);

	Settings::DefaultConfig(); 
	Settings::ConfigPath = StringAdd(GetAppDataPath(), "\\config");
	Settings::LoadConfig();
	Settings::Initialized = TRUE;

	Render::CreateOverlay();
	Render::DirectXInit();

	Cheat::Init();
	Cheat::Present();	
}
