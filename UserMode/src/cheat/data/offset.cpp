#include "offsets.h"

// ================================================= //
//	Offsets with U- or A- are all the names of the 	 //
//	official Fortnite SDK Classes / Instances.		 //
//													 //
//	Offsets with i- are all self calculated offsets  //
//  and/or arrays and other stuff.					 //
// ================================================= //
namespace offset {
	uintptr_t UWorld = 0x8483049F; //  wrong uworld -> this is right one 0x124BF388;
	uintptr_t AGameStateBase = 0;						// [+] : UWorld -> AGameStateBase // 0x160
	uintptr_t PersistentLevel = 0;						// [+] : UWorld -> ULevel	  // 0x30
	uintptr_t UGameInstance = 0;						// [+] : UWorld -> UGameInstance // 0x1d8
	uintptr_t ULocalPlayers = 0;						// [+] : UWorld -> UGameInstance -> TArray<ULocalPlayer*> // 0x38
	uintptr_t APlayerController = 9;					// [+] : UWorld -> UGameInstance -> TArray<ULocalPlayer*> -> APlayerController // 0x30
	uintptr_t ULocalPawn = 0;							// [+] : UWorld -> UGameInstance -> TArray<ULocalPlayer*> -> ULocalPlayer[LocalPawn] -> APlayerController // 0x338

	uintptr_t iPlayerArray = 0;						// [+] : UWorld -> AGameStateBase -> TArray<APlayerState*> // 0x2A8
	uintptr_t iPlayerCount = 0;	//	0xB0				// [+] : UWorld -> AGameStateBase -> TArray<APlayerState*> + sizeof(uintptr_t) // 0xA8
	uintptr_t iPlayerSize = 0x8;							// [+] : sizeof(uintptr_t)

	uintptr_t AFortPlayerStateAthena = 0x2B0;				// [+] : [?] ->
	uintptr_t APawn = 0x308;						// [+] : APlayerState -> PawnPrivate

	uintptr_t UTargetedPawn = 0x18a0;						// [+] : UWorld -> UGameInstance -> TArray<ULocalPlayer*> -> ULocalPlayer[LocalPawn] -> AFortPlayerController -> TargetedFortPawn
	uintptr_t bIsABot = 0x29a;							// [+] : APlayerState -> bIsABot[4]

	uintptr_t AWorldSettings = 0;						// [+] : UWorld -> ULevel -> AWorldSettings // 0x2a0
	uintptr_t WorldGravityZ = 0x310;						// [+] : UWorld -> ULevel -> AWorldSettings -> WorldGravityZ // 0x310

	uintptr_t AFortWeapon = 0xa68;						// [+] : [X] -> AFortPawn -> AFortWeapon
	uintptr_t AFortWeaponData = 0x4f0;					// [+] : [X] -> AFortPawn -> AFortWeapon -> WeaponData
	uintptr_t AFortWeaponFText = 0x4D0;					// [+] : [X] -> AFortPawn -> AFortWeapon -> WeaponData


	uintptr_t RootComponent = 0x198;
	uintptr_t RelativeLocation = 0x120; //0x128
	uintptr_t ComponentVelocity = 0x168; //0x128


	uintptr_t TEAM_INDEX = 0x1251; //Class FortniteGame.AFortPlayerController - > TargetedFortPawn
	uintptr_t MESH = 0x318;
	uintptr_t COMPONENT_TO_WORLD = 0x1c0; // 0x1c0 // 0x1D0

	uintptr_t BONE_ARRAY = 0x5B0;
	uintptr_t BONE_ARRAY_CACHE = 0x5F8;

	uintptr_t LAST_SUMBIT_TIME = 0x358;
	uintptr_t LAST_SUMBIT_TIME_ON_SCREEN = 0x360;

	//uintptr_t DISPLAYNAME = 0x98;
	//uintptr_t iWEAPONDATA = 0x4d0;
	//uintptr_t iCURRENTWEAPON = 0xa20;

	uintptr_t bonec = 0x60;// ??



	int bIsJumping = 0xa18; //Class FortniteGame.AFortPawn - > char bIsJumping : 1; 
	int bIsSliding = 0x75b; //Class FortniteGame.AFortPawn - > char bIsSliding : 1;
	// team check for TargetedFortPawn
	//			    0x758                 -  0x758 - 0x28	+
	// [+] : TargetedFortPawn (AFortPawn) - Offset to AAcor + 

	uintptr_t MY_FORT_PAWN = 0xaa0;



	uintptr_t OnSetFirstPersonCamera = 0xf80;
	uintptr_t bAutoRunOn = 0xf55;
}

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
// 
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
// 
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
