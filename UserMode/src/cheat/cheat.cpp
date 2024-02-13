#include <iomanip>
#include "cheat.h"
#include "sdk/sdk.h"
#include "data/offsets.h"
#include "driver/driver.h"
#include "../util/util.h"
#include "../render/render.h"

uintptr_t Cheat::TargetPawn = 0;
float Cheat::ClosestDistance = FLT_MAX;
int Cheat::FovSize = 200;
float Cheat::Smooth = 1;
bool Cheat::bAimbot = true;
bool Cheat::bIsTargeting = false;
std::vector<Vector3> Cheat::ActorArray{};

void Cheat::Init() {
	cache::uWorld				= driver::read<address>((BaseId + offset::UWORLD));
	cache::GameState			= driver::read<uintptr_t>(cache::uWorld + offset::GAME_STATE);
	cache::GameInstance			= driver::read<uintptr_t>(cache::uWorld + offset::GAME_INSTANCE);
	cache::LocalPlayers			= driver::read<uintptr_t>(driver::read<uintptr_t>(cache::GameInstance + offset::LOCAL_PLAYERS));
	cache::PlayerController		= driver::read<uintptr_t>(cache::LocalPlayers + offset::PLAYER_CONTROLLER);
	cache::LocalPawn			= driver::read<uintptr_t>(cache::PlayerController + offset::LOCAL_PAWN);
	cache::PlayerArray = driver::read<uintptr_t>(cache::GameState + offset::PLAYER_ARRAY);
	cache::PlayerCount = driver::read<int>(cache::GameState + (offset::PLAYER_ARRAY + sizeof(uintptr_t)));
	std::cout << "-> game_state :: " << cache::GameState << std::endl;
	std::cout << "-> uworld :: " << cache::uWorld << std::endl;
	std::cout << "-> game_instance :: " << cache::GameInstance << std::endl;
	std::cout << "-> local_players :: " << cache::LocalPlayers << std::endl;
	std::cout << "-> player_controller :: " << cache::PlayerController << std::endl;
	std::cout << "-> local_pawn :: " << cache::LocalPawn << std::endl;
	std::cout << "-> player_count :: " << cache::PlayerCount << std::endl;

	if (cache::LocalPawn != 0)
	{
		cache::RootComponent	=	driver::read<uintptr_t>(cache::LocalPawn + offset::ROOT_COMPONENT);
		cache::PlayerState		=	driver::read<uintptr_t>(cache::LocalPawn + offset::PLAYER_STATE);
		cache::TeamId			=	driver::read<int>(cache::PlayerState + offset::TEAM_INDEX);
		cache::RelativeLocation =	driver::read<Vector3>(cache::RootComponent + offset::RELATIVE_LOCATION);
		std::cout << "-> root_component :: " << cache::RootComponent << std::endl;
		std::cout << "-> player_state :: " << cache::PlayerState << std::endl;
		std::cout << "-> my_team_id :: " << cache::TeamId << std::endl;
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
		Util::Print3D("[+] ", pos);
	}
}

void Cheat::Present() {
	ZeroMemory(&Render::Message, sizeof(MSG));
	for (;Render::Message.message != WM_QUIT;) {
		Render::HandleMessage();

		Cheat::ActorArray.clear();
		cache::uWorld = driver::read<address>((BaseId + offset::UWORLD));
		cache::GameState = driver::read<uintptr_t>(cache::uWorld + offset::GAME_STATE);
		cache::GameInstance = driver::read<uintptr_t>(cache::uWorld + offset::GAME_INSTANCE);
		cache::LocalPlayers = driver::read<uintptr_t>(driver::read<uintptr_t>(cache::GameInstance + offset::LOCAL_PLAYERS));
		cache::PlayerController = driver::read<uintptr_t>(cache::LocalPlayers + offset::PLAYER_CONTROLLER);
		cache::LocalPawn = driver::read<uintptr_t>(cache::PlayerController + offset::LOCAL_PAWN);

		cache::PlayerArray = driver::read<address>(cache::GameState + offset::PLAYER_ARRAY);
		cache::PlayerCount = driver::read<int>(cache::GameState + (offset::PLAYER_ARRAY + sizeof(uintptr_t)));

		if (cache::LocalPawn != 0)
		{
			cache::RootComponent = driver::read<uintptr_t>(cache::LocalPawn + offset::ROOT_COMPONENT);
			cache::PlayerState = driver::read<uintptr_t>(cache::LocalPawn + offset::PLAYER_STATE);
			cache::TeamId = driver::read<int>(cache::PlayerState + offset::TEAM_INDEX);
			cache::RelativeLocation = driver::read<Vector3>(cache::RootComponent + offset::RELATIVE_LOCATION);
		}

		Cheat::ESP();

		Render::FovCircle();
		if (GetAsyncKeyState(VK_RBUTTON)) {
			Cheat::Aimbot();
		}


		//do shit
		Render::render();
		//Render::Menu();

		Render::EndOfFrame();
	}


	Render::CloseRender();
}





