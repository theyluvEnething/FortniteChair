#pragma once
#include "../../util/util.h"
#include "../sdk/sdk.h"

namespace cache
{
	inline struct Camera Camera;
	inline address UWorld;
	inline address UGameInstance;
	inline address ULocalPlayers;
	inline address UPlayerController;
	inline address ULocalPawn;
	inline address PersistentLevel;
	inline address AWorldSettings;
	inline address RootComponent;
	inline address PlayerState;
	inline address AGameStateBase;
	inline address iPlayerArray;
	inline address iPlayerCount;
	inline Vector3 RelativeLocation;
	inline address TargetedFortPawn;
	inline address CurrentWeapon;
	inline float closest_distance;
	inline uintptr_t closest_mesh;
	inline float WorldGravityZ;
	inline int TeamId;
	inline bool InLobby;
	inline float ProjectileSpeed;
	inline float ProjectileGravityScale;
};

