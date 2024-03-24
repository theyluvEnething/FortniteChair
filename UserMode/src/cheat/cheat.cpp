#include <cmath>
#include <chrono>
#include <iomanip>
#include <bitset>
#include "cheat.h"
#include "sdk/sdk.h"
#include "data/offsets.h"
#include "../driver/driver.h"
#include "../util/util.h"
#include "../util/skStr.h"
#include "../render/render.h"
#include "settings/settings.h"
#include "data/input.h"

long long framecount;
#define clamp(x, minVal, maxVal) min(max(x, minVal), maxVal)
uintptr_t Cheat::TargetPawn = 0;
uint64_t Cheat::TargetMesh = 0;
float Cheat::ClosestDistance2D = FLT_MAX;
float Cheat::ClosestDistance3D = FLT_MAX;
char Cheat::TargetPawnTeamId = 0;
bool Cheat::locked = FALSE;
bool Cheat::useCloseRangeFov = FALSE;
bool updated = FALSE;

static void leftMouseClick();
static void DrawSkeleton(uint64_t Mesh, BYTE Enemy);
static void DrawSkeletonOnSelf(uint64_t Mesh, BYTE Enemy);
static void DrawSkeleton(uint64_t Mesh, BYTE Enemy, float Distance);

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

	cache::UWorld = driver::read<address>((BaseId + offset::UWorld));
	cache::PersistentLevel = driver::read<uintptr_t>(cache::UWorld + offset::PersistentLevel);
	cache::AWorldSettings = driver::read<uintptr_t>(cache::PersistentLevel + offset::AWorldSettings);
	cache::AGameStateBase = driver::read<uintptr_t>(cache::UWorld + offset::AGameStateBase);
	cache::UGameInstance = driver::read<uintptr_t>(cache::UWorld + offset::UGameInstance);
	cache::ULocalPlayers = driver::read<uintptr_t>(driver::read<uintptr_t>(cache::UGameInstance + offset::ULocalPlayers));
	cache::UPlayerController = driver::read<uintptr_t>(cache::ULocalPlayers + offset::APlayerController);
	cache::ULocalPawn = driver::read<uintptr_t>(cache::UPlayerController + offset::ULocalPawn);
	cache::WorldGravityZ = driver::read<float>(cache::WorldGravityZ + offset::WorldGravityZ);
	cache::iPlayerArray = driver::read<uintptr_t>(cache::AGameStateBase + offset::iPlayerArray);
	cache::iPlayerCount = driver::read<int>(cache::AGameStateBase + (offset::iPlayerArray + sizeof(uintptr_t)));
	cache::InLobby = (cache::iPlayerCount == 1 && !cache::ULocalPawn) ? true : false;


	std::cout << skCrypt("-> game_state :: ") << cache::AGameStateBase << std::endl;
	std::cout << skCrypt("-> uworld :: ") << cache::UWorld << std::endl;
	std::cout << skCrypt("-> game_instance :: ") << cache::UGameInstance << std::endl;
	std::cout << skCrypt("-> local_players :: ") << cache::ULocalPlayers << std::endl;
	std::cout << skCrypt("-> player_controller :: ") << cache::UPlayerController << std::endl;
	std::cout << skCrypt("-> local_pawn :: ") << cache::ULocalPawn << std::endl;
	std::cout << skCrypt("-> player_count :: ") << cache::iPlayerCount << std::endl;
	std::cout << skCrypt("-> in-lobby :: ") << cache::InLobby << std::endl;
	if (cache::ULocalPawn != 0)
	{
		cache::RootComponent = driver::read<uintptr_t>(cache::ULocalPawn + offset::RootComponent);
		cache::PlayerState = driver::read<uintptr_t>(cache::ULocalPawn + offset::AFortPlayerStateAthena);
		cache::TeamId = driver::read<int>(cache::PlayerState + offset::TEAM_INDEX);
		cache::RelativeLocation = driver::read<Vector3>(cache::RootComponent + offset::RelativeLocation);
	}
	/*std::cout << skCrypt("-> root_component :: ") << cache::RootComponent << std::endl;
	std::cout << skCrypt("-> player_state :: ") << cache::PlayerState << std::endl;
				DrawSkeleton(Mesh, 2);
	std::cout << skCrypt("-> my_team_id :: ") << cache::TeamId << std::endl;*/




	for (int i = 0; i < cache::iPlayerCount; i++) {
		auto APlayerState = driver::read<uintptr_t>(cache::iPlayerArray + i * offset::iPlayerSize);
		auto CurrentPawn = driver::read<uintptr_t>(APlayerState + offset::UPawnPrivate);
		auto teamId = driver::read<char>(APlayerState + offset::TEAM_INDEX);
		if (CurrentPawn == NULL)
			continue;
		uint64_t mesh = driver::read<uint64_t>(CurrentPawn + offset::MESH);
		uintptr_t BoneA = driver::read<uintptr_t>(mesh + offset::BONE_ARRAY);
		if (BoneA == NULL)
			BoneA = driver::read<uintptr_t>(mesh + offset::BONE_ARRAY + 0x10);
		FTransform Bone = driver::read<FTransform>(BoneA + (109 * offset::bonec));
		FTransform Comp = driver::read<FTransform>(mesh + offset::COMPONENT_TO_WORLD);
		D3DMATRIX matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), Comp.ToMatrixWithScale());

		Vector3 pos = Vector3(matrix._41, matrix._42, matrix._43);
		//std::cout << "[" << pos.x << " " << pos.z << " " << pos.y << "]" << std::endl;


		//uintptr_t USceneComponent = driver::read<uintptr_t>(APlayerState + 0x2e8);
		//std::cout << "-> USceneComponent :: " << USceneComponent << std::endl;
		//Vector3 vel = Vector3(-1, -1, -1);
		//vel.x = driver::read<double>(USceneComponent + 0x168);
		//vel.y = driver::read<double>(USceneComponent + 0x168 + 8);
		//vel.z = driver::read<double>(USceneComponent + 0x168 + 16);
		//std::cout << "[" << vel.x << " " << vel.z << " " << vel.y << "]" << std::endl;

		//Util::Print3D("[+] :: ", driver::read<Vector3>(USceneComponent + offset::RelativeLocation));



		auto PlayerId = driver::read<int32_t>(APlayerState + offset::RootComponent);
		bool IsABot = driver::read<unsigned char>(APlayerState + 0x29a); // & 0x00001000;
		//std::cout << PlayerId << " - ";
		bool flagBool = driver::read<unsigned char>(APlayerState + 0x29a);
		/*std::cout << "0x";
		for (int i = 0; i < 8; i++) {
			std::cout << ((flagBool >> i) & 0x1);
		}
		std::cout << std::endl;*/
	}
	std::thread t(Cheat::MouseAimbotThread);
	t.detach();
}

