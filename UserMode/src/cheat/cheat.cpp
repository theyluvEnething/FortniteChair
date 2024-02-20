#include <thread>
#include <chrono>
#include <iomanip>
#include "cheat.h"
#include "sdk/sdk.h"
#include "data/offsets.h"
#include "driver/driver.h"
#include "../util/util.h"
#include "../util/skStr.h"
#include "../render/render.h"
#include "settings/settings.h"
#include "data/input.h"


ImColor get_trace_color_based_on_distance(ImColor color, float distance) {

	ImColor newColor =  ImColor(color.Value.x, color.Value.y, color.Value.z, 255-distance/1.7);
	return newColor;
}

long long framecount;
#define clamp(x, minVal, maxVal) min(max(x, minVal), maxVal)
uintptr_t Cheat::TargetPawn = 0;
uint64_t Cheat::TargetMesh = 0;
float Cheat::ClosestDistance = FLT_MAX;
std::vector<APlayerInformation> Cheat::PlayerArray{};

static void leftMouseClick();
static void DrawSkeleton(uint64_t Mesh, bool Enemy);

void CtrlHandler(DWORD fdwCtrlType) {
	if (fdwCtrlType != CTRL_CLOSE_EVENT)
		return;

	Settings::SaveConfig();
}

void Cheat::Init() {
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	if (!librarys::init())
	{
		printf(skCrypt("The librarys was not initialized"));
		Sleep(3000);
		exit(0);
	}

	if (!input::init())
	{
		printf(skCrypt("The input was not initialized"));
		Sleep(3000);
		exit(0);
	}

	cache::uWorld				= driver::read<address>((BaseId + offset::UWORLD));
	cache::GameState			= driver::read<uintptr_t>(cache::uWorld + offset::GAME_STATE);
	cache::GameInstance			= driver::read<uintptr_t>(cache::uWorld + offset::GAME_INSTANCE);
	cache::LocalPlayers			= driver::read<uintptr_t>(driver::read<uintptr_t>(cache::GameInstance + offset::LOCAL_PLAYERS));
	cache::PlayerController		= driver::read<uintptr_t>(cache::LocalPlayers + offset::PLAYER_CONTROLLER);
	cache::LocalPawn			= driver::read<uintptr_t>(cache::PlayerController + offset::LOCAL_PAWN);
	cache::PlayerArray = driver::read<uintptr_t>(cache::GameState + offset::PLAYER_ARRAY);
	cache::PlayerCount = driver::read<int>(cache::GameState + (offset::PLAYER_ARRAY + sizeof(uintptr_t)));
	cache::InLobby = cache::PlayerCount == 1 ? true : false;
	std::cout << skCrypt("-> game_state :: ") << cache::GameState << std::endl;
	std::cout << skCrypt("-> uworld :: ") << cache::uWorld << std::endl;
	std::cout << skCrypt("-> game_instance :: ") << cache::GameInstance << std::endl;
	std::cout << skCrypt("-> local_players :: ") << cache::LocalPlayers << std::endl;
	std::cout << skCrypt("-> player_controller :: ") << cache::PlayerController << std::endl;
	std::cout << skCrypt("-> local_pawn :: ") << cache::LocalPawn << std::endl;
	std::cout << skCrypt("-> player_count :: ") << cache::PlayerCount << std::endl;

	if (cache::LocalPawn != 0)
	{
		cache::RootComponent	=	driver::read<uintptr_t>(cache::LocalPawn + offset::ROOT_COMPONENT);
		cache::PlayerState		=	driver::read<uintptr_t>(cache::LocalPawn + offset::PLAYER_STATE);
		cache::TeamId			=	driver::read<int>(cache::PlayerState + offset::TEAM_INDEX);
		cache::RelativeLocation =	driver::read<Vector3>(cache::RootComponent + offset::RELATIVE_LOCATION);
		std::cout << skCrypt("-> root_component :: ") << cache::RootComponent << std::endl;
		std::cout << skCrypt("-> player_state :: ") << cache::PlayerState << std::endl;
		std::cout << skCrypt("-> my_team_id :: ") << cache::TeamId << std::endl;
	}
	

	for (int i = 0; i < cache::PlayerCount; i++) {
		auto player = driver::read<uintptr_t>(cache::PlayerArray + i * offset::PLAYERSIZE);
		auto currentActor = driver::read<uintptr_t>(player + offset::PAWNPRIV);
		auto teamId = driver::read<int>(player + offset::TEAM_INDEX);
		auto CurrentWeapon = driver::read<uintptr_t>(currentActor + 0x9F8);
		if (currentActor == NULL)
			continue;
		uint64_t mesh = driver::read<uint64_t>(currentActor + offset::MESH);
		uintptr_t BoneA = driver::read<uintptr_t>(mesh + offset::BONE_ARRAY);
		if (BoneA == NULL)
			BoneA = driver::read<uintptr_t>(mesh + offset::BONE_ARRAY + 0x10);
		FTransform Bone = driver::read<FTransform>(BoneA + (109 * offset::bonec));
		FTransform Comp = driver::read<FTransform>(mesh + offset::COMPONENT_TO_WORLD);
		D3DMATRIX matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), Comp.ToMatrixWithScale());


		Vector3 pos = Vector3(matrix._41, matrix._42, matrix._43);
		//Util::Print3D("[+] ", pos);
	}
}

