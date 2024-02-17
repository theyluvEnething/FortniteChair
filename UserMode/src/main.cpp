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
//#include "cheat/data/input.h"

auto main() -> void
{
	std::cout << "[+] Function Hook UserMode started!" << std::endl;

	if (driver::check())
		std::cout << "[>] Driver is running." << std::endl;
	else 
		std::cout << "[!] Driver not running." << std::endl;



	ProcId = driver::find_process("FortniteClient-Win64-Shipping.exe");
	BaseId = driver::find_image();
	std::cout << "[>] ProcessId: " << ProcId << " | 0x" << std::hex << ProcId << std::dec << std::endl;
	std::cout << "[>] BaseAddress: " << BaseId  << " | 0x" << std::hex << BaseId << std::dec << std::endl;


	if (!ProcId || !BaseId || ProcId == 976)
		std::cout << "[!] Please open Fortnite." << std::endl;
		
	for (;!BaseId;)
		BaseId = driver::find_image();

	
	std::cout << "[+] Succesfully found Fortnite: " << ProcId << " | " << std::hex << BaseId << std::dec << std::endl;

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