ImColor CloseRangeColor(ImColor NormalColor, ImColor CloseColor, double Distance) {
	if (!Settings::CloseRange::Enabled) {
		return NormalColor;
	}
	if (Settings::CloseRange::InterpolateColor) {
		return Render::FadeColor(NormalColor, CloseColor, (double)Settings::CloseRange::distance / (double)Distance);
	} else {
		return Distance < Settings::CloseRange::distance ? CloseColor : NormalColor;
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

void LimitBetterFPS(float targetFPS) {
	static std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point currentFrameTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentFrameTime - lastFrameTime;
	float targetFrameTime = 1.0f / targetFPS;

	if (deltaTime < std::chrono::duration<float>(targetFrameTime)) {
		std::chrono::duration<float> sleepTime(targetFrameTime - deltaTime.count());
		std::this_thread::sleep_for(sleepTime);
	}

	lastFrameTime = std::chrono::steady_clock::now();
}

void write_angle(float x, float y) {
	uintptr_t PCameraMan = driver::read<uintptr_t>(cache::UPlayerController + 0x348); // PlayerCameraManager 0x348(0x08)
	if (PCameraMan != 0) {
		driver::write<float>(PCameraMan + 0x2388, x); // float ViewPitchMax; // 0x2388(0x04)
		driver::write<float>(PCameraMan + 0x2384, x); // float ViewPitchMin; // 0x2384(0x04)
		driver::write<float>(PCameraMan + 0x238c, y); // float ViewYawMin; // 0x238c(0x04)
		driver::write<float>(PCameraMan + 0x2390, y);  // float ViewYawMax; // 0x2390(0x04)
	}
}

void reset_angles() {
	float ViewPitchMin = -89.9999f;
	float ViewPitchMax = 89.9999f;
	float ViewYawMin = 0.0000f;
	float ViewYawMax = 359.9999f;
	uintptr_t PCameraMan = driver::read<uintptr_t>(cache::UPlayerController + 0x348); // PlayerCameraManager 0x348(0x08)
	if (PCameraMan != 0) {
		driver::write<float>(PCameraMan + 0x2388, ViewPitchMax); // float ViewPitchMax; // 0x2388(0x04)
		driver::write<float>(PCameraMan + 0x2384, ViewPitchMin); // float ViewPitchMin; // 0x2384(0x04)
		driver::write<float>(PCameraMan + 0x238c, ViewYawMin); // float ViewYawMin; // 0x238c(0x04)
		driver::write<float>(PCameraMan + 0x2390, ViewYawMax);  // float ViewYawMax; // 0x2390(0x04)
	}
}

void PredictBulletDrop(Vector3& Target, Vector3 TargetVelocity, float ProjectileSpeed, float ProjectileGravityScale, float Distance) {
	float horizontalTime = Distance / ProjectileSpeed;
	float verticalTime = Distance / ProjectileSpeed;

	Target.x += TargetVelocity.x * horizontalTime;
	Target.y += TargetVelocity.y * horizontalTime;
	Target.y += TargetVelocity.z * verticalTime + abs(cache::WorldGravityZ * ProjectileGravityScale) * 0.5f * (verticalTime * verticalTime);
}

void Cheat::Present() {

	std::thread([&]() { Cheat::LateUpdate(); }).detach();
	std::thread([&]() { Cheat::Update(); }).detach();


	ZeroMemory(&Render::Message, sizeof(MSG));
	for (; Render::Message.message != WM_QUIT;) {
		Render::HandleMessage();

		if (!updated) {
			Cheat::TargetPawn = NULL;
			Cheat::TargetMesh = NULL;
			Cheat::TargetPawnTeamId = NULL;
			Cheat::ClosestDistance2D = FLT_MAX;
			Cheat::ClosestDistance3D = FLT_MAX;
		}
		updated = FALSE;

		Cheat::Esp();

		//Cheat::MemoryAimbot();
		//Cheat::MouseAimbot();

		Cheat::TriggerBot();


		Render::FovCircle();
		Render::render();
		Render::Menu();

		Render::EndOfFrame();
		//LimitBetterFPS(Settings::Misc::FPSLimit);
	}

	Settings::SaveConfig();
	Render::CloseRender();
}


constexpr std::chrono::milliseconds intervaltrigger(100);
auto start_triggerbot = std::chrono::steady_clock::now();
void Cheat::TriggerBot() {
	if (!Settings::Misc::TriggerBot)
		return;

	if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey) and Settings::Misc::OnlyWhenAimbot)
		return;

	cache::TargetedFortPawn = driver::read<address>(cache::UPlayerController + offset::UTargetedPawn);

	if (!cache::TargetedFortPawn || TargetPawnTeamId == cache::TeamId)
		return;

	if (Settings::CloseRange::TriggerBot) {
		Vector3 pos = driver::read<Vector3>(driver::read<uintptr_t>(cache::TargetedFortPawn + offset::RootComponent) + offset::RelativeLocation);
		float distance = cache::RelativeLocation.Distance(pos) / 100;
		if (distance > Settings::CloseRange::distance)
			return;
	}

	auto end_t = std::chrono::steady_clock::now();
	if (end_t - start_triggerbot < intervaltrigger)
		return;

	leftMouseClick();
	start_triggerbot = end_t;
}




