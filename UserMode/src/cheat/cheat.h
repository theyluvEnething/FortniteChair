#pragma once
#include <vector>
#include "data/cache.h"
#include "../util/util.h"

class Cheat
{
public:

	static uintptr_t TargetPawn;
	static uint64_t TargetMesh;
	static char TargetPawnTeamId;
	static float ClosestDistance2D;
	static float ClosestDistance3D;
	static float ActualDistance3D;


	static bool locked;
	static bool IsCloseRange;
public:
	static void Init(); 
	static void Present();
	static void Update();
	static void LateUpdate();
	static void MouseAimbotThread();
	static void MemoryAimbot();
	static void MouseAimbot();
	static void Esp();
	static void GetWeaponData();
	static void PredictBulletDrop(Vector3& Position, Vector3 Velocity, float ProjectileSpeed, float ProjectileGravityScale, float Distance);
	static void TriggerBot();
};

