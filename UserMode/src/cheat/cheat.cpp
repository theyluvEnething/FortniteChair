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
	ZeroMemory(&Render::Message, sizeof(MSG));
	for (;Render::Message.message != WM_QUIT;) {
		Render::HandleMessage();

		Cheat::LateUpdate();
		Cheat::Update();
		

		Cheat::TriggerBot();
		Cheat::Esp();
		Cheat::Aimbot();


		Render::FovCircle();
		Render::render();
		Render::Menu();

		Render::EndOfFrame();
		LimitFPS(240);
	}

	Settings::SaveConfig();
	Render::CloseRender();
}


constexpr std::chrono::milliseconds intervaltrigger(100);
auto start_triggerbot = std::chrono::steady_clock::now();
void Cheat::TriggerBot() {
	if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey) 
		and Settings::Misc::OnlyWhenAimbot)
		return;
	if (!Settings::Misc::TriggerBot)
		return;
	if (!cache::TargetedFortPawn)
		return;
	//std::cout << "LocalPawnTeam: " << cache::TeamId << " | TargetedPawnTeam: " << driver::read<int>(TargetPawn + offset::TEAM_INDEX) << std::endl;

	//if (cache::TeamId == driver::read<int>(TargetPawn + offset::TEAM_INDEX)) 
	//	return;

	auto end_t = std::chrono::steady_clock::now();
	if (end_t - start_triggerbot < intervaltrigger)
		return;
	
	leftMouseClick();
	start_triggerbot = end_t;
}