void Cheat::Esp() {

	if (Settings::Visuals::BoneOnSelf) {
		uint64_t Mesh = driver::read<uint64_t>(cache::ULocalPawn + offset::MESH);
		DrawSkeletonOnSelf(Mesh, 1);
	}

	for (int i = 0; i < cache::iPlayerCount; i++) {
		auto Player = driver::read<uintptr_t>(cache::iPlayerArray + i * offset::iPlayerSize);
		auto CurrentPawn = driver::read<uintptr_t>(Player + offset::UPawnPrivate);
		if (!CurrentPawn) continue;
		auto TeamId = driver::read<int>(Player + offset::TEAM_INDEX);
		auto PlayerState = driver::read<uintptr_t>(CurrentPawn + offset::AFortPlayerStateAthena);
		auto IsBot = driver::read<bool>(PlayerState + offset::bIsABot) >> 3 & 1;
		// ALSO UPDATE OFFSET FIRST
		// auto CurrentWeapon = driver::read<uintptr_t>(CurrentActor + 0x9F8);
		if (CurrentPawn == cache::ULocalPawn || NULL == cache::ULocalPawn) continue;

		uint64_t Mesh = driver::read<uint64_t>(CurrentPawn + offset::MESH);
		Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 110);
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
		TextSize.x = (TextSize.x * Settings::Visuals::TextSize) / 2;
		TextSize.y = (TextSize.y * Settings::Visuals::TextSize) / 2;
		std::string dist = "[" + std::to_string(static_cast<int>(distance)) + "m]";


		bool isCloseRange = distance < Settings::CloseRange::distance && Settings::CloseRange::Enabled;

		if (TeamId != cache::TeamId) {




			auto crosshairDist = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);
			if (crosshairDist < Settings::CloseRange::CurrentFov && crosshairDist < ClosestDistance2D) {
				if (isCloseRange && Settings::CloseRange::DynamicFov) {
					float factor = Settings::CloseRange::InstantInterpolation ? 1 : Clamp(((double)Settings::CloseRange::distance / (double)distance), 0.5, 1);
					float TargetFov = Clamp(Settings::Aimbot::Fov + factor * (Settings::CloseRange::MaxFov - Settings::Aimbot::Fov), Settings::Aimbot::Fov, Settings::CloseRange::MaxFov);
					Settings::CloseRange::CurrentFov = Settings::CloseRange::CurrentFov + (Settings::CloseRange::InstantInterpolation ? 1 : 0.1f) * (TargetFov - Settings::CloseRange::CurrentFov);
				}
				else {
					Settings::CloseRange::CurrentFov = Settings::CloseRange::CurrentFov + (Settings::CloseRange::InstantInterpolation ? 1 : 0.1f) * (Settings::Aimbot::Fov - Settings::CloseRange::CurrentFov);
					Settings::CloseRange::CurrentFov = Clamp(Settings::CloseRange::CurrentFov, Settings::Aimbot::Fov, Settings::CloseRange::MaxFov);
				}

				if (!locked) {
					ClosestDistance2D = crosshairDist;
					ClosestDistance3D = distance;
					TargetPawn = CurrentPawn;
					TargetMesh = Mesh;
					TargetPawnTeamId = TeamId;
					updated = TRUE;
				}
			}
		}

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
		if (!Settings::Visuals::Enabled)
			continue;

		if (IsBot) {
			if (Settings::Visuals::BoxOnBot)
				Render::DrawOutlinedCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y - CornerHeight * 0.075f, CornerWidth, CornerHeight + CornerHeight * 0.075f, Settings::Visuals::BotBoxColor, Settings::Visuals::BoxLineThickness);
			if (Settings::Visuals::FillBoxOnBot)
				Render::DrawFilledBox(Head2D.x - (CornerWidth / 2), Head2D.y - CornerHeight * 0.075f, CornerWidth, CornerHeight + CornerHeight * 0.075f, Settings::Visuals::BotBoxFillColor);
			if (Settings::Visuals::TracesOnBot)
				Render::DrawLine(Width / 2, Settings::Visuals::TracesHeight, Head2D.x, TracesConnectHeight, Settings::Visuals::BotTracesColor, Settings::Visuals::TraceLineThickness);
			if (Settings::Visuals::DistanceOnBot)
				Render::DrawOutlinedText((Head2D.x - TextSize.x * 1.8f), (Head2D.y - (CornerHeight * 0.05f) - CornerHeight * 0.075f), TextSize.x, Settings::Visuals::BotBoxColor, distanceString);
			if (Settings::Visuals::BoneOnBot && distance < Settings::Visuals::BoneDisplayRange)
				DrawSkeleton(Mesh, 2);
			if (cache::InLobby)
				DrawSkeleton(Mesh, 0);
			continue;
		}

		if (TeamId == cache::TeamId) {
			if (Settings::Visuals::BoxOnTeam)
				Render::DrawOutlinedCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y - CornerHeight * 0.075f, CornerWidth, CornerHeight + CornerHeight * 0.075f, Settings::Visuals::TeamBoxColor, Settings::Visuals::BoxLineThickness);
			if (Settings::Visuals::FillBoxOnTeam)
				Render::DrawFilledBox(Head2D.x - (CornerWidth / 2), Head2D.y - CornerHeight * 0.075f, CornerWidth, CornerHeight + CornerHeight * 0.075f, Settings::Visuals::TeamBoxFillColor);
			if (Settings::Visuals::TracesOnTeam)
				Render::DrawLine(Width / 2, Settings::Visuals::TracesHeight, Head2D.x, TracesConnectHeight, Settings::Visuals::TeamTracesColor, Settings::Visuals::TraceLineThickness);
			if (Settings::Visuals::DistanceOnTeam)
				Render::DrawOutlinedText((Head2D.x - TextSize.x * 1.8f), (Head2D.y - (CornerHeight * 0.05f) - CornerHeight * 0.075f), TextSize.x, Settings::Visuals::TeamBoxColor, distanceString);
			if (Settings::Visuals::BoneOnTeam && distance < Settings::Visuals::BoneDisplayRange)
				DrawSkeleton(Mesh, 1);
			if (cache::InLobby)
				DrawSkeleton(Mesh, 0);
			continue;
		}

		if (Settings::CloseRange::Enabled) {
			if (Settings::Visuals::Box)
				Render::DrawOutlinedCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y - (CornerHeight * 0.075f), CornerWidth, CornerHeight + (CornerHeight * 0.075f), CloseRangeColor(Settings::Visuals::BoxColor, Settings::CloseRange::BoxColor, distance), Settings::Visuals::BoxLineThickness);
			if (Settings::Visuals::FillBox)
				Render::DrawFilledBox(Head2D.x - (CornerWidth / 2), Head2D.y - (CornerHeight * 0.075f), CornerWidth, CornerHeight + (CornerHeight * 0.075f), Settings::Visuals::BoxFillColor);
			if (Settings::Visuals::Traces)
				Render::DrawLine(Width / 2, Settings::Visuals::TracesHeight, Head2D.x, TracesConnectHeight, CloseRangeColor(Settings::Visuals::TracesColor, Settings::CloseRange::TracesColor, distance), Settings::Visuals::TraceLineThickness);
			if (Settings::Visuals::Distance)
				Render::DrawOutlinedText((Head2D.x - TextSize.x * 1.8f), (Head2D.y - (CornerHeight * 0.05f) - CornerHeight * 0.075f), TextSize.x, CloseRangeColor(Settings::Visuals::BoxColor, Settings::CloseRange::BoxColor, (double)distance), distanceString);
			if (Settings::Visuals::Bone || cache::InLobby)
				DrawSkeleton(Mesh, 0, distance);
		}
		else {
			if (Settings::Visuals::Box)
				Render::DrawOutlinedCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y - (CornerHeight * 0.075f), CornerWidth, CornerHeight + (CornerHeight * 0.075f), Settings::Visuals::BoxColor, Settings::Visuals::BoxLineThickness);
			if (Settings::Visuals::FillBox)
				Render::DrawFilledBox(Head2D.x - (CornerWidth / 2), Head2D.y - (CornerHeight * 0.075f), CornerWidth, CornerHeight + (CornerHeight * 0.075f), Settings::Visuals::BoxFillColor);
			if (Settings::Visuals::Traces)
				Render::DrawLine(Width / 2, Settings::Visuals::TracesHeight, Head2D.x, TracesConnectHeight, Settings::Visuals::TracesColor, Settings::Visuals::TraceLineThickness);
			if (Settings::Visuals::Distance)
				Render::DrawOutlinedText((Head2D.x - TextSize.x * 1.8f), (Head2D.y - (CornerHeight * 0.05f) - CornerHeight * 0.075f), TextSize.x, Settings::Visuals::BoxColor, distanceString);
			if (Settings::Visuals::Bone && distance < Settings::Visuals::BoneDisplayRange || cache::InLobby)
				DrawSkeleton(Mesh, 0);
		}
	}
	//if (TargetPawn == NULL) {
	//	Settings::CloseRange::CurrentFov = Settings::CloseRange::CurrentFov + (Settings::CloseRange::InstantInterpolation ? 1 : 0.1f) * (Settings::Aimbot::Fov - Settings::CloseRange::CurrentFov);
	//}
}