void LimitFPS(float targetFPS) {
	float deltatime = ImGui::GetIO().DeltaTime;
	float targetFrameTime = 1.0f / targetFPS;
	if (deltatime < targetFrameTime) {
		std::chrono::duration<float> sleepTime(targetFrameTime - deltatime);
		std::this_thread::sleep_for(sleepTime);
	}
}

void Cheat::Present() {

	
	std::thread([&]() { LateUpdate(); }).detach();
	std::thread([&]() { Update(); }).detach(); 

	std::thread([&]() { TriggerBot(); }).detach();

	std::thread([&]() { ActorLoop(); }).detach();
	std::thread([&]() { Aimbot(); }).detach();

	std::thread([&]() { Aimbot(); }).detach();


	std::thread([&]() { Render::RenderAll(); }).join();
	
	Settings::SaveConfig();
	Render::CloseRender();
}


void Cheat::TriggerBot() {
	for (;;) {
		if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey)
			and Settings::Misc::OnlyWhenAimbot) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue;
		}
		if (!Settings::Misc::TriggerBot) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue;
		}
		if (!cache::TargetedFortPawn) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue;
		}

		leftMouseClick();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


void Cheat::ActorLoop() {
	for (;;) {
		Cheat::PlayerArray.clear();
		for (int i = 0; i < cache::PlayerCount; i++) {

			APlayerInformation PlayerInfo = { 0 };

			auto Player = driver::read<uintptr_t>(cache::PlayerArray + i * offset::PLAYERSIZE);
			auto CurrentPawn = driver::read<uintptr_t>(Player + offset::PAWNPRIV);
			if (!CurrentPawn) continue;
			auto TeamId = driver::read<int>(Player + offset::TEAM_INDEX);
			// ALSO UPDATE OFFSET FIRST
			// auto CurrentWeapon = driver::read<uintptr_t>(CurrentActor + 0x9F8);
			if (CurrentPawn == cache::LocalPawn) continue;

			uint64_t Mesh = driver::read<uint64_t>(CurrentPawn + offset::MESH);
			Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 109);
			Vector3 Bottom3D = SDK::GetBoneWithRotation(Mesh, 0);


			PlayerInfo.FortnitePawn = CurrentPawn;
			PlayerInfo.Mesh = Mesh;
			PlayerInfo.TeamId = TeamId;
			PlayerInfo.Head3D = Head3D;
			PlayerInfo.Bottom3D = Bottom3D;
			PlayerInfo.IsBot = false;

			PlayerArray.push_back(PlayerInfo);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Cheat::Aimbot() {
	for (;;) {
		if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue;
		}
		if (!Settings::Aimbot::Enabled) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue;
		}
		if (!TargetPawn) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); continue;
		}

		uint8_t Bone = 109; // head
		switch (Settings::Aimbot::CurrentTargetPart) {
			case 1: { // neck 
				Bone = 67;
			} break;
			case 2: { // hip 
				Bone = 2;
			} break;
			case 3: { // feet 
				Bone = 73;
			} break;
		}

		Vector3 Head3D = SDK::GetBoneWithRotation(TargetMesh, Bone);
		Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);
		Vector2 target{};

		if (Head2D.x != 0)
		{
			if (Head2D.x > Width/2)
			{
				target.x = -(Width/2 - Head2D.x);
				target.x /= Settings::Aimbot::SmoothX;
				if (target.x + Width/2 > Width/2 * 2) target.x = 0;
			}
			if (Head2D.x < Width/2)
			{
				target.x = Head2D.x - Width/2;
				target.x /= Settings::Aimbot::SmoothX;
				if (target.x + Width/2 < 0) target.x = 0;
			}
		}
		if (Head2D.y != 0)
		{
			if (Head2D.y > Height/2)
			{
				target.y = -(Height/2 - Head2D.y);
				target.y /= Settings::Aimbot::SmoothY;
				if (target.y + Height/2 > Height/2 * 2) target.y = 0;
			}
			if (Head2D.y < Height/2)
			{
				target.y = Head2D.y - Height/2;
				target.y /= Settings::Aimbot::SmoothY;
				if (target.y + Height/2 < 0) target.y = 0;
			}
		}
		target.x = clamp(target.x, -8, 8);
		target.y = clamp(target.y, -8, 8);

		input::move_mouse(target.x, target.y);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


