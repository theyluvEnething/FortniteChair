#include <mutex>
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

#include <Xinput.h>
#include <iostream>

#pragma comment(lib, "Xinput.lib") // Link with XInput library

long long framecount;
#define clamp(x, minVal, maxVal) min(max(x, minVal), maxVal)
uintptr_t Cheat::TargetPawn = 0;
uint64_t Cheat::TargetMesh = 0;
float Cheat::ClosestDistance2D = FLT_MAX;
float Cheat::ClosestDistance3D = FLT_MAX;
char Cheat::TargetPawnTeamId = 0;
bool Cheat::locked = FALSE;
bool Cheat::IsCloseRange = FALSE;
bool updated = FALSE;

class item {
public:
	uintptr_t
		Actor;

	std::string
		Name,
		isVehicle,
		isChest,
		isPickup,
		isAmmoBox;
	float
		distance;
};
std::vector<item> item_pawns;


XINPUT_STATE state;
DWORD dwResult;

inline std::mutex mutex;

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
	//std::cout << "UWORLD OFFSET :: " << offset::UWorld << std::endl;
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


	//std::cout << skCrypt("-> game_state :: ") << cache::AGameStateBase << std::endl;
	//std::cout << skCrypt("-> uworld :: ") << cache::UWorld << std::endl;
	//std::cout << skCrypt("-> game_instance :: ") << cache::UGameInstance << std::endl;
	//std::cout << skCrypt("-> local_players :: ") << cache::ULocalPlayers << std::endl;
	//std::cout << skCrypt("-> player_controller :: ") << cache::UPlayerController << std::endl;
	//std::cout << skCrypt("-> local_pawn :: ") << cache::ULocalPawn << std::endl;
	//std::cout << skCrypt("-> player_count :: ") << cache::iPlayerCount << std::endl;
	//std::cout << skCrypt("-> in-lobby :: ") << cache::InLobby << std::endl;
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
static void CacheLevels();


uintptr_t RootComponent(uintptr_t actor)
{
	return driver::read<uintptr_t>(actor + offset::RootComponent);
}

char* wchar_to_char(const wchar_t* pwchar)
{
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}