uintptr_t LockedMesh = 0;
uintptr_t LockedPawn = 0;
uintptr_t UCharacterMovementComponent = 0;

void Cheat::MouseAimbotThread() {
	while (true)
	{
		uintptr_t meeesh;
		if (!locked)
		{

			cache::closest_distance = NULL;
			meeesh = NULL;
		}
		for (int i = 0; i < cache::iPlayerCount; i++) {
			auto Player = driver::read<uintptr_t>(cache::iPlayerArray + i * offset::iPlayerSize);
			auto CurrentPawn = driver::read<uintptr_t>(Player + offset::UPawnPrivate);
			if (!CurrentPawn) continue;
			auto TeamId = driver::read<int>(Player + offset::TEAM_INDEX);
			auto PlayerState = driver::read<uintptr_t>(CurrentPawn + offset::AFortPlayerStateAthena);
			auto IsBot = driver::read<bool>(PlayerState + offset::bIsABot) >> 3 & 1;
			// ALSO UPDATE OFFSET FIRST
			// auto CurrentWeapon = driver::read<uintptr_t>(CurrentActor + 0x9F8);
			if (CurrentPawn == cache::ULocalPawn || NULL == cache::ULocalPawn) continue;

			uint64_t Mesh = driver::read<uint64_t>(CurrentPawn + offset::MESH);
			Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 110);
			Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);

			double dx = Head2D.x - Width / 2;
			double dy = Head2D.y - Height / 2;
			float dist = sqrtf(dx * dx + dy * dy);
			//printf("etesting eitntif\n");

			auto crosshairDist = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);

			float distance = cache::RelativeLocation.Distance(Head3D) / 100;
			if (TeamId != cache::TeamId) {
				if (crosshairDist < Settings::Aimbot::Fov && distance < ClosestDistance2D) {
					if (!locked)
					{
						//printf("MEEESH\n");
						ClosestDistance2D = distance;
						meeesh = Mesh;
					}
				}
			}
			//if (dist <= Settings::Aimbot::Fov && dist < cache::closest_distance)
			//{
			//	cache::closest_distance = dist;
			//	meeesh = Mesh;
			//	printf("enterd entitfy\n");
			//}
		}
		if (!Settings::Aimbot::Enabled)
		{
			Sleep(1);
			continue;
		}
		if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey))
		{
			Sleep(1);
			//printf("unlocked\n");
			locked = FALSE;
			LockedMesh = 0;
			continue;
		}
		if (!meeesh)
		{
			continue;
		}
		else
		{
			LockedMesh = meeesh;
			locked = TRUE;
		}

		


		Vector3 head3d = SDK::GetBoneWithRotation(LockedMesh, 110);
		Vector2 head2d = SDK::ProjectWorldToScreen(head3d);
		Vector2 target{};
		if (head2d.x != 0)
		{
			if (head2d.x > Width / 2)
			{
				target.x = -(Width / 2 - head2d.x);
				target.x /= Settings::Aimbot::SmoothX;
				//if (target.x + Width / 2 > Width / 2 * 2) target.x = 0;
			}
			if (head2d.x < Width / 2)
			{
				target.x = head2d.x - Width / 2;
				target.x /= Settings::Aimbot::SmoothX;
				//if (target.x + Width / 2 < 0) target.x = 0;
			}
		}
		if (head2d.y != 0)
		{
			if (head2d.y > Height / 2)
			{
				target.y = -(Height / 2 - head2d.y);
				target.y /= Settings::Aimbot::SmoothY;
				//if (target.y + Height / 2 > Height / 2 * 2) target.y = 0;
			}
			if (head2d.y < Height / 2)
			{
				target.y = head2d.y - Height / 2;
				target.y /= Settings::Aimbot::SmoothY;
				//if (target.y + Height / 2 < 0) target.y = 0;
			}
		}
		input::move_mouse(target.x, target.y);
		Sleep(1);
		//LimitBetterFPS(Settings::Misc::FPSLimit);
	}
}