void Cheat::Update() {
	for (;;) {
		switch (Settings::Aimbot::CurrentAimkey) {
		case 0: {
			Settings::Aimbot::CurrentKey = VK_LBUTTON;
		} break;
		case 1: {
			Settings::Aimbot::CurrentKey = VK_RBUTTON;
		} break;
		}

		Cheat::TargetPawn = NULL;
		Cheat::TargetMesh = NULL;
		Cheat::ClosestDistance = FLT_MAX;

		cache::PlayerCount = driver::read<int>(cache::GameState + (offset::PLAYER_ARRAY + sizeof(uintptr_t)));
		cache::InLobby = (cache::PlayerCount == 1 && !cache::LocalPawn) ? true : false;
		cache::TargetedFortPawn = driver::read<address>(cache::PlayerController + offset::TARGETED_FORT_PAWN);
		if (cache::LocalPawn) {
			cache::RelativeLocation = driver::read<Vector3>(cache::RootComponent + offset::RELATIVE_LOCATION);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


void Cheat::LateUpdate() {

	for (;;) {
		cache::uWorld = driver::read<address>((BaseId + offset::UWORLD));
		cache::GameState = driver::read<uintptr_t>(cache::uWorld + offset::GAME_STATE);
		cache::GameInstance = driver::read<uintptr_t>(cache::uWorld + offset::GAME_INSTANCE);
		cache::LocalPlayers = driver::read<uintptr_t>(driver::read<uintptr_t>(cache::GameInstance + offset::LOCAL_PLAYERS));
		cache::PlayerController = driver::read<uintptr_t>(cache::LocalPlayers + offset::PLAYER_CONTROLLER);
		cache::LocalPawn = driver::read<uintptr_t>(cache::PlayerController + offset::LOCAL_PAWN);
		cache::PlayerArray = driver::read<address>(cache::GameState + offset::PLAYER_ARRAY);
		if (cache::LocalPawn) {
			cache::RootComponent = driver::read<uintptr_t>(cache::LocalPawn + offset::ROOT_COMPONENT);
			cache::PlayerState = driver::read<uintptr_t>(cache::LocalPawn + offset::PLAYER_STATE);
			cache::TeamId = driver::read<int>(cache::PlayerState + offset::TEAM_INDEX);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}


static void leftMouseClick() {
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dx = 0;
	input.mi.dy = 0;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	input.mi.dwExtraInfo = 0;
	input.mi.time = 0;

	SendInput(1, &input, sizeof(INPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

