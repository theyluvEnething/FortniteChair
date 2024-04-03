#include <iostream>
#include <Windows.h>
#include <memory>
#include <cstdint>
#include <vector>
#include <string_view>
#include "cheat/cheat.h"
#include "render/render.h"
#include "cheat/data/offsets.h"
#include "driver/driver.h"
#include "cheat/settings/settings.h"
#include "util/skStr.h"
#include "util/prints.h"
#include "util/keyauth/keyauth.h"
#include "driver/mapper/include/mappermain.hpp"
//#include "cheat/data/input.h"

auto main(int argc, char* argv[]) -> void
{	
	/*std::cout << "Current ppid: " << getppid() << std::endl;
	std::cout << "Current cmd line: " << GetCommandLineA() << std::endl;
	std::cout << "argc: " << argc << std::endl;*/

	// Tried redirecting stdout when not started by render.cpp function (argv[1] != "final", for some reason didnt work. Not necessary to fix but would be clearner
	/*HANDLE hStdout = CreateFile(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	std::cout << "set std outp: " << SetStdHandle(STD_OUTPUT_HANDLE, hStdout) << std::endl;*/


	bool preStart = true;
	if (argc > 1) {
		if (strcmp(argv[1], "final")) {
			preStart = false;
		}
	}

	if (preStart) {
		if (!Render::InitGui())
		{
			printf(skCrypt("please start as admin..."));
			Sleep(3000);
			exit(1);
		}
	}

	std::string AppdataPath = GetAppDataPath();
	keyauth::KeyPath	 = StringAdd(GetAppDataPath(), "\\key");


	std::cout << skCrypt("") << std::endl;
	printCenteredColoredText(skCrypt("renewable").decrypt(), 9);
	printCenteredColoredText(skCrypt("discord.gg/renewable").decrypt(), 8);
	std::cout << skCrypt("") << std::endl;
	

	
	keyauth::check();
	if (!keyauth::KeyAuthCheckPassed) {
		printLog(skCrypt("incorrct key!").decrypt());
		std::getchar();
		return;
	}

		// F6969369

	std::string offsets = keyauth::get_var(std::string() + skCrypt("F48384").decrypt());
	offset::ULocalPawn = handleFromString(getOffset(offsets, "ULocalPawn"));
	

	if (!driver::setup())
	{
		printLog(skCrypt("failed setting up driver").decrypt());
		Sleep(3000);
		exit(1);
	}


	offset::iPlayerCount = handleFromString(getOffset(offsets, "iPlayerCount"));

	printLog(skCrypt("checking driver...").decrypt());
	if (driver::check()) {
		printLog(skCrypt("driver is running!").decrypt());
	}
	else {
		printLog(skCrypt("driver is not running!").decrypt());
		if (IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe"))) {
			printLog(skCrypt("please close fortnite").decrypt());
			printLog(skCrypt("press any key to exit...").decrypt());
			std::getchar(); return;
		}
		else {
			printLog(skCrypt("mapping driver...").decrypt());
			mappermain();
			printLog(skCrypt("successfully mapped driver!").decrypt());
		}
	}
	
	offset::ULocalPlayers = handleFromString(getOffset(offsets, "ULocalPlayers"));

	
	if (!IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe"))) {
		printLog(skCrypt("please open fortnite to continue!").decrypt());
	}

	while (!IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe"))) {
		Sleep(1000);
	}
	offset::WorldGravityZ = handleFromString(getOffset(offsets, "WorldGravityZ"));
	printLog(skCrypt("found FortniteClient-Win64-Shipping.exe!").decrypt());
	offset::AWorldSettings = handleFromString(getOffset(offsets, "AWorldSettings"));
	// FortniteClient-Win64-Shipping //ITS FUCKING .exe
	ProcId = driver::find_process(skCrypt("FortniteClient-Win64-Shipping.exe"));
	BaseId = driver::find_image();
	std::cout << skCrypt("[>] ProcessId: ") << ProcId << skCrypt(" | 0x") << std::hex << ProcId << std::dec << std::endl;
	std::cout << skCrypt("[>] BaseAddress: ") << BaseId  << skCrypt(" | 0x") << std::hex << BaseId << std::dec << std::endl;

	offset::UGameInstance = handleFromString(getOffset(offsets, "UGameInstance"));
	
	while ((!ProcId || !BaseId) && IsProcessRunning(skCrypt("FortniteClient-Win64-Shipping.exe")))
	{
		printLog(skCrypt("failed getting base address or process id!").decrypt());
		Sleep(1000);
		ProcId = driver::find_process(skCrypt("FortniteClient-Win64-Shipping.exe"));
		BaseId = driver::find_image();
	}

	offset::AGameStateBase = handleFromString(getOffset(offsets, "AGameStateBase"));
	Render::GameHwnd = Util::get_process_wnd(ProcId);
	Settings::DefaultConfig();	
	offset::UWorld = handleFromString(getOffset(offsets, "UWorld"));
	Settings::ConfigPath = StringAdd(GetAppDataPath(), "\\config");
	Settings::LoadConfig();
	Settings::Initialized = TRUE;


	if (!Render::InitGui())
	{
		printf(skCrypt("failed with overlay! :( (start as admin maybe)"));
		Sleep(3000);
		exit(1);
	}
	
	offset::PersistentLevel = handleFromString(getOffset(offsets, "PersistentLevel"));
	Render::CreateOverlay();
	offset::APlayerController = handleFromString(getOffset(offsets, "UPlayerController"));
	Render::DirectXInit();
	offset::iPlayerArray = handleFromString(getOffset(offsets, "iPlayerArray"));

	Cheat::Init();
	Cheat::Present();	
}