void Cheat::MouseAimbot() {
	if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey)) {
		locked = FALSE;
		LockedMesh = 0;
		LockedPawn = 0;
		return;
	}
	if (!Settings::Aimbot::Enabled)
		return;
	if (!TargetPawn && !LockedMesh)
		return;

	if (TargetMesh) {
		LockedPawn = TargetPawn;
		LockedMesh = TargetMesh;
		UCharacterMovementComponent = driver::read<uintptr_t>(LockedPawn + 0x320);
	}

	locked = TRUE;

	//std::cout << TargetMesh << std::endl;
	//std::cout << LockedMesh << std::endl;

	uint8_t Bone = 110; // head
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
	Vector3 Velocity = driver::read<Vector3>(UCharacterMovementComponent + 0x348);
	Vector3 Pos3D = SDK::GetBoneWithRotation(LockedMesh, Bone);
	float Distance = cache::RelativeLocation.Distance(Pos3D) / 100;

	// Util::Print3D("", Velocity);
	// 
	// BulletDrop only adjusted for (most) snipers.
	// muss no if statement adden des wieter unten ausgeklammert isch des checkt welche Waffe
	// es isch weil mir sel bis iatz probleme geben hot.
	if (Settings::Aimbot::Predict)
		PredictBulletDrop(Pos3D, Velocity, 30000.f, 0.12f, Distance);

	Vector2 Pos2D = SDK::ProjectWorldToScreen(Pos3D);
	Vector2 target{};

	bool isCloseRange = Settings::CloseRange::Enabled && Distance < Settings::CloseRange::distance && Settings::CloseRange::SmartSmooth;
	float SmoothX = isCloseRange ? Settings::CloseRange::SmoothX : Settings::Aimbot::SmoothX;
	float SmoothY = isCloseRange ? Settings::CloseRange::SmoothY : Settings::Aimbot::SmoothY;

	if (Pos2D.x != 0)
	{
		if (Pos2D.x > Width / 2)
		{
			target.x = -(Width / 2 - Pos2D.x);
			target.x /= SmoothX;
			//if (target.x + Width / 2 > Width / 2 * 2) target.x = 0;
		}
		if (Pos2D.x < Width / 2)
		{
			target.x = Pos2D.x - Width / 2;
			target.x /= SmoothX;
			//if (target.x + Width / 2 < 0) target.x = 0;
		}
	}
	if (Pos2D.y != 0)
	{
		if (Pos2D.y > Height / 2)
		{
			target.y = -(Height / 2 - Pos2D.y);
			target.y /= SmoothY;
			//if (target.y + Height / 2 > Height / 2 * 2) target.y = 0;
		}
		if (Pos2D.y < Height / 2)
		{
			target.y = Pos2D.y - Height / 2;
			target.y /= SmoothY;
			//if (target.y + Height / 2 < 0) target.y = 0;
		}
	}

	input::move_mouse(target.x, target.y);
}

//uintptr_t AFortWeapon = driver::read<uintptr_t>(cache::ULocalPawn + offset::AFortWeapon);
//uintptr_t AFortWeaponData = driver::read<uintptr_t>(AFortWeapon + offset::AFortWeaponData);
////uintptr_t ItemName = driver::read<uintptr_t>(AFortWeaponData + 0x38);
//uint8_t EFortDisplayTier = driver::read<uint8_t>(AFortWeaponData + 0x12b);

//std::cout << "-> AFortWeapon :: " << AFortWeapon  << std::endl;
//std::cout << "-> WeaponData :: " << AFortWeaponData << std::endl;
//std::cout << "-> Tier :: " << EFortDisplayTier << std::endl;
////std::cout << "-> FName :: " << ItemName << std::endl;

//uintptr_t FTextData;
//uint32_t FTextLength;
//uintptr_t FTextPtr;

////if (driver::is_valid(ItemName)) {
//
//	FText FText = { 0 };
//	FTextPtr = driver::read<uintptr_t>(AFortWeaponData + 0x38 + 0x0);
//	FTextData = driver::read<uintptr_t>(FTextPtr + 0x28);
//	FTextLength = driver::read<uint32_t>(FTextPtr + 0x40);
//	
//	//uint64_t FTextData = driver::read<uint64_t>(ItemName);
//	//int FTextLength = driver::read<int>(ItemName + 0x8);
//	//int FTextMax = driver::read<int>(FTextData + 0x8 + 0x4);
//	std::string name;

//	//std::cout << "-> FTextData :: " << FTextData << std::endl;
//	//std::cout << "-> FTextLength :: " << FTextLength << std::endl;
//	//std::cout << "-> FTextMax :: " << FTextMax << std::endl;

//	std::cout << "-> FTextPtr :: " << FTextPtr << std::endl;
//	std::cout << "-> FTextData :: " << FTextData << std::endl;
//	std::cout << "-> FTextLength :: " << FTextLength << std::endl;


//	if (FTextLength > 0 && FTextLength < 50) {
//		wchar_t* ftext_buf = new wchar_t[FTextLength];
//		driver::read(FTextData, ftext_buf, FTextLength * sizeof(wchar_t));
//		std::wstring wstr_buf(ftext_buf);
//		name = std::string(wstr_buf.begin(), wstr_buf.end());

//		std::cout << name << std::endl;
//		delete[] ftext_buf;
//	}
//}