void LevelRender()
{

	auto levelListCopy = item_pawns;

	for (auto entity : levelListCopy)
	{
		//printf("here1");
		if (cache::ULocalPawn && entity.Actor)
		{

			if (Settings::Visuals::vehicleEsp && strstr(entity.Name.c_str(), skCrypt("Vehicle_Hoverboard_C").decrypt()))
			{

				Vector3 Location = driver::read<Vector3>(RootComponent(entity.Actor) + offset::RelativeLocation);
				Vector2 PickupPosition = SDK::ProjectWorldToScreen(Location);
				std::string wtf2 = "Hoverboard";
				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
				float text_x = PickupPosition.x - (text_size.x / 2);
				Render::DrawOutlinedText(text_x, PickupPosition.y - 18, 27, ImColor(255,255,255,255), wtf2.c_str());
			}
			if (Settings::Visuals::vehicleEsp && strstr(entity.Name.c_str(), skCrypt("Valet_BasicSUV_Vehicle_C").decrypt()))
			{

				Vector3 Location = driver::read<Vector3>(RootComponent(entity.Actor) + offset::RelativeLocation);
				Vector2 PickupPosition = SDK::ProjectWorldToScreen(Location);
				std::string wtf2 = "SUV Car";
				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
				float text_x = PickupPosition.x - (text_size.x / 2);
				Render::DrawOutlinedText(text_x, PickupPosition.y - 18, 27, ImColor(255, 255, 255, 255), wtf2.c_str());
			}
			if (Settings::Visuals::vehicleEsp && strstr(entity.Name.c_str(), skCrypt("Valet_SportsCar_Vehicle_C").decrypt()))
			{

				Vector3 Location = driver::read<Vector3>(RootComponent(entity.Actor) + offset::RelativeLocation);
				Vector2 PickupPosition = SDK::ProjectWorldToScreen(Location);
				std::string wtf2 = "Lamborgini";
				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
				float text_x = PickupPosition.x - (text_size.x / 2);
				Render::DrawOutlinedText(text_x, PickupPosition.y - 18, 27, ImColor(255, 255, 255, 255), wtf2.c_str());
			}
			if (Settings::Visuals::vehicleEsp && strstr(entity.Name.c_str(), skCrypt("Motorcycle_DirtBike_Vehicle_C").decrypt()))
			{

				Vector3 Location = driver::read<Vector3>(RootComponent(entity.Actor) + offset::RelativeLocation);
				Vector2 PickupPosition = SDK::ProjectWorldToScreen(Location);
				std::string wtf2 = "Dirtbike";
				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
				float text_x = PickupPosition.x - (text_size.x / 2);
				Render::DrawOutlinedText(text_x, PickupPosition.y - 18, 27, ImColor(255, 255, 255, 255), wtf2.c_str());
			}
			//printf("here1");
			if (Settings::Visuals::lootEsp && strstr(entity.Name.c_str(), skCrypt("FortPickupAthena").decrypt()) || strstr(entity.Name.c_str(), skCrypt("Fort_Pickup_Creative_C").decrypt()))
			{

				if (entity.distance <= Settings::Visuals::lootDist)
				{
					auto definition = driver::read<uint64_t>(entity.Actor + 0x350 + 0x18);
					if (driver::is_valid(definition))
					{
						BYTE tier = driver::read<BYTE>(definition + 0x13c);

						ImColor Color;
						Vector3 Location = driver::read<Vector3>(RootComponent(entity.Actor) + offset::RelativeLocation);

						Vector2 PickupPosition = SDK::ProjectWorldToScreen(Location);


						uint64_t ftext_ptr = driver::read<uint64_t>(definition + 0x40);

						if (driver::is_valid(ftext_ptr)) {
							uint64_t ftext_data = driver::read<uint64_t>(ftext_ptr + 0x28);
							int ftext_length = driver::read<int>(ftext_ptr + 0x30);
							//printf("length: %i\n", ftext_length);
							if (ftext_length > 0 && ftext_length < 50) {
								wchar_t* ftext_buf = new wchar_t[ftext_length];
								driver::read((uintptr_t)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
								wchar_t* WeaponName = ftext_buf;
								delete[] ftext_buf;

								std::string Text = wchar_to_char(WeaponName);

								//std::cout << "weaponname:" << WeaponName << std::endl;

								/*if (!ud::show_floor_ammo && Text.find("Ammo") != std::string::npos) {
									continue;
								}

								if (!ud::show_floor_materials && Text.find("Wood") != std::string::npos) {
									continue;
								}

								if (!ud::show_floor_materials && Text.find("Stone") != std::string::npos) {
									continue;
								}

								if (!ud::show_floor_materials && Text.find("Metal") != std::string::npos) {
									continue;
								}

								if (!ud::show_floor_gold && Text.find("Bars") != std::string::npos) {
									continue;
								}*/


								std::string wtf2 = Text + " [" + std::to_string((int)entity.distance) + ("m]");

								if (tier == 1)
								{
									Color = ImColor(0, 204, 34, 255);
								}
								else if ((tier == 2))
								{
									Color = ImColor(0, 112, 221, 255);
								}
								else if ((tier == 3))
								{
									Color = ImColor(163, 53, 238, 255);
								}
								else if ((tier == 4))
								{
									Color = ImColor(255, 128, 0, 255);
								}
								else if ((tier == 5) || (tier == 6))
								{
									Color = ImColor(254, 202, 33, 255);
								}

								else if ((tier == 0))
								{
									Color = ImColor(170, 165, 169, 255);
								}
								else if (tier != 0 || tier != 1, tier != 02, tier != 3, tier != 4, tier != 5, tier != 6)
								{
									Color = ImColor(255, 255, 255, 255);
								}
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								//ImVec2 text_size = ImVec2(12, 12);
								float text_x = PickupPosition.x - (text_size.x / 2);

								Render::DrawOutlinedText(text_x, PickupPosition.y - 18, 18, Color, wtf2.c_str());
								/*framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(wtf2.c_str());

								if (ud::u_textoutline)
								{
									m_framework->text(
										wtf2.c_str(),
										PickupPosition.x - (text_size.x / 2),
										PickupPosition.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, PickupPosition.x - (text_size.x / 2), PickupPosition.y - 18, Color, false, true, wtf2.c_str());
								}
								else
								{
									m_framework->text(
										wtf2.c_str(),
										PickupPosition.x - (text_size.x / 2),
										PickupPosition.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, PickupPosition.x - (text_size.x / 2), PickupPosition.y - 18, Color, false, false, wtf2.c_str());
								}*/

							}
						}
					}
				}
			}
		}
	}
}


void Cheat::Present() {

	std::thread([&]() { Cheat::LateUpdate(); }).detach();
	std::thread([&]() { Cheat::Update(); }).detach();
	std::thread([&]() { Cheat::MouseAimbotThread(); }).detach();
	std::thread([&]() { CacheLevels(); }).detach();


	ZeroMemory(&Render::Message, sizeof(MSG));
	for (; Render::Message.message != WM_QUIT;) {
		Render::HandleMessage();

		Settings::CloseRange::DynamicFov = FALSE;
		Cheat::IsCloseRange = FALSE;

		if (!updated) {
			Cheat::TargetPawn = NULL;
			Cheat::TargetMesh = NULL;
			Cheat::ClosestDistance2D = FLT_MAX;
			Cheat::ClosestDistance3D = FLT_MAX;
			//Cheat::TargetPawnTeamId = NULL;
		}
		updated = FALSE;

		Cheat::Esp();

		//CacheLevels();
		LevelRender();

		//Cheat::MemoryAimbot();
		//Cheat::MouseAimbot();

		Cheat::TriggerBot();

		if (Settings::Visuals::showWatermark)
		{
			Render::DrawOutlinedText(Width / 2 - 150, 50, 24, Settings::Misc::MenuColor, "discord.gg/renewable");
		}


		Render::FovCircle();
		Render::render();
		Render::Menu();

		Render::EndOfFrame();
		//LimitBetterFPS(Settings::Misc::FPSLimit);
	}

	Settings::SaveConfig();
	Render::CloseRender();
}


#define FNAMEPOOL_OFFSET  0x12107500 //0x1198BD00´// 0x1200051C // 0x12107500

static std::string GetNameFromIndex(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;
	uint64_t NamePoolChunk = driver::read<uint64_t>(BaseId + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 0x10) + (unsigned int)(2 * NameOffset);
	uint16_t nameEntry = driver::read<uint16_t>(NamePoolChunk);
	int nameLength = nameEntry >> 6;
	char buff[1024] = {};

	unsigned int v4 = nameLength; // ebx
	char* v2 = buff; // r8
	unsigned int v5; // eax
	int v6; // edx
	unsigned int v7; // r8d
	__int64 v8; // rax


	v5 = driver::read<unsigned int>((BaseId + 0x1200051C));
	v6 = (v5 << 8) | (v5 >> 8);
	v7 = v5 >> 7;
	if (v4)
	{
		driver::read((uintptr_t)(NamePoolChunk + 2), (PVOID)buff, (int)2 * nameLength);
		v8 = v4;
		do
		{
			v6 += v7;
			*v2++ ^= v6;
			--v8;
		} while (v8);
		//buff = v2;
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	return std::string("");


	/*char* v2; // rdi
	int v4; // ebx
	__int16 result; // ax
	int v6; // edx
	int v7; // ecx
	int v8; // eax
	unsigned int v9; // ecx

	v2 = buff;
	v4 = nameLength;
	v6 = 0;
	v7 = 24;
	if (v4)
	{
		driver::read((uintptr_t)(NamePoolChunk + 2), (PVOID)buff, (int)2 * nameLength);
		do
		{
			v8 = v6++;
			v9 = (v8 | 0xB000) + v7;
			result = v9 ^ ~*v2;
			v7 = v9 >> 2;
			*v2++ = result;
		} while (v6 < v4);
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	return std::string("");*/
}

static std::string GetNameFromFName(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = driver::read<uint64_t>(BaseId + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
	if (driver::read<uint16_t>(NamePoolChunk) < 64)
	{
		auto a1 = driver::read<DWORD>(NamePoolChunk + 2);
		return GetNameFromIndex(a1);
	}
	else
	{
		return GetNameFromIndex(key);
	}
}

static auto CacheLevels() -> void {

	while (true)
	{

		if (cache::ULocalPawn)
		{
			//if (!ud::draw_chests && !ud::lamma && !ud::pickups && !ud::draw_vehicles) continue;
			if (!cache::UWorld) continue;
			std::vector<item> mrxd;
			uintptr_t ItemLevels = driver::read<uintptr_t>(cache::UWorld + 0x178); // 0x170

			for (int i = 0; i < driver::read<DWORD>(cache::UWorld + (0x178 + sizeof(PVOID))); ++i) {

				uintptr_t ItemLevel = driver::read<uintptr_t>(ItemLevels + (i * sizeof(uintptr_t)));

				for (int i = 0; i < driver::read<DWORD>(ItemLevel + (0xA0 + sizeof(PVOID))); ++i) {

					uintptr_t ItemsPawns = driver::read<uintptr_t>(ItemLevel + 0xA0);

					uintptr_t CurrentItemPawn = driver::read<uintptr_t>(ItemsPawns + (i * sizeof(uintptr_t))); // CurrentActor

					Vector3 ItemPosition = driver::read<Vector3>(RootComponent(CurrentItemPawn) + offset::RelativeLocation);
					float ItemDist = cache::RelativeLocation.Distance(ItemPosition) / 100.f;

					int ItemIndex = driver::read<int>(CurrentItemPawn + 0x18);

					auto CurrentItemPawnName = GetNameFromFName(ItemIndex);



					//if (ud::dev2)
					//{
					//std::cout << CurrentItemPawnName << std::endl;
					//std::cout << ("fname -> ") << FNAMEPOOL_OFFSET << std::endl;
					//}

					bool shouldUpdate;
					bool bIsPickup;

					if (driver::is_valid(CurrentItemPawn))
					{
							bIsPickup = true;
							shouldUpdate = true;
					}

					if (shouldUpdate)
					{
						//Vector2 onScreenLoc = SDK::ProjectWorldToScreen(ItemPosition);
						//Render::DrawOutlinedText(onScreenLoc.x, onScreenLoc.y, 25, ImColor(1, 1, 1, 1), "Item");
						//Render::DrawOutlinedText(onScreenLoc.x, onScreenLoc.y, 25, ImColor(1, 1, 1, 1), CurrentItemPawnName.c_str());
						item item{};
						item.Actor = CurrentItemPawn;
						item.Name = CurrentItemPawnName;


						item.isPickup = bIsPickup;
						item.distance = ItemDist;

						mrxd.push_back(item);

					}
				}
			}
			item_pawns.clear();
			item_pawns = mrxd;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}


constexpr std::chrono::milliseconds intervaltrigger(50);
auto start_triggerbot = std::chrono::steady_clock::now();
void Cheat::TriggerBot() {
	if (!Settings::Misc::TriggerBot)
		return;

	if (Settings::Misc::OnlyWhenAimbot && !locked)
		return;

	cache::TargetedFortPawn = driver::read<address>(cache::UPlayerController + offset::UTargetedPawn);

	if (!cache::TargetedFortPawn || TargetPawnTeamId == cache::TeamId)
		return;

	if (Settings::CloseRange::TriggerBot && cache::closest_distance > Settings::CloseRange::distance)
		return;

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		return;
	}

	auto end_t = std::chrono::steady_clock::now();
	if (end_t - start_triggerbot < intervaltrigger)
		return;

	leftMouseClick();
	start_triggerbot = end_t;
}


void Cheat::Esp() {
	/*uintptr_t ULevelArray = driver::read<uintptr_t>(cache::UWorld + 0x170);
	for (int OwningWorld = 0; OwningWorld < driver::read<int>(cache::UWorld + 0x170 + (0x170 + sizeof(uintptr_t))); ++OwningWorld) { // The World that has this level in its Levels array
		if (OwningWorld >= driver::read<int>(cache::UWorld + 0x170 + 0x178))
			break;
=======



	/*uintptr_t ULevelArray = driver::read<uintptr_t>(cache::UWorld + 0x178);
	for (int OwningWorld = 0; OwningWorld < driver::read<int>(cache::UWorld + (0x178 + sizeof(uintptr_t))); ++OwningWorld) { // The World that has this level in its Levels array
		//if (OwningWorld >= driver::read<int>(cache::UWorld + 0x178)) 
		//	break;

		printf("Owningworld loop\n");
>>>>>>> Stashed changes

		uintptr_t PersistentLevel = driver::read<uintptr_t>(ULevelArray + sizeof(uintptr_t) * OwningWorld); // The level object, contains the level's actor list. Every Level has a World as its Outer and when streamed in the OwningWorld represents the World that it is a part of

		printf("PersistentLevel: %p\n", PersistentLevel);
		uintptr_t ULevelActorContainer = driver::read<uintptr_t>(ULevelArray + 0x98);
		printf("ULevelActorContainer: %p\n", ULevelActorContainer);
		int test1 = driver::read<int>(ULevelArray + (0xA0 + sizeof(uintptr_t)));
		int test2 = driver::read<int>(ULevelActorContainer + (0x28 + sizeof(uintptr_t)));
		printf("test1 %i\n", test1);
		printf("test2 %i\n", test2);
		for (int AActors = 0; AActors < 0; ++AActors) {
			uintptr_t ActorArray = driver::read<uintptr_t>(PersistentLevel + 0xe0);  // Array of all the actors in the level
			uintptr_t ActorArray2 = driver::read<uintptr_t>(ActorArray + 0x28);  // Array of all the actors in the level
			uintptr_t ActorAddr = driver::read<uintptr_t>(ActorArray2 + sizeof(uintptr_t) * AActors);  // Object that can be placed or spawned in a level

			printf("AActors loop\n");

			int FName = driver::read<int>(ActorAddr + 0x8); // Object FNames in level

			uintptr_t RootComponent = driver::read<uintptr_t>(ActorAddr + 0x198);
			Vector3 Location = driver::read<Vector3>(RootComponent + 0x120); // Location of actor's in the level
			Vector2 Screen = Vector2();
			Vector2 onScreenLoc = SDK::ProjectWorldToScreen(Location);

			char distanceString[64] = { 0 };
			sprintf_s(distanceString, skCrypt("[%.0fm]"), FName);
			ImVec2 TextSize = ImGui::CalcTextSize(distanceString);
			TextSize.x = (TextSize.x * Settings::Visuals::TextSize) / 2;
			TextSize.y = (TextSize.y * Settings::Visuals::TextSize) / 2;
			//std::string dist = "[" + std::to_string(static_cast<int>(distance)) + "m]";;
			Render::DrawOutlinedText(onScreenLoc.x, onScreenLoc.y, TextSize.x, ImColor(1, 1, 1, 1), distanceString);

			// Sort objects in the level by the object FNames
		}
	}*/

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
		if (isCloseRange) Cheat::IsCloseRange = true;
		
		/*
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
		}*/

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
		float FOV = 0;
		float SmoothX = 0;
		float SmoothY = 0;
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

			if (distance < Settings::CloseRange::distance)
			{
				FOV = Settings::CloseRange::CurrentFov;
			}
			else
			{
				FOV = Settings::Aimbot::Fov;
			}

			if (TeamId != cache::TeamId) {
				if (crosshairDist < FOV && distance < ClosestDistance3D) {
					if (!locked)
					{
						TargetPawnTeamId = TeamId;
						cache::closest_distance = distance;
						ClosestDistance3D = distance;
						ClosestDistance2D = crosshairDist;
						TargetPawn = CurrentPawn;
						TargetMesh = Mesh;
						meeesh = Mesh;
					}
				}
			}
		}

		if (!Settings::Aimbot::Enabled) {
			Sleep(1);
			continue;
		}
		if (!Settings::Aimbot::ControllerMode)
		{

			if (!GetAsyncKeyState(Settings::Aimbot::CurrentKey)) {
				locked = FALSE;
				LockedMesh = 0;
				continue;
			}
		}
		else
		{
			DWORD result = XInputGetState(0, &state);
			if (!(state.Gamepad.bLeftTrigger > 0))
			{
				locked = FALSE;
				LockedMesh = 0;
				continue;
			}
			//printf("here2");
			/*dwResult = XInputGetState(0, &state);
			if (dwResult == ERROR_SUCCESS) {
				// Check if the LB button is pressed (XINPUT_GAMEPAD_LEFT_SHOULDER)
				if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
					std::cout << "LB button is pressed." << std::endl;
				}

				else
				{
					locked = FALSE;
					LockedMesh = 0;
					continue;
				}
				std::cout << "checkecheck." << std::endl;
			}*/
		}
		if (!meeesh) {
			continue;
		} else {
			LockedMesh = meeesh;
			locked = TRUE;
		}

		Vector3 head3d = SDK::GetBoneWithRotation(LockedMesh, 110);
		Vector2 head2d = SDK::ProjectWorldToScreen(head3d);
		Vector2 target{};

		if (cache::closest_distance < Settings::CloseRange::distance) {
			SmoothX = Settings::CloseRange::SmoothX;
			SmoothY = Settings::CloseRange::SmoothY;
		} else {
			SmoothX = Settings::Aimbot::SmoothX;
			SmoothY = Settings::Aimbot::SmoothY;
		}

		if (head2d.x != 0)
		{
			if (head2d.x > Width / 2)
			{
				target.x = -(Width / 2 - head2d.x);
				target.x /= SmoothX;
				//if (target.x + Width / 2 > Width / 2 * 2) target.x = 0;
			}
			if (head2d.x < Width / 2)
			{
				target.x = head2d.x - Width / 2;
				target.x /= SmoothX;
				//if (target.x + Width / 2 < 0) target.x = 0;
			}
		}
		if (head2d.y != 0)
		{
			if (head2d.y > Height / 2)
			{
				target.y = -(Height / 2 - head2d.y);
				target.y /= SmoothY;
				//if (target.y + Height / 2 > Height / 2 * 2) target.y = 0;
			}
			if (head2d.y < Height / 2)
			{
				target.y = head2d.y - Height / 2;
				target.y /= SmoothY;
				//if (target.y + Height / 2 < 0) target.y = 0;
			}
		}

		target.x = (target.x > 0) ? ((target.x < 1) ? (1) : target.x) : ((target.x > -1) ? (-1) : target.x);
		target.y = (target.y > 0) ? ((target.y < 1) ? (1) : target.y) : ((target.y > -1) ? (-1) : target.y);
		//target.x = (target.x > 0) ? ((target.x < 1) ?	  (0.5f)	: target.x) : ((target.x > -1) ?	  (-0.5f)    : target.x);
		//target.y = (target.y > 0) ? ((target.y < 1) ?	  (0.5f)	: target.y) : ((target.y > -1) ?	  (-0.5f)    : target.y);

		target.x = clamp(target.x, -250.0f, 250.0f);
		target.y = clamp(target.y, -250.0f, 250.0f);

		input::move_mouse(target.x, target.y);
		//Sleep(1);
		std::this_thread::sleep_for(std::chrono::nanoseconds(1500)); 
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
	/*if (Settings::Aimbot::Predict)
		PredictBulletDrop(Pos3D, Velocity, 30000.f, 0.12f, Distance);*/

	Vector2 Pos2D = SDK::ProjectWorldToScreen(Pos3D);
	Vector2 target{};

	bool isCloseRange = Settings::CloseRange::Enabled && Settings::CloseRange::SmartSmooth && Distance < Settings::CloseRange::distance;
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