void Cheat::ESP() {
	for (int i = 0; i < cache::PlayerCount; i++) {
		auto Player = driver::read<uintptr_t>(cache::PlayerArray + i * offset::PLAYERSIZE);
		auto CurrentActor = driver::read<uintptr_t>(Player + offset::PAWNPRIV);
		auto TeamId = driver::read<int>(Player + offset::TEAM_INDEX);
		auto CurrentWeapon = driver::read<uintptr_t>(CurrentActor + 0x9F8);
		
		if (CurrentActor == cache::LocalPawn)
			continue;

		uint64_t Mesh = driver::read<uint64_t>(CurrentActor + offset::MESH);
		uintptr_t BoneA = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY);
		if (BoneA == NULL)
			BoneA = driver::read<uintptr_t>(Mesh + offset::BONE_ARRAY + 0x10);
		FTransform Bone = driver::read<FTransform>(BoneA + (109 * offset::bonec));
		FTransform Comp = driver::read<FTransform>(Mesh + offset::COMPONENT_TO_WORLD);
		D3DMATRIX HeadMatrix = MatrixMultiplication(Bone.ToMatrixWithScale(), Comp.ToMatrixWithScale());


		Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 109);// Vector3(HeadMatrix._41, HeadMatrix._42, HeadMatrix._43);
		//ector2 HeadBox2D = SDK::ProjectWorldToScreen(Vector3(Head3D.x, Head3D.y, Head3D.z + 15));
		Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);

		Vector3 Bottom3D = SDK::GetBoneWithRotation(Mesh, 0);
		Vector2 Bottom2D = SDK::ProjectWorldToScreen(Bottom3D);


		float BoxHeight = (float)(Head2D.y - Bottom2D.y);
		float CornerHeight = abs(Head2D.y - Bottom2D.y);
		float CornerWidth = BoxHeight * 0.8f;

		float distance = cache::RelativeLocation.Distance(Head3D) / 80;
		
		{
			Util::DrawCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y, CornerWidth, CornerHeight, ImColor(255, 0, 255), 3);
			Util::DrawLine(Width/2, Height, Head2D.x, Head2D.y, ImColor(255, 0, 0), 1);
		}

		auto dist = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);
		if (dist < FovSize && dist < ClosestDistance) {
			ClosestDistance = dist;
			TargetPawn = Player;
		}
	}
}

void Cheat::Aimbot() {
	if (!TargetPawn) return;

	auto mesh = read<uintptr_t>(TargetPawn + offset::MESH); //https://fn.dumps.host/?class=ACharacter&member=Mesh
	if (!mesh) {
		ClosestDistance = FLT_MAX;
		TargetPawn = NULL;
		bIsTargeting = FALSE;
	}
	Vector3 Head3D = SDK::GetBoneWithRotation(mesh, 109);
	Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);


	auto distance = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);
	std::cout << distance << std::endl;


	if (distance > FovSize || Head2D.x == 0 || Head2D.y == 0) {
		ClosestDistance = FLT_MAX;
		TargetPawn = NULL;
		bIsTargeting = FALSE;
		return;
	}

	float x = Head2D.x; float y = Head2D.y;
	float AimSpeed = Smooth;

	Vector2 ScreenCenter = { (double)Width / 2 , (double)Height / 2 };
	Vector2 Target;

	if (x != 0)
	{
		if (x > ScreenCenter.x)
		{
			Target.x = -(ScreenCenter.x - x);
			Target.x /= AimSpeed;
			if (Target.x + ScreenCenter.x > ScreenCenter.x * 2) Target.x = 0;
		}

		if (x < ScreenCenter.x)
		{
			Target.x = x - ScreenCenter.x;
			Target.x /= AimSpeed;
			if (Target.x + ScreenCenter.x < 0) Target.x = 0;
		}
	}
	if (y != 0)
	{
		if (y > ScreenCenter.y)
		{
			Target.y = -(ScreenCenter.y - y);
			Target.y /= AimSpeed;
			if (Target.y + ScreenCenter.y > ScreenCenter.y * 2) Target.y = 0;
		}

		if (y < ScreenCenter.y)
		{
			Target.y = y - ScreenCenter.y;
			Target.y /= AimSpeed;
			if (Target.y + ScreenCenter.y < 0) Target.y = 0;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, Target.x, Target.y, NULL, NULL);


}