void Cheat::MemoryAimbot() {
	//std::cout << "Reading.. " << driver::read<double>(rotation_pointer) << " " << driver::read<double>(rotation_pointer + 0x20) << " " << driver::read<double>(rotation_pointer + 0x1D0) << std::endl;
	if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey))
		return;
	if (!Settings::Aimbot::Enabled)
		return;
	if (!TargetPawn)
		return;



	uint8_t Bone = 110; // head
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

	Vector3 Pos3D = SDK::GetBoneWithRotation(TargetMesh, Bone);

	if (Settings::Aimbot::Predict)
		PredictBulletDrop(Pos3D, Vector3{}, 1, 1, 1);

	Vector2 Pos2D = SDK::ProjectWorldToScreen(Pos3D);
	Vector2 target{};

	if (Pos2D.x != 0)
	{
		if (Pos2D.x > Width / 2)
		{
			target.x = -(Width / 2 - Pos2D.x);
			target.x /= Settings::Aimbot::SmoothX;
			if (target.x + Width / 2 > Width / 2 * 2) target.x = 0;
		}
		if (Pos2D.x < Width / 2)
		{
			target.x = Pos2D.x - Width / 2;
			target.x /= Settings::Aimbot::SmoothX;
			if (target.x + Width / 2 < 0) target.x = 0;
		}
	}
	if (Pos2D.y != 0)
	{
		if (Pos2D.y > Height / 2)
		{
			target.y = -(Height / 2 - Pos2D.y);
			target.y /= Settings::Aimbot::SmoothY;
			if (target.y + Height / 2 > Height / 2 * 2) target.y = 0;
		}
		if (Pos2D.y < Height / 2)
		{
			target.y = Pos2D.y - Height / 2;
			target.y /= Settings::Aimbot::SmoothY;
			if (target.y + Height / 2 < 0) target.y = 0;
		}
	}

	float Snappiness = 1.0f;
	Vector3 Angles;
	Vector3 CurrentAngles = SDK::GetViewAngles().Angle;
	Vector3 CurrentRotation = SDK::GetViewAngles().Rotation;

	if (CurrentRotation.y < 0)
	{
		CurrentRotation.y = 180.0 + (180.0 - abs(CurrentRotation.y));
	}

	float NewTargetY;
	float NewTargetX;


	NewTargetX = CurrentRotation.y + (target.x / Settings::Aimbot::SmoothX);
	NewTargetY = CurrentRotation.x - (target.y / Settings::Aimbot::SmoothY);
	//else {
	//	NewTargetX = CurrentAngles.y + (target.x / 10);
	//	NewTargetY = CurrentAngles.x - (target.y / 10);
	//}

	//NewTargetY = (1 - Snappiness) * CurrentAngles.x + Snappiness * NewTargetY;
	//NewTargetX = (1 - Snappiness) * CurrentAngles.y + Snappiness * NewTargetX;

	Angles = Vector3{ NewTargetY, NewTargetX, 0 };

	//std::cout << "RotationX: " << CurrentRotation.x << std::endl;
	//std::cout << "RotationY: " << CurrentRotation.y << std::endl;
	/*std::cout << "TargetX: " << NewTargetY << std::endl;
	std::cout << "targetminiX: " << target.y << std::endl;
	std::cout << "head3d: " << Head3D.x << std::endl;
	std::cout << "head2d: " << Head2D.x << std::endl;
	std::cout << "Anglex: " << Angles.y << std::endl;
	std::cout << "Angley: " << Angles.x << std::endl;*/
	//Sleep(50);

	write_angle(Angles.x, Angles.y);
	Sleep(1);
	reset_angles();
	// 
	// 
	//driver::write<double>(rotation_pointer, Angles.x);
	//driver::write<double>(rotation_pointer + 0x20, Angles.y);

	//std::cout << "Writing.. " << driver::read<double>(rotation_pointer) << " " << driver::read<double>(rotation_pointer + 0x20) << " " << driver::read<double>(rotation_pointer + 0x1D0) << std::endl;

	//driver::write<double>(rotation_pointer + 0x1D0, Angles.z);



	//input::move_mouse(target.x, target.y);
}

/*void Cheat::Aimbot() {
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
	//target.x = clamp(target.x, -8, 8);
	//target.y = clamp(target.y, -8, 8);


	input::move_mouse(target.x, target.y);
}*/

//void Cheat::Aimbot() {
//	uintptr_t rotation_pointer = driver::read<uintptr_t>(cache::uWorld + 0x120);
//	std::cout << "Reading.. " << driver::read<double>(rotation_pointer) << " " << driver::read<double>(rotation_pointer + 0x20) << " " << driver::read<double>(rotation_pointer + 0x1D0) << std::endl;
//	if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey))
//		return;
//	//if (!Settings::Aimbot::Enabled)
//	//	return;
//	if (!TargetPawn)
//		return;
//
//
//	uint8_t Bone = 109; // head
//	//switch (Settings::Aimbot::CurrentTargetPart) {
//	//	case 1: { // neck 
//	//		Bone = 67;
//	//	} break;
//	//	case 2: { // hip 
//	//		Bone = 2;
//	//	} break;
//	//	case 3: { // feet 
//	//		Bone = 73;
//	//	} break;
//	//}
//
//	Vector3 Head3D = SDK::GetBoneWithRotation(TargetMesh, Bone);
//	Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);
//	Vector2 target{};
//
//	if (Head2D.x != 0)
//	{
//		if (Head2D.x > Width / 2)
//		{
//			target.x = -(Width / 2 - Head2D.x);
//			target.x /= Settings::Aimbot::SmoothX;
//			if (target.x + Width / 2 > Width / 2 * 2) target.x = 0;
//		}
//		if (Head2D.x < Width / 2)
//		{
//			target.x = Head2D.x - Width / 2;
//			target.x /= 1; Settings::Aimbot::SmoothX;
//			if (target.x + Width / 2 < 0) target.x = 0;
//		}
//	}
//	if (Head2D.y != 0)
//	{
//		if (Head2D.y > Height / 2)
//		{
//			target.y = -(Height / 2 - Head2D.y);
//			target.y /= Settings::Aimbot::SmoothY;
//			if (target.y + Height / 2 > Height / 2 * 2) target.y = 0;
//		}
//		if (Head2D.y < Height / 2)
//		{
//			target.y = Head2D.y - Height / 2;
//			target.y /= Settings::Aimbot::SmoothY;
//			if (target.y + Height / 2 < 0) target.y = 0;
//		}
//	}
//	//target.x = clamp(target.x, -8, 8);
//	//target.y = clamp(target.y, -8, 8);
//
//
//
//	float Snappiness = 0.5f; // 0.221 * tanh(cache::RelativeLocation.Distance(Head3D) - 3750);
//
//	//std::cout << "Mouse: " << target.x << " " << target.y << " : " << heightCorrection << " " << cache::RelativeLocation.Distance(Head3D) << std::endl;
//	//mouse_event(MOUSEEVENTF_MOVE, target.x, target.y+ heightCorrection, NULL, NULL);
//
//	Vector3 Angles;
//	Vector3 CurrentAngles = SDK::GetViewAngles().Angle;
//
//	float NewTargetY;
//	float NewTargetX;
//
//
//	NewTargetX = CurrentAngles.y + (target.x / 5);
//	NewTargetY = CurrentAngles.x - (target.y / 5);
//
//	//else {
//	//	NewTargetX = CurrentAngles.y + (target.x / 10);
//	//	NewTargetY = CurrentAngles.x - (target.y / 10);
//	//}
//
//	NewTargetY = (1 - Snappiness) * CurrentAngles.x + Snappiness * NewTargetY;
//	NewTargetX = (1 - Snappiness) * CurrentAngles.y + Snappiness * NewTargetX;
//
//	Angles = Vector3{ NewTargetY, NewTargetX, 0 };
//
//	//uintptr_t rotation_pointer = driver::read<uintptr_t>(cache::uWorld + 0x120);
//
//	driver::write<double>(rotation_pointer, Angles.x);
//	driver::write<double>(rotation_pointer + 0x20, Angles.y);
//
//	std::cout << "Writing.. " << driver::read<double>(rotation_pointer) << " " << driver::read<double>(rotation_pointer + 0x20) << " " << driver::read<double>(rotation_pointer + 0x1D0) << std::endl;
//
//	//driver::write<double>(rotation_pointer + 0x1D0, Angles.z);
//
//
//
//	//input::move_mouse(target.x, target.y);
//}

