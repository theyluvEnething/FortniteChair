#pragma once
#include <vector>
#include <Windows.h>

namespace offset {
	const ULONG64 UWORLD = 0x117B7E88;
	const ULONG64 GAME_INSTANCE = 0x1d0;
	const ULONG64 LOCAL_PAWN = 0x338;
	const ULONG64 LOCAL_PLAYERS = 0x38;
	const ULONG64 VIEWPOINT = 0x10F90A10;
	const ULONG64 PLAYER_CONTROLLER = 0x30;
	const ULONG64 PAWN_PRIVATE = 0x308ULL;
	const ULONG64 ROOT_COMPONENT = 0x198;
	const ULONG64 RELATIVE_LOCATION = 0x128;
	const ULONG64 PLAYER_STATE = 0x2b0;
	const ULONG64 TEAM_INDEX = 0x10F1ULL;
	const ULONG64 GAME_STATE = 0x158;
	const ULONG64 MESH = 0x318;
	const ULONG64 COMPONENT_TO_WORLD = 0x1c0; // 0x1c0 // 0x1D0

	const ULONG64 BONE_ARRAY = 0x598;
	const ULONG64 BONE_ARRAY_CACHE = 0x60;

	const ULONG64 LAST_SUMBIT_TIME = 0x358;
	const ULONG64 LAST_SUMBIT_TIME_ON_SCREEN = 0x360;

	const ULONG64 PLAYER_ARRAY = 0x2A8;
	const uintptr_t PLAYERCOUNT = 0xA0;
	const uintptr_t PLAYERSIZE = 0x8;
	//uintptr_t DISPLAYNAME = 0x98;
	//uintptr_t iWEAPONDATA = 0x4d0;
	//uintptr_t iCURRENTWEAPON = 0xa20;

	const uintptr_t PAWNPRIV = 0x308; //struct APawn* PawnPrivate;
	const uintptr_t comptowrld = 0x1c0; // MIGHT NEED TO CHANGE
	const uintptr_t bonec = 0x60;// ??
};

//[+] : UWorld -> 0x11770D88
//[+] : GObjects -> 0x11749AC0
//[+] : GNames -> 0x11944BC0
//[+] : ACharacter::Mesh -> 0x318
//[+] : AActor::RootComponent -> 0x198
//[+] : AActor::CustomTimeDilation -> 0x68
//[+] : UPlayer::PlayerController -> 0x30
//[+] : UGameInstance::LocalPlayers -> 0x38
//[+] : UWorld::OwningGameInstance -> 0x1D0
//[+] : APlayerState::PawnPrivate -> 0x308
//[+] : UWorld::GameState -> 0x158
//[+] : AGameStateBase::PlayerArray -> 0x2A8
//[+] : AFortPickup::PrimaryPickupItemEntry -> 0x350
//[+] : FFortItemEntry::ItemDefinition -> 0x18
//[+] : UFortItemDefinition::Tier -> 0x11B
//[+] : UItemDefinitionBase::ItemName -> 0x30
//[+] : APlayerController::PlayerCameraManager -> 0x348
//[+] : APlayerController::InputPitchScale -> 0x53C
//[+] : APlayerController::InputYawScale -> 0x538
//[+] : APlayerController::InputRollScale -> 0x540
//[+] : APlayerCameraManager::DefaultFOV -> 0x2A4
//[+] : APlayerCameraManager::ViewPitchMin -> 0x2384
//[+] : APlayerCameraManager::ViewPitchMax -> 0x2388
//[+] : APlayerCameraManager::ViewYawMin -> 0x238C
//[+] : APlayerCameraManager::ViewYawMax -> 0x2390
//[+] : APlayerCameraManager::ViewRollMin -> 0x2394
//[+] : APlayerCameraManager::ViewRollMax -> 0x2398
//[+] : APlayerCameraManager::ViewTarget -> 0x320
//[+] : ABuildingWeakSpot::bActive -> 0x2C0
//[+] : ABuildingWeakSpot::HitCount -> 0x2C4
//[+] : ABuildingContainer::bAlreadySearched -> 0xD92
//[+] : ULocalPlayer::ViewportClient -> 0x78
//[+] : AFortPawn::CurrentWeapon -> 0xA20
//[+] : AFortPawn::PreviousWeapon -> 0xA28
//[+] : AFortPawn::CurrentWeaponList -> 0xA30
//[+] : AFortPlayerPawn::CurrentVehicle -> 0x2880
//[+] : AFortWeapon::WeaponData -> 0x4D0
//[+] : AFortWeapon::CurrentReticleColor -> 0xD28
//[+] : AFortWeapon::AmmoCount -> 0xE1C
//[+] : AFortWeapon::ChargeTime -> 0xE30
//[+] : AFortWeapon::AccumulatedChargeTime -> 0xE34
//[+] : AFortWeapon::LastChargeTime -> 0xE38
//[+] : AFortWeapon::bIsReloadingWeapon -> 0x368
//[+] : AFortWeapon::bIsChargingWeapon -> 0x369
//[+] : AFortWeapon::bIsEquippingWeapon -> 0x350
//[+] : AFortWeapon::TimeToEquip -> 0x340
//[+] : AFortWeapon::bIgnoreTryToFireSlotCooldownRestriction -> 0x13A9
//[+] : UMaterial::Wireframe -> 0x1B8
//[+] : UMaterial::BlendMode -> 0x129
//[+] : UMaterialInstance::ScalarParameterValues -> 0x160
//[+] : UMaterialInstance::VectorParameterValues -> 0x170
//[+] : AWorldSettings::WorldGravityZ -> 0x310
//[+] : UFortClientSettingsRecord::HUDScale -> 0x4C0
//[+] : UFortClientSettingsRecord::InitialHUDScale -> 0x4C8
//[+] : UFortClientSettingsRecord::ScopedSlow -> 0x54C
//[+] : UFortClientSettingsRecord::TargetingSlow -> 0x548
//[+] : UFortClientSettingsRecord::MouseSensitivity -> 0x574
//[+] : UFortClientSettingsRecord::MouseSensitivityX -> 0x578
//[+] : UFortClientSettingsRecord::MouseSensitivityY -> 0x57C
//[+] : UFortClientSettingsRecord::MusicVolume -> 0x900
//[+] : UFortClientSettingsRecord::MasterVolume -> 0x8FC
//[+] : UFortClientSettingsRecord::FOVMinimum -> 0x994
//[+] : UFortClientSettingsRecord::FOVMaximum -> 0x998
//[+] : UFortClientSettingsRecord::bAnonymousMode -> 0x925
//[+] : USceneComponent::RelativeRotation -> 0x138
//[+] : USceneComponent::RelativeLocation -> 0x120
//[+] : USceneComponent::ComponentVelocity -> 0x168
//[+] : USceneComponent::RelativeScale3D -> 0x150
//[+] : USceneComponent::bAbsoluteLocation -> 0x180
//[+] : USceneComponent::PhysicsVolume -> 0xA8
//[+] : USkinnedMeshComponent::bForceWireframe -> 0x75E
//[+] : USkinnedMeshComponent::SkeletalMesh -> 0x540
//[+] : AFortPlayerStateAthena::TeamIndex -> 0x10F1