void Cheat::Esp() {
	for (int i = 0; i < cache::PlayerCount; i++) {
		auto Player = driver::read<uintptr_t>(cache::PlayerArray + i * offset::PLAYERSIZE);
		auto CurrentActor = driver::read<uintptr_t>(Player + offset::PAWNPRIV);
		if (!CurrentActor) continue;
		auto TeamId = driver::read<int>(Player + offset::TEAM_INDEX);
		// ALSO UPDATE OFFSET FIRST
		// auto CurrentWeapon = driver::read<uintptr_t>(CurrentActor + 0x9F8);
		if (CurrentActor == cache::LocalPawn) continue;

		uint64_t Mesh = driver::read<uint64_t>(CurrentActor + offset::MESH);
		Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 109);
		Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);
		Vector3 Bottom3D = SDK::GetBoneWithRotation(Mesh, 0);
		Vector2 Bottom2D = SDK::ProjectWorldToScreen(Bottom3D);


		float BoxHeight = (float)(Head2D.y - Bottom2D.y);
		float CornerHeight = abs(Head2D.y - Bottom2D.y);
		float CornerWidth = BoxHeight * 0.45f;
		float distance = cache::RelativeLocation.Distance(Head3D) / 100;
		
		char distanceString[64] = { 0 };
		sprintf_s(distanceString, skCrypt("[%.0fm]"), distance);
		ImVec2 TextSize = ImGui::CalcTextSize(distanceString);
		TextSize.x /= 2;
		TextSize.y /= 2;
		std::string dist = "[" + std::to_string(static_cast<int>(distance)) + "m]";

		float TracesConnectHeight = Head2D.y;
		if (Settings::Visuals::CurrentTracesOption == 0) {
			TracesConnectHeight = Head2D.y + CornerHeight;
			Settings::Visuals::TracesHeight = Height;
		}
		else if (Settings::Visuals::CurrentTracesOption == 1) {
			TracesConnectHeight = Head2D.y - (CornerHeight * 0.075f);
			Settings::Visuals::TracesHeight = CenterY;
		}
		else if (Settings::Visuals::CurrentTracesOption == 2) {
			TracesConnectHeight = Head2D.y - (CornerHeight * 0.075f);
			Settings::Visuals::TracesHeight = 0;
		}

		if (TeamId != cache::TeamId) {
			auto crosshairDist = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);
			if (crosshairDist < Settings::Aimbot::Fov && crosshairDist < ClosestDistance) {
				ClosestDistance = crosshairDist;
				TargetPawn = Player;
				TargetMesh = Mesh;
			}
		}

		if (!Settings::Visuals::Enabled)
			return;

		if (TeamId == cache::TeamId) {
			if (Settings::Visuals::Box)
				Render::DrawOutlinedCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y - CornerHeight*0.075f, CornerWidth, CornerHeight+ CornerHeight*0.075f, Settings::Visuals::TeamBoxColor, Settings::Visuals::BoxLineThickness);
			if (Settings::Visuals::FillBox)
				Render::DrawFilledBox(Head2D.x - (CornerWidth / 2), Head2D.y - CornerHeight * 0.075f, CornerWidth, CornerHeight+CornerHeight * 0.075f, Settings::Visuals::TeamBoxFillColor);
			if (Settings::Visuals::Traces)
				Render::DrawLine(Width / 2, Settings::Visuals::TracesHeight, Head2D.x, TracesConnectHeight, Settings::Visuals::TeamTracesColor, Settings::Visuals::TraceLineThickness);
			if (Settings::Visuals::Distance)
				Render::DrawOutlinedText((Head2D.x - TextSize.x*1.8f), (Head2D.y - (CornerHeight*0.05f) - CornerHeight * 0.075f), TextSize.x, Settings::Visuals::TeamBoxColor, distanceString);
			if ((Settings::Visuals::BoneOnTeam && Settings::Visuals::Bone && (distance < Settings::Visuals::BoneDisplayRange)) || cache::InLobby)
				DrawSkeleton(Mesh, FALSE);

		} else {
			if (Settings::Visuals::Box)
				Render::DrawOutlinedCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y - (CornerHeight * 0.075f), CornerWidth, CornerHeight + (CornerHeight * 0.075f), get_trace_color_based_on_distance(Settings::Visuals::BoxColor, distance), Settings::Visuals::BoxLineThickness);
			if (Settings::Visuals::FillBox)
				Render::DrawFilledBox(Head2D.x - (CornerWidth / 2), Head2D.y - (CornerHeight * 0.075f), CornerWidth, CornerHeight + (CornerHeight * 0.075f), get_trace_color_based_on_distance(Settings::Visuals::BoxFillColor, distance));
			if (Settings::Visuals::Traces)
				Render::DrawLine(Width / 2, Settings::Visuals::TracesHeight, Head2D.x, TracesConnectHeight, get_trace_color_based_on_distance(Settings::Visuals::TracesColor, distance), Settings::Visuals::TraceLineThickness);
			if (Settings::Visuals::Distance)
				Render::DrawOutlinedText((Head2D.x - TextSize.x * 1.8f), (Head2D.y - (CornerHeight * 0.05f) - CornerHeight * 0.075f), TextSize.x, get_trace_color_based_on_distance(Settings::Visuals::BoxColor, distance), distanceString);
			if (Settings::Visuals::Bone && distance < Settings::Visuals::BoneDisplayRange || cache::InLobby)
				DrawSkeleton(Mesh, TRUE);
		}
	}
}

void Cheat::Aimbot() {
	if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey))
		return;
	if (!Settings::Aimbot::Enabled)
		return;
	if (!TargetPawn) 
		return;

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

	float heightCorrection = 0; // 0.221 * tanh(cache::RelativeLocation.Distance(Head3D) - 3750);

	//std::cout << "Mouse: " << target.x << " " << target.y << " : " << heightCorrection << " " << cache::RelativeLocation.Distance(Head3D) << std::endl;
	//mouse_event(MOUSEEVENTF_MOVE, target.x, target.y+ heightCorrection, NULL, NULL);
	input::move_mouse(target.x, target.y);
}


void Cheat::Update() {
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
}