void Cheat::Update() {
	for (;;) {
		cache::Camera = SDK::GetViewAngles();
		if (cache::ULocalPawn) {
			cache::RelativeLocation = driver::read<Vector3>(cache::RootComponent + offset::RelativeLocation);
		}
	}
}

void Cheat::LateUpdate() {
	for (;;) {
		cache::UWorld = driver::read<address>((BaseId + offset::UWorld));
		cache::AGameStateBase = driver::read<uintptr_t>(cache::UWorld + offset::AGameStateBase);
		cache::UGameInstance = driver::read<uintptr_t>(cache::UWorld + offset::UGameInstance);
		cache::ULocalPlayers = driver::read<uintptr_t>(driver::read<uintptr_t>(cache::UGameInstance + offset::ULocalPlayers));
		cache::UPlayerController = driver::read<uintptr_t>(cache::ULocalPlayers + offset::APlayerController);
		cache::iPlayerArray = driver::read<address>(cache::AGameStateBase + offset::iPlayerArray);
		cache::PersistentLevel = driver::read<uintptr_t>(cache::UWorld + offset::PersistentLevel);
		cache::AWorldSettings = driver::read<uintptr_t>(cache::PersistentLevel + offset::AWorldSettings);
		cache::WorldGravityZ = driver::read<float>(cache::WorldGravityZ + offset::WorldGravityZ);
		cache::InLobby = (cache::iPlayerCount == 1 && !cache::ULocalPawn) ? true : false;
		cache::iPlayerCount = driver::read<int>(cache::AGameStateBase + (offset::iPlayerArray + sizeof(uintptr_t)));

		cache::ULocalPawn = driver::read<uintptr_t>(cache::UPlayerController + offset::ULocalPawn);
		if (cache::ULocalPawn) {
			cache::RootComponent = driver::read<uintptr_t>(cache::ULocalPawn + offset::RootComponent);
			cache::PlayerState = driver::read<uintptr_t>(cache::ULocalPawn + offset::AFortPlayerStateAthena);
			cache::TeamId = driver::read<int>(cache::PlayerState + offset::TEAM_INDEX);
		}
		Sleep(1000);
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

//Vector3 BoneRotations[] = {
//	SDK::GetBoneWithRotation(Comp, BoneA, 109),		// HeadBone
//	SDK::GetBoneWithRotation(Comp, BoneA, 2),		// Hip
//	SDK::GetBoneWithRotation(Comp, BoneA, 67),		// Neck
//	SDK::GetBoneWithRotation(Comp, BoneA, 9),		// UpperArmLeft
//	SDK::GetBoneWithRotation(Comp, BoneA, 38),		// UpperArmRight
//	SDK::GetBoneWithRotation(Comp, BoneA, 10),		// LeftHand
//	SDK::GetBoneWithRotation(Comp, BoneA, 39),		// RightHand
//	SDK::GetBoneWithRotation(Comp, BoneA, 11),		// LeftHand1
//	SDK::GetBoneWithRotation(Comp, BoneA, 40),		// RightHand1
//	SDK::GetBoneWithRotation(Comp, BoneA, 78),		// RightThigh
//	SDK::GetBoneWithRotation(Comp, BoneA, 71),		// LeftThigh
//	SDK::GetBoneWithRotation(Comp, BoneA, 79),		// RightCalf
//	SDK::GetBoneWithRotation(Comp, BoneA, 72),		// LeftCalf
//	SDK::GetBoneWithRotation(Comp, BoneA, 73),		// LeftFoot
//	SDK::GetBoneWithRotation(Comp, BoneA, 80)		// RightFoot
//};


static Vector2 pBonePositions[14];
static Vector2 ppBonePositions[14];
void DrawSkeletonOnSelf(uint64_t Mesh, BYTE PawnType) {
	uintptr_t Bone = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY);
	if (Bone == NULL)
	{
		Bone = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY + 0x10);
	}
	FTransform Comp = driver::read<FTransform>(Mesh + offset::COMPONENT_TO_WORLD);


	Vector3 BoneRotations[] = {
		SDK::GetBoneWithRotation(Comp, Bone, 110),		// HeadBone
		SDK::GetBoneWithRotation(Comp, Bone, 2),		// Hip
		SDK::GetBoneWithRotation(Comp, Bone, 67),		// Neck
		SDK::GetBoneWithRotation(Comp, Bone, 9),		// UpperArmLeft
		SDK::GetBoneWithRotation(Comp, Bone, 38),		// UpperArmRight
		SDK::GetBoneWithRotation(Comp, Bone, 10),		// LeftHand
		SDK::GetBoneWithRotation(Comp, Bone, 39),		// RightHand
		SDK::GetBoneWithRotation(Comp, Bone, 11),		// LeftHand1
		SDK::GetBoneWithRotation(Comp, Bone, 40),		// RightHand1
		SDK::GetBoneWithRotation(Comp, Bone, 78),		// RightThigh
		SDK::GetBoneWithRotation(Comp, Bone, 71),		// LeftThigh
		SDK::GetBoneWithRotation(Comp, Bone, 79),		// RightCalf
		SDK::GetBoneWithRotation(Comp, Bone, 72),		// LeftCalf
		SDK::GetBoneWithRotation(Comp, Bone, 73),		// LeftFoot
		SDK::GetBoneWithRotation(Comp, Bone, 80)		// RightFoot
	};



	Vector2 BonePositions[14];
	for (int i = 0; i < 14+1; i++) {
		Vector2 LocalPawnBonePosition = SDK::ProjectWorldToScreen(BoneRotations[i]);
		BonePositions[i] = (LocalPawnBonePosition + pBonePositions[i] + ppBonePositions[i]) * 0.33333333333333f;
		pBonePositions[i] = LocalPawnBonePosition;
		ppBonePositions[i] = pBonePositions[i];
	}

	const ImColor BoneColor = Settings::Visuals::TeamBoneColor;

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

void DrawSkeleton(uint64_t Mesh, BYTE PawnType) {
	/*uintptr_t BoneA = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY);
	if (BoneA == NULL)
	{
		BoneA = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY + 0x10);
	}*/

	uint64_t BoneA = driver::read<uint64_t>(Mesh + 0x5B0 + (driver::read<int>(Mesh + 0x5F8) * 0x10)); // updated
	FTransform Comp = driver::read<FTransform>(Mesh + offset::COMPONENT_TO_WORLD);

	//typedef struct _BoneArray {
	//	FTransform Bone;
	//	char padding[16];
	//} BoneArray;

	//BoneArray boneArray01[9] = { 0 };
	//driver::read(BoneA + (2 * offset::bonec), &boneArray01, sizeof(boneArray01));

	//BoneArray boneArray02[42] = { 0 };
	//driver::read(BoneA + (38 * offset::bonec), &boneArray02, sizeof(boneArray02));


	/*Vector3 BoneRotations[] = {
		SDK::GetBoneWithRotation(Comp, BoneA, 110),		// HeadBone
		SDK::GetBoneWithRotation(Comp, BoneA, 2),		// Hip
		SDK::GetBoneWithRotation(Comp, BoneA, 67),		// Neck
		SDK::GetBoneWithRotation(Comp, BoneA, 9),		// UpperArmLeft
		SDK::GetBoneWithRotation(Comp, BoneA, 38),		// UpperArmRight
		SDK::GetBoneWithRotation(Comp, BoneA, 10),		// LeftHand
		SDK::GetBoneWithRotation(Comp, BoneA, 39),		// RightHand
		SDK::GetBoneWithRotation(Comp, BoneA, 11),		// LeftHand1
		SDK::GetBoneWithRotation(Comp, BoneA, 40),		// RightHand1
		SDK::GetBoneWithRotation(Comp, BoneA, 78),		// RightThigh
		SDK::GetBoneWithRotation(Comp, BoneA, 71),		// LeftThigh
		SDK::GetBoneWithRotation(Comp, BoneA, 79),		// RightCalf
		SDK::GetBoneWithRotation(Comp, BoneA, 72),		// LeftCalf
		SDK::GetBoneWithRotation(Comp, BoneA, 73),		// LeftFoot
		SDK::GetBoneWithRotation(Comp, BoneA, 80)		// RightFoot
	};*/

	Vector3 BoneRotations[] = {
		SDK::GetBoneWithRotation(Mesh, 110),		// HeadBone
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
	for (int i = 0; i < 14+1; ++i) {
		BonePositions[i] = SDK::ProjectWorldToScreen(BoneRotations[i]);
	}

	ImColor BoneColor = PawnType > 0 ? (PawnType == 1 ? Settings::Visuals::TeamBoneColor : Settings::Visuals::BotBoneColor) : Settings::Visuals::BoneColor;

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


void DrawSkeleton(uint64_t Mesh, BYTE PawnType, float Distance) {
	uintptr_t BoneA = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY);
	if (BoneA == NULL)
	{
		BoneA = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY + 0x10);
	}
	FTransform Comp = driver::read<FTransform>(Mesh + offset::COMPONENT_TO_WORLD);

	//typedef struct _BoneArray {
	//	FTransform Bone;
	//	char padding[16];
	//} BoneArray;

	//BoneArray boneArray01[9] = { 0 };
	//driver::read(BoneA + (2 * offset::bonec), &boneArray01, sizeof(boneArray01));

	//BoneArray boneArray02[42] = { 0 };
	//driver::read(BoneA + (38 * offset::bonec), &boneArray02, sizeof(boneArray02));

	Vector3 BoneRotations[] = {
		SDK::GetBoneWithRotation(Comp, BoneA, 110),		// HeadBone
		SDK::GetBoneWithRotation(Comp, BoneA, 2),		// Hip
		SDK::GetBoneWithRotation(Comp, BoneA, 67),		// Neck
		SDK::GetBoneWithRotation(Comp, BoneA, 9),		// UpperArmLeft
		SDK::GetBoneWithRotation(Comp, BoneA, 38),		// UpperArmRight
		SDK::GetBoneWithRotation(Comp, BoneA, 10),		// LeftHand
		SDK::GetBoneWithRotation(Comp, BoneA, 39),		// RightHand
		SDK::GetBoneWithRotation(Comp, BoneA, 11),		// LeftHand1
		SDK::GetBoneWithRotation(Comp, BoneA, 40),		// RightHand1
		SDK::GetBoneWithRotation(Comp, BoneA, 78),		// RightThigh
		SDK::GetBoneWithRotation(Comp, BoneA, 71),		// LeftThigh
		SDK::GetBoneWithRotation(Comp, BoneA, 79),		// RightCalf
		SDK::GetBoneWithRotation(Comp, BoneA, 72),		// LeftCalf
		SDK::GetBoneWithRotation(Comp, BoneA, 73),		// LeftFoot
		SDK::GetBoneWithRotation(Comp, BoneA, 80)		// RightFoot
	};


	Vector2 BonePositions[16];
	for (int i = 0; i < 14+1; ++i) {
		BonePositions[i] = SDK::ProjectWorldToScreen(BoneRotations[i]);
	}

	ImColor BoneColor = PawnType > 0 ? (PawnType == 1 ? Settings::Visuals::TeamBoneColor : Settings::Visuals::BotBoneColor)
		: Render::FadeColor(Settings::Visuals::BoneColor, Settings::CloseRange::BoneColor, (double)Settings::CloseRange::distance / (double)Distance);

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