constexpr std::chrono::seconds interval(1);
auto start_t = std::chrono::steady_clock::now();
void Cheat::LateUpdate() {
	auto end_t = std::chrono::steady_clock::now();
	if (end_t - start_t < interval)
		return;

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
	start_t = end_t;
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

static void DrawSkeleton(uint64_t Mesh, bool Enemy) {

	Vector3 BoneRotations[] = {
		SDK::GetBoneWithRotation(Mesh, 109),	// HeadBone
		SDK::GetBoneWithRotation(Mesh, 2),		// Hip
		SDK::GetBoneWithRotation(Mesh, 67),		// Neck
		SDK::GetBoneWithRotation(Mesh, 9),		// UpperArmLeft
		SDK::GetBoneWithRotation(Mesh, 38),		// UpperArmRight
		SDK::GetBoneWithRotation(Mesh, 10),		// LeftHand
		SDK::GetBoneWithRotation(Mesh, 39),		// RightHand
		SDK::GetBoneWithRotation(Mesh, 11),		// LeftHand1
		SDK::GetBoneWithRotation(Mesh, 40),		// RightHand1
		SDK::GetBoneWithRotation(Mesh, 78),		// RightThigh
		SDK::GetBoneWithRotation(Mesh, 71),		// LeftThigh
		SDK::GetBoneWithRotation(Mesh, 79),		// RightCalf
		SDK::GetBoneWithRotation(Mesh, 72),		// LeftCalf
		SDK::GetBoneWithRotation(Mesh, 73),		// LeftFoot
		SDK::GetBoneWithRotation(Mesh, 80)		// RightFoot
	};

	Vector2 BonePositions[16];
	for (int i = 0; i < 16; ++i) {
		BonePositions[i] = SDK::ProjectWorldToScreen(BoneRotations[i]);
	}

	ImColor BoneColor = Enemy ? Settings::Visuals::BoneColor : Settings::Visuals::TeamBoneColor;

	Render::DrawLine(BonePositions[0].x, BonePositions[0].y, BonePositions[2].x, BonePositions[2].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[1].x, BonePositions[1].y, BonePositions[2].x, BonePositions[2].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[3].x, BonePositions[3].y, BonePositions[2].x, BonePositions[2].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[4].x, BonePositions[4].y, BonePositions[2].x, BonePositions[2].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[5].x, BonePositions[5].y, BonePositions[3].x, BonePositions[3].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[6].x, BonePositions[6].y, BonePositions[4].x, BonePositions[4].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[5].x, BonePositions[5].y, BonePositions[7].x, BonePositions[7].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[6].x, BonePositions[6].y, BonePositions[8].x, BonePositions[8].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[10].x, BonePositions[10].y, BonePositions[1].x, BonePositions[1].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[9].x, BonePositions[9].y, BonePositions[1].x, BonePositions[1].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[11].x, BonePositions[11].y, BonePositions[9].x, BonePositions[9].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[12].x, BonePositions[12].y, BonePositions[10].x, BonePositions[10].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[13].x, BonePositions[13].y, BonePositions[12].x, BonePositions[12].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);
	Render::DrawLine(BonePositions[14].x, BonePositions[14].y, BonePositions[11].x, BonePositions[11].y, ImColor(0, 0, 0, 255), Settings::Visuals::BoneLineThickness + 2);

	Render::DrawLine(BonePositions[0].x, BonePositions[0].y, BonePositions[2].x, BonePositions[2].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[1].x, BonePositions[1].y, BonePositions[2].x, BonePositions[2].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[3].x, BonePositions[3].y, BonePositions[2].x, BonePositions[2].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[4].x, BonePositions[4].y, BonePositions[2].x, BonePositions[2].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[5].x, BonePositions[5].y, BonePositions[3].x, BonePositions[3].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[6].x, BonePositions[6].y, BonePositions[4].x, BonePositions[4].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[5].x, BonePositions[5].y, BonePositions[7].x, BonePositions[7].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[6].x, BonePositions[6].y, BonePositions[8].x, BonePositions[8].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[10].x, BonePositions[10].y, BonePositions[1].x, BonePositions[1].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[9].x, BonePositions[9].y, BonePositions[1].x, BonePositions[1].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[11].x, BonePositions[11].y, BonePositions[9].x, BonePositions[9].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[12].x, BonePositions[12].y, BonePositions[10].x, BonePositions[10].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[13].x, BonePositions[13].y, BonePositions[12].x, BonePositions[12].y, BoneColor, Settings::Visuals::BoneLineThickness);
	Render::DrawLine(BonePositions[14].x, BonePositions[14].y, BonePositions[11].x, BonePositions[11].y, BoneColor, Settings::Visuals::BoneLineThickness);
